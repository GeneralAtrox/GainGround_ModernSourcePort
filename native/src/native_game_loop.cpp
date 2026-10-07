#include "gain_ground/native_game_loop.h"
#include "gain_ground/direct_boot_audio.h"
#include <utility>

namespace gain_ground {
namespace {
constexpr std::uint32_t kServiceReturn = 0x80118U;
FunctionContext service_context(RuntimeHost &host) {
    FunctionContext c{};
    c.host = &host; c.cpu = 0; c.state = 0xff;
    c.registers.status = 0x2000;
    c.registers.program_counter = kServiceReturn;
    return c;
}
void word(RuntimeHost &host, unsigned region, unsigned offset, unsigned value) {
    host.write_memory_word(static_cast<std::uint16_t>(region), offset,
        static_cast<std::uint16_t>(value), 0xffff);
}
}
FunctionResult NativeGameLoop::invoke(FunctionContext &c, std::uint32_t target, std::uint32_t resume) {
    auto &r = c.registers;
    r.address[7] -= 4U;
    const auto region = c.cpu ? 2U : 3U;
    const auto sp = r.address[7] & 0x3ffffU;
    word(host_, region, sp, resume >> 16U); word(host_, region, sp + 2U, resume);
    r.program_counter = target;
    return host_.run(c);
}
bool NativeGameLoop::returned(const FunctionResult &result, const FunctionContext &c,
                             std::uint32_t resume, std::uint32_t sp) {
    if (host_.faulted()) return false;
    if (result.status != TranslationStatus::complete || result.control != 1U ||
        result.exit_program_counter != resume || c.registers.program_counter != resume ||
        c.registers.address[7] != sp) {
        host_.stop("Native service did not return to its owner");
        return false;
    }
    return true;
}
bool NativeGameLoop::start(DirectAssetLoader &assets) {
    auto setup = service_context(host_);
    if (!host_.prepare_direct_boot(assets, setup)) return false;
    host_.attach_devices(devices_);
    devices_.use_native_events();
    prepare_direct_boot_devices(devices_);
    host_.set_native_services(&NativeGameLoop::service, this);
    host_.select_cpu(0);
    auto boot = setup;
    const auto audio = run_direct_boot_audio(boot);
    if (audio.status != TranslationStatus::complete || audio.control != 1U || host_.faulted()) return false;
    const auto initialized = host_.run(setup);
    if (initialized.status != TranslationStatus::complete || initialized.control != 10U ||
        initialized.exit_program_counter != 0x80104U || host_.faulted()) return false;
    ready_ = true;
    game_ = {};
    game_.host = &host_; game_.cpu = 1; game_.state = 0xdd;
    game_.registers.status = 0x2000;
    game_.registers.address[7] = 0x7ffe;
    game_.registers.program_counter = 0x84da;
    word(host_, 3, 0x38002, 1);
    devices_.write(0xa00004, 0x18, 0xffff);
    host_.select_cpu(1);
    const auto started = host_.run(game_);
    return !host_.faulted() && started.status == TranslationStatus::complete &&
        started.control == 10U && started.exit_program_counter == 0x8572U;
}
void NativeGameLoop::service(void *self) { static_cast<NativeGameLoop *>(self)->services(); }
bool NativeGameLoop::service_interrupt(unsigned level) {
    auto c = service_context(host_);
    // Existing sound/rowscroll routines own their register save and real RTE.
    // Their ABI receives a six-byte exception frame on the service stack.
    c.registers.address[7] = 0xfffffffaU;
    word(host_, 3, 0x3fffa, 0x2000);
    word(host_, 3, 0x3fffc, kServiceReturn >> 16U);
    word(host_, 3, 0x3fffe, kServiceReturn);
    c.registers.status = static_cast<std::uint16_t>(0x2000U | (level << 8U));
    c.registers.program_counter = level == 5 ? 0x80fda : level == 4 ? 0x80fc6 : 0x80f96;
    devices_.acknowledge_native_event(0, level);
    const auto result = host_.run(c);
    if (level == 3 || level == 5) ++sound_services_;
    if (host_.faulted()) return false;
    if (result.status != TranslationStatus::complete || result.control != 2U ||
        c.registers.program_counter != kServiceReturn || c.registers.address[7] != 0U ||
        c.registers.status != 0x2000U) {
        host_.stop("Native timed service did not complete its return"); return false;
    }
    return true;
}
void NativeGameLoop::services() {
    if (!ready_ || servicing_ || host_.selected_context() == 0U || host_.faulted()) return;
    servicing_ = true;
    host_.select_cpu(0);
    // Consume each latched board event once. Services share one timeline;
    // their writes are complete before gameplay resumes.
    while (!host_.faulted()) {
        const auto level = devices_.irq_level(0);
        if (level < 3 || !service_interrupt(level)) break;
    }
    if (!host_.faulted() && (host_.region_bytes(3)[0x38400] & 0x80U)) {
        host_.write_memory_word(3, 0x38400, 0, 0xff00);
        auto c = service_context(host_);
        for (const auto target : {0x80356U, 0x80414U, 0x80612U}) {
            const auto result = invoke(c, target, kServiceReturn);
            if (result.status == TranslationStatus::complete && result.control == 10U &&
                result.exit_program_counter == 0x80104U) break; // Original reset completed.
            if (!returned(result, c, kServiceReturn, 0U)) break;
        }
    }
    host_.select_cpu(1);
    servicing_ = false;
}
bool NativeGameLoop::frame() {
    if (!ready_ || host_.faulted()) return false;
    host_.select_cpu(1);
    auto &r = game_.registers;
    r.address[7] = 0x7ffe;
    // Native orchestration of the original frame-wait, input, actor and sprite
    // services. The actor's actual scheduler return address remains its ABI.
    for (const auto call : {std::pair{0x85b0U, 0x857aU}, {0x85beU, 0x8580U}}) {
        const auto result = invoke(game_, call.first, call.second);
        if (!returned(result, game_, call.second, 0x7ffe)) return false;
    }
    r.address[5] = 0x1400U;
    word(host_, 2, 0x822, host_.read_memory_word(2, 0x824, 0xffff));
    do {
        const auto actor = r.address[5];
        const auto state = host_.read_memory_word(2, actor, 0xff00);
        r.status = static_cast<std::uint16_t>((r.status & ~0xfU) |
            (state & 0x8000U ? 8U : 0U) | (state == 0U ? 4U : 0U));
        if (state & 0x8000U) {
            r.address[0] = (std::uint32_t(host_.read_memory_word(2, actor + 2, 0xffff)) << 16U) |
                host_.read_memory_word(2, actor + 4, 0xffff);
            const auto result = invoke(game_, r.address[0], 0x8594);
            if (result.status == TranslationStatus::complete && result.control == 10U &&
                result.exit_program_counter == 0x8572U) return true; // Original stack-reset transfer.
            if (!returned(result, game_, 0x8594, 0x7ffe)) return false;
        }
        r.address[5] += 0x80U;
        const auto remaining = static_cast<std::uint16_t>(host_.read_memory_word(2, 0x822, 0xffff) - 1U);
        word(host_, 2, 0x822, remaining);
        r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) |
            (remaining & 0x8000U ? 8U : 0U) | (remaining == 0U ? 4U : 0U) |
            (remaining == 0xffffU ? 0x11U : 0U) | (remaining == 0x7fffU ? 2U : 0U));
        if (!remaining) break;
        if (r.address[5] >= 0x8000U) { host_.stop("Native actor count exceeds the actor arena"); return false; }
    } while (!host_.faulted());
    const auto sprites = invoke(game_, 0x16d16, 0x85a4);
    if (!returned(sprites, game_, 0x85a4, 0x7ffe)) return false;
    ++updates_;
    return true;
}
}

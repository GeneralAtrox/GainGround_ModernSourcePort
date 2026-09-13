// Implemented but unverified. Source: original primary F209 and secondary F224.
#include "legacy_gameplay_bridge.h"
#include "gain_ground/gameplay/attack_phase.h"
#include "../translated/unverified_cpu_b_machine.h"

namespace gain_ground::gameplay {
namespace {
class LegacyAttackPhase final : public AttackPhase {
public:
    LegacyAttackPhase(FunctionContext &context, AttackSlot slot)
        : context_(context), host_(*context.host), registers_(context.registers),
          effects_{host_, registers_, 1U, 0x72U},
          secondary_(slot == AttackSlot::secondary), entry_record_(registers_.address[5]),
          entry_pc_(registers_.program_counter) {}

    bool invoke_callbacks() override {
        if (entry_pc_ == 0x10a4aU || entry_pc_ == 0x10cdcU || resuming_spawn()) return true;
        const auto profile = word(record() + (secondary_ ? 0x3aU : 0x38U));
        effects_.dw(0U, profile); effects_.logic(profile, 16U);
        effects_.dw(7U, profile); effects_.logic(profile, 16U);
        auto d0 = effects_.add(profile, profile, 16U);
        effects_.dw(0U, d0);
        effects_.dw(1U, d0); effects_.logic(d0, 16U);
        d0 = effects_.add(d0, d0, 16U); effects_.dw(0U, d0);
        const auto d1 = effects_.add(registers_.data[1], d0, 16U);
        effects_.dw(1U, d1);
        effects_.asl_word(0U, 3U);
        d0 = effects_.add(registers_.data[0], d1, 16U);
        effects_.dw(0U, d0);
        // 38-byte descriptors: each button uses its own selector/table.
        registers_.address[3] = (secondary_ ? 0x11744U : 0x115a2U)
            + static_cast<std::int16_t>(static_cast<std::uint16_t>(d0));
        if (!call(secondary_ ? 225U : 210U,
                  secondary_ ? 0x10cd8U : 0x10a46U,
                  secondary_ ? 0x10d34U : 0x10a92U,
                  secondary_ ? 0x10cdcU : 0x10a4aU)) return false;
        if (!secondary_) registers_.program_counter = 0x10a4aU;
        return true;
    }

    void decrement_cooldown() override {
        if (resuming_spawn()) return;
        const auto address = record() + (secondary_ ? 0x41U : 0x40U);
        const auto timer = byte(address);
        effects_.logic(timer, 8U);
        if (timer == 0U) return;
        const auto previous = byte(address);
        const auto next = static_cast<std::uint8_t>(previous - 1U);
        if (!secondary_) (void)effects_.sub(previous, 1U, 8U);
        byte(address, next);
        if (secondary_) (void)effects_.sub(previous, 1U, 8U);
    }

    bool active() override {
        if (resuming_spawn()) return true;
        const auto mode = byte(record() + 0x3eU);
        const auto expected = secondary_ ? 2U : 1U;
        (void)effects_.sub(mode, expected, 8U, true);
        return mode == expected;
    }

    bool advance_animation_clock() override {
        if (resuming_spawn()) return true;
        const auto previous = byte(record() + 0x3cU);
        const auto next = static_cast<std::uint8_t>(previous + 1U);
        if (!secondary_) (void)effects_.add(previous, 1U, 8U);
        byte(record() + 0x3cU, next);
        if (secondary_) (void)effects_.add(previous, 1U, 8U);
        registers_.address[0] = secondary_ ? 0x1154eU : 0x11542U;
        selector_ = static_cast<std::int16_t>(static_cast<std::uint16_t>(registers_.data[7]));
        const auto period = byte(registers_.address[0] + selector_);
        effects_.db(0U, period); effects_.logic(period, 8U);
        const auto current = byte(record() + 0x3cU);
        (void)effects_.sub(period, current, 8U, true);
        return static_cast<std::int8_t>(period) <= static_cast<std::int8_t>(current);
    }

    bool advance_animation_phase() override {
        if (resuming_spawn()) {
            // 10D1A follows the spawn callback: do not clear/increment again.
            phase_ = byte(record() + 0x3dU);
            effects_.db(0U, phase_); effects_.logic(phase_, 8U);
            return true;
        }
        (void)byte(record() + 0x3cU);
        byte(record() + 0x3cU, 0U); effects_.logic(0U, 8U);
        const auto previous = byte(record() + 0x3dU);
        effects_.db(0U, previous); effects_.logic(previous, 8U);
        phase_ = static_cast<std::uint8_t>(effects_.add(previous, 1U, 8U));
        effects_.db(0U, phase_);
        byte(record() + 0x3dU, phase_); effects_.logic(phase_, 8U);
        if (secondary_) {
            (void)effects_.sub(phase_, 1U, 8U, true);
            if (phase_ == 1U) {
                if (!call(230U, 0x10d14U, 0x10e60U, 0x10d1aU)) return false;
                phase_ = byte(record() + 0x3dU);
                effects_.db(0U, phase_); effects_.logic(phase_, 8U);
            }
        }
        return true;
    }

    bool animation_complete() override {
        registers_.address[0] = secondary_ ? 0x1156eU : 0x11562U;
        selector_ = static_cast<std::int16_t>(static_cast<std::uint16_t>(registers_.data[7]));
        const auto limit = byte(registers_.address[0] + selector_);
        (void)effects_.sub(phase_, limit, 8U, true);
        return phase_ >= limit;
    }

    void return_to_idle() override {
        (void)byte(record() + 0x3eU);
        byte(record() + 0x3eU, 0U); effects_.logic(0U, 8U);
        byte(record() + 0x3dU, 1U); effects_.logic(1U, 8U);
    }

    FunctionResult finish() {
        const auto high = word(registers_.address[7]);
        const auto low = word(registers_.address[7] + 2U);
        registers_.address[7] += 4U;
        registers_.program_counter = (static_cast<std::uint32_t>(high) << 16U) | low;
        return FunctionResult::complete(1U, registers_.program_counter);
    }

    FunctionResult child_result() const noexcept { return child_; }

private:
    FunctionContext &context_;
    ExecutionHost &host_;
    CpuRegisters &registers_;
    translated::unverified::Machine effects_;
    bool secondary_;
    std::uint32_t entry_record_;
    std::uint32_t entry_pc_;
    std::int16_t selector_{};
    std::uint8_t phase_{};
    FunctionResult child_;

    bool resuming_spawn() const noexcept { return entry_pc_ == 0x10d1aU; }

    std::uint32_t record() const noexcept {
        return secondary_ ? entry_record_ : registers_.address[5];
    }
    std::uint16_t word(std::uint32_t address) {
        return host_.read_memory_word(2U, secondary_ ? address & 0xffffffU : address, 0xffffU);
    }
    std::uint8_t byte(std::uint32_t address) {
        const auto mask = static_cast<std::uint16_t>((address & 1U) ? 0xffU : 0xff00U);
        return static_cast<std::uint8_t>(host_.read_memory_word(2U, address & ~1U, mask)
            >> ((address & 1U) ? 0U : 8U));
    }
    void byte(std::uint32_t address, std::uint8_t value) {
        const auto mask = static_cast<std::uint16_t>((address & 1U) ? 0xffU : 0xff00U);
        const auto data = static_cast<std::uint16_t>(static_cast<unsigned>(value)
            << ((address & 1U) ? 0U : 8U));
        host_.write_memory_word(2U, address & ~1U, data, mask);
    }
    bool call(std::uint32_t function, std::uint32_t site,
              std::uint32_t target, std::uint32_t continuation) {
        registers_.address[7] -= 4U;
        host_.write_memory_word(2U, registers_.address[7],
            static_cast<std::uint16_t>(continuation >> 16U), 0xffffU);
        host_.write_memory_word(2U, registers_.address[7] + 2U,
            static_cast<std::uint16_t>(continuation), 0xffffU);
        registers_.program_counter = target;
        child_ = host_.call_function(function, 1U, 0x72U, 2U, site, target, context_);
        return child_.status == TranslationStatus::complete && child_.control == 1U &&
               registers_.program_counter == continuation;
    }
};
} // namespace

bool run_character_attack_phase(const Character &character, FunctionContext &context,
                                 FunctionResult &result)
{
    const auto pc = context.registers.program_counter;
    const auto slot = (pc == 0x10cc0U || pc == 0x10cdcU || pc == 0x10d1aU)
        ? AttackSlot::secondary : AttackSlot::primary;
    LegacyAttackPhase phase(context, slot);
    result = character.advance_attack(slot, phase) ? phase.finish() : phase.child_result();
    return true;
}

} // namespace gain_ground::gameplay

#include "gain_ground/contract_types.h"
#include "gain_ground/sound_caller_timing.h"

#include <array>
#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

[[nodiscard]] std::uint16_t read_word(SoundCallerTiming &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & kAddressMask, kWordMask);
}

void prefetch(SoundCallerTiming &host, std::uint32_t address)
{
    (void)read_word(host, address);
}

void prefetch_span(SoundCallerTiming &host, std::uint32_t first, std::uint32_t last)
{
    for (auto address = first; address <= last; address += 2U)
        prefetch(host, address);
}

void set_logic_word(CpuRegisters &registers, std::uint16_t value)
{
    auto flags = static_cast<std::uint16_t>(registers.status & 0x0010U);
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_bit_test(CpuRegisters &registers, bool set)
{
    if (set)
        registers.status = static_cast<std::uint16_t>(registers.status & ~0x0004U);
    else
        registers.status = static_cast<std::uint16_t>(registers.status | 0x0004U);
}

void set_word(std::uint32_t &target, std::uint16_t value)
{
    target = (target & 0xffff0000U) | value;
}

[[nodiscard]] FunctionResult finish(SoundCallerTiming &host, CpuRegisters &registers,
                                    bool discard_parent, bool stack_entry_prefetched = false)
{
    if (discard_parent) {
        prefetch_span(host, stack_entry_prefetched ? 0x00083ee0U : 0x00083edeU,
                      0x00083ee4U);
        registers.address[7] += 4U;
    }
    const auto high = read_word(host, registers.address[7]);
    const auto low = read_word(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.program_counter = target;
    return FunctionResult::complete(discard_parent ? 8U : 1U, target);
}
} // namespace

FunctionResult cpu_a_sound_update_packed_control_word(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    SoundCallerTiming host(context);
    auto &registers = context.registers;
    host.begin(0x83e0cU);
    registers.data[2] = 0U;
    set_logic_word(registers, 0U);
    prefetch_span(host, 0x00083e10U, 0x00083e12U);
    const auto packed = read_word(host, registers.address[6] + 0x32U);
    registers.data[2] = packed;
    set_logic_word(registers, packed);
    prefetch(host, 0x00083e14U);

    auto write_upper = [&](bool direct_zero) -> FunctionResult {
        if (direct_zero) {
            prefetch_span(host, 0x00083e4eU, 0x00083e54U);
        } else {
            prefetch_span(host, 0x00083e46U, 0x00083e54U);
            const auto selected = static_cast<std::uint16_t>(registers.data[2] & 0x0780U);
            set_word(registers.data[2], selected);
            set_logic_word(registers, selected);
            const auto upper = static_cast<std::uint16_t>(registers.data[0] & 0xf800U);
            set_word(registers.data[0], upper);
            set_logic_word(registers, upper);
        }
        const auto result = static_cast<std::uint16_t>(registers.data[2] | registers.data[0]);
        set_word(registers.data[2], result);
        set_logic_word(registers, result);
        host.write_memory_word(kRegion, (registers.address[6] + 0x32U) & kAddressMask,
                               result, kWordMask);
        set_logic_word(registers, result);
        prefetch(host, 0x00083e56U);
        return finish(host, registers, false);
    };

    if (packed == 0U) {
        host.clocks(2U); // BEQ.w taken; remaining direct path is bus-only.
        return write_upper(true);
    }
    host.clocks(4U); // BEQ.w not taken; two sequential prefetches follow.

    constexpr std::array<unsigned, 5> upper_bits{15U, 14U, 13U, 12U, 11U};
    for (std::size_t index = 0; index < upper_bits.size(); ++index) {
        const auto test = 0x00083e16U + static_cast<std::uint32_t>(index * 8U);
        prefetch_span(host, test, test + 6U);
        host.clocks(2U); // BTST #n,D2 internal tail (all bit numbers < 16).
        const bool selected = (packed & (1U << upper_bits[index])) != 0U;
        set_bit_test(registers, selected);
        if (!selected) {
            host.clocks(4U); // BNE.w not taken.
            continue;
        }

        host.clocks(2U); // BNE.w taken.
        const auto handler = 0x00083e92U + static_cast<std::uint32_t>(index * 8U);
        prefetch_span(host, handler, handler + 6U);
        const auto mask = static_cast<std::uint16_t>(0xffffU << upper_bits[index]);
        const auto matched = static_cast<std::uint16_t>(registers.data[3] & mask);
        set_word(registers.data[3], matched);
        set_logic_word(registers, matched);
        if (matched != 0U) {
            host.clocks(2U); // BNE.b taken to the upper write.
            return write_upper(false);
        }

        host.clocks(4U); // BNE.b not taken.
        prefetch(host, handler + 8U);
        host.clocks(2U); // BRA.b to 83e56.
        prefetch_span(host, 0x00083e56U, 0x00083e5eU);
        auto upper = static_cast<std::uint16_t>(registers.data[0]);
        set_word(registers.data[3], upper);
        set_logic_word(registers, upper);
        upper = static_cast<std::uint16_t>(upper & 0xf800U);
        set_word(registers.data[3], upper);
        set_logic_word(registers, upper);
        if (upper != 0U) {
            host.clocks(2U); // BNE.w taken to the stack-discard return.
            return finish(host, registers, true);
        }
        host.clocks(4U); // BNE.w not taken to the lower-priority tests.
        goto lower_priority;
    }

    prefetch_span(host, 0x00083e3eU, 0x00083e44U);
    {
        const auto upper = static_cast<std::uint16_t>(registers.data[3] & 0xf800U);
        set_word(registers.data[3], upper);
        set_logic_word(registers, upper);
        if (upper != 0U) {
            host.clocks(4U); // BEQ.w not taken.
            return write_upper(false);
        }
        host.clocks(2U); // BEQ.w taken to 83e60.
    }

lower_priority:
    prefetch_span(host, 0x00083e60U, 0x00083e62U);
    set_word(registers.data[3], static_cast<std::uint16_t>(registers.data[0]));
    set_logic_word(registers, static_cast<std::uint16_t>(registers.data[3]));
    constexpr std::array<unsigned, 4> lower_bits{10U, 9U, 8U, 7U};
    constexpr std::array<std::uint32_t, 4> lower_handlers{
        0x00083ebaU, 0x00083ec4U, 0x00083eceU, 0x00083ed8U};
    for (std::size_t index = 0; index < lower_bits.size(); ++index) {
        const auto test = 0x00083e62U + static_cast<std::uint32_t>(index * 8U);
        prefetch_span(host, index == 0U ? test + 2U : test, test + 6U);
        host.clocks(2U); // BTST #n,D2 internal tail.
        const bool selected = (packed & (1U << lower_bits[index])) != 0U;
        set_bit_test(registers, selected);
        if (!selected) {
            host.clocks(4U); // BNE.w not taken.
            continue;
        }

        host.clocks(2U); // BNE.w taken.
        const auto handler = lower_handlers[index];
        prefetch_span(host, handler, handler + 6U);
        const auto mask = static_cast<std::uint16_t>(0x0780U &
                          static_cast<std::uint16_t>(0xffffU << lower_bits[index]));
        const auto matched = static_cast<std::uint16_t>(registers.data[3] & mask);
        set_word(registers.data[3], matched);
        set_logic_word(registers, matched);
        if (matched == 0U) {
            host.clocks(4U); // BNE.b not taken.
            if (index != 3U) {
                prefetch(host, handler + 8U);
                host.clocks(2U); // BRA.w to 83ede.
            } else
                return finish(host, registers, true, true);
            return finish(host, registers, true);
        }
        host.clocks(2U); // BNE.b taken to 83e82.
        goto write_lower;
    }

write_lower:
    prefetch_span(host, 0x00083e82U, 0x00083e90U);
    const auto preserved = static_cast<std::uint16_t>(registers.data[2] & 0xf800U);
    set_word(registers.data[2], preserved);
    set_logic_word(registers, preserved);
    const auto lower = static_cast<std::uint16_t>(registers.data[0] & 0x0780U);
    set_word(registers.data[0], lower);
    set_logic_word(registers, lower);
    const auto result = static_cast<std::uint16_t>(preserved | lower);
    set_word(registers.data[2], result);
    set_logic_word(registers, result);
    host.write_memory_word(kRegion, (registers.address[6] + 0x32U) & kAddressMask,
                           result, kWordMask);
    set_logic_word(registers, result);
    prefetch(host, 0x00083e92U);
    return finish(host, registers, false);
}

} // namespace gain_ground::translated

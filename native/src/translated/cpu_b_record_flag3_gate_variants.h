#pragma once

#include "gain_ground/contract_types.h"

#include <array>
#include <cstdint>

namespace gain_ground::translated::record_flag3_detail {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

inline std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto word = host.read_memory_word(kRegion, address & ~1U, mask);
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

inline void write_byte(ExecutionHost &host, std::uint32_t address,
    std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    host.write_memory_word(kRegion, address & ~1U,
        static_cast<std::uint16_t>(value) << (odd ? 0U : 8U),
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U));
}

inline void set_test_flags(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x10U;
    if ((value & 0x80U) != 0U) flags |= 0x08U;
    if (value == 0U) flags |= 0x04U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x1fU) | flags);
}

inline void increment_byte(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t address)
{
    const auto old_value = read_byte(host, address);
    const auto value = static_cast<std::uint8_t>(old_value + 1U);
    write_byte(host, address, value);
    std::uint16_t flags = 0U;
    if (old_value == 0xffU) flags |= 0x11U;
    if (old_value == 0x7fU) flags |= 0x02U;
    if ((value & 0x80U) != 0U) flags |= 0x08U;
    if (value == 0U) flags |= 0x04U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x1fU) | flags);
}

inline FunctionResult call_child(FunctionContext &context,
    std::uint32_t id, std::uint32_t site, std::uint32_t target)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto continuation = site + 4U;
    registers.address[7] -= 4U;
    host.write_memory_word(kRegion, registers.address[7],
        static_cast<std::uint16_t>(continuation >> 16U), kWordMask);
    host.write_memory_word(kRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(continuation), kWordMask);
    registers.program_counter = target;
    return host.call_function(id, 1U, 0x72U, 2U, site, target, context);
}

inline FunctionResult return_from(FunctionContext &context)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto high = host.read_memory_word(kRegion,
        registers.address[7], kWordMask);
    const auto low = host.read_memory_word(kRegion,
        registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

template<std::uint32_t Entry, std::uint32_t CounterOffset,
    std::uint32_t NormalFirstId, bool ReverseInitialChildren>
FunctionResult execute(FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != Entry)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto record = registers.address[5];
    const auto flag = read_byte(host, record + 0x41U);
    const bool alternate = (flag & 0x08U) != 0U;
    if (alternate)
        registers.status = static_cast<std::uint16_t>(registers.status & ~0x04U);
    else
        registers.status = static_cast<std::uint16_t>(registers.status | 0x04U);

    const auto gate = read_byte(host, record + 0x3fU);
    set_test_flags(registers, gate);
    if (gate != 0U) {
        registers.address[6] = 0x3400U;
        increment_byte(host, registers, registers.address[6] + CounterOffset);
    }

    if (alternate) {
        constexpr std::array<std::uint32_t, 6> ids{
            334U, 352U, 353U, 357U, 360U, 365U};
        constexpr std::array<std::uint32_t, 6> targets{
            0x0001cef6U, 0x0001dbc4U, 0x0001dbf0U,
            0x0001e124U, 0x0001e304U, 0x0001ece4U};
        for (std::size_t index = 0; index < ids.size(); ++index) {
            const auto result = call_child(context, ids[index],
                Entry + 0x16U + static_cast<std::uint32_t>(index) * 4U,
                targets[index]);
            if (result.status != TranslationStatus::complete
                || result.control != 1U)
                return result;
        }
        return return_from(context);
    }

    constexpr std::array<std::uint32_t, 8> ids{
        NormalFirstId,
        ReverseInitialChildren ? 368U : 356U,
        ReverseInitialChildren ? 356U : 368U,
        352U, 353U, 355U, 357U, 360U};
    constexpr std::array<std::uint32_t, 8> targets{
        NormalFirstId == 331U ? 0x0001c37eU : 0x0001cc66U,
        ReverseInitialChildren ? 0x0001efdcU : 0x0001e0eeU,
        ReverseInitialChildren ? 0x0001e0eeU : 0x0001efdcU,
        0x0001dbc4U, 0x0001dbf0U, 0x0001de30U,
        0x0001e124U, 0x0001e304U};
    for (std::size_t index = 0; index < ids.size(); ++index) {
        const auto result = call_child(context, ids[index],
            Entry + 0x3eU + static_cast<std::uint32_t>(index) * 4U,
            targets[index]);
        if (result.status != TranslationStatus::complete
            || result.control != 1U)
            return result;
    }
    return return_from(context);
}
} // namespace gain_ground::translated::record_flag3_detail

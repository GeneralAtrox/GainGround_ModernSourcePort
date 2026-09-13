#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;

void set_move_word_flags(CpuRegisters &r, std::uint16_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_move_long_flags(CpuRegisters &r, std::uint32_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void clear_long(ExecutionHost &host, CpuRegisters &r, std::uint32_t address)
{
    (void)host.read_memory_word(kRegion, address, kMask);
    (void)host.read_memory_word(kRegion, address + 2U, kMask);
    host.write_memory_word(kRegion, address + 2U, 0U, kMask);
    host.write_memory_word(kRegion, address, 0U, kMask);
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | (r.status & 0x0010U) | 0x0004U);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kRegion, address, static_cast<std::uint16_t>(value >> 16U), kMask);
    host.write_memory_word(kRegion, address + 2U, static_cast<std::uint16_t>(value), kMask);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto high = host.read_memory_word(kRegion, r.address[7], kMask);
    const auto low = host.read_memory_word(kRegion, r.address[7] + 2U, kMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_clear_and_load_descriptor_pointers(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;
    clear_long(host, r, r.address[6] + 0x1eU);
    clear_long(host, r, r.address[6] + 0x26U);
    r.address[0] = static_cast<std::uint32_t>(
        0x00010804LL + static_cast<std::int16_t>(r.data[0]));

    for (const auto destination : {r.address[6] + 0x1eU, r.address[6] + 0x26U}) {
        const auto index = host.read_memory_word(kRegion, r.address[0], kMask);
        r.address[0] += 2U;
        r.data[0] = (r.data[0] & 0xffff0000U) | index;
        set_move_word_flags(r, index);
        if ((index & 0x8000U) == 0U) {
            const auto source = static_cast<std::uint32_t>(
                static_cast<std::int64_t>(r.address[1]) + static_cast<std::int16_t>(index));
            const auto value = read_long(host, source);
            write_long(host, destination, value);
            set_move_long_flags(r, value);
        }
    }
    const auto return_address = pop_return(host, r);
    r.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}
} // namespace gain_ground::translated

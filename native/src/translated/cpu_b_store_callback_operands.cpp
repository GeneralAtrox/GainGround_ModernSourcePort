#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

[[nodiscard]] constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kRegion, address,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, address + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(
        host.read_memory_word(kRegion, address, kWordMask)) << 16U)
        | host.read_memory_word(kRegion, address + 2U, kWordMask);
}

void set_move_long_flags(CpuRegisters &r, std::uint32_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void sbcd(CpuRegisters &r, std::uint8_t source, std::uint8_t destination,
    std::uint8_t &result)
{
    const int extend = (r.status & 0x0010U) != 0U ? 1 : 0;
    const int binary = static_cast<int>(destination)
        - static_cast<int>(source) - extend;
    int adjusted = binary;
    if (static_cast<int>(destination & 0x0fU)
            - static_cast<int>(source & 0x0fU) - extend < 0)
        adjusted -= 0x06;
    const bool borrow = binary < 0;
    if (borrow) adjusted -= 0x60;
    result = static_cast<std::uint8_t>(adjusted);

    std::uint16_t flags = 0U;
    if (borrow) flags |= 0x0011U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if ((r.status & 0x0004U) != 0U && result == 0U) flags |= 0x0004U;
    if ((binary & 0x80) != 0 && (result & 0x80U) == 0U) flags |= 0x0002U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void clear_long(ExecutionHost &host, CpuRegisters &r, std::uint32_t address)
{
    static_cast<void>(host.read_memory_word(kRegion, address, kWordMask));
    static_cast<void>(host.read_memory_word(kRegion, address + 2U, kWordMask));
    host.write_memory_word(kRegion, address + 2U, 0U, kWordMask);
    host.write_memory_word(kRegion, address, 0U, kWordMask);
    set_move_long_flags(r, 0U);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto target = read_long(host, r.address[7]);
    r.address[7] += 4U;
    return target;
}
} // namespace

FunctionResult cpu_b_store_callback_operands(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;

    r.address[0] = 0x00000826U;
    write_long(host, r.address[0], r.data[0]);
    set_move_long_flags(r, r.data[0]);
    r.address[0] += 4U;
    r.address[1] = r.address[0];
    write_long(host, r.address[1], r.data[1]);
    set_move_long_flags(r, r.data[1]);
    r.address[1] += 4U;
    r.status = static_cast<std::uint16_t>(r.status & ~0x001fU);

    for (unsigned index = 0U; index < 4U; ++index) {
        --r.address[0];
        --r.address[1];
        const auto source = read_byte(host, r.address[0]);
        const auto destination = read_byte(host, r.address[1]);
        std::uint8_t result{};
        sbcd(r, source, destination, result);
        write_byte(host, r.address[1], result);
    }

    if ((r.status & 0x0001U) != 0U)
        clear_long(host, r, r.address[1]);
    r.data[0] = read_long(host, r.address[1]);
    set_move_long_flags(r, r.data[0]);

    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated

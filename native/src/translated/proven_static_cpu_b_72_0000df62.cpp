#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
std::uint16_t word(std::uint32_t value) { return static_cast<std::uint16_t>(value); }
void put(std::uint32_t &destination, std::uint16_t value)
{
    destination = (destination & 0xffff0000U) | value;
}
void logic(CpuRegisters &r, std::uint16_t value)
{
    r.status = static_cast<std::uint16_t>((r.status & ~0x000fU)
        | (value == 0U ? 4U : 0U) | ((value & 0x8000U) != 0U ? 8U : 0U));
}
void subtract(CpuRegisters &r, std::uint16_t left, std::uint16_t right, bool compare)
{
    const auto result = word(left - right);
    const auto carry = left < right;
    const auto overflow = ((left ^ right) & (left ^ result) & 0x8000U) != 0U;
    const auto x = compare ? (r.status & 0x10U) : (carry ? 0x10U : 0U);
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | x
        | (carry ? 1U : 0U) | (overflow ? 2U : 0U)
        | (result == 0U ? 4U : 0U) | ((result & 0x8000U) != 0U ? 8U : 0U));
}
void add(CpuRegisters &r, std::uint16_t left, std::uint16_t right)
{
    const auto result = word(left + right);
    logic(r, result);
    r.status = static_cast<std::uint16_t>((r.status & ~0x13U)
        | (static_cast<std::uint32_t>(left) + right > 0xffffU ? 0x11U : 0U)
        | ((~(left ^ right) & (left ^ result) & 0x8000U) != 0U ? 2U : 0U));
}
bool greater(const CpuRegisters &r)
{
    return (r.status & 4U) == 0U && ((r.status >> 3U) & 1U) == ((r.status >> 1U) & 1U);
}
std::uint32_t signed_word(std::uint32_t value)
{
    const auto v = word(value);
    return (v & 0x8000U) != 0U ? (0xffff0000U | v) : v;
}
}

FunctionResult proven_static_cpu_b_72_0000df62(FunctionContext &context) noexcept
{
    auto &r = context.registers;
    if (context.host == nullptr || r.program_counter != 0xdf62U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    auto &host = *context.host;
    const auto read = [&](std::uint32_t address) {
        return host.read_memory_word(2U, address & 0x3ffffU, 0xffffU);
    };
    const auto write = [&](std::uint32_t address, std::uint16_t value) {
        host.write_memory_word(2U, address & 0x3ffffU, value, 0xffffU);
    };
    const auto increment_memory = [&](std::uint32_t address) {
        const auto before = read(address);
        add(r, before, 1U);
        write(address, word(before + 1U));
    };
    const auto decrement = [&](std::uint32_t &reg) {
        const auto before = word(reg);
        subtract(r, before, 1U, false);
        put(reg, word(before - 1U));
    };
    const auto clear_tile = [&]() {
        const auto offset = r.address[0] - 0x200000U;
        (void)host.read_memory_word(5U, offset, 0xffffU);
        host.write_memory_word(5U, offset, 0U, 0xffffU);
        logic(r, 0U);
    };
    // DF62: BPL uses N alone, including subtraction overflow cases.
    subtract(r, read(r.address[5] + 0x14U), 0x580U, true);
    if ((r.status & 8U) == 0U) goto finish;
    put(r.data[0], read(r.address[5] + 0x12U)); logic(r, word(r.data[0]));
    increment_memory(r.address[5] + 0x12U);
    put(r.data[1], word(r.data[0])); logic(r, word(r.data[1]));
    put(r.data[1], word(r.data[1] & 7U)); logic(r, word(r.data[1]));
    if ((r.status & 4U) == 0U) goto counter;
    {
        auto value = word(r.data[0]);
        bool carry = false;
        for (unsigned i = 0; i < 2U; ++i) {
            carry = (value & 1U) != 0U;
            value = word((value >> 1U) | (value & 0x8000U));
        }
        put(r.data[0], value); logic(r, value);
        r.status = static_cast<std::uint16_t>((r.status & ~0x11U) | (carry ? 0x11U : 0U));
        subtract(r, value, 0x7cU, false);
        value = word(value - 0x7cU); put(r.data[0], value);
        subtract(r, 0U, value, false);
        put(r.data[0], word(0U - value));
    }
    put(r.data[0], word(r.data[0] & 0x7fU)); logic(r, word(r.data[0]));
    r.address[0] = 0x200780U;
    r.address[0] += signed_word(r.data[0]);
    put(r.data[0], read(r.address[5] + 0x14U)); logic(r, word(r.data[0]));
    add(r, word(r.data[0]), word(r.data[0])); put(r.data[0], word(r.data[0] * 2U));
    r.address[1] = 0xe1ceU;
    r.address[1] += signed_word(r.data[0]);
    put(r.data[2], 0x20U); logic(r, word(r.data[2]));
next_word:
    increment_memory(r.address[5] + 0x14U);
    put(r.data[0], read(r.address[1])); r.address[1] += 2U; logic(r, word(r.data[0]));
    if ((r.status & 4U) != 0U) goto zero_run;
    subtract(r, word(r.data[0]), 0x14U, true);
    if (!greater(r)) goto short_run;
    host.write_memory_word(5U, r.address[0] - 0x200000U, word(r.data[0]), 0xffffU);
    logic(r, word(r.data[0]));
    r.address[0] += 0x80U;
    decrement(r.data[2]);
    if (greater(r)) goto next_word;
    goto counter;
zero_run:
    clear_tile();
    r.address[0] += 0x80U;
    decrement(r.data[2]);
    if (greater(r)) goto zero_run;
    goto counter;
short_run:
    subtract(r, word(r.data[2]), word(r.data[0]), false);
    put(r.data[2], word(r.data[2] - r.data[0]));
short_clear:
    clear_tile();
    r.address[0] += 0x80U;
    decrement(r.data[0]);
    if (greater(r)) goto short_clear;
    goto next_word;
counter:
    r.address[0] = 0x50eU;
    increment_memory(r.address[0]);
    {
        const auto value = word(read(r.address[0]) & 0xfffU);
        logic(r, value); write(r.address[0], value);
    }
finish:
    const auto high = read(r.address[7]);
    const auto low = read(r.address[7] + 2U);
    r.address[7] += 4U;
    r.program_counter = (static_cast<std::uint32_t>(high) << 16U) | low;
    return FunctionResult::complete(1U, r.program_counter);
}
} // namespace gain_ground::translated

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
void shift(CpuRegisters &r, std::uint32_t &reg, unsigned count, bool left)
{
    auto value = word(reg);
    bool carry = false;
    for (unsigned i = 0; i < count; ++i) {
        carry = left ? (value & 0x8000U) != 0U : (value & 1U) != 0U;
        value = left ? word(value << 1U) : word(value >> 1U);
    }
    put(reg, value);
    logic(r, value);
    r.status = static_cast<std::uint16_t>((r.status & ~0x11U) | (carry ? 0x11U : 0U));
}
void increment(CpuRegisters &r)
{
    const auto before = word(r.data[0]);
    const auto value = word(before + 1U);
    put(r.data[0], value);
    logic(r, value);
    r.status = static_cast<std::uint16_t>((r.status & ~0x13U)
        | (before == 0xffffU ? 0x11U : 0U) | (before == 0x7fffU ? 2U : 0U));
}
}

FunctionResult proven_static_cpu_b_72_0000df10(FunctionContext &context) noexcept
{
    auto &r = context.registers;
    if (context.host == nullptr || r.program_counter != 0xdf10U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    auto &host = *context.host;
    const auto read = [&](std::uint32_t address) {
        return host.read_memory_word(2U, address & 0x3ffffU, 0xffffU);
    };
    const auto store = [&]() {
        host.write_memory_word(5U, r.address[0] - 0x200000U, word(r.data[0]), 0xffffU);
        logic(r, word(r.data[0]));
    };
    // DF10-DF1E: both branches test N, not the signed-comparison N xor V.
    put(r.data[0], read(r.address[5] + 0x10U));
    logic(r, word(r.data[0]));
    const auto input = word(r.data[0]);
    subtract(r, input, 30U, false);
    put(r.data[0], word(input - 30U));
    if ((r.status & 8U) == 0U) {
        subtract(r, word(r.data[0]), 0x200U, true);
        if ((r.status & 8U) != 0U) {
            put(r.data[1], word(r.data[0])); logic(r, word(r.data[1]));
            put(r.data[1], word(r.data[1] & 0xc0U)); logic(r, word(r.data[1]));
            r.address[0] = 0x200420U;
            shift(r, r.data[1], 5U, false);
            r.address[0] -= word(r.data[1]);
            r.address[0] -= word(r.data[1]);
            r.address[0] -= word(r.data[1]);
            put(r.data[1], word(r.data[0])); logic(r, word(r.data[1]));
            put(r.data[1], word(r.data[1] & 0x3cU)); logic(r, word(r.data[1]));
            shift(r, r.data[1], 6U, true);
            r.address[0] += word(r.data[1]);
            put(r.data[0], word(r.data[0] & 0x1fcU)); logic(r, word(r.data[0]));
            shift(r, r.data[0], 1U, false);
            r.address[1] = 0xe0aeU;
            put(r.data[0], read(r.address[1] + word(r.data[0]))); logic(r, word(r.data[0]));
            store(); r.address[0] += 2U;
            increment(r); store();
            r.address[0] += 0x7eU;
            increment(r); store(); r.address[0] += 2U;
            increment(r); store();
        }
    }
    const auto high = read(r.address[7]);
    const auto low = read(r.address[7] + 2U);
    r.address[7] += 4U;
    r.program_counter = (static_cast<std::uint32_t>(high) << 16U) | low;
    return FunctionResult::complete(1U, r.program_counter);
}
} // namespace gain_ground::translated

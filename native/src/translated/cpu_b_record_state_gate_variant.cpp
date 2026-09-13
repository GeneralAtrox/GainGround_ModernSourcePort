#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
struct ByteLocation { std::uint32_t address; std::uint16_t mask; unsigned shift; };

[[nodiscard]] ByteLocation locate_byte(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}
[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{ return host.read_memory_word(kRegion, address & 0x00ffffffU, kWordMask); }
void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{ host.write_memory_word(kRegion, address & 0x00ffffffU, value, kWordMask); }
[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto byte = locate_byte(address & 0x00ffffffU);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, byte.address, byte.mask) >> byte.shift);
}
void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto byte = locate_byte(address & 0x00ffffffU);
    host.write_memory_word(kRegion, byte.address,
        static_cast<std::uint16_t>(value) << byte.shift, byte.mask);
}
void logic_byte(CpuRegisters &r, std::uint8_t value)
{
    std::uint16_t flags = r.status & 0x10U;
    if ((value & 0x80U) != 0U) flags |= 8U;
    if (value == 0U) flags |= 4U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | flags);
}
void add_word(CpuRegisters &r, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 8U;
    if (result == 0U) flags |= 4U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 2U;
    if (static_cast<std::uint32_t>(left) + right > 0xffffU) flags |= 0x11U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | flags);
}
void change_bit(ExecutionHost &host, CpuRegisters &r,
    std::uint32_t address, unsigned bit, bool set)
{
    const auto before = read_byte(host, address);
    const auto mask = static_cast<std::uint8_t>(1U << bit);
    write_byte(host, address, static_cast<std::uint8_t>(
        set ? before | mask : before & static_cast<std::uint8_t>(~mask)));
    r.status = static_cast<std::uint16_t>((r.status & ~4U)
        | (((before & mask) == 0U) ? 4U : 0U));
}
[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto target = (static_cast<std::uint32_t>(read_word(host, r.address[7])) << 16U)
        | read_word(host, r.address[7] + 2U);
    r.address[7] += 4U;
    return target;
}
} // namespace

FunctionResult cpu_b_record_state_gate_variant(FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x0001bdc0U)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];
    const auto finish = [&]() -> FunctionResult {
        const auto target = pop_return(host, r);
        r.program_counter = target;
        return FunctionResult::complete(1U, target);
    };
    const auto tail = [&](std::uint32_t id, std::uint32_t callsite,
        std::uint32_t target) -> FunctionResult {
        r.program_counter = target;
        return host.call_function(id, 1U, 0x72U, 1U,
            callsite, target, context);
    };

    const auto reload = read_byte(host, base + 0x3fU);
    logic_byte(r, reload);
    if (reload != 0U)
        return tail(550U, 0x0001bdc6U, 0x0001d9daU);
    const auto phase = read_byte(host, base + 0x3eU);
    logic_byte(r, phase);
    if (phase != 0U)
        return tail(551U, 0x0001bdd0U, 0x0001d9eaU);

    const auto counter = read_word(host, base + 0x54U);
    const auto incremented = static_cast<std::uint16_t>(counter + 1U);
    write_word(host, base + 0x54U, incremented);
    add_word(r, counter, 1U, incremented);
    change_bit(host, r, base + 0x40U, 1U, false);
    return finish();
}

} // namespace gain_ground::translated

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
[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}
void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
}
void logic_byte(CpuRegisters &r, std::uint8_t value)
{
    std::uint16_t flags = r.status & 0x10U;
    if ((value & 0x80U) != 0U) flags |= 8U;
    if (value == 0U) flags |= 4U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | flags);
}
void logic_word(CpuRegisters &r, std::uint16_t value)
{
    std::uint16_t flags = r.status & 0x10U;
    if ((value & 0x8000U) != 0U) flags |= 8U;
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
void sub_byte(CpuRegisters &r, std::uint8_t left,
    std::uint8_t right, std::uint8_t result)
{
    std::uint16_t flags{};
    if ((result & 0x80U) != 0U) flags |= 8U;
    if (result == 0U) flags |= 4U;
    if (((left ^ right) & (left ^ result) & 0x80U) != 0U) flags |= 2U;
    if (left < right) flags |= 0x11U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | flags);
}
[[nodiscard]] bool test_bit(ExecutionHost &host, CpuRegisters &r,
    std::uint32_t address, unsigned bit)
{
    const auto value = read_byte(host, address);
    const bool set = (value & (1U << bit)) != 0U;
    r.status = static_cast<std::uint16_t>((r.status & ~4U) | (set ? 0U : 4U));
    return set;
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
[[nodiscard]] FunctionResult call(FunctionContext &context, std::uint32_t id,
    std::uint32_t site, std::uint32_t target, std::uint32_t continuation)
{
    context.registers.address[7] -= 4U;
    write_long(*context.host, context.registers.address[7], continuation);
    context.registers.program_counter = target;
    return context.host->call_function(id, 1U, 0x72U, 2U, site, target, context);
}
} // namespace

FunctionResult cpu_b_record_state_phase_gate(FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x0001bf1cU)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];
    const auto finish = [&]() -> FunctionResult {
        const auto target = read_long(host, r.address[7]);
        r.address[7] += 4U;
        r.program_counter = target;
        return FunctionResult::complete(1U, target);
    };
    const auto complete = [](const FunctionResult &result) {
        return result.status == TranslationStatus::complete && result.control == 1U;
    };
    const auto finish_common = [&]() -> FunctionResult {
        const auto value = read_byte(host, base + 0x5fU);
        write_byte(host, base + 0x5eU, value);
        logic_byte(r, value);
        auto child = call(context, 364U,
            0x0001bf8aU, 0x0001ec00U, 0x0001bf90U);
        if (!complete(child)) return child;
        const auto selected = static_cast<std::uint16_t>(r.data[1]);
        write_word(host, base + 0x5cU, selected);
        logic_word(r, selected);
        write_word(host, base + 0x3aU, selected);
        logic_word(r, selected);
        change_bit(host, r, base + 0x40U, 0U, false);
        change_bit(host, r, base + 0x40U, 3U, true);
        change_bit(host, r, base + 0x40U, 2U, true);
        return finish();
    };
    const auto finish_clear4 = [&]() -> FunctionResult {
        change_bit(host, r, base + 0x41U, 4U, false);
        return finish_common();
    };
    const auto finish_clear1 = [&]() -> FunctionResult {
        change_bit(host, r, base + 0x40U, 1U, false);
        return finish_clear4();
    };

    const auto reload = read_byte(host, base + 0x3fU);
    logic_byte(r, reload);
    if (reload != 0U) {
        write_long(host, base + 2U, 0x0001ed98U);
        write_word(host, base + 0x54U, 0U);
        logic_word(r, 0U);
        return finish();
    }
    const auto phase = read_byte(host, base + 0x3eU);
    logic_byte(r, phase);
    if (phase != 0U) {
        write_byte(host, base + 0x54U, 1U);
        const auto operand = read_byte(host, base + 0x3eU);
        const auto decremented = static_cast<std::uint8_t>(operand - 1U);
        write_byte(host, base + 0x3eU, decremented);
        sub_byte(r, operand, 1U, decremented);
        const auto reread = read_byte(host, base + 0x3eU);
        const auto even = static_cast<std::uint8_t>(reread & 0xfeU);
        write_byte(host, base + 0x3eU, even);
        logic_byte(r, even);
        change_bit(host, r, base + 0x40U, 1U, true);
        change_bit(host, r, base + 0x40U, 3U, true);
        const auto cursor = read_word(host, base + 0x5cU);
        const auto advanced = static_cast<std::uint16_t>(cursor + 0x80U);
        write_word(host, base + 0x5cU, advanced);
        add_word(r, cursor, 0x80U, advanced);
        const auto mask_operand = read_word(host, base + 0x5cU);
        const auto masked = static_cast<std::uint16_t>(mask_operand & 0x0780U);
        write_word(host, base + 0x5cU, masked);
        logic_word(r, masked);
        write_byte(host, base + 0x5eU, 9U);
        logic_byte(r, 9U);
        return finish();
    }

    const auto counter = read_word(host, base + 0x54U);
    const auto incremented = static_cast<std::uint16_t>(counter + 1U);
    write_word(host, base + 0x54U, incremented);
    add_word(r, counter, 1U, incremented);
    if (test_bit(host, r, base + 0x40U, 7U)) return finish_clear4();

    auto child = call(context, 340U,
        0x0001bf3cU, 0x0001d5b4U, 0x0001bf40U);
    if (!complete(child)) return child;
    if (test_bit(host, r, base + 0x41U, 5U)) return finish_clear1();

    auto value = read_word(host, base + 0x54U);
    r.data[0] = (r.data[0] & 0xffff0000U) | value;
    logic_word(r, value);
    value = static_cast<std::uint16_t>(value & 0x001fU);
    r.data[0] = (r.data[0] & 0xffff0000U) | value;
    logic_word(r, value);
    if (value == 0U) {
        child = call(context, 347U,
            0x0001bf52U, 0x0001da58U, 0x0001bf56U);
        if (!complete(child)) return child;
        return finish_common();
    }
    if (test_bit(host, r, base + 0x40U, 5U)) {
        write_byte(host, base + 0x60U, 0U);
        logic_byte(r, 0U);
        child = call(context, 347U,
            0x0001bfc6U, 0x0001da58U, 0x0001bfcaU);
        if (!complete(child)) return child;
        return finish_clear1();
    }
    if (test_bit(host, r, base + 0x40U, 6U)) return finish_clear1();
    change_bit(host, r, base + 0x40U, 7U, false);
    if (!test_bit(host, r, base + 0x41U, 4U)) return finish();

    change_bit(host, r, base + 0x40U, 1U, false);
    const auto reference = read_word(host, base + 0x3aU);
    write_word(host, base + 0x5cU, reference);
    logic_word(r, reference);
    return finish_clear4();
}

} // namespace gain_ground::translated

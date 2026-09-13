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
void compare_word(CpuRegisters &r, std::uint16_t left, std::uint16_t right)
{
    const auto result = static_cast<std::uint16_t>(left - right);
    std::uint16_t flags = r.status & 0x10U;
    if ((result & 0x8000U) != 0U) flags |= 8U;
    if (result == 0U) flags |= 4U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 2U;
    if (left < right) flags |= 1U;
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
void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    write_long(host, r.address[7], value);
}
[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto target = read_long(host, r.address[7]);
    r.address[7] += 4U;
    return target;
}
[[nodiscard]] FunctionResult call(FunctionContext &context, std::uint32_t id,
    std::uint32_t site, std::uint32_t target, std::uint32_t continuation)
{
    push_return(*context.host, context.registers, continuation);
    context.registers.program_counter = target;
    return context.host->call_function(id, 1U, 0x72U, 2U, site, target, context);
}
} // namespace

FunctionResult cpu_b_record_state_phase_gate_2(FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x0001c108U)
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
    const auto complete = [](const FunctionResult &result) {
        return result.status == TranslationStatus::complete && result.control == 1U;
    };
    const auto finish_core_flags = [&]() -> FunctionResult {
        change_bit(host, r, base + 0x40U, 0U, false);
        change_bit(host, r, base + 0x40U, 3U, true);
        change_bit(host, r, base + 0x40U, 2U, true);
        return finish();
    };
    const auto finish_flags = [&]() -> FunctionResult {
        change_bit(host, r, base + 0x41U, 4U, false);
        return finish_core_flags();
    };

    const auto reload = read_byte(host, base + 0x3fU);
    logic_byte(r, reload);
    if (reload != 0U) {
        r.program_counter = 0x0001d9daU;
        return host.call_function(550U, 1U, 0x72U, 1U,
            0x0001c10eU, 0x0001d9daU, context);
    }
    const auto phase = read_byte(host, base + 0x3eU);
    logic_byte(r, phase);
    if (phase != 0U) {
        r.program_counter = 0x0001d9eaU;
        return host.call_function(551U, 1U, 0x72U, 1U,
            0x0001c118U, 0x0001d9eaU, context);
    }

    const auto counter = read_word(host, base + 0x54U);
    const auto incremented = static_cast<std::uint16_t>(counter + 1U);
    write_word(host, base + 0x54U, incremented);
    add_word(r, counter, 1U, incremented);
    if (test_bit(host, r, base + 0x41U, 1U)) {
        change_bit(host, r, base + 0x40U, 1U, true);
        return finish();
    }

    auto child = call(context, 335U,
        0x0001c130U, 0x0001d142U, 0x0001c134U);
    if (!complete(child)) return child;
    if (test_bit(host, r, base + 0x41U, 5U)) {
        r.address[0] = read_long(host, base + 0x66U);
        const auto timer = read_word(host, r.address[0] + 0x10U);
        write_word(host, base + 0x74U, timer);
        logic_word(r, timer);
        change_bit(host, r, base + 0x41U, 1U, false);
        change_bit(host, r, base + 0x41U, 0U, true);
        change_bit(host, r, base + 0x41U, 2U, true);
        return finish_flags();
    }
    if (test_bit(host, r, base + 0x40U, 7U)) return finish_flags();

    auto value = read_word(host, base + 0x54U);
    r.data[0] = (r.data[0] & 0xffff0000U) | value;
    logic_word(r, value);
    value = static_cast<std::uint16_t>(value & 0x001fU);
    r.data[0] = (r.data[0] & 0xffff0000U) | value;
    logic_word(r, value);
    if (value == 0U) {
        child = call(context, 345U,
            0x0001c150U, 0x0001da1aU, 0x0001c154U);
        if (!complete(child)) return child;
        change_bit(host, r, base + 0x40U, 1U, false);
        return finish_core_flags();
    }
    if (test_bit(host, r, base + 0x40U, 5U)
        || test_bit(host, r, base + 0x40U, 6U)) {
        change_bit(host, r, base + 0x40U, 1U, false);
        return finish_flags();
    }
    if (!test_bit(host, r, base + 0x41U, 4U)) return finish();

    change_bit(host, r, base + 0x40U, 1U, false);
    value = read_word(host, base + 0x5cU);
    r.data[0] = (r.data[0] & 0xffff0000U) | value;
    logic_word(r, value);
    const auto reference = read_word(host, base + 0x3aU);
    compare_word(r, value, reference);
    if (value == reference) {
        change_bit(host, r, base + 0x41U, 4U, false);
        return finish();
    }
    const auto byte = read_byte(host, base + 0x39U);
    r.data[1] = (r.data[1] & 0xffffff00U) | byte;
    logic_byte(r, byte);
    const auto signed_word = static_cast<std::uint16_t>(
        static_cast<std::int16_t>(static_cast<std::int8_t>(byte)));
    r.data[1] = (r.data[1] & 0xffff0000U) | signed_word;
    logic_word(r, signed_word);
    const auto sum = static_cast<std::uint16_t>(value + signed_word);
    r.data[0] = (r.data[0] & 0xffff0000U) | sum;
    add_word(r, value, signed_word, sum);
    const auto masked = static_cast<std::uint16_t>(sum & 0x07ffU);
    r.data[0] = (r.data[0] & 0xffff0000U) | masked;
    logic_word(r, masked);
    write_word(host, base + 0x5cU, masked);
    logic_word(r, masked);
    return finish_core_flags();
}

} // namespace gain_ground::translated

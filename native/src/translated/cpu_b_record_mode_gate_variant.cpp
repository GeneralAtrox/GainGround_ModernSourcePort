#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
struct ByteLocation { std::uint32_t address; std::uint16_t mask; unsigned shift; };

ByteLocation locate_byte(std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}
std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto x = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(kRegion, x.address, x.mask) >> x.shift);
}
void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto x = locate_byte(address);
    host.write_memory_word(kRegion, x.address,
        static_cast<std::uint16_t>(value) << x.shift, x.mask);
}
std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address, kWordMask);
}
void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address, value, kWordMask);
}
std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}
void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
}
void logic(CpuRegisters &r, std::uint32_t value, std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x10U;
    if ((value & sign) != 0U) flags |= 8U;
    if ((value & mask) == 0U) flags |= 4U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | flags);
}
void sub_word_flags(CpuRegisters &r,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 8U;
    if (result == 0U) flags |= 4U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 2U;
    if (left < right) flags |= 0x11U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | flags);
}
void bit_zero(CpuRegisters &r, bool zero)
{
    r.status = static_cast<std::uint16_t>((r.status & ~4U) | (zero ? 4U : 0U));
}
void change_bit(ExecutionHost &host, CpuRegisters &r,
    std::uint32_t address, unsigned bit, bool set)
{
    const auto before = read_byte(host, address);
    const auto mask = static_cast<std::uint8_t>(1U << bit);
    write_byte(host, address, static_cast<std::uint8_t>(
        set ? before | mask : before & static_cast<std::uint8_t>(~mask)));
    bit_zero(r, (before & mask) == 0U);
}
void push(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    write_long(host, r.address[7], value);
}
std::uint32_t pop(ExecutionHost &host, CpuRegisters &r)
{
    const auto value = read_long(host, r.address[7]);
    r.address[7] += 4U;
    return value;
}
FunctionResult call(FunctionContext &context, std::uint32_t id,
    std::uint32_t site, std::uint32_t target, std::uint32_t continuation)
{
    push(*context.host, context.registers, continuation);
    context.registers.program_counter = target;
    return context.host->call_function(id, 1U, 0x72U, 2U, site, target, context);
}
} // namespace

FunctionResult cpu_b_record_mode_gate_variant(FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x0001f1fcU)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];
    const auto finish = [&]() {
        const auto target = pop(host, r);
        r.program_counter = target;
        return FunctionResult::complete(1U, target);
    };
    const auto complete = [](const FunctionResult &x) {
        return x.status == TranslationStatus::complete && x.control == 1U;
    };

    auto byte = read_byte(host, base + 0x3eU);
    logic(r, byte, 0x80U, 0xffU);
    if (byte != 0U) return finish();

    byte = read_byte(host, base + 0x41U);
    bit_zero(r, (byte & 2U) == 0U);
    if ((byte & 2U) != 0U) {
        r.program_counter = 0x0001f02aU;
        return host.call_function(556U, 1U, 0x72U, 1U,
            0x0001f20aU, 0x0001f02aU, context);
    }

    const auto timer = read_word(host, base + 0x74U);
    logic(r, timer, 0x8000U, 0xffffU);
    if (timer != 0U) {
        static_cast<void>(read_word(host, base + 0x74U));
        const auto next = static_cast<std::uint16_t>(timer - 1U);
        write_word(host, base + 0x74U, next);
        sub_word_flags(r, timer, 1U, next);
        return finish();
    }

    auto result = call(context, 374U,
        0x0001f21aU, 0x0001fae6U, 0x0001f21eU);
    if (!complete(result)) return result;
    if ((r.status & 1U) == 0U) return finish();

    r.address[0] = read_long(host, base + 0x66U);
    byte = read_byte(host, r.address[0] + 0x12U);
    write_byte(host, base + 0x72U, byte);
    logic(r, byte, 0x80U, 0xffU);
    change_bit(host, r, base + 0x41U, 1U, true);
    change_bit(host, r, base + 0x41U, 0U, false);
    change_bit(host, r, base + 0x41U, 2U, true);
    write_word(host, base + 0x74U, 6U);
    logic(r, 6U, 0x8000U, 0xffffU);
    result = call(context, 347U,
        0x0001f244U, 0x0001da58U, 0x0001f248U);
    if (!complete(result)) return result;
    return finish();
}

} // namespace gain_ground::translated

#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{ return host.read_memory_word(kRegion, address & kAddressMask, kWordMask); }

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{ return (static_cast<std::uint32_t>(read_word(host, address)) << 16U) | read_word(host, address + 2U); }

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{ host.write_memory_word(kRegion, address & kAddressMask, value, kWordMask); }

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{ const auto value = host.read_memory_word(kRegion, (address & kAddressMask) & ~1U, (address & 1U) ? 0x00ffU : 0xff00U); return static_cast<std::uint8_t>((address & 1U) ? value : value >> 8U); }

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{ const bool odd = (address & 1U) != 0U; host.write_memory_word(kRegion, (address & kAddressMask) & ~1U, static_cast<std::uint16_t>(value) << (odd ? 0U : 8U), odd ? 0x00ffU : 0xff00U); }

void set_move_word_flags(CpuRegisters &r, std::uint16_t value)
{ std::uint16_t flags = r.status & 0x0010U; if (value & 0x8000U) flags |= 0x0008U; if (value == 0U) flags |= 0x0004U; r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags); }

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{ const auto target = read_long(host, r.address[7]); r.address[7] += 4U; return target; }
}

FunctionResult cpu_b_initialize_record_from_descriptor_compact(FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x00010efeU)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host; auto &r = context.registers;
    const auto a5 = r.address[5]; const auto a6 = r.address[6];
    r.address[0] = read_long(host, r.address[3] + 0x10U);
    auto high = read_word(host, r.address[0]);
    auto low = read_word(host, r.address[0] + 2U);
    write_word(host, a6 + 2U, high); write_word(host, a6 + 4U, low);
    r.address[0] += 4U;
    const auto left = read_word(host, a5 + 0x66U); const auto right = read_word(host, r.address[0]); r.address[0] += 2U;
    const auto sum = static_cast<std::uint16_t>(left + right); r.data[0] = (r.data[0] & 0xffff0000U) | sum; write_word(host, a6 + 8U, sum);
    r.address[0] += 1U;
    write_byte(host, a6 + 0x0bU, read_byte(host, r.address[0]++));
    high = read_word(host, r.address[0]);
    low = read_word(host, r.address[0] + 2U);
    write_word(host, a6 + 0x22U, high); write_word(host, a6 + 0x24U, low);
    r.address[0] += 4U;
    write_word(host, a6 + 0x3aU, read_word(host, r.address[0])); r.address[0] += 2U;
    write_word(host, a6 + 0x36U, static_cast<std::uint16_t>(a5));
    (void)read_byte(host, a6 + 0x3fU); write_byte(host, a6 + 0x3fU, 0U);
    write_word(host, a6 + 6U, read_word(host, r.address[0])); r.address[0] += 2U;
    const auto final_value = read_word(host, r.address[0]); r.address[0] += 2U; write_word(host, a6, final_value); set_move_word_flags(r, final_value);
    const auto target = pop_return(host, r); r.program_counter = target; return FunctionResult::complete(1U, target);
}
}

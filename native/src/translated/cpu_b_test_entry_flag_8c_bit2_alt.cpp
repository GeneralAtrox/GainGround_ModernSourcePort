#include "gain_ground/contract_types.h"
#include "gground_functions.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address, kMask);
}

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto value = host.read_memory_word(kRegion, address & ~1U,
        odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? value : value >> 8U);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address, value, kMask);
}

void set_logic_word(CpuRegisters &r, std::uint16_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_logic_long(CpuRegisters &r, std::uint32_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

bool test_bit_two(CpuRegisters &r, std::uint8_t value)
{
    const bool set = (value & 0x04U) != 0U;
    if (set) r.status = static_cast<std::uint16_t>(r.status & ~0x0004U);
    else r.status = static_cast<std::uint16_t>(r.status | 0x0004U);
    return set;
}

void push_long(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    write_word(host, r.address[7], static_cast<std::uint16_t>(value >> 16U));
    write_word(host, r.address[7] + 2U, static_cast<std::uint16_t>(value));
}

std::uint32_t pop_long(ExecutionHost &host, CpuRegisters &r)
{
    const auto value = read_long(host, r.address[7]);
    r.address[7] += 4U;
    return value;
}

FunctionResult finish(FunctionContext &context)
{
    auto &r = context.registers;
    const auto target = pop_long(*context.host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

std::uint32_t first_callback_id(std::uint32_t target)
{
    switch (target) {
    case 0x00010f2eU: return 234U;
    case 0x00010f68U: return 426U;
    default: return generated::kFunctions.size();
    }
}

std::uint32_t second_callback_id(std::uint32_t target)
{
    switch (target) {
    case 0x00010c3eU: return 221U;
    case 0x000111ecU: return 431U;
    default: return generated::kFunctions.size();
    }
}

std::uint32_t third_callback_id(std::uint32_t target)
{
    switch (target) {
    case 0x00010c8aU: return 222U;
    case 0x0001130eU: return 242U;
    case 0x0001131cU: return 243U;
    default: return generated::kFunctions.size();
    }
}

FunctionResult call_callback(FunctionContext &context, std::uint32_t id,
    std::uint32_t callsite, std::uint32_t continuation, std::uint32_t target)
{
    if (id >= generated::kFunctions.size())
        return {TranslationStatus::contract_violation, 0U, target};
    auto &r = context.registers;
    push_long(*context.host, r, continuation);
    r.program_counter = target;
    auto child = context.host->call_function(id, 1U, 0x72U, 2U,
        callsite, target, context);
    if (target == 0x0001130eU && child.status == TranslationStatus::complete
        && child.control == 3U && r.program_counter == 0x0001131cU)
        child = cpu_b_return_record_helper(context);
    return child;
}
} // namespace

FunctionResult cpu_b_test_entry_flag_8c_bit2_alt(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;

    if (!test_bit_two(r, read_byte(host, r.address[4] + 0x8cU)))
        return finish(context);

    auto target = read_long(host, r.address[3] + 0x0cU);
    r.address[0] = target;
    auto child = call_callback(context, first_callback_id(target),
        0x00010da8U, 0x00010daaU, target);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    target = read_long(host, r.address[3] + 0x14U);
    r.address[0] = target;
    child = call_callback(context, second_callback_id(target),
        0x00010daeU, 0x00010db0U, target);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    target = read_long(host, r.address[3] + 0x1cU);
    r.address[0] = target;
    child = call_callback(context, third_callback_id(target),
        0x00010db4U, 0x00010db6U, target);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    const auto parameter = read_word(host, r.address[3] + 0x24U);
    r.data[0] = (r.data[0] & 0xffff0000U) | parameter;
    set_logic_word(r, parameter);
    r.address[7] -= 2U;
    write_word(host, r.address[7], static_cast<std::uint16_t>(r.data[7]));
    set_logic_word(r, static_cast<std::uint16_t>(r.data[7]));
    r.address[7] -= 4U;
    // 68000 predecrement longword stores issue the low word first.
    write_word(host, r.address[7] + 2U, static_cast<std::uint16_t>(r.address[4]));
    write_word(host, r.address[7], static_cast<std::uint16_t>(r.address[4] >> 16U));
    set_logic_long(r, r.address[4]);

    push_long(host, r, 0x00010dc4U);
    r.program_counter = 0x00016ff8U;
    child = host.call_function(308U, 1U, 0x72U, 2U,
        0x00010dbeU, 0x00016ff8U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    r.address[4] = pop_long(host, r);
    const auto saved_d7 = read_word(host, r.address[7]);
    r.address[7] += 2U;
    r.data[7] = (r.data[7] & 0xffff0000U) | saved_d7;
    set_logic_word(r, saved_d7);
    return finish(context);
}

} // namespace gain_ground::translated

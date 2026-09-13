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

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address, value, kMask);
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
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

void push_long(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    write_long(host, r.address[7], value);
}

std::uint32_t pop_long(ExecutionHost &host, CpuRegisters &r)
{
    const auto value = read_long(host, r.address[7]);
    r.address[7] += 4U;
    return value;
}

std::uint32_t first_callback_id(std::uint32_t target)
{
    switch (target) {
    case 0x00010ec4U: return 233U;
    case 0x00010efeU: return 425U;
    case 0x00010f2eU: return 234U;
    case 0x00010f68U: return 426U;
    case 0x00010fc0U: return 235U;
    case 0x00010ff0U: return 236U;
    case 0x00011034U: return 427U;
    case 0x0001106eU: return 237U;
    case 0x000110aeU: return 428U;
    case 0x000110f2U: return 429U;
    case 0x00011154U: return 430U;
    default: return generated::kFunctions.size();
    }
}

std::uint32_t second_callback_id(std::uint32_t target)
{
    switch (target) {
    case 0x00010c3eU: return 221U;
    case 0x000111a4U: return 238U;
    case 0x000111ecU: return 431U;
    case 0x00011246U: return 239U;
    case 0x0001129eU: return 432U;
    default: return generated::kFunctions.size();
    }
}

std::uint32_t third_callback_id(std::uint32_t target)
{
    switch (target) {
    case 0x00010c8aU: return 222U;
    case 0x000112faU: return 240U;
    case 0x00011300U: return 241U;
    case 0x0001130eU: return 242U;
    case 0x0001131cU: return 243U;
    case 0x0001131eU: return 244U;
    case 0x00011340U: return 433U;
    case 0x00011358U: return 434U;
    case 0x0001136cU: return 435U;
    default: return generated::kFunctions.size();
    }
}

FunctionResult call_callback(FunctionContext &context, std::uint32_t id,
    std::uint32_t callsite, std::uint32_t continuation, std::uint32_t target)
{
    if (id >= generated::kFunctions.size())
        return {TranslationStatus::contract_violation, 0U, target};
    auto &host = *context.host;
    auto &r = context.registers;
    push_long(host, r, continuation);
    r.program_counter = target;
    auto child = host.call_function(id, 1U, 0x72U, 2U,
        callsite, target, context);
    if (target == 0x000112faU && child.status == TranslationStatus::complete
        && child.control == 3U && r.program_counter == 0x00011300U)
        child = cpu_b_set_constant_18(context);
    if (target == 0x0001131eU && child.status == TranslationStatus::complete
        && child.control == 3U && r.program_counter == 0x00011332U)
        child = cpu_b_set_entry_flag_bit7(context);
    return child;
}
} // namespace

FunctionResult cpu_b_load_entry_pointer_0c(FunctionContext &context) noexcept
{
    if (!context.host) return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;
    // Original F230 instruction/return entries: a resumed spawn must not replay
    // an initializer, positional callback, sound command or stack save.
    for (;;) {
        switch (r.program_counter) {
        case 0x10e60U: case 0x10e66U: case 0x10e6cU: {
            const auto pc = r.program_counter;
            const auto offset = pc == 0x10e60U ? 0x0cU : pc == 0x10e66U ? 0x14U : 0x1cU;
            r.address[0] = read_long(host, r.address[3] + offset);
            r.program_counter = pc + 4U; break;
        }
        case 0x10e64U: case 0x10e6aU: case 0x10e70U: {
            const auto site = r.program_counter;
            const auto target = r.address[0];
            const auto id = site == 0x10e64U ? first_callback_id(target)
                : site == 0x10e6aU ? second_callback_id(target) : third_callback_id(target);
            const auto child = call_callback(context, id, site, site + 2U, target);
            if (child.status != TranslationStatus::complete || child.control != 1U ||
                r.program_counter != site + 2U) return child;
            break;
        }
        case 0x10e72U: {
            const auto parameter = read_word(host, r.address[3] + 0x24U);
            r.data[0] = (r.data[0] & 0xffff0000U) | parameter;
            set_logic_word(r, parameter); r.program_counter = 0x10e76U; break;
        }
        case 0x10e76U:
            r.address[7] -= 2U;
            write_word(host, r.address[7], static_cast<std::uint16_t>(r.data[7]));
            set_logic_word(r, static_cast<std::uint16_t>(r.data[7]));
            r.program_counter = 0x10e78U; break;
        case 0x10e78U:
            r.address[7] -= 4U;
            // MOVE.L to -(SP) writes low then high; JSR saves high then low.
            write_word(host, r.address[7] + 2U, static_cast<std::uint16_t>(r.address[4]));
            write_word(host, r.address[7], static_cast<std::uint16_t>(r.address[4] >> 16U));
            set_logic_long(r, r.address[4]); r.program_counter = 0x10e7aU; break;
        case 0x10e7aU: {
            push_long(host, r, 0x10e80U); r.program_counter = 0x16ff8U;
            const auto child = host.call_function(308U, 1U, 0x72U, 2U, 0x10e7aU, 0x16ff8U, context);
            if (child.status != TranslationStatus::complete || child.control != 1U ||
                r.program_counter != 0x10e80U) return child;
            break;
        }
        case 0x10e80U: r.address[4] = pop_long(host, r); r.program_counter = 0x10e82U; break;
        case 0x10e82U: {
            const auto saved = read_word(host, r.address[7]); r.address[7] += 2U;
            r.data[7] = (r.data[7] & 0xffff0000U) | saved;
            set_logic_word(r, saved); r.program_counter = 0x10e84U; break;
        }
        case 0x10e84U: {
            const auto target = pop_long(host, r); r.program_counter = target;
            return FunctionResult::complete(1U, target);
        }
        default: return {TranslationStatus::contract_violation, 0U, r.program_counter};
        }
    }
}

} // namespace gain_ground::translated

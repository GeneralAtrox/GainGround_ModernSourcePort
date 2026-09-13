#include "gain_ground/contract_types.h"
#include "gain_ground/cpu_b_interrupt.h"

#include <cstdint>
#include <optional>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kTileRegion = 5U;
constexpr std::uint16_t kWindowRegion = 7U;
constexpr std::uint16_t kWordMask = 0xffffU;

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

void set_sub_word(CpuRegisters &r, std::uint16_t a, std::uint16_t b, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((a ^ b) & (a ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (b > a) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void asl_word(CpuRegisters &r, unsigned count)
{
    auto value = static_cast<std::uint16_t>(r.data[0]);
    bool overflow{};
    bool carry{};
    for (unsigned i = 0; i != count; ++i) {
        const bool old_sign = (value & 0x8000U) != 0U;
        carry = old_sign;
        value = static_cast<std::uint16_t>(value << 1U);
        overflow = overflow || (old_sign != ((value & 0x8000U) != 0U));
    }
    std::uint16_t flags{};
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    if (carry) flags |= 0x0011U;
    r.data[0] = (r.data[0] & 0xffff0000U) | value;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &h, CpuRegisters &r, std::uint32_t pc)
{
    r.address[7] -= 4U;
    h.write_memory_word(kPrivateRegion, r.address[7], static_cast<std::uint16_t>(pc >> 16U), kWordMask);
    h.write_memory_word(kPrivateRegion, r.address[7] + 2U, static_cast<std::uint16_t>(pc), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &h, CpuRegisters &r)
{
    const auto high = h.read_memory_word(kPrivateRegion, r.address[7], kWordMask);
    const auto low = h.read_memory_word(kPrivateRegion, r.address[7] + 2U, kWordMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_tile_word(ExecutionHost &h, std::uint32_t address, std::uint16_t value)
{
    if (address >= 0x0020c000U)
        h.write_memory_word(kWindowRegion, address & 0x00003fffU, value, kWordMask);
    else
        h.write_memory_word(kTileRegion, address - 0x00200000U, value, kWordMask);
}

std::uint16_t read_private_word(ExecutionHost &h, std::uint32_t address)
{
    return h.read_memory_word(kPrivateRegion, address, kWordMask);
}

void run_window_rows_helper(ExecutionHost &h, CpuRegisters &r)
{
    auto d0 = read_private_word(h, r.address[2]);
    r.address[2] += 2U;
    r.data[0] = (r.data[0] & 0xffff0000U) | d0;
    set_logic_word(r, d0);
    r.address[0] = 0x0020c000U;
    const auto base_offset = read_private_word(h, r.address[2]);
    r.address[2] += 2U;
    r.address[0] += static_cast<std::int16_t>(base_offset);
    r.data[2] = 2U;
    set_logic_word(r, 2U);
    r.address[1] = 0x0000a0e4U;
    for (;;) {
        auto d1 = read_private_word(h, r.address[1]);
        r.address[1] += 2U;
        r.data[1] = (r.data[1] & 0xffff0000U) | d1;
        set_logic_word(r, d1);
        for (;;) {
            write_tile_word(h, r.address[0], d0);
            set_logic_word(r, d0);
            r.address[0] += 8U;
            d1 = static_cast<std::uint16_t>(d1 - 1U);
            r.data[1] = (r.data[1] & 0xffff0000U) | d1;
            if (d1 == 0xffffU) break;
        }
        const auto displacement = read_private_word(h, r.address[1]);
        r.address[1] += 2U;
        r.address[0] += static_cast<std::int16_t>(displacement);
        const auto d2 = static_cast<std::uint16_t>(r.data[2] - 1U);
        r.data[2] = (r.data[2] & 0xffff0000U) | d2;
        if (d2 == 0xffffU) break;
    }
}

[[nodiscard]] FunctionResult call_child(FunctionContext &context, std::uint32_t id,
    std::uint32_t callsite, std::uint32_t target, std::uint32_t return_pc)
{
    auto &h = *context.host;
    push_return(h, context.registers, return_pc);
    context.registers.program_counter = target;
    return h.call_function(id, 1U, 0x72U, 2U, callsite, target, context);
}

[[nodiscard]] std::optional<FunctionResult> check_interrupt(
    FunctionContext &context, std::uint32_t completed_pc, std::uint32_t next_pc)
{
    auto &h = *context.host;
    auto &r = context.registers;
    if (h.resumes_interrupts_inline())
        return cpu_b_interrupt_boundary(context, completed_pc, next_pc);
    const auto interrupt = h.consume_pending_interrupt(1U, 0x72U, completed_pc);
    const auto mask = static_cast<std::uint8_t>((r.status >> 8U) & 7U);
    if (!interrupt.asserted || interrupt.level <= mask) return std::nullopt;

    const auto saved_status = r.status;
    r.address[7] -= 4U;
    h.write_memory_word(kPrivateRegion, r.address[7] + 2U,
        static_cast<std::uint16_t>(next_pc), kWordMask);
    r.address[7] -= 2U;
    h.write_memory_word(kPrivateRegion, r.address[7], saved_status, kWordMask);
    h.write_memory_word(kPrivateRegion, r.address[7] + 2U,
        static_cast<std::uint16_t>(next_pc >> 16U), kWordMask);
    r.status = static_cast<std::uint16_t>((saved_status & 0x38ffU) | 0x2000U
        | (static_cast<std::uint16_t>(interrupt.level) << 8U));
    const auto vector = static_cast<std::uint32_t>(24U + interrupt.level) * 4U;
    const auto target = (static_cast<std::uint32_t>(
        h.read_memory_word(kPrivateRegion, vector, kWordMask)) << 16U)
        | h.read_memory_word(kPrivateRegion, vector + 2U, kWordMask);
    r.program_counter = target;
    context.state = 0x04U;
    (void)h.call_function(static_cast<std::uint32_t>(96U + interrupt.level),
        1U, 0x04U, 6U, completed_pc, target, context);
    return FunctionResult::complete(5U, target);
}
} // namespace

FunctionResult cpu_b_clear_window_mask_table(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;

    const auto entry_pc = r.program_counter;
    const bool resume_clear = entry_pc == 0x00009d4eU || entry_pc == 0x00009d50U;
    if (!resume_clear) {
        r.address[0] = 0x0020c000U;
        r.data[0] = (r.data[0] & 0xffff0000U) | 0x02ffU;
        set_logic_word(r, 0x02ffU);
    }
    bool clear_complete = false;
    if (entry_pc == 0x00009d50U) {
        const auto count = static_cast<std::uint16_t>(r.data[0] - 1U);
        r.data[0] = (r.data[0] & 0xffff0000U) | count;
        const bool repeat = count != 0xffffU;
        if (auto interrupted = check_interrupt(
                context, 0x00009d50U, repeat ? 0x00009d4eU : 0x00009d54U))
            return *interrupted;
        clear_complete = !repeat;
    }
    while (!clear_complete) {
        const auto offset = r.address[0] & 0x00003fffU;
        (void)h.read_memory_word(kWindowRegion, offset, kWordMask);
        (void)h.read_memory_word(kWindowRegion, offset + 2U, kWordMask);
        h.write_memory_word(kWindowRegion, offset + 2U, 0U, kWordMask);
        h.write_memory_word(kWindowRegion, offset, 0U, kWordMask);
        set_logic_long(r, 0U);
        r.address[0] += 4U;
        if (auto interrupted = check_interrupt(
                context, 0x00009d4eU, 0x00009d50U))
            return *interrupted;
        const auto count = static_cast<std::uint16_t>(r.data[0] - 1U);
        r.data[0] = (r.data[0] & 0xffff0000U) | count;
        const bool repeat = count != 0xffffU;
        if (auto interrupted = check_interrupt(
                context, 0x00009d50U, repeat ? 0x00009d4eU : 0x00009d54U))
            return *interrupted;
        if (!repeat) clear_complete = true;
    }

    r.address[0] = 0x002023e8U;
    r.address[1] = 0x0000a056U;
    const auto copy_result = call_child(context, 288U, 0x00009d5eU, 0x00015f5eU, 0x00009d64U);
    if (copy_result.status != TranslationStatus::complete || copy_result.control != 1U)
        return copy_result;

    const auto d5 = read_private_word(h, 0x00000836U);
    r.data[5] = (r.data[5] & 0xffff0000U) | d5;
    set_logic_word(r, d5);
    if ((d5 & 0x8000U) == 0U) {
        r.data[0] = (r.data[0] & 0xffff0000U) | d5;
        set_logic_word(r, d5);
        const auto subtrahend = read_private_word(h, 0x00000838U);
        const auto difference = static_cast<std::uint16_t>(d5 - subtrahend);
        r.data[0] = (r.data[0] & 0xffff0000U) | difference;
        set_sub_word(r, d5, subtrahend, difference);
        asl_word(r, 3U);
        r.address[4] = 0x0020245eU;
        r.address[4] -= static_cast<std::int16_t>(static_cast<std::uint16_t>(r.data[0]));
        r.address[7] -= 2U;
        h.write_memory_word(kPrivateRegion, r.address[7], static_cast<std::uint16_t>(r.data[0]), kWordMask);

        const auto object_result = call_child(context, 136U, 0x00009d7cU, 0x00009e06U, 0x00009d80U);
        if (object_result.status != TranslationStatus::complete || object_result.control != 1U)
            return object_result;
        const auto saved_d0 = read_private_word(h, r.address[7]);
        r.address[7] += 2U;
        r.data[0] = (r.data[0] & 0xffff0000U) | saved_d0;
        set_logic_word(r, saved_d0);
        r.address[2] = 0x0000a094U;
        r.address[2] += static_cast<std::int16_t>(saved_d0);

        push_return(h, r, 0x00009d8cU);
        run_window_rows_helper(h, r);
        r.program_counter = pop_return(h, r);
        const auto table_word = read_private_word(h, r.address[2]);
        set_logic_word(r, table_word);
        if (table_word != 0U) {
            push_return(h, r, 0x00009d94U);
            run_window_rows_helper(h, r);
            r.program_counter = pop_return(h, r);
        }
    }

    const auto mask_result = call_child(context, 134U, 0x00009d94U, 0x00009dbcU, 0x00009d98U);
    if (mask_result.status != TranslationStatus::complete || mask_result.control != 1U)
        return mask_result;
    const auto return_address = pop_return(h, r);
    r.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated

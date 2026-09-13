#include "gain_ground/contract_types.h"
#include "gain_ground/cpu_b_interrupt.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivate = 2U, kShared = 3U, kTile = 5U, kMask = 0xffffU;

void set_word(CpuRegisters &r, std::uint16_t v)
{
    std::uint16_t f = r.status & 0x0010U;
    if ((v & 0x8000U) != 0U) f |= 8U;
    if (v == 0U) f |= 4U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}
void set_long(CpuRegisters &r, std::uint32_t v)
{
    std::uint16_t f = r.status & 0x0010U;
    if ((v & 0x80000000U) != 0U) f |= 8U;
    if (v == 0U) f |= 4U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}
std::uint32_t read_long(ExecutionHost &h, std::uint16_t region, std::uint32_t a)
{
    const auto hi = h.read_memory_word(region, a, kMask);
    const auto lo = h.read_memory_word(region, a + 2U, kMask);
    return (static_cast<std::uint32_t>(hi) << 16U) | lo;
}
void write_long(ExecutionHost &h, std::uint16_t region, std::uint32_t a, std::uint32_t v)
{
    h.write_memory_word(region, a, static_cast<std::uint16_t>(v >> 16U), kMask);
    h.write_memory_word(region, a + 2U, static_cast<std::uint16_t>(v), kMask);
}
void clear_long(ExecutionHost &h, CpuRegisters &r, std::uint32_t a)
{
    (void)read_long(h, kTile, a);
    h.write_memory_word(kTile, a + 2U, 0U, kMask);
    h.write_memory_word(kTile, a, 0U, kMask);
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | (r.status & 0x0010U) | 4U);
}
void push_long(ExecutionHost &h, CpuRegisters &r, std::uint32_t v)
{
    r.address[7] -= 4U;
    write_long(h, kPrivate, r.address[7], v);
}
std::uint32_t pop_long(ExecutionHost &h, CpuRegisters &r)
{
    const auto v = read_long(h, kPrivate, r.address[7]);
    r.address[7] += 4U;
    return v;
}
std::optional<FunctionResult> interrupt(ExecutionHost &h, FunctionContext &c,
                                        std::uint32_t pc, std::uint32_t resume)
{
    if (h.resumes_interrupts_inline()) return cpu_b_interrupt_boundary(c, pc, resume);
    auto &r = c.registers;
    const auto p = h.consume_pending_interrupt(1U, 0x72U, pc);
    if (!p.asserted || p.level <= ((r.status >> 8U) & 7U)) return std::nullopt;
    const auto saved = r.status;
    r.address[7] -= 4U;
    h.write_memory_word(kPrivate, r.address[7] + 2U, static_cast<std::uint16_t>(resume), kMask);
    r.address[7] -= 2U;
    h.write_memory_word(kPrivate, r.address[7], saved, kMask);
    h.write_memory_word(kPrivate, r.address[7] + 2U, static_cast<std::uint16_t>(resume >> 16U), kMask);
    r.status = static_cast<std::uint16_t>((saved & 0x38ffU) | 0x2000U
        | (static_cast<std::uint16_t>(p.level) << 8U));
    const auto vector = static_cast<std::uint32_t>(24U + p.level) * 4U;
    const auto target = read_long(h, kPrivate, vector);
    r.program_counter = target;
    c.state = 0x04U;
    (void)h.call_function(static_cast<std::uint32_t>(96U + p.level),
        1U, 0x04U, 6U, pc, target, c);
    return FunctionResult::complete(5U, target);
}
} // namespace

FunctionResult cpu_b_copy_stage_tables_to_video_ram(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;
    auto pc = r.program_counter;
    if (pc == 0x0000a774U) {
        for (;;) {
            const auto gate = h.read_memory_word(kShared, 0x38006U, kMask);
            set_word(r, gate);
            if (gate == 0U) break;
            push_long(h, r, 0x0000a780U);
            r.program_counter = 0x000085acU;
            const auto child = h.call_function(
                117U, 1U, 0x72U, 2U, 0x0000a77aU, 0x000085acU, context);
            if (child.status != TranslationStatus::complete || child.control != 1U)
                return child;
        }
        r.address[0] = 0x00200000U;
        r.address[1] = 0xffff8402U;
        r.data[1] = (r.data[1] & 0xffff0000U) | 0x002fU;
        pc = 0x0000a790U;
    }

    while (pc != 0x0000a7c4U) {
        switch (pc) {
        case 0x0000a790U: {
            const auto dst = r.address[0] - 0x00200000U;
            (void)h.read_memory_word(kTile, dst, kMask);
            h.write_memory_word(kTile, dst, 0U, kMask);
            r.address[0] += 2U; set_word(r, 0U);
            if (const auto interrupted = interrupt(h, context, pc, 0x0000a792U))
                return *interrupted;
            pc = 0x0000a792U; break;
        }
        case 0x0000a792U:
            r.address[0] += 8U; pc = 0x0000a794U; break;
        case 0x0000a794U:
            clear_long(h, r, r.address[0] - 0x00200000U);
            r.address[0] += 4U; pc = 0x0000a796U; break;
        case 0x0000a796U:
            clear_long(h, r, r.address[0] - 0x00200000U);
            r.address[0] += 4U; pc = 0x0000a798U; break;
        case 0x0000a798U: {
            const auto word = h.read_memory_word(kShared, r.address[1] & 0x3ffffU, kMask);
            r.address[1] += 2U;
            h.write_memory_word(kTile, r.address[0] - 0x00200000U, word, kMask);
            r.address[0] += 2U; set_word(r, word); pc = 0x0000a79aU; break;
        }
        case 0x0000a79aU:
            r.data[0] = (r.data[0] & 0xffff0000U) | 0x0019U;
            set_long(r, 0x19U); pc = 0x0000a79cU; break;
        case 0x0000a79cU: {
            const auto value = read_long(h, kShared, r.address[1] & 0x3ffffU);
            r.address[1] += 4U;
            write_long(h, kTile, r.address[0] - 0x00200000U, value);
            r.address[0] += 4U; set_long(r, value);
            if (const auto interrupted = interrupt(h, context, pc, 0x0000a79eU))
                return *interrupted;
            pc = 0x0000a79eU; break;
        }
        case 0x0000a79eU: {
            const auto count = static_cast<std::uint16_t>(r.data[0] - 1U);
            r.data[0] = (r.data[0] & 0xffff0000U) | count;
            const auto next = count == 0xffffU ? 0x0000a7a2U : 0x0000a79cU;
            if (const auto interrupted = interrupt(h, context, pc, next))
                return *interrupted;
            pc = next; break;
        }
        case 0x0000a7a2U:
            r.address[0] += 4U; pc = 0x0000a7a4U; break;
        case 0x0000a7a4U: {
            const auto count = static_cast<std::uint16_t>(r.data[1] - 1U);
            r.data[1] = (r.data[1] & 0xffff0000U) | count;
            const auto next = count == 0xffffU ? 0x0000a7a8U : 0x0000a790U;
            if (const auto interrupted = interrupt(h, context, pc, next))
                return *interrupted;
            pc = next; break;
        }
        case 0x0000a7a8U:
            r.address[0] = 0x00204000U; pc = 0x0000a7aeU; break;
        case 0x0000a7aeU:
            r.address[1] = 0xffff97e2U; pc = 0x0000a7b2U; break;
        case 0x0000a7b2U:
            r.data[1] = (r.data[1] & 0xffff0000U) | 0x002fU;
            pc = 0x0000a7b6U; break;
        case 0x0000a7b6U:
            r.data[0] = (r.data[0] & 0xffff0000U) | 0x001eU;
            set_long(r, 0x1eU); pc = 0x0000a7b8U; break;
        case 0x0000a7b8U: {
            const auto value = read_long(h, kShared, r.address[1] & 0x3ffffU);
            r.address[1] += 4U;
            write_long(h, kTile, r.address[0] - 0x00200000U, value);
            r.address[0] += 4U; set_long(r, value);
            if (const auto interrupted = interrupt(h, context, pc, 0x0000a7baU))
                return *interrupted;
            pc = 0x0000a7baU; break;
        }
        case 0x0000a7baU: {
            const auto count = static_cast<std::uint16_t>(r.data[0] - 1U);
            r.data[0] = (r.data[0] & 0xffff0000U) | count;
            const auto next = count == 0xffffU ? 0x0000a7beU : 0x0000a7b8U;
            if (const auto interrupted = interrupt(h, context, pc, next))
                return *interrupted;
            pc = next; break;
        }
        case 0x0000a7beU:
            r.address[0] += 4U; pc = 0x0000a7c0U; break;
        case 0x0000a7c0U: {
            const auto count = static_cast<std::uint16_t>(r.data[1] - 1U);
            r.data[1] = (r.data[1] & 0xffff0000U) | count;
            pc = count == 0xffffU ? 0x0000a7c4U : 0x0000a7b6U; break;
        }
        default:
            return {TranslationStatus::contract_violation, 0U, pc};
        }
    }
    push_long(h, r, 0x0000a7c8U);
    r.program_counter = 0x0000a7d4U;
    (void)h.call_function(144U, 1U, 0x72U, 2U, 0x0000a7c4U, 0x0000a7d4U, context);
    const auto ret = pop_long(h, r);
    r.program_counter = ret;
    return FunctionResult::complete(1U, ret);
}
} // namespace gain_ground::translated

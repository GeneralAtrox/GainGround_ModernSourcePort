#include "gain_ground/contract_types.h"
#include "gground_functions.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgram = 1U, kShared = 3U, kCharacter = 8U;
constexpr std::uint16_t kWord = 0xffffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;
void fetch(ExecutionHost &h, std::uint32_t a) { (void)h.read_memory_word(kProgram, a, kWord); }
std::uint16_t read(ExecutionHost &h, std::uint32_t a) { return h.read_memory_word(kShared, a & kSharedMask, kWord); }
void write(ExecutionHost &h, std::uint32_t a, std::uint16_t v) { h.write_memory_word(kShared, a & kSharedMask, v, kWord); }
void write_long(ExecutionHost &h, std::uint32_t a, std::uint32_t v)
{
    h.write_memory_word(kCharacter, a - 0x00280000U, static_cast<std::uint16_t>(v >> 16U), kWord);
    h.write_memory_word(kCharacter, a - 0x00280000U + 2U, static_cast<std::uint16_t>(v), kWord);
}
void move_flags(CpuRegisters &r, std::uint32_t v)
{
    std::uint16_t f = r.status & 0x10U;
    if (v & 0x80000000U) f |= 8U;
    if (!v) f |= 4U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
void irq4(FunctionContext &c, std::uint32_t pc, std::uint32_t next, std::uint8_t kind)
{
    auto &h = *c.host; auto &r = c.registers;
    const auto pending = h.consume_pending_interrupt(0U, 0xffU, pc);
    const auto mask = static_cast<std::uint8_t>((r.status >> 8U) & 7U);
    if (!pending.asserted || pending.level <= mask) return;
    const auto saved = r.status;
    r.address[7] -= 4U; write(h, r.address[7] + 2U, static_cast<std::uint16_t>(next));
    r.address[7] -= 2U; write(h, r.address[7], saved);
    write(h, r.address[7] + 2U, static_cast<std::uint16_t>(next >> 16U));
    r.status = static_cast<std::uint16_t>((saved & 0x38ffU) | 0x2000U
        | (static_cast<std::uint16_t>(pending.level) << 8U));
    fetch(h, 0x70U); fetch(h, 0x72U); (void)read(h, 0x48U); (void)read(h, 0x4aU);
    r.program_counter = 0x00080048U;
    (void)h.call_function(47U, 0U, 0xffU, kind, pc, 0x00080048U, c);
    if (kind == 0U) (void)cpu_a_irq4_vector_trampoline(c);
    r.program_counter = next;
}
} // namespace

FunctionResult cpu_a_clear_character_ram(FunctionContext &c) noexcept
{
    if (!c.host) return {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
    auto &h = *c.host; auto &r = c.registers;
    r.data[1] = 0U; r.status = static_cast<std::uint16_t>(r.status & ~0x0fU);
    fetch(h, 0x0bb0U); fetch(h, 0x0bb2U); r.address[0] = 0x00280000U;
    fetch(h, 0x0bb4U); fetch(h, 0x0bb6U);
    r.data[0] = (r.data[0] & 0xffff0000U) | 0x7fffU;
    r.status = static_cast<std::uint16_t>(r.status & ~0x0fU);
    fetch(h, 0x0bb8U); fetch(h, 0x0bbaU);
    for (;;) {
        write_long(h, r.address[0], r.data[1]); r.address[0] += 4U; move_flags(r, r.data[1]);
        fetch(h, 0x0bbcU); irq4(c, 0x0bb8U, 0x0bbaU, 0U);
        const auto counter = static_cast<std::uint16_t>(r.data[0] - 1U);
        r.data[0] = (r.data[0] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
        fetch(h, 0x0bb8U); fetch(h, 0x0bbaU); irq4(c, 0x0bbaU, 0x0bb8U, 1U);
    }
    fetch(h, 0x0bb8U); fetch(h, 0x0bbeU); fetch(h, 0x0bc0U);
    const auto target = (static_cast<std::uint32_t>(read(h, r.address[7])) << 16U)
        | read(h, r.address[7] + 2U);
    r.address[7] += 4U; fetch(h, target); fetch(h, target + 2U);
    r.program_counter = target; return FunctionResult::complete(1U, target);
}
} // namespace gain_ground::translated

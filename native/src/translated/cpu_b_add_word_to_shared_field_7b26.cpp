// Implemented but unverified. Original credit-field addition and sequential owner.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_add_word_to_shared_field_7b26(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U || r.program_counter != 0x8364U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    t.prefetch(0x8368U); const auto value = m.add(t.word(0x7b26U), r.data[1], 16U);
    t.prefetch(0x836aU); t.word(0x7b26U, static_cast<std::uint16_t>(value));
    if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, 0x8364U};
    r.program_counter = 0x8368U; t.stop();
    if (const auto event = m.interrupt(c, 0x8364U, 0x8368U)) return *event;
    return c.host->call_function(113U, 1U, 0x72U, 0U, 0x8364U, 0x8368U, c);
}
} // namespace gain_ground::translated

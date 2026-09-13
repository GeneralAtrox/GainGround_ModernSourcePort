#include "gain_ground/contract_types.h"
#include <cstdint>

namespace gain_ground::translated {
FunctionResult cpu_b_decrement_dual_record_timers(FunctionContext &) noexcept;
namespace {
struct Machine249 {
    ExecutionHost &h;
    CpuRegisters &r;
    std::uint16_t word(std::uint32_t a) { return h.read_memory_word(2U, a & 0xffffffU, 0xffffU); }
    std::uint8_t byte(std::uint32_t a) {
        const auto v = h.read_memory_word(2U, (a & 0xffffffU) & ~1U, (a & 1U) ? 0xffU : 0xff00U);
        return static_cast<std::uint8_t>((a & 1U) ? v : v >> 8U);
    }
    std::uint32_t lng(std::uint32_t a) {
        const auto hi = word(a); const auto lo = word(a + 2U);
        return (static_cast<std::uint32_t>(hi) << 16U) | lo;
    }
    void word(std::uint32_t a, std::uint32_t v) { h.write_memory_word(2U, a & 0xffffffU, static_cast<std::uint16_t>(v), 0xffffU); }
    void byte(std::uint32_t a, std::uint32_t v) {
        h.write_memory_word(2U, (a & 0xffffffU) & ~1U,
            static_cast<std::uint16_t>((a & 1U) ? (v & 0xffU) : ((v & 0xffU) << 8U)), (a & 1U) ? 0xffU : 0xff00U);
    }
    void long_rmw(std::uint32_t a, std::uint32_t v) { word(a + 2U, v); word(a, v >> 16U); }
    static std::uint32_t mask(unsigned bits) { return bits == 32U ? 0xffffffffU : (1U << bits) - 1U; }
    void flags(std::uint16_t f) { r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f); }
    void logic(std::uint32_t v, unsigned bits) {
        v &= mask(bits); flags(static_cast<std::uint16_t>((r.status & 0x10U) | (v == 0U ? 4U : 0U) | ((v & (1U << (bits - 1U))) ? 8U : 0U)));
    }
    std::uint32_t arithmetic(std::uint32_t a, std::uint32_t b, unsigned bits, bool sub = false, bool compare = false) {
        const auto m = mask(bits), sign = 1U << (bits - 1U); a &= m; b &= m;
        const auto v = (sub ? a - b : a + b) & m;
        const bool carry = sub ? a < b : (static_cast<std::uint64_t>(a) + b > m);
        const bool overflow = ((sub ? (a ^ b) : ~(a ^ b)) & (a ^ v) & sign) != 0U;
        flags(static_cast<std::uint16_t>((compare ? (r.status & 0x10U) : (carry ? 0x10U : 0U)) |
            (carry ? 1U : 0U) | (overflow ? 2U : 0U) | (v == 0U ? 4U : 0U) | ((v & sign) ? 8U : 0U)));
        return v;
    }
    void dw(unsigned n, std::uint32_t v) { r.data[n] = (r.data[n] & 0xffff0000U) | (v & 0xffffU); }
    void db(unsigned n, std::uint32_t v) { r.data[n] = (r.data[n] & 0xffffff00U) | (v & 0xffU); }
    void asl2() {
        auto v = r.data[0] & 0xffffU; bool carry = false, overflow = false;
        for (unsigned i = 0; i < 2U; ++i) { carry = (v & 0x8000U) != 0U; const auto n = (v << 1U) & 0xffffU; overflow |= ((v ^ n) & 0x8000U) != 0U; v = n; }
        dw(0U, v); flags(static_cast<std::uint16_t>((carry ? 0x11U : 0U) | (overflow ? 2U : 0U) | (v == 0U ? 4U : 0U) | ((v & 0x8000U) ? 8U : 0U)));
    }
    bool carry() const { return (r.status & 1U) != 0U; }
    bool zero() const { return (r.status & 4U) != 0U; }
    bool negative() const { return (r.status & 8U) != 0U; }
    bool le() const { return zero() || (negative() != ((r.status & 2U) != 0U)); }
    void clear_byte(std::uint32_t a) { (void)byte(a); byte(a, 0U); logic(0U, 8U); }
    void toggle(std::uint32_t a) {
        const auto v = byte(a); byte(a, v ^ 1U);
        r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((v & 1U) ? 0U : 4U));
    }
    void phase(std::uint32_t a) { const auto v = byte(a); byte(a, arithmetic(v, 1U, 8U)); }
    void phase_mask(std::uint32_t a) { const auto v = byte(a) & 3U; byte(a, v); logic(v, 8U); }
    FunctionResult call(FunctionContext &c, std::uint32_t id, std::uint32_t site, std::uint32_t target, std::uint32_t next) {
        r.address[7] -= 4U; word(r.address[7], next >> 16U); word(r.address[7] + 2U, next);
        r.program_counter = target; return h.call_function(id, 1U, 0x72U, 2U, site, target, c);
    }
    FunctionResult ret() { const auto pc = lng(r.address[7]); r.address[7] += 4U; r.program_counter = pc; return FunctionResult::complete(1U, pc); }
};
bool returned(const FunctionResult &v) { return v.status == TranslationStatus::complete && v.control == 1U; }
}

FunctionResult cpu_b_dispatch_record_state_42(FunctionContext &c) noexcept {
    if (c.host == nullptr || c.registers.program_counter != 0x120aeU)
        return {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
    auto &r = c.registers; Machine249 m{*c.host, r}; const auto base = r.address[5];
    std::uint32_t v = 0U, value = 0U; FunctionResult result{};
    m.dw(0U, m.word(base + 0x42U)); m.logic(r.data[0], 16U); m.asl2();
    switch (r.data[0] & 0xffffU) {
    case 0U: goto state0;
    case 4U: goto state1;
    case 8U: goto state2;
    case 12U: goto state3;
    default: return {TranslationStatus::contract_violation, 0U, 0x120b4U};
    }
state0:
    result = m.call(c, 253U, 0x120c8U, 0x1283cU, 0x120ccU); if (!returned(result)) return result;
    if (m.carry()) goto become3;
    result = m.call(c, 259U, 0x120ceU, 0x12c3cU, 0x120d2U);
    if (result.status == TranslationStatus::complete && result.control == 8U)
        return FunctionResult::complete(1U, result.exit_program_counter);
    if (!returned(result)) return result;
    (void)m.arithmetic(r.data[4], 0x100U, 16U, true, true);
    if (!m.carry()) { result = m.call(c, 263U, 0x120d8U, 0x12d3aU, 0x120dcU); if (!returned(result)) return result; }
    (void)m.arithmetic(r.data[4], 0x400U, 16U, true, true);
    if (m.carry()) goto motion;
    m.logic(r.data[4], 8U); if (!m.negative()) goto advance_clear;
become3:
    m.clear_byte(base + 0x3cU); m.word(base + 0x42U, 3U); m.logic(3U, 16U);
    v = m.byte(base + 0x3fU); m.byte(base + 0x3fU, v | 0x80U); m.logic(v, 8U);
    goto clear_phase;
motion:
    r.data[0] = m.lng(base + 0x1eU); m.logic(r.data[0], 32U);
    v = m.lng(base + 0x12U); m.long_rmw(base + 0x12U, m.arithmetic(v, r.data[0], 32U));
    r.data[0] = m.lng(base + 0x22U); m.logic(r.data[0], 32U);
    v = m.lng(base + 0x22U); m.long_rmw(base + 0x22U, m.arithmetic(v, 0x4000U, 32U, true));
    v = m.lng(base + 0x16U); m.long_rmw(base + 0x16U, m.arithmetic(v, r.data[0], 32U));
    r.data[0] = m.lng(base + 0x26U); m.logic(r.data[0], 32U);
    v = m.lng(base + 0x1aU); m.long_rmw(base + 0x1aU, m.arithmetic(v, r.data[0], 32U));
    m.toggle(base + 0x3cU); if (!m.zero()) goto bounds;
    m.phase(base + 0x3dU); m.phase_mask(base + 0x3dU); goto animation;
state1:
    result = m.call(c, 253U, 0x12130U, 0x1283cU, 0x12134U); if (!returned(result)) return result;
    if (m.carry()) goto become3;
    m.toggle(base + 0x3cU); if (!m.zero()) goto bounds;
    m.phase(base + 0x3dU); m.phase_mask(base + 0x3dU); if (!m.zero()) goto animation;
    m.word(base + 0x46U, 0x3cU); m.logic(0x3cU, 16U); goto advance;
state2:
    result = m.call(c, 253U, 0x12152U, 0x1283cU, 0x12156U); if (!returned(result)) return result;
    if (m.carry()) goto become3;
    v = m.word(base + 0x46U); m.word(base + 0x46U, m.arithmetic(v, 1U, 16U, true)); if (m.le()) goto become3;
    m.toggle(base + 0x3cU); if (!m.zero()) goto bounds;
    m.phase(base + 0x3dU); m.phase_mask(base + 0x3dU); goto animation;
state3:
    m.toggle(base + 0x3cU); if (!m.zero()) goto bounds;
    m.phase(base + 0x3dU); v = m.byte(base + 0x3dU); (void)m.arithmetic(v, 3U, 8U, true, true);
    if (m.carry()) goto animation;
    result = m.call(c, 261U, 0x12186U, 0x12d08U, 0x1218aU);
    // The host has already recorded 261's sequential entry into 262.
    // Continue its instructions here without emitting the boundary twice.
    if (result.status == TranslationStatus::complete && result.control == 3U
        && r.program_counter == 0x12d14U)
        result = cpu_b_decrement_dual_record_timers(c);
    if (!returned(result)) return result;
    return m.ret();
advance_clear:
    m.clear_byte(base + 0x3cU);
advance:
    v = m.word(base + 0x42U); m.word(base + 0x42U, m.arithmetic(v, 1U, 16U));
clear_phase:
    m.clear_byte(base + 0x3dU);
animation:
    m.dw(0U, m.word(base + 0x42U)); m.logic(r.data[0], 16U); m.asl2();
    v = m.byte(base + 0x3dU); m.db(0U, m.arithmetic(r.data[0], v, 8U)); m.asl2();
    r.address[0] = 0x12e14U + static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[0])));
    v = m.word(r.address[0]); r.address[0] += 2U; m.word(base + 6U, v); m.logic(v, 16U);
    v = m.byte(r.address[0]); ++r.address[0]; m.byte(base + 1U, v); m.logic(v, 8U);
bounds:
    m.dw(0U, m.word(base + 0x42U)); m.logic(r.data[0], 16U); m.asl2();
    v = m.byte(base + 0x3dU); m.db(0U, m.arithmetic(r.data[0], v, 8U));
    m.dw(0U, m.arithmetic(r.data[0], r.data[0], 16U));
    r.address[0] = 0x12e50U + static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[0])));
    m.dw(0U, m.word(base + 0x16U)); m.logic(r.data[0], 16U);
    v = r.data[0] & 0xffffU; value = (v >> 1U) | (v & 0x8000U); m.dw(0U, value);
    m.flags(static_cast<std::uint16_t>(((v & 1U) ? 0x11U : 0U) | (value == 0U ? 4U : 0U) | ((value & 0x8000U) ? 8U : 0U)));
    m.dw(1U, r.data[0]); m.logic(r.data[1], 16U);
    v = m.byte(r.address[0]); ++r.address[0]; m.db(0U, m.arithmetic(r.data[0], v, 8U)); if (m.le()) goto offscreen;
    v = m.byte(r.address[0]); ++r.address[0]; m.db(1U, m.arithmetic(r.data[1], v, 8U)); if (!m.le()) goto draw;
offscreen:
    m.clear_byte(base); return m.ret();
draw:
    m.byte(base + 0x10U, r.data[0]); m.logic(r.data[0], 8U);
    m.byte(base + 0x11U, r.data[1]); m.logic(r.data[1], 8U);
    result = m.call(c, 280U, 0x121e0U, 0x15d24U, 0x121e6U); if (!returned(result)) return result;
    result = m.call(c, 281U, 0x121e6U, 0x15d3cU, 0x121ecU); if (!returned(result)) return result;
    result = m.call(c, 282U, 0x121ecU, 0x15df2U, 0x121f2U); if (!returned(result)) return result;
    return m.ret();
}
} // namespace gain_ground::translated

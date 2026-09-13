#include "gain_ground/contract_types.h"
#include "gain_ground/cpu_b_interrupt.h"
#include <cstdint>
#include <optional>

namespace gain_ground::translated {
namespace {
struct Machine565 {
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
    bool bit(std::uint32_t a, unsigned n, int change = 0) {
        const auto v = byte(a); const bool set = (v & (1U << n)) != 0U;
        if (change != 0) byte(a, change > 0 ? v | (1U << n) : v & ~(1U << n));
        r.status = static_cast<std::uint16_t>((r.status & ~4U) | (set ? 0U : 4U));
        return set;
    }
    FunctionResult call(FunctionContext &c, std::uint32_t id, std::uint32_t site, std::uint32_t target, std::uint32_t next) {
        r.address[7] -= 4U; word(r.address[7], next >> 16U); word(r.address[7] + 2U, next);
        r.program_counter = target; return h.call_function(id, 1U, 0x72U, 2U, site, target, c);
    }
    FunctionResult tail(FunctionContext &c, std::uint32_t id, std::uint32_t site, std::uint32_t target) {
        r.program_counter = target; return h.call_function(id, 1U, 0x72U, 1U, site, target, c);
    }
    std::optional<FunctionResult> interrupt(FunctionContext &c, std::uint32_t pc, std::uint32_t next) {
        if (h.resumes_interrupts_inline()) return cpu_b_interrupt_boundary(c, pc, next);
        const auto event = h.consume_pending_interrupt(1U, 0x72U, pc);
        if (!event.asserted || event.level <= ((r.status >> 8U) & 7U)) return std::nullopt;
        const auto status = r.status;
        r.address[7] -= 4U; word(r.address[7] + 2U, next);
        r.address[7] -= 2U; word(r.address[7], status); word(r.address[7] + 2U, next >> 16U);
        r.status = static_cast<std::uint16_t>((status & 0x38ffU) | 0x2000U | (event.level << 8U));
        const auto target = lng((24U + event.level) * 4U);
        r.program_counter = target; c.state = 0x04U;
        (void)h.call_function(96U + event.level, 1U, 0x04U, 6U, pc, target, c);
        return FunctionResult::complete(5U, target);
    }
    FunctionResult ret() { const auto pc = lng(r.address[7]); r.address[7] += 4U; r.program_counter = pc; return FunctionResult::complete(1U, pc); }
};
bool returned(const FunctionResult &v) { return v.status == TranslationStatus::complete && v.control == 1U; }
}
FunctionResult proven_static_cpu_b_72_0001d214(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (c.host == nullptr || r.program_counter != 0x1d214U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    Machine565 m{*c.host, r};
    const auto base = r.address[5];
    std::uint32_t v = m.byte(base + 0x59U); m.logic(v, 8U);
    if (v != 0U) return m.ret();
    if (!m.bit(base + 0x41U, 5U)) {
        const auto result = m.call(c, 341U, 0x1d226U, 0x1d6d8U, 0x1d22aU);
        if (!returned(result)) return result;
        if (m.bit(base + 0x41U, 5U)) m.bit(base + 0x40U, 1U, 1);
        return m.ret();
    }
    v = m.byte(base + 0x58U); m.logic(v, 8U);
    if (v == 0U) {
        r.data[0] = 0U; m.logic(0U, 32U);
        m.db(0U, m.byte(base + 0x60U)); m.logic(r.data[0], 8U);
        m.db(0U, m.arithmetic(r.data[0], 9U, 8U));
        r.address[4] = 0x3400U;
        v = m.byte(r.address[4] + static_cast<std::int16_t>(r.data[0])); m.logic(v, 8U);
        if (v == 0U) goto clear;
        {
            const auto result = m.call(c, 347U, 0x1d25aU, 0x1da58U, 0x1d25eU);
            if (!returned(result)) return result;
        }
        m.dw(1U, 0x200U); m.logic(r.data[1], 16U);
        m.byte(base + 0x39U, 0xe0U); m.logic(0xe0U, 8U);
        m.dw(0U, m.word(base + 0x5cU)); m.logic(r.data[0], 16U);
        m.dw(0U, m.arithmetic(r.data[0], 0x200U, 16U));
        m.dw(0U, r.data[0] & 0x400U); m.logic(r.data[0], 16U);
        if ((r.status & 4U) == 0U) {
            m.byte(base + 0x39U, 0x20U); m.logic(0x20U, 8U);
            m.dw(1U, 0xfe00U); m.logic(r.data[1], 16U);
        }
        m.dw(0U, m.word(base + 0x5cU)); m.logic(r.data[0], 16U);
        m.dw(0U, m.arithmetic(r.data[0], r.data[1], 16U));
        m.dw(1U, r.data[0]); m.logic(r.data[1], 16U);
        m.dw(1U, m.arithmetic(r.data[1], 0x400U, 16U));
        m.dw(0U, r.data[0] & 0x7ffU); m.logic(r.data[0], 16U);
        m.dw(1U, r.data[1] & 0x7ffU); m.logic(r.data[1], 16U);
        m.word(base + 0x5cU, r.data[0]); m.logic(r.data[0], 16U);
        m.word(base + 0x3aU, r.data[1]); m.logic(r.data[1], 16U);
        m.byte(base + 0x58U, 1U); m.logic(1U, 8U);
        return m.ret();
    }
    m.dw(0U, m.word(base + 0x5cU)); m.logic(r.data[0], 16U);
    v = m.word(base + 0x3aU); (void)m.arithmetic(r.data[0], v, 16U, true, true);
    if ((r.status & 4U) != 0U) {
        // Original MOVE.B preserves D0 bits 8..31 before the signed word index.
        m.db(0U, m.byte(base + 0x60U)); m.logic(r.data[0], 8U);
        m.db(0U, m.arithmetic(r.data[0], 9U, 8U));
        r.address[4] = 0x3400U;
        v = m.byte(r.address[4] + static_cast<std::int16_t>(r.data[0])); m.logic(v, 8U);
        if (v != 0U) {
            const auto result = m.call(c, 347U, 0x1d2c2U, 0x1da58U, 0x1d2c6U);
            if (!returned(result)) return result;
        }
        goto clear;
    }
    m.db(1U, m.byte(base + 0x40U)); m.logic(r.data[1], 8U);
    m.db(1U, r.data[1] & 0xe0U); m.logic(r.data[1], 8U);
    if ((r.status & 4U) == 0U) goto clear;
    r.address[0] = m.lng(base + 0x6eU);
    m.db(1U, m.byte(r.address[0] + 3U)); m.logic(r.data[1], 8U);
    v = m.byte(base + 0x37U); m.byte(base + 0x36U, v); m.logic(v, 8U);
    v = m.byte(base + 0x36U); m.byte(base + 0x36U, m.arithmetic(v, r.data[1], 8U));
    m.db(1U, m.byte(base + 0x39U)); m.logic(r.data[1], 8U);
    m.dw(1U, static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[1])))); m.logic(r.data[1], 16U);
    m.dw(0U, m.arithmetic(r.data[0], r.data[1], 16U));
    m.dw(0U, r.data[0] & 0x7ffU); m.logic(r.data[0], 16U);
    m.word(base + 0x5cU, r.data[0]); m.logic(r.data[0], 16U);
    return m.ret();
clear:
    m.bit(base + 0x41U, 5U, -1);
    m.byte(base + 0x58U, 0U); m.logic(0U, 8U);
    return m.ret();
}
} // namespace gain_ground::translated

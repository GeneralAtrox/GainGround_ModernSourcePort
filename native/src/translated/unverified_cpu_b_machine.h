#pragma once
#include "gain_ground/contract_types.h"
#include "gain_ground/m68000_interrupt_entry.h"
#include "gain_ground/cpu_b_interrupt.h"
#include "gain_ground/native_function_registry.h"
#include "gground_functions.h"
#include "gground_memory_map.h"
#include <optional>

// Code-first translation support. Not certified for parity.
namespace gain_ground::translated::unverified {
struct Machine {
    ExecutionHost &h;
    CpuRegisters &r;
    std::uint8_t cpu = 1U;
    std::uint8_t state = 0x72U;
    bool unresolved_bus = false;
    struct Address { std::uint16_t region; std::uint32_t offset; };
    Address resolve(std::uint32_t a) {
        a &= 0xffffffU;
        for (const auto &window : generated::kMemoryWindows) {
            if (window.address_space != "program" || !(window.cpu_mask & (1U << cpu))) continue;
            const auto normalized = a & ~window.mirror;
            // Byte-wide device declarations are visited through aligned word accesses.
            if (normalized < (window.start & ~1U) || normalized > window.end) continue;
            const auto offset = normalized - (window.start & ~1U);
            const auto store = window.backing_store;
            if (store == "subcpu") return {2U, offset};
            if (store == "share1") return {3U, offset};
            if (store == "maincpu_rom") return {static_cast<std::uint16_t>(window.start == 0U ? 1U : 4U), offset};
            if (store == "tile_ram") {
                if (offset < 0x8000U) return {5U, offset};
                if (offset < 0xc000U) return {6U, offset - 0x8000U};
                return {7U, offset - 0xc000U};
            }
            if (store == "character_ram") return {8U, offset};
            if (store == "palette_ram") return {9U, offset};
            if (store == "sprite_ram") return {11U, offset};
            if (window.owner == "mixer") return {10U, offset};
            return {0U, a};
        }
        unresolved_bus = !h.allows_unmapped_program_access();
        return {0U, a};
    }
    std::uint16_t read(std::uint32_t a, std::uint16_t mask) {
        const auto mapped = resolve(a);
        if (unresolved_bus) return 0U;
        if (mapped.region == 0U) return h.read_hardware(1U, cpu, state, r.program_counter, a & 0xfffffeU, mask);
        return h.read_memory_word(mapped.region, mapped.offset, mask);
    }
    void write(std::uint32_t a, std::uint32_t v, std::uint16_t mask) {
        const auto mapped = resolve(a);
        if (unresolved_bus) return;
        if (mapped.region == 0U) {
            const auto data = static_cast<std::uint16_t>(mask == 0xffU ? (v & 0xffU) * 0x101U
                : mask == 0xff00U ? ((v >> 8U) & 0xffU) * 0x101U : v);
            h.write_hardware(2U, cpu, state, r.program_counter, a & 0xfffffeU, data, mask);
            if (((a & 0x00e001fcU) == 0x00800100U) && (mask & 0xffU))
                h.write_hardware(3U, cpu, state, r.program_counter,
                    0x800100U + ((a >> 1U) & 1U), data & 0xffU, 0xffU);
            return;
        }
        h.write_memory_word(mapped.region, mapped.offset, static_cast<std::uint16_t>(v), mask);
    }
    std::uint16_t word(std::uint32_t a) {
        return read(a, 0xffffU);
    }
    void word(std::uint32_t a, std::uint32_t v) {
        write(a, v, 0xffffU);
    }
    std::uint8_t byte(std::uint32_t a) {
        const auto v = read(a & ~1U, (a & 1U) ? 0xffU : 0xff00U);
        return static_cast<std::uint8_t>((a & 1U) ? v : v >> 8U);
    }
    void byte(std::uint32_t a, std::uint32_t v) {
        write(a & ~1U,
            static_cast<std::uint16_t>((a & 1U) ? v & 0xffU : (v & 0xffU) << 8U),
            (a & 1U) ? 0xffU : 0xff00U);
    }
    std::uint32_t lng(std::uint32_t a) {
        const auto hi = word(a); const auto lo = word(a + 2U);
        return (static_cast<std::uint32_t>(hi) << 16U) | lo;
    }
    void lng(std::uint32_t a, std::uint32_t v) {
        word(a, v >> 16U); word(a + 2U, v);
    }
    void logic(std::uint32_t v, unsigned bits) {
        const auto mask = bits == 32U ? 0xffffffffU : (1U << bits) - 1U;
        v &= mask;
        r.status = static_cast<std::uint16_t>((r.status & ~0x0fU) |
            (v == 0U ? 4U : 0U) | (v & (1U << (bits - 1U)) ? 8U : 0U));
    }
    std::uint32_t add(std::uint32_t a, std::uint32_t b, unsigned bits) {
        const auto mask = bits == 32U ? 0xffffffffU : (1U << bits) - 1U;
        const auto sign = 1U << (bits - 1U);
        a &= mask; b &= mask;
        const auto v = (a + b) & mask;
        const bool carry = static_cast<std::uint64_t>(a) + b > mask;
        r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) |
            (carry ? 0x11U : 0U) | ((~(a ^ b) & (a ^ v) & sign) ? 2U : 0U) |
            (v == 0U ? 4U : 0U) | (v & sign ? 8U : 0U));
        return v;
    }
    void dw(unsigned n, std::uint32_t v) {
        r.data[n] = (r.data[n] & 0xffff0000U) | (v & 0xffffU);
    }
    void db(unsigned n, std::uint32_t v) {
        r.data[n] = (r.data[n] & 0xffffff00U) | (v & 0xffU);
    }
    std::uint32_t sub(std::uint32_t a, std::uint32_t b, unsigned bits, bool compare = false) {
        const auto mask = bits == 32U ? 0xffffffffU : (1U << bits) - 1U;
        const auto sign = 1U << (bits - 1U);
        a &= mask; b &= mask;
        const auto v = (a - b) & mask;
        const bool carry = a < b;
        r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) |
            (compare ? r.status & 0x10U : carry ? 0x10U : 0U) |
            (carry ? 1U : 0U) | (((a ^ b) & (a ^ v) & sign) ? 2U : 0U) |
            (v == 0U ? 4U : 0U) | (v & sign ? 8U : 0U));
        return v;
    }
    void asl_word(unsigned n, unsigned count) {
        auto v = r.data[n] & 0xffffU;
        bool carry = false, overflow = false;
        for (unsigned i = 0; i < count; ++i) {
            carry = (v & 0x8000U) != 0U;
            const auto shifted = (v << 1U) & 0xffffU;
            overflow = overflow || ((v ^ shifted) & 0x8000U) != 0U;
            v = shifted;
        }
        dw(n, v);
        r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) |
            (count == 0U ? r.status & 0x10U : carry ? 0x10U : 0U) |
            (carry ? 1U : 0U) | (overflow ? 2U : 0U) |
            (v == 0U ? 4U : 0U) | (v & 0x8000U ? 8U : 0U));
    }
    void shift_word(unsigned n, unsigned count, bool left, bool arithmetic) {
        auto value = r.data[n] & 0xffffU;
        bool carry = false;
        for (unsigned i = 0U; i < count; ++i) {
            carry = (value & (left ? 0x8000U : 1U)) != 0U;
            value = left ? (value << 1U) & 0xffffU
                         : (value >> 1U) | (arithmetic ? value & 0x8000U : 0U);
        }
        dw(n, value);
        const auto extend = count == 0U ? r.status & 0x10U : carry ? 0x10U : 0U;
        r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | extend |
            (carry ? 1U : 0U) | (value == 0U ? 4U : 0U) | (value & 0x8000U ? 8U : 0U));
    }
    void rotate_right(unsigned n, unsigned count, unsigned bits) {
        const auto mask = bits == 32U ? 0xffffffffU : (1U << bits) - 1U;
        auto value = r.data[n] & mask;
        bool carry = false;
        for (unsigned i = 0; i < count; ++i) {
            carry = (value & 1U) != 0U;
            value = (value >> 1U) | (carry ? 1U << (bits - 1U) : 0U);
        }
        r.data[n] = (r.data[n] & ~mask) | value;
        logic(value, bits);
        r.status = static_cast<std::uint16_t>((r.status & ~1U) | (carry ? 1U : 0U));
    }
    void divide(unsigned n, std::uint32_t raw_divisor, bool signed_division) {
        // Flag behavior follows the pinned Musashi DIVS/DIVU instruction handlers.
        r.status = static_cast<std::uint16_t>(r.status & ~1U);
        if (signed_division) {
            const auto divisor = static_cast<std::int16_t>(raw_divisor);
            const auto dividend = static_cast<std::int32_t>(r.data[n]);
            if (dividend == (-2147483647 - 1) && divisor == -1) {
                r.data[n] = 0U; logic(0U, 16U); return;
            }
            const auto quotient = static_cast<std::int64_t>(dividend) / divisor;
            const auto remainder = static_cast<std::int64_t>(dividend) % divisor;
            if (quotient < -32768 || quotient > 32767) { r.status |= 2U; return; }
            r.data[n] = (static_cast<std::uint32_t>(remainder) << 16U) | (static_cast<std::uint32_t>(quotient) & 0xffffU);
            logic(static_cast<std::uint32_t>(quotient), 16U);
        } else {
            const auto divisor = raw_divisor & 0xffffU;
            const auto quotient = r.data[n] / divisor;
            const auto remainder = r.data[n] % divisor;
            if (quotient > 0xffffU) { r.status |= 2U; return; }
            r.data[n] = (remainder << 16U) | quotient;
            logic(quotient, 16U);
        }
    }
    std::uint32_t sbcd(std::uint32_t dst, std::uint32_t src) {
        auto result = (dst & 0xfU) - (src & 0xfU) - ((r.status >> 4U) & 1U);
        const auto correction = result > 0xfU ? 6U : 0U;
        result += (dst & 0xf0U) - (src & 0xf0U);
        const auto before = result;
        bool carry = false;
        if (result > 0xffU) { result += 0xa0U; carry = true; }
        else if (result < correction) carry = true;
        result = (result - correction) & 0xffU;
        r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) |
            (carry ? 0x11U : 0U) | ((before & ~result & 0x80U) ? 2U : 0U) |
            (result & 0x80U ? 8U : 0U) | (result == 0U ? r.status & 4U : 0U));
        return result;
    }
    FunctionResult call(FunctionContext &c, std::uint32_t id, std::uint32_t site,
                        std::uint32_t target, std::uint32_t next) {
        r.address[7] -= 4U; lng(r.address[7], next);
        r.program_counter = target;
        // IRQ entry may occur after BSR saves its return address but before
        // the child starts. The IRQ frame must resume at the child entry.
        if (auto event = interrupt(c, site, target)) return *event;
        return h.call_function(id, cpu, state, 2U, site, target, c);
    }
    FunctionResult call_prefetched(FunctionContext &c, std::uint32_t id, std::uint32_t site,
                                   std::uint32_t target, std::uint32_t next) {
        // m68000 bsr_rel16_df: return high/low words, then target pipeline.
        r.address[7] -= 4U; lng(r.address[7], next);
        (void)word(target); (void)word(target + 2U);
        r.program_counter = target;
        return h.call_function(id, cpu, state, 2U, site, target, c);
    }
    FunctionResult dispatch(FunctionContext &c, std::uint32_t site, std::uint32_t target,
                            std::uint8_t kind, std::uint8_t target_state) {
        target &= 0xffffffU;
        r.program_counter = target;
        if (const auto *function = native_registry::find(cpu, target_state, target))
            return h.call_function(function->id, cpu, target_state, kind, site, target, c);
        return {TranslationStatus::contract_violation, 0U, target};
    }
    FunctionResult indirect_call(FunctionContext &c, std::uint32_t site,
                                 std::uint32_t target, std::uint32_t next) {
        r.address[7] -= 4U; lng(r.address[7], next);
        return dispatch(c, site, target, 2U, state);
    }
    FunctionResult exception(FunctionContext &c, unsigned vector, std::uint32_t site,
                             std::uint32_t next, std::uint8_t kind = 4U) {
        const auto saved = r.status;
        r.address[7] -= 2U; word(r.address[7], next);
        r.address[7] -= 4U; word(r.address[7], saved); word(r.address[7] + 2U, next >> 16U);
        r.status = static_cast<std::uint16_t>((saved & 0x38ffU) | 0x2000U);
        const auto target = lng(vector * 4U);
        const auto target_state = static_cast<std::uint8_t>(cpu == 1U ? 0x04U : 0xffU);
        c.state = target_state;
        return dispatch(c, site, target, kind, target_state);
    }
    FunctionResult rte() {
        (void)word(r.address[7]);
        const auto status = word(r.address[7]); r.address[7] += 2U;
        const auto target = lng(r.address[7]); r.address[7] += 4U;
        r.status = status; r.program_counter = target;
        return FunctionResult::complete(2U, target);
    }
    FunctionResult ret() {
        const auto target = lng(r.address[7]); r.address[7] += 4U;
        r.program_counter = target;
        return FunctionResult::complete(1U, target);
    }
    std::optional<FunctionResult> interrupt(FunctionContext &c, std::uint32_t pc,
                                           std::uint32_t next) {
        if (cpu == 1U && state == 0x72U && h.resumes_interrupts_inline())
            return cpu_b_interrupt_boundary(c, pc, next);
        const auto event = h.consume_pending_interrupt(cpu, state, pc);
        if (!event.asserted || event.level <= ((r.status >> 8U) & 7U)) return std::nullopt;
        std::uint32_t target;
        if (cpu == 0U && h.resumes_interrupts_inline()) {
            const auto entered = enter_cpu_a_autovector(c, event.level, next,
                [&](std::uint32_t address) -> std::optional<std::uint16_t> {
                    const auto value = word(address);
                    if (unresolved_bus) return std::nullopt;
                    return static_cast<std::uint16_t>(value);
                },
                [&](std::uint32_t address, std::uint16_t value) {
                    word(address, value);
                    return !unresolved_bus;
                });
            if (entered.status != TranslationStatus::complete) return entered;
            if (unresolved_bus)
                return FunctionResult{TranslationStatus::contract_violation, 0U, r.program_counter};
            target = entered.exit_program_counter;
        } else {
            const auto status = r.status;
            r.address[7] -= 4U; word(r.address[7] + 2U, next);
            r.address[7] -= 2U; word(r.address[7], status); word(r.address[7] + 2U, next >> 16U);
            r.status = static_cast<std::uint16_t>((status & 0x38ffU) | 0x2000U | (event.level << 8U));
            target = lng((24U + event.level) * 4U);
        }
        r.program_counter = target;
        const auto target_state = static_cast<std::uint8_t>(cpu == 1U ? 0x04U : 0xffU);
        // The host validates the transition before committing the destination state.
        // Preserve the source state in c until that validation has completed.
        const auto result = dispatch(c, pc, target, 6U, target_state);
        if (result.status != TranslationStatus::complete) return result;
        if (h.resumes_interrupts_inline()) {
            // The ISR already ran to RTE. Continue this C++ instruction/call,
            // including a BSR whose return address was pushed before the IRQ.
            // Unwinding here would lose that pending native child invocation.
            if (result.control != 2U || r.program_counter != next ||
                result.exit_program_counter != next || c.state != state)
                return FunctionResult{TranslationStatus::contract_violation, result.control, r.program_counter};
            return std::nullopt;
        }
        return FunctionResult::complete(5U, target);
    }
};
}

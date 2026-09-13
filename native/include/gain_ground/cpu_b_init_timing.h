#pragma once
#include "gain_ground/sound_caller_timing.h"
#include "gain_ground/cpu_b_interrupt.h"

namespace gain_ground {
// Implemented but unverified. Ordinary bus phases from the existing pinned
// m68000-sdf.cpp: rmil1/2/3, pinw/pinl, mmiw/mmil, DBcc and RTS.
// CPU-B exception entry owns a separate timing scope in cpu_b_interrupt.h.
class CpuBInitTiming {
public:
    explicit CpuBInitTiming(FunctionContext &context) : c_(context), timing_(context) {
        begin(context.registers.program_counter);
    }
    void prefetch(std::uint32_t address) {
        // FD1094 opcode reads are distinct from encrypted AS_PROGRAM data.
        // This timing scope remains explicitly unobserved when the host has
        // no opcode view; do not substitute a data-RAM read for that fetch.
        std::uint16_t value{};
        (void)c_.host->read_timing_program_word(1U, c_.state, address, value);
        clocks(4U);
    }
    void sequential(std::uint32_t pc, unsigned words) {
        for (unsigned i = 0; i < words; ++i) prefetch(pc + 4U + i * 2U);
    }
    void logic(std::uint32_t value, unsigned bits) {
        const auto sign = bits == 8U ? 0x80U : bits == 16U ? 0x8000U : 0x80000000U;
        if (bits != 32U) value &= bits == 8U ? 0xffU : 0xffffU;
        auto &sr = c_.registers.status;
        sr = static_cast<std::uint16_t>((sr & ~0xfU) |
            (value == 0U ? 4U : 0U) | ((value & sign) ? 8U : 0U));
    }
    void immediate_word(unsigned reg, std::uint16_t value) {
        auto &d = c_.registers.data[reg];
        d = (d & 0xffff0000U) | value;
        logic(value, 16U);
    }
    void immediate_byte(unsigned reg, std::uint8_t value) {
        auto &d = c_.registers.data[reg];
        d = (d & 0xffffff00U) | value;
        logic(value, 8U);
    }
    void move_immediate_word(std::uint32_t pc, unsigned reg, std::uint16_t value) {
        prefetch(pc + 4U); immediate_word(reg, value); prefetch(pc + 6U);
    }
    void move_immediate_byte(std::uint32_t pc, unsigned reg, std::uint8_t value) {
        prefetch(pc + 4U); immediate_byte(reg, value); prefetch(pc + 6U);
    }
    void lea_absolute_long(std::uint32_t pc, unsigned reg, std::uint32_t value) {
        auto &address = c_.registers.address[reg];
        address = (address & 0xffffU) | (value & 0xffff0000U);
        prefetch(pc + 4U); address = value;
        prefetch(pc + 6U); prefetch(pc + 8U);
    }
    void moveq(unsigned reg, std::int8_t value, std::uint32_t pc) {
        auto &d = c_.registers.data[reg];
        d = static_cast<std::uint32_t>(static_cast<std::int32_t>(value));
        logic(d, 32U); sequential(pc, 1U);
    }
    void add(unsigned reg, std::uint32_t amount, unsigned bits) {
        auto &r = c_.registers;
        const auto mask = bits == 8U ? 0xffU : 0xffffU;
        const auto sign = bits == 8U ? 0x80U : 0x8000U;
        const auto before = r.data[reg] & mask;
        const auto operand = amount & mask;
        const auto sum = before + operand;
        const auto value = sum & mask;
        r.data[reg] = (r.data[reg] & ~mask) | value;
        r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) |
            ((value & sign) ? 8U : 0U) | (value == 0U ? 4U : 0U) |
            ((~(before ^ operand) & (before ^ value) & sign) ? 2U : 0U) |
            (sum > mask ? 0x11U : 0U));
    }
    void clocks(std::uint32_t count) { timing_.clocks(count); }
    std::uint16_t read_word(std::uint16_t region, std::uint32_t offset) {
        return timing_.read_memory_word(region, offset, 0xffffU);
    }
    std::uint8_t read_byte(std::uint16_t region, std::uint32_t offset) {
        const bool odd = (offset & 1U) != 0U;
        const auto value = timing_.read_memory_word(region, offset & ~1U, odd ? 0xffU : 0xff00U);
        return static_cast<std::uint8_t>(odd ? value : value >> 8U);
    }
    void write_word(std::uint16_t region, std::uint32_t offset, std::uint16_t value) {
        timing_.write_memory_word(region, offset, value, 0xffffU);
    }
    void store_long(std::uint16_t region, std::uint32_t offset, std::uint32_t value) {
        timing_.write_memory_word(region, offset, static_cast<std::uint16_t>(value >> 16U), 0xffffU);
        timing_.write_memory_word(region, offset + 2U, static_cast<std::uint16_t>(value), 0xffffU);
    }
    void store_postincrement(std::uint16_t region, std::uint32_t base, std::uint32_t pc,
                             unsigned source_reg = 1U) {
        auto &r = c_.registers;
        const auto destination = r.address[0] - base;
        const auto value = r.data[source_reg];
        write_word(region, destination, static_cast<std::uint16_t>(value >> 16U));
        logic(value, 16U); // rmil2 updates low-word NZVC before the low write.
        write_word(region, destination + 2U, static_cast<std::uint16_t>(value));
        r.address[0] += 4U;
        logic(value, 32U); // rmil3 merges high-word N/Z before final prefetch.
        prefetch(pc + 4U); // MOVE.L D1,(A0)+: two writes, one prefetch.
    }
    std::uint32_t dbf(std::uint32_t pc, std::uint32_t target, unsigned reg = 0U) {
        timing_.clocks(2U);
        prefetch(target); // The original probes the target on the terminal path too.
        auto &d = c_.registers.data[reg];
        const auto value = static_cast<std::uint16_t>(d - 1U);
        d = (d & 0xffff0000U) | value; // DBF does not change CCR.
        if (value != 0xffffU) {
            prefetch(target + 2U);
            return target;
        }
        prefetch(pc + 4U);
        prefetch(pc + 6U);
        return pc + 4U;
    }
    void copy_postincrement(std::uint32_t pc, bool longword) {
        auto &r = c_.registers;
        const auto source = r.address[1];
        if (longword) {
            const auto high = read_word(2U, source);
            // pinl3 commits source postincrement before the low-word read.
            r.address[1] += 4U;
            const auto low = read_word(2U, source + 2U);
            const auto value = (std::uint32_t(high) << 16U) | low;
            const auto destination = r.address[0];
            logic(low, 16U);
            write_word(2U, destination, high);
            logic(value, 32U);
            write_word(2U, destination + 2U, low);
            r.address[0] += 4U;
        } else {
            // pinw2 commits the word source postincrement before its read.
            r.address[1] += 2U;
            const auto value = read_word(2U, source);
            logic(value, 16U);
            write_word(2U, r.address[0], value);
            r.address[0] += 2U;
        }
        prefetch(pc + 4U);
    }
    std::uint32_t rts() {
        auto &r = c_.registers;
        const auto high = timing_.read_memory_word(2U, r.address[7], 0xffffU);
        const auto low = timing_.read_memory_word(2U, r.address[7] + 2U, 0xffffU);
        r.address[7] += 4U;
        const auto target = (std::uint32_t(high) << 16U) | low;
        prefetch(target); prefetch(target + 2U);
        return target;
    }
    FunctionResult bsr(std::uint32_t id, std::uint32_t pc, std::uint32_t target) {
        auto &r = c_.registers;
        const auto next = pc + 4U;
        const auto saved_sp = r.address[7];
        // bsrw1/2/3: two internal clocks, high/low return stores, target fetches.
        clocks(2U);
        r.address[7] -= 4U;
        store_long(2U, r.address[7], next);
        prefetch(target); prefetch(target + 2U);
        if (auto result = boundary(pc, target)) return *result;
        timing_.stop();
        const auto child = c_.host->call_function(id, 1U, 0x72U, 2U, pc, target, c_);
        if (child.status != TranslationStatus::complete || child.control != 1U) return child;
        if (child.exit_program_counter != next ||
            r.program_counter != next || r.address[7] != saved_sp)
            return {TranslationStatus::contract_violation, child.control, r.program_counter};
        begin(next);
        return child;
    }
    FunctionResult branch(std::uint32_t id, std::uint32_t pc, std::uint32_t target) {
        clocks(2U); prefetch(target); prefetch(target + 2U);
        if (auto result = boundary(pc, target)) return *result;
        timing_.stop();
        return c_.host->call_function(id, 1U, 0x72U, 1U, pc, target, c_);
    }
    std::optional<FunctionResult> boundary(std::uint32_t pc, std::uint32_t next) {
        auto &host = *c_.host;
        auto &r = c_.registers;
        const auto interrupt = host.consume_pending_interrupt(1U, 0x72U, pc);
        if (!interrupt.asserted || interrupt.level <= ((r.status >> 8U) & 7U)) {
            r.program_counter = next;
            return std::nullopt;
        }
        // Suspend this deadline across the real child. Its elapsed time must
        // not be included in, or skipped by, the resumed parent's deadline.
        timing_.stop();
        const auto result = service_cpu_b_autovector(c_, interrupt.level, pc, next);
        if (result.status != TranslationStatus::complete || result.control == 3U) return result;
        if (!host.resumes_interrupts_inline()) return result;
        begin(next);
        return std::nullopt;
    }
private:
    void begin(std::uint32_t pc) {
        timing_.begin(pc, "CPU-B initialization bus timing; complete instruction/exception trace coverage remains open");
    }
    FunctionContext &c_;
    SoundCallerTiming timing_;
};
} // namespace gain_ground

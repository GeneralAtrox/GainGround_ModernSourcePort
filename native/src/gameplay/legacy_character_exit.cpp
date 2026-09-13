// Implemented but unverified. Original F205 exit-tile and companion handling.
#include "legacy_gameplay_bridge.h"
#include "gain_ground/gameplay/character_exit.h"
#include "../translated/unverified_cpu_b_machine.h"

namespace gain_ground::gameplay {
namespace {
class LegacyExit final : public CharacterExit {
public:
    explicit LegacyExit(FunctionContext &context)
        : context_(context), host_(*context.host), registers_(context.registers),
          effects_{host_, registers_, 1U, 0x72U} {}

    ExitProbe probe_exit() override {
        const auto x = word(record() + 0x12U);
        effects_.dw(0U, x); effects_.logic(x, 16U);
        const auto y = word(record() + 0x1aU);
        effects_.dw(1U, y); effects_.logic(y, 16U);
        if (!call(287U, 0x1037cU, 0x15edeU, 0x10382U)) return ExitProbe::interrupted;
        const auto address = registers_.address[0];
        const bool shared_alias = (address & 0xffff0000U) == 0xffff0000U;
        const auto offset = shared_alias ? 0x30000U | (address & 0xffffU) : address & mask_;
        const auto marker = byte_at(shared_alias ? 3U : 2U, offset);
        const bool inside = (marker & 8U) != 0U;
        registers_.status = static_cast<std::uint16_t>(
            (registers_.status & ~4U) | (inside ? 0U : 4U));
        return inside ? ExitProbe::inside : ExitProbe::outside;
    }

    bool leave_stage() override {
        word(record() + 0x44U, 7U); effects_.logic(7U, 16U);
        if (!call(197U, 0x1038eU, 0xfd10U, 0x10392U)) return false;
        word(record() + 0x52U, 0xffffU); effects_.logic(0xffffU, 16U);
        const auto type = byte(record() + 0x4bU);
        effects_.db(2U, type); effects_.logic(type, 8U);
        return true;
    }

    bool transfer_companion() override {
        const auto linked = word(record() + 0x5cU);
        effects_.logic(linked, 16U);
        if (linked == 0U) return false;
        const auto pointer = word(record() + 0x5cU);
        registers_.address[6] = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(static_cast<std::int16_t>(pointer)));
        word(registers_.address[6] + 0x44U, 3U); effects_.logic(3U, 16U);
        auto position = read_long(record() + 0x12U);
        write_long(registers_.address[6] + 0x62U, position); effects_.logic(position, 32U);
        position = read_long(record() + 0x1aU);
        write_long(registers_.address[6] + 0x66U, position); effects_.logic(position, 32U);
        return true;
    }

    bool roster_has_room() override {
        const auto count = byte(player() + 0x40U);
        (void)effects_.sub(count, 0x3eU, 8U, true);
        return count < 0x3eU;
    }

    void record_rescue() override {
        const auto rescued = word(0xc12U);
        word(0xc12U, static_cast<std::uint16_t>(rescued + 1U));
        (void)effects_.add(rescued, 1U, 16U);
        const auto count = byte(player() + 0x40U);
        byte(player() + 0x40U, static_cast<std::uint8_t>(count + 1U));
        (void)effects_.add(count, 1U, 8U);

        auto decimal = byte(player() + 0x41U);
        effects_.db(0U, decimal); effects_.logic(decimal, 8U);
        registers_.data[1] = 1U; effects_.logic(1U, 32U);
        registers_.status = static_cast<std::uint16_t>(registers_.status & ~0x1fU);
        decimal = increment_bcd(decimal);
        effects_.db(0U, decimal);
        byte(player() + 0x41U, decimal); effects_.logic(decimal, 8U);

        const auto current_count = byte(player() + 0x40U);
        const auto index = effects_.add(registers_.data[1], current_count, 8U);
        effects_.db(1U, index);
        const auto type = byte(registers_.address[6] + 0x4bU);
        const auto destination = player() + 0x40U + static_cast<std::int16_t>(
            static_cast<std::uint16_t>(registers_.data[1]));
        byte(destination, type); effects_.logic(type, 8U);
    }

    FunctionResult finish() {
        const auto target = read_long(registers_.address[7]);
        registers_.address[7] += 4U;
        registers_.program_counter = target;
        return FunctionResult::complete(1U, target);
    }
    FunctionResult child_result() const noexcept { return child_; }

private:
    static constexpr std::uint32_t mask_ = 0x3ffffU;
    FunctionContext &context_;
    ExecutionHost &host_;
    CpuRegisters &registers_;
    translated::unverified::Machine effects_;
    FunctionResult child_;

    std::uint32_t record() const noexcept { return registers_.address[5]; }
    std::uint32_t player() const noexcept { return registers_.address[4]; }
    std::uint16_t word(std::uint32_t address) {
        return host_.read_memory_word(2U, address & mask_, 0xffffU);
    }
    void word(std::uint32_t address, std::uint16_t value) {
        host_.write_memory_word(2U, address & mask_, value, 0xffffU);
    }
    std::uint8_t byte_at(std::uint16_t region, std::uint32_t address) {
        const auto mask = static_cast<std::uint16_t>((address & 1U) ? 0xffU : 0xff00U);
        return static_cast<std::uint8_t>(host_.read_memory_word(region, address & ~1U, mask)
            >> ((address & 1U) ? 0U : 8U));
    }
    std::uint8_t byte(std::uint32_t address) { return byte_at(2U, address & mask_); }
    void byte(std::uint32_t address, std::uint8_t value) {
        const auto mask = static_cast<std::uint16_t>((address & 1U) ? 0xffU : 0xff00U);
        const auto data = static_cast<std::uint16_t>(static_cast<unsigned>(value)
            << ((address & 1U) ? 0U : 8U));
        host_.write_memory_word(2U, (address & mask_) & ~1U, data, mask);
    }
    std::uint32_t read_long(std::uint32_t address) {
        const auto high = word(address);
        const auto low = word(address + 2U);
        return (static_cast<std::uint32_t>(high) << 16U) | low;
    }
    void write_long(std::uint32_t address, std::uint32_t value) {
        word(address, static_cast<std::uint16_t>(value >> 16U));
        word(address + 2U, static_cast<std::uint16_t>(value));
    }
    bool call(std::uint32_t function, std::uint32_t site,
              std::uint32_t target, std::uint32_t continuation) {
        registers_.address[7] -= 4U;
        write_long(registers_.address[7], continuation);
        registers_.program_counter = target;
        child_ = host_.call_function(function, 1U, 0x72U, 2U, site, target, context_);
        return child_.status == TranslationStatus::complete && child_.control == 1U;
    }
    std::uint8_t increment_bcd(std::uint8_t value) {
        const auto extend = static_cast<unsigned>((registers_.status & 0x10U) != 0U);
        const auto binary = static_cast<unsigned>(value) + 1U + extend;
        auto adjusted = binary;
        if ((value & 15U) + 1U + extend > 9U) adjusted += 6U;
        const bool carry = adjusted > 0x99U;
        if (carry) adjusted += 0x60U;
        const auto result = static_cast<std::uint8_t>(adjusted);
        const auto flags = (carry ? 0x11U : 0U) | ((result & 0x80U) ? 8U : 0U)
            | (result == 0U ? registers_.status & 4U : 0U)
            | (((~binary) & adjusted & 0x80U) ? 2U : 0U);
        registers_.status = static_cast<std::uint16_t>((registers_.status & ~0x1fU) | flags);
        return result;
    }
};
} // namespace

bool run_character_exit(const Character &character, FunctionContext &context,
                         FunctionResult &result)
{
    LegacyExit exit(context);
    result = character.process_exit(exit) ? exit.finish() : exit.child_result();
    return true;
}

} // namespace gain_ground::gameplay

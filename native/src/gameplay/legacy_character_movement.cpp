// Implemented but unverified. Source: F200 FE54 movement/animation preparation.
#include "legacy_gameplay_bridge.h"
#include "gain_ground/gameplay/character_movement.h"
#include "../translated/unverified_cpu_b_machine.h"

namespace gain_ground::gameplay {
namespace {
class LegacyMovement final : public CharacterMovement {
public:
    explicit LegacyMovement(FunctionContext &context)
        : host_(*context.host), registers_(context.registers),
          effects_{host_, registers_, 1U, 0x72U} {}

    void clear_pending_motion() override {
        clear_long(record() + 0x1eU);
        clear_long(record() + 0x26U);
    }

    std::int16_t read_direction() override {
        // Input byte +8B -> direction table 107E4. Negative entries mean
        // no movement; keep the original combination handling in the table.
        effects_.db(0U, byte(registers_.address[4] + 0x8bU));
        auto index = static_cast<std::uint16_t>(registers_.data[0] & 0xf0U);
        effects_.logic(index, 16U);
        const bool carry = (index & 4U) != 0U;
        index = static_cast<std::uint16_t>(index >> 3U);
        effects_.dw(0U, index);
        effects_.logic(index, 16U);
        registers_.status = static_cast<std::uint16_t>(
            (registers_.status & ~0x11U) | (carry ? 0x11U : 0U));
        registers_.address[0] = 0x107e4U + static_cast<std::int16_t>(index);
        const auto direction = word(registers_.address[0]);
        effects_.dw(0U, direction);
        effects_.logic(direction, 16U);
        return static_cast<std::int16_t>(direction);
    }

    void select_direction(std::int16_t direction) override {
        word(record() + 0x58U, static_cast<std::uint16_t>(direction));
        effects_.asl_word(0U, 2U);
        registers_.address[0] = 0x10804U + signed_index();
    }

    void select_movement_profile() override {
        const auto profile = word(record() + 0x36U);
        effects_.dw(0U, profile);
        effects_.logic(profile, 16U);
        effects_.asl_word(0U, 4U);
        registers_.address[1] = 0x10824U + signed_index();
    }

    bool read_axis_index() override {
        const auto index = word(registers_.address[0]);
        registers_.address[0] += 2U;
        effects_.dw(0U, index);
        effects_.logic(index, 16U);
        return (index & 0x8000U) == 0U;
    }

    void load_pending_motion(MovementAxis axis) override {
        const auto value = read_long(registers_.address[1] + signed_index());
        const auto destination = record() + (axis == MovementAxis::x ? 0x1eU : 0x26U);
        word(destination, static_cast<std::uint16_t>(value >> 16U));
        word(destination + 2U, static_cast<std::uint16_t>(value));
        effects_.logic(value, 32U);
    }

    bool already_idle_pose() override {
        const auto phase = byte(record() + 0x3dU);
        effects_.db(0U, phase);
        const auto decremented = effects_.sub(phase, 1U, 8U);
        effects_.db(0U, decremented);
        return decremented == 0U;
    }

    bool attack_active() override {
        const auto mode = byte(record() + 0x3eU);
        effects_.logic(mode, 8U);
        return mode != 0U;
    }

    bool advance_walk_clock() override {
        const auto previous = byte(record() + 0x3cU);
        const auto counter = static_cast<std::uint8_t>(previous + 1U);
        byte(record() + 0x3cU, counter);
        (void)effects_.add(previous, 1U, 8U);
        const auto profile = word(record() + 0x36U);
        effects_.dw(0U, profile);
        effects_.logic(profile, 16U);
        registers_.address[0] = 0x108d4U;
        const auto period = byte(registers_.address[0] + static_cast<std::int16_t>(profile));
        effects_.db(0U, period);
        effects_.logic(period, 8U);
        const auto current = byte(record() + 0x3cU);
        (void)effects_.sub(period, current, 8U, true);
        return static_cast<std::int8_t>(period) <= static_cast<std::int8_t>(current);
    }

    void advance_walk_pose() override {
        (void)byte(record() + 0x3cU);
        byte(record() + 0x3cU, 0U);
        effects_.logic(0U, 8U);
        const auto previous = byte(record() + 0x3dU);
        effects_.db(0U, previous);
        const auto incremented = effects_.add(previous, 1U, 8U);
        effects_.db(0U, incremented);
        const auto phase = static_cast<std::uint8_t>(incremented & 3U);
        effects_.db(0U, phase);
        effects_.logic(phase, 8U);
        byte(record() + 0x3dU, phase);
        effects_.logic(phase, 8U);
    }

    FunctionResult finish() {
        const auto target = read_long(registers_.address[7]);
        registers_.address[7] += 4U;
        registers_.program_counter = target;
        return FunctionResult::complete(1U, target);
    }

private:
    ExecutionHost &host_;
    CpuRegisters &registers_;
    translated::unverified::Machine effects_;

    std::uint32_t record() const noexcept { return registers_.address[5]; }
    std::int16_t signed_index() const noexcept {
        return static_cast<std::int16_t>(static_cast<std::uint16_t>(registers_.data[0]));
    }
    std::uint16_t word(std::uint32_t address) {
        return host_.read_memory_word(2U, address, 0xffffU);
    }
    void word(std::uint32_t address, std::uint16_t value) {
        host_.write_memory_word(2U, address, value, 0xffffU);
    }
    std::uint8_t byte(std::uint32_t address) {
        const auto mask = static_cast<std::uint16_t>((address & 1U) ? 0xffU : 0xff00U);
        return static_cast<std::uint8_t>(host_.read_memory_word(2U, address & ~1U, mask)
            >> ((address & 1U) ? 0U : 8U));
    }
    void byte(std::uint32_t address, std::uint8_t value) {
        const auto mask = static_cast<std::uint16_t>((address & 1U) ? 0xffU : 0xff00U);
        const auto data = static_cast<std::uint16_t>(static_cast<unsigned>(value)
            << ((address & 1U) ? 0U : 8U));
        host_.write_memory_word(2U, address & ~1U, data, mask);
    }
    std::uint32_t read_long(std::uint32_t address) {
        const auto high = word(address);
        const auto low = word(address + 2U);
        return (static_cast<std::uint32_t>(high) << 16U) | low;
    }
    void clear_long(std::uint32_t address) {
        (void)word(address); (void)word(address + 2U);
        word(address + 2U, 0U); word(address, 0U);
        effects_.logic(0U, 32U);
    }
};
} // namespace

bool run_character_movement(const Character &character, FunctionContext &context,
                             FunctionResult &result)
{
    LegacyMovement movement(context);
    character.prepare_movement(movement);
    result = movement.finish();
    return true;
}

} // namespace gain_ground::gameplay

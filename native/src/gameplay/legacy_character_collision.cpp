// Implemented but unverified. F204's original bounds, tile probes and bus order.
#include "legacy_gameplay_bridge.h"
#include "gain_ground/gameplay/character_collision.h"
#include "../translated/unverified_cpu_b_machine.h"

namespace gain_ground::gameplay {
namespace {
class LegacyCollision final : public CharacterCollision {
public:
    explicit LegacyCollision(FunctionContext &context)
        : context_(context), host_(*context.host), registers_(context.registers),
          effects_{host_, registers_, 1U, 0x72U}, record_(registers_.address[5]) {}

    bool begin_axis(MovementAxis axis) override {
        vertical_ = axis == MovementAxis::y;
        pending_ = read_long(record_ + pending_offset());
        registers_.data[axis_register()] = pending_;
        effects_.logic(pending_, 32U);
        return pending_ != 0U;
    }

    bool inside_leading_bound() override {
        const bool positive = (pending_ & 0x80000000U) == 0U;
        const auto position = read_long(record_ + position_offset());
        const auto sum = effects_.add(pending_, position, 32U);
        const auto swapped = (sum << 16U) | (sum >> 16U);
        effects_.logic(swapped, 32U);
        const auto integer = static_cast<std::uint16_t>(swapped);
        const auto edge = positive ? effects_.add(integer, 6U, 16U)
            : effects_.sub(integer, vertical_ ? 5U : 7U, 16U);
        registers_.data[axis_register()] = (swapped & 0xffff0000U) | edge;
        const auto bound = positive ? (vertical_ ? 0x1bfU : 0x17cU)
            : (vertical_ ? 8U : 4U);
        (void)effects_.sub(edge, bound, 16U, true);
        const auto signed_edge = static_cast<std::int16_t>(static_cast<std::uint16_t>(edge));
        return positive ? signed_edge <= static_cast<std::int16_t>(bound)
                        : signed_edge >= static_cast<std::int16_t>(bound);
    }

    CollisionProbe probe_corner(unsigned corner) override {
        const auto perpendicular_register = vertical_ ? 0U : 1U;
        const auto position = host_.read_memory_word(2U,
            record_ + (vertical_ ? 0x12U : 0x1aU), 0xffffU);
        effects_.dw(perpendicular_register, position);
        effects_.logic(position, 16U);
        // Y: left then right. X: bottom then top.
        const bool positive = vertical_ ? corner == 1U : corner == 0U;
        const auto edge = positive ? effects_.add(position, 6U, 16U)
            : effects_.sub(position, vertical_ ? 7U : 5U, 16U);
        effects_.dw(perpendicular_register, edge);
        const auto bound = positive ? (vertical_ ? 0x17cU : 0x1bfU)
            : (vertical_ ? 4U : 8U);
        (void)effects_.sub(edge, bound, 16U, true);
        const auto signed_edge = static_cast<std::int16_t>(static_cast<std::uint16_t>(edge));
        if (positive ? signed_edge > static_cast<std::int16_t>(bound)
                     : signed_edge < static_cast<std::int16_t>(bound))
            return CollisionProbe::blocked;

        const std::uint32_t site = vertical_ ? (corner == 0U ? 0x102dcU : 0x102f4U)
                                             : (corner == 0U ? 0x10340U : 0x10358U);
        const auto continuation = site + 6U;
        registers_.address[7] -= 4U;
        host_.write_memory_word(2U, registers_.address[7] & mask_,
            static_cast<std::uint16_t>(continuation >> 16U), 0xffffU);
        host_.write_memory_word(2U, (registers_.address[7] + 2U) & mask_,
            static_cast<std::uint16_t>(continuation), 0xffffU);
        registers_.program_counter = 0x15edeU;
        child_ = host_.call_function(287U, 1U, 0x72U, 2U,
            site, 0x15edeU, context_);
        if (child_.status != TranslationStatus::complete || child_.control != 1U)
            return CollisionProbe::interrupted;
        const auto address = registers_.address[0];
        const auto memory_mask = static_cast<std::uint16_t>((address & 1U) ? 0xffU : 0xff00U);
        const auto marker = static_cast<std::uint8_t>(host_.read_memory_word(
            address <= mask_ ? 2U : 3U, (address & mask_) & ~1U, memory_mask)
            >> ((address & 1U) ? 0U : 8U));
        const bool blocked = (marker & 1U) != 0U;
        registers_.status = static_cast<std::uint16_t>(
            (registers_.status & ~4U) | (blocked ? 0U : 4U));
        return blocked ? CollisionProbe::blocked : CollisionProbe::clear;
    }

    void reject_motion() override {
        const auto address = record_ + pending_offset();
        (void)read_long(address);
        write_long(address, 0U);
        effects_.logic(0U, 32U);
    }

    void commit_motion() override {
        const auto pending = read_long(record_ + pending_offset());
        registers_.data[0] = pending;
        effects_.logic(pending, 32U);
        const auto position = read_long(record_ + position_offset());
        write_long(record_ + position_offset(), position + pending);
        (void)effects_.add(position, pending, 32U);
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
    std::uint32_t record_;
    bool vertical_{};
    std::uint32_t pending_{};
    FunctionResult child_;

    unsigned axis_register() const noexcept { return vertical_ ? 1U : 0U; }
    std::uint32_t pending_offset() const noexcept { return vertical_ ? 0x26U : 0x1eU; }
    std::uint32_t position_offset() const noexcept { return vertical_ ? 0x1aU : 0x12U; }
    std::uint32_t read_long(std::uint32_t address) {
        const auto high = host_.read_memory_word(2U, address & mask_, 0xffffU);
        const auto low = host_.read_memory_word(2U, (address + 2U) & mask_, 0xffffU);
        return (static_cast<std::uint32_t>(high) << 16U) | low;
    }
    void write_long(std::uint32_t address, std::uint32_t value) {
        // F204's read-modify-write commits the low word before the high word.
        host_.write_memory_word(2U, (address + 2U) & mask_,
            static_cast<std::uint16_t>(value), 0xffffU);
        host_.write_memory_word(2U, address & mask_,
            static_cast<std::uint16_t>(value >> 16U), 0xffffU);
    }
};
} // namespace

bool run_character_collision(const Character &character, FunctionContext &context,
                              FunctionResult &result)
{
    LegacyCollision collision(context);
    result = character.commit_motion(collision) ? collision.finish() : collision.child_result();
    return true;
}

} // namespace gain_ground::gameplay

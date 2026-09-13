// Implemented but unverified. Callbacks 11FCC and 12058 are used by the
// original attack descriptors of starting characters 0 and 8.
#include "legacy_gameplay_bridge.h"
#include "gain_ground/gameplay/projectile.h"
#include "../translated/cpu_b_record_deadline_detail.h"

namespace gain_ground::gameplay {
namespace {
namespace legacy = translated::record_deadline_detail;

class LegacyProjectile final : public ProjectileRuntime {
public:
    LegacyProjectile(FunctionContext &context, bool ballistic)
        : context_(context), host_(*context.host), registers_(context.registers),
          ballistic_(ballistic), entry_record_(registers_.address[5]) {}

    ProjectileProbe check_bounds() override {
        const auto entry = ballistic_ ? 0x12058U : 0x11fccU;
        result_ = legacy::call_child(context_, 253U, entry, 0x1283cU, entry + 4U);
        if (!returned()) return ProjectileProbe::stopped;
        expiry_site_ = entry + 4U;
        return (registers_.status & legacy::kCarry) ? ProjectileProbe::expired : ProjectileProbe::clear;
    }

    bool advance_lifetime() override {
        const auto timer = legacy::read_word(host_, record() + 0x46U);
        const auto remaining = static_cast<std::uint16_t>(timer - 1U);
        legacy::write_word(host_, record() + 0x46U, remaining);
        legacy::subtract_word(registers_, timer, 1U, remaining);
        return (registers_.status & legacy::kZero) == 0U
            && ((registers_.status & legacy::kNegative) != 0U)
                == ((registers_.status & legacy::kOverflow) != 0U);
    }

    ProjectileProbe check_contacts() override {
        const auto site = ballistic_ ? 0x12060U : 0x11fdcU;
        result_ = legacy::call_child(context_, ballistic_ ? 258U : 257U,
            site, ballistic_ ? 0x12bb0U : 0x12b32U, site + 4U);
        if (result_.status != TranslationStatus::complete) return ProjectileProbe::stopped;
        // Original contact helpers can consume their caller's return frame.
        // Preserve that completed nonlocal return instead of popping again.
        if (result_.control == 8U) {
            result_ = FunctionResult::complete(1U, result_.exit_program_counter);
            return ProjectileProbe::stopped;
        }
        if (result_.control != 1U) return ProjectileProbe::stopped;
        expiry_site_ = site + 4U;
        return (registers_.status & legacy::kCarry) ? ProjectileProbe::expired : ProjectileProbe::clear;
    }

    void move_xy() override {
        load_motion(0x1eU); add_position(0x12U);
        load_motion(0x26U); add_position(0x1aU);
    }

    void move_height() override {
        load_motion(0x22U);
        const auto velocity = legacy::read_long(host_, record() + 0x22U);
        const auto next_velocity = velocity - 0x4000U;
        legacy::write_long_read_modify_write(host_, record() + 0x22U, next_velocity);
        legacy::subtract_long(registers_, velocity, 0x4000U, next_velocity);
        // D0 still holds the pre-deceleration velocity for this height update.
        add_position(0x16U);
        auto height = legacy::read_word(host_, record() + 0x16U);
        registers_.data[0] = (registers_.data[0] & 0xffff0000U) | height;
        legacy::logic(registers_, height, 0x8000U, 0xffffU);
        const auto shifted = legacy::arithmetic_shift_right_word(registers_, height);
        registers_.data[0] = (registers_.data[0] & 0xffff0000U) | shifted;
        height = static_cast<std::uint16_t>(shifted + 0x3fU);
        registers_.data[0] = (registers_.data[0] & 0xffff0000U) | height;
        legacy::add(registers_, shifted, 0x3fU, height, 0x8000U, 0xffffU);
        const auto value = static_cast<std::uint8_t>(height);
        legacy::write_byte(host_, record() + 0x10U, value);
        legacy::logic(registers_, value, 0x80U, 0xffU);
        legacy::write_byte(host_, record() + 0x11U, value);
        legacy::logic(registers_, value, 0x80U, 0xffU);
    }

    void expire() override {
        if (ballistic_) {
            registers_.program_counter = 0x12d08U;
            result_ = host_.call_function(261U, 1U, 0x72U, 1U,
                expiry_site_, 0x12d08U, context_);
        } else {
            result_ = legacy::expire(context_);
        }
    }

    void publish() override {
        const auto site = ballistic_ ? 0x1209aU : 0x11ff4U;
        result_ = legacy::call_child(context_, 280U, site, 0x15d24U, site + 6U);
        if (!returned()) return;
        result_ = legacy::call_child(context_, 281U, site + 6U, 0x15d3cU, site + 12U);
        if (!returned()) return;
        result_ = legacy::call_child(context_, 282U, site + 12U, 0x15df2U, site + 18U);
        if (!returned()) return;
        result_ = legacy::return_from(context_);
    }

    FunctionResult result() const noexcept { return result_; }

private:
    FunctionContext &context_;
    ExecutionHost &host_;
    CpuRegisters &registers_;
    bool ballistic_;
    std::uint32_t entry_record_;
    std::uint32_t expiry_site_{};
    FunctionResult result_;

    bool returned() const noexcept {
        return result_.status == TranslationStatus::complete && result_.control == 1U;
    }
    std::uint32_t record() const noexcept {
        return ballistic_ ? registers_.address[5] : entry_record_;
    }
    void load_motion(std::uint32_t offset) {
        registers_.data[0] = legacy::read_long(host_, record() + offset);
        legacy::logic(registers_, registers_.data[0], 0x80000000U, 0xffffffffU);
    }
    void add_position(std::uint32_t offset) {
        const auto current = legacy::read_long(host_, record() + offset);
        const auto sum = current + registers_.data[0];
        legacy::write_long_read_modify_write(host_, record() + offset, sum);
        legacy::add(registers_, current, registers_.data[0], sum, 0x80000000U, 0xffffffffU);
    }
};
} // namespace

bool run_projectile_update(FunctionContext &context, FunctionResult &result)
{
    const auto pc = context.registers.program_counter;
    if (pc != 0x11fccU && pc != 0x12058U) return false;
    const bool ballistic = pc == 0x12058U;
    LegacyProjectile runtime(context, ballistic);
    if (ballistic) BallisticProjectile{}.update(runtime);
    else TimedProjectile{}.update(runtime);
    result = runtime.result();
    return true;
}

} // namespace gain_ground::gameplay

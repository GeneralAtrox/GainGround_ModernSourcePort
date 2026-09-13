#pragma once

namespace gain_ground::gameplay {

// Original callback dispatch remains behind the port while attack eligibility,
// cooldowns and animation progression migrate into the shared Attack class.
class AttackPhase {
public:
    virtual ~AttackPhase() = default;
    virtual bool invoke_callbacks() = 0;
    virtual void decrement_cooldown() = 0;
    virtual bool active() = 0;
    virtual bool advance_animation_clock() = 0;
    virtual bool advance_animation_phase() = 0;
    virtual bool animation_complete() = 0;
    virtual void return_to_idle() = 0;
};

} // namespace gain_ground::gameplay

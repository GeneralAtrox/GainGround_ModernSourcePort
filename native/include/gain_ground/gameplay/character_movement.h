#pragma once

#include <cstdint>

namespace gain_ground::gameplay {

enum class MovementAxis { x, y };

// Typed access to the original input/direction/movement tables and live state.
// The legacy implementation preserves the surrounding CPU-visible effects.
class CharacterMovement {
public:
    virtual ~CharacterMovement() = default;
    virtual void clear_pending_motion() = 0;
    virtual std::int16_t read_direction() = 0;
    virtual void select_direction(std::int16_t direction) = 0;
    virtual void select_movement_profile() = 0;
    virtual bool read_axis_index() = 0;
    virtual void load_pending_motion(MovementAxis) = 0;
    virtual bool already_idle_pose() = 0;
    virtual bool attack_active() = 0;
    virtual bool advance_walk_clock() = 0;
    virtual void advance_walk_pose() = 0;
};

} // namespace gain_ground::gameplay

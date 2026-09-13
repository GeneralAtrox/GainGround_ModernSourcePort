#pragma once

#include "gain_ground/gameplay/character_movement.h"

namespace gain_ground::gameplay {

enum class CollisionProbe { clear, blocked, interrupted };

class CharacterCollision {
public:
    virtual ~CharacterCollision() = default;
    virtual bool begin_axis(MovementAxis) = 0;
    virtual bool inside_leading_bound() = 0;
    virtual CollisionProbe probe_corner(unsigned corner) = 0;
    virtual void reject_motion() = 0;
    virtual void commit_motion() = 0;
};

} // namespace gain_ground::gameplay

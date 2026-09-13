#pragma once

namespace gain_ground::gameplay {

enum class HitContact { mark_always, mark_when_protected, leave_unmarked };

class CharacterDamage {
public:
    virtual ~CharacterDamage() = default;
    virtual void mark_contact() = 0;
    virtual bool protected_from_hit() = 0;
    virtual void ignore_hit() = 0;
    virtual void defeat() = 0;
};

} // namespace gain_ground::gameplay

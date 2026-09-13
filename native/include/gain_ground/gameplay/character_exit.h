#pragma once

namespace gain_ground::gameplay {

enum class ExitProbe { outside, inside, interrupted };

class CharacterExit {
public:
    virtual ~CharacterExit() = default;
    virtual ExitProbe probe_exit() = 0;
    virtual bool leave_stage() = 0;
    virtual bool transfer_companion() = 0;
    virtual bool roster_has_room() = 0;
    virtual void record_rescue() = 0;
};

} // namespace gain_ground::gameplay

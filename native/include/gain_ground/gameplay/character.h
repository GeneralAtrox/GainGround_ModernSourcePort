#pragma once

#include <cstdint>

namespace gain_ground::gameplay {

class CharacterMovement;
class AttackPhase;
class CharacterCollision;
class CharacterExit;
class CharacterDamage;
enum class HitContact;

enum class CharacterStep {
    timer, movement, contacts, attacks, commit_motion, transition,
    animation, appearance, finish, stopped
};
enum class AttackSlot { primary, secondary };

// One live state owner: the existing character record. This handle is a view,
// not another writable copy of position, cooldowns or animation state.
struct CharacterState {
    std::uint32_t record_address{};
};

// CPU state and return/interrupt bookkeeping stay behind this migration seam.
// Each operation executes once at the existing scheduler's update boundary.
class CharacterRuntime {
public:
    virtual ~CharacterRuntime() = default;
    virtual CharacterStep step() const noexcept = 0;
    virtual void advance(CharacterStep) = 0;
    virtual bool attack(AttackSlot) = 0;
};

class Attack {
public:
    virtual ~Attack() = default;
    virtual bool update(CharacterRuntime &) const = 0;
    bool advance(AttackPhase &) const;
};

class OriginalPrimaryAttack final : public Attack {
public:
    bool update(CharacterRuntime &runtime) const override {
        return runtime.attack(AttackSlot::primary);
    }
};

class OriginalSecondaryAttack final : public Attack {
public:
    bool update(CharacterRuntime &runtime) const override {
        return runtime.attack(AttackSlot::secondary);
    }
};

class Character {
public:
    virtual ~Character() = default;
    CharacterState state() const noexcept { return state_; }
    virtual std::uint8_t original_id() const noexcept = 0;
    void update(CharacterRuntime &) const;
    bool update_attacks(CharacterRuntime &) const;
    void prepare_movement(CharacterMovement &) const;
    bool advance_attack(AttackSlot, AttackPhase &) const;
    bool commit_motion(CharacterCollision &) const;
    bool process_exit(CharacterExit &) const;
    void take_hit(CharacterDamage &, HitContact) const;

protected:
    explicit Character(CharacterState state) : state_(state) {}
    virtual const Attack &primary_attack() const noexcept;
    virtual const Attack &secondary_attack() const noexcept;

private:
    CharacterState state_;
};

// All twenty original roster types inherit the same lifecycle and attacks.
// Their movement and independent button profiles come from the definition
// table when their live record is initialized; no duplicate mutable state.
template<std::uint8_t Id>
class OriginalCharacter final : public Character {
    static_assert(Id < 20U);
public:
    explicit OriginalCharacter(CharacterState state) : Character(state) {}
    std::uint8_t original_id() const noexcept override { return Id; }
};
using OriginalCharacter00 = OriginalCharacter<0U>;
using OriginalCharacter08 = OriginalCharacter<8U>;

} // namespace gain_ground::gameplay

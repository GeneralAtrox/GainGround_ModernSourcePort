#include "gain_ground/gameplay/character.h"
#include "gain_ground/gameplay/character_movement.h"
#include "gain_ground/gameplay/attack_phase.h"
#include "gain_ground/gameplay/character_collision.h"
#include "gain_ground/gameplay/character_exit.h"
#include "gain_ground/gameplay/character_damage.h"

namespace gain_ground::gameplay {

void Character::take_hit(CharacterDamage &damage, HitContact contact) const
{
    if (contact == HitContact::mark_always) damage.mark_contact();
    if (damage.protected_from_hit()) {
        if (contact == HitContact::mark_when_protected) damage.mark_contact();
        damage.ignore_hit();
    } else {
        damage.defeat();
    }
}

bool Character::process_exit(CharacterExit &exit) const
{
    const auto location = exit.probe_exit();
    if (location == ExitProbe::interrupted) return false;
    if (location == ExitProbe::outside) return true;
    if (!exit.leave_stage()) return false;
    if (exit.transfer_companion() && exit.roster_has_room()) exit.record_rescue();
    return true;
}

bool Character::commit_motion(CharacterCollision &collision) const
{
    // Original F204 resolves Y first. X then sees the committed Y position.
    // Each axis checks its leading edge and then the two corners in order.
    constexpr MovementAxis axes[]{MovementAxis::y, MovementAxis::x};
    for (const auto axis : axes) {
        if (!collision.begin_axis(axis)) continue;
        bool blocked = !collision.inside_leading_bound();
        for (unsigned corner = 0U; corner < 2U && !blocked; ++corner) {
            const auto probe = collision.probe_corner(corner);
            if (probe == CollisionProbe::interrupted) return false;
            blocked = probe == CollisionProbe::blocked;
        }
        if (blocked) collision.reject_motion();
        else collision.commit_motion();
    }
    return true;
}

bool Attack::advance(AttackPhase &phase) const
{
    if (!phase.invoke_callbacks()) return false;
    phase.decrement_cooldown();
    if (!phase.active() || !phase.advance_animation_clock()) return true;
    if (!phase.advance_animation_phase()) return false;
    if (phase.animation_complete()) phase.return_to_idle();
    return true;
}

bool Character::advance_attack(AttackSlot slot, AttackPhase &phase) const
{
    const auto &attack = slot == AttackSlot::primary ? primary_attack() : secondary_attack();
    return attack.advance(phase);
}

void Character::prepare_movement(CharacterMovement &movement) const
{
    movement.clear_pending_motion();
    const auto direction = movement.read_direction();
    if (direction >= 0) {
        movement.select_direction(direction);
        movement.select_movement_profile();
        if (movement.read_axis_index()) movement.load_pending_motion(MovementAxis::x);
        if (movement.read_axis_index()) movement.load_pending_motion(MovementAxis::y);
    } else if (movement.already_idle_pose()) {
        return;
    }

    // Attack phases own the same animation counters while an attack is active.
    if (!movement.attack_active() && movement.advance_walk_clock())
        movement.advance_walk_pose();
}

void Character::update(CharacterRuntime &runtime) const
{
    // F178 FE18..FE3A owns this order. The bridge also supplies the current
    // phase when execution resumes after an original child/interrupt boundary.
    for (;;) {
        const auto current = runtime.step();
        if (current == CharacterStep::stopped) return;
        switch (current) {
        case CharacterStep::timer:
        case CharacterStep::movement:
        case CharacterStep::contacts:
        case CharacterStep::attacks:
        case CharacterStep::commit_motion:
        case CharacterStep::transition:
        case CharacterStep::animation:
        case CharacterStep::appearance:
        case CharacterStep::finish:
            runtime.advance(current);
            break;
        case CharacterStep::stopped: return;
        }
    }
}

bool Character::update_attacks(CharacterRuntime &runtime) const
{
    // Primary must finish before secondary observes the shared attack mode.
    return primary_attack().update(runtime) && secondary_attack().update(runtime);
}

const Attack &Character::primary_attack() const noexcept
{
    static const OriginalPrimaryAttack attack;
    return attack;
}

const Attack &Character::secondary_attack() const noexcept
{
    static const OriginalSecondaryAttack attack;
    return attack;
}

} // namespace gain_ground::gameplay

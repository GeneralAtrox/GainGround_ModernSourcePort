#include "gain_ground/gameplay/enemy.h"

namespace gain_ground::gameplay {
void Enemy::update(EnemyRuntime &runtime) const
{
    for (;;) {
        const auto current = runtime.phase();
        switch (current) {
        case EnemyPhase::stopped: return;
        case EnemyPhase::movement: move(runtime); break;
        case EnemyPhase::attack: attack(runtime); break;
        default: runtime.advance(current); break;
        }
    }
}

void Enemy::die(EnemyDamage &damage) const
{
    damage.select_defeat_award();
    damage.mark_defeated();
}

void Enemy::take_hit(EnemyDamage &damage, bool secondary) const
{
    damage.select_hit_award();
    if (!damage.subtract_attack_damage()) die(damage);
    if (secondary) damage.mark_secondary_contact();
}
} // namespace gain_ground::gameplay

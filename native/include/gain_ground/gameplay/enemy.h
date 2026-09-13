#pragma once
#include "gain_ground/contract_types.h"
#include <span>

namespace gain_ground::gameplay {

struct EnemyState { std::uint32_t record_address{}; };
enum class EnemyPhase { behavior, contacts, movement, attack, appearance, finish, stopped };
enum class EnemyKind { pipeline, scripted, boss };
struct EnemyCall {
    EnemyPhase phase;
    std::uint32_t site, target, continuation;
};
struct EnemyDefinition {
    std::uint32_t callback;
    EnemyKind kind;
    std::span<const EnemyCall> calls;
    std::uint32_t return_pc;
};

// Guest record memory remains the sole mutable state. The runtime preserves
// each original child frame and IRQ continuation when performing a phase.
class EnemyRuntime {
public:
    virtual ~EnemyRuntime() = default;
    virtual EnemyPhase phase() const = 0;
    virtual void advance(EnemyPhase) = 0;
    virtual void run_script() = 0;
};

class EnemyDamage {
public:
    virtual ~EnemyDamage() = default;
    virtual void select_hit_award() = 0;
    virtual bool subtract_attack_damage() = 0; // true when HP remains positive
    virtual void select_defeat_award() = 0;
    virtual void mark_defeated() = 0;
    virtual void mark_secondary_contact() = 0;
};

class Enemy {
public:
    explicit Enemy(EnemyState state) : state_(state) {}
    virtual ~Enemy() = default;
    EnemyState state() const noexcept { return state_; }
    virtual void update(EnemyRuntime &) const;
    void take_hit(EnemyDamage &, bool secondary) const;
protected:
    void move(EnemyRuntime &runtime) const { runtime.advance(EnemyPhase::movement); }
    void attack(EnemyRuntime &runtime) const { runtime.advance(EnemyPhase::attack); }
    void die(EnemyDamage &) const;
private:
    EnemyState state_;
};

class PipelineEnemy final : public Enemy { public: using Enemy::Enemy; };
class ScriptedEnemy : public Enemy {
public:
    using Enemy::Enemy;
    void update(EnemyRuntime &runtime) const override { runtime.run_script(); }
};
// Boss-specific state machines and multipart records retain their original
// callbacks; they still share the enemy damage/defeat interface.
class BossEnemy final : public ScriptedEnemy { public: using ScriptedEnemy::ScriptedEnemy; };

} // namespace gain_ground::gameplay

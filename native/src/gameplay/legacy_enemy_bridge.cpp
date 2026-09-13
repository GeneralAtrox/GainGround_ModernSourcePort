// Shared native enemy lifecycle. Implemented but unverified.
#include "legacy_enemy_bridge.h"
#include "gain_ground/gameplay/game_definitions.h"
#include "../translated/unverified_cpu_b_machine.h"
#include <optional>

namespace gain_ground::gameplay {
namespace {
class LegacyEnemyRuntime final : public EnemyRuntime {
public:
    LegacyEnemyRuntime(const EnemyDefinition &definition, const FunctionContract &owner,
                       FunctionContext &context)
        : definition_(definition), owner_(owner), context_(context),
          machine_{*context.host, context.registers, 1U, 0x72U} {}

    EnemyPhase phase() const override {
        if (result_) return EnemyPhase::stopped;
        const auto pc = context_.registers.program_counter;
        if (pc == definition_.return_pc) return EnemyPhase::finish;
        for (const auto &call : definition_.calls)
            if (call.site == pc) return call.phase;
        return EnemyPhase::stopped;
    }
    void advance(EnemyPhase requested) override {
        const auto pc = context_.registers.program_counter;
        if (requested == EnemyPhase::finish && pc == definition_.return_pc) {
            result_ = machine_.ret();
            if (auto event = machine_.interrupt(context_, pc, context_.registers.program_counter))
                result_ = *event;
            return;
        }
        for (const auto &call : definition_.calls) {
            if (call.site != pc || call.phase != requested) continue;
            const auto *child = native_registry::find(1U, 0x72U, call.target);
            if (!child) {
                result_ = FunctionResult{TranslationStatus::contract_violation, 0U, call.target};
                return;
            }
            const auto result = machine_.call(context_, child->id, call.site, call.target, call.continuation);
            if (result.status != TranslationStatus::complete || result.control != 1U ||
                context_.registers.program_counter != call.continuation) {
                result_ = result;
                return;
            }
            if (auto event = machine_.interrupt(context_, call.site, call.continuation)) result_ = *event;
            return;
        }
        result_ = FunctionResult{TranslationStatus::contract_violation, 0U, pc};
    }
    void run_script() override { result_ = owner_.entry(context_); }
    std::optional<FunctionResult> result() const { return result_; }
private:
    const EnemyDefinition &definition_;
    const FunctionContract &owner_;
    FunctionContext &context_;
    translated::unverified::Machine machine_;
    std::optional<FunctionResult> result_;
};
}

bool run_enemy(const FunctionContract &owner, FunctionContext &context, FunctionResult &result)
{
    if (context.cpu != 1U || context.state != 0x72U) return false;
    const auto *definition = enemy_definition(owner.address);
    if (!definition) return false;
    LegacyEnemyRuntime runtime(*definition, owner, context);
    // Decline unsupported interior entries before touching guest state. Once
    // an update has begun, a missing continuation must fail instead of
    // falling back to a body that could repeat completed child calls.
    if (definition->kind == EnemyKind::pipeline && runtime.phase() == EnemyPhase::stopped)
        return false;
    const EnemyState state{context.registers.address[5]};
    switch (definition->kind) {
    case EnemyKind::pipeline: PipelineEnemy(state).update(runtime); break;
    case EnemyKind::scripted: ScriptedEnemy(state).update(runtime); break;
    case EnemyKind::boss: BossEnemy(state).update(runtime); break;
    }
    result = runtime.result().value_or(FunctionResult{
        TranslationStatus::contract_violation, 0U, context.registers.program_counter});
    return true;
}
} // namespace gain_ground::gameplay

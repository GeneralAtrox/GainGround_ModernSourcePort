// Implemented but unverified. No new timing/parity claim accompanies migration.
#include "legacy_gameplay_bridge.h"
#include "../translated/unverified_cpu_b_machine.h"

#include <array>
#include <optional>

namespace gain_ground::gameplay {
namespace {
struct Phase {
    CharacterStep step;
    std::uint32_t site, function, target, continuation;
};
constexpr std::array<Phase, 8> phases{{
    {CharacterStep::timer,         0xfe18U, 199U, 0xfe3cU, 0xfe1cU},
    {CharacterStep::movement,      0xfe1cU, 200U, 0xfe54U, 0xfe20U},
    {CharacterStep::contacts,      0xfe20U, 201U, 0xfed6U, 0xfe24U},
    {CharacterStep::attacks,       0xfe24U, 208U, 0x10a24U, 0xfe2aU},
    {CharacterStep::commit_motion, 0xfe2aU, 204U, 0x102aaU, 0xfe2eU},
    {CharacterStep::transition,    0xfe2eU, 205U, 0x10374U, 0xfe32U},
    {CharacterStep::animation,     0xfe32U, 206U, 0x103e4U, 0xfe36U},
    {CharacterStep::appearance,    0xfe36U, 195U, 0xfcaeU, 0xfe3aU},
}};

bool returned(const FunctionResult &result) noexcept
{
    return result.status == TranslationStatus::complete && result.control == 1U;
}

class LegacyCharacterRuntime final : public CharacterRuntime {
public:
    explicit LegacyCharacterRuntime(FunctionContext &context)
        : context_(context), machine_{*context.host, context.registers, 1U, 0x72U} {}

    CharacterStep step() const noexcept override {
        if (result_) return CharacterStep::stopped;
        const auto pc = context_.registers.program_counter;
        if (pc == 0xfe3aU) return CharacterStep::finish;
        for (const auto &phase : phases)
            if (phase.site == pc) return phase.step;
        return CharacterStep::stopped;
    }

    void advance(CharacterStep requested) override {
        auto &registers = context_.registers;
        if (requested == CharacterStep::finish) {
            result_ = machine_.ret();
            if (const auto event = machine_.interrupt(context_, 0xfe3aU,
                                                       registers.program_counter))
                result_ = *event;
            return;
        }
        for (const auto &phase : phases) {
            if (phase.step != requested || phase.site != registers.program_counter) continue;
            const auto result = machine_.call(context_, phase.function, phase.site,
                                               phase.target, phase.continuation);
            if (!returned(result)) { result_ = result; return; }
            if (machine_.unresolved_bus) {
                result_ = FunctionResult{TranslationStatus::contract_violation, 0U, phase.site};
                return;
            }
            if (const auto event = machine_.interrupt(context_, phase.site,
                                                       registers.program_counter))
                result_ = *event;
            return;
        }
        result_ = FunctionResult{TranslationStatus::contract_violation, 0U,
                                 registers.program_counter};
    }

    bool attack(AttackSlot slot) override {
        // F208's own call frames are retained here. Unlike F178 it did not
        // sample an interrupt between pushing a return and entering a child.
        const bool primary = slot == AttackSlot::primary;
        const std::uint32_t site = primary ? 0x10a24U : 0x10a28U;
        const std::uint32_t target = primary ? 0x10a2eU : 0x10cc0U;
        const std::uint32_t continuation = primary ? 0x10a28U : 0x10a2cU;
        auto &registers = context_.registers;
        // Character's common primary/secondary order also serves resumed F208
        // entries. The PC tells us which slot has already completed.
        if (registers.program_counter == 0x10a2cU ||
            (primary && registers.program_counter == 0x10a28U)) return true;
        if (registers.program_counter != site) {
            result_ = FunctionResult{TranslationStatus::contract_violation, 0U,
                                     registers.program_counter};
            return false;
        }
        registers.address[7] -= 4U;
        write_long(registers.address[7], continuation);
        registers.program_counter = target;
        const auto child = context_.host->call_function(primary ? 209U : 224U,
            1U, 0x72U, 2U, site, target, context_);
        if (!returned(child) || registers.program_counter != continuation) {
            result_ = child; return false;
        }
        return true;
    }

    void finish_attacks() {
        auto &registers = context_.registers;
        const auto high = context_.host->read_memory_word(2U, registers.address[7], 0xffffU);
        const auto low = context_.host->read_memory_word(2U, registers.address[7] + 2U, 0xffffU);
        registers.address[7] += 4U;
        registers.program_counter = (static_cast<std::uint32_t>(high) << 16U) | low;
        result_ = FunctionResult::complete(1U, registers.program_counter);
    }

    const std::optional<FunctionResult> &result() const noexcept { return result_; }

private:
    FunctionContext &context_;
    translated::unverified::Machine machine_;
    std::optional<FunctionResult> result_;

    void write_long(std::uint32_t address, std::uint32_t value) {
        context_.host->write_memory_word(2U, address,
            static_cast<std::uint16_t>(value >> 16U), 0xffffU);
        context_.host->write_memory_word(2U, address + 2U,
            static_cast<std::uint16_t>(value), 0xffffU);
    }
};
} // namespace

bool is_character_update_entry(std::uint32_t pc) noexcept
{
    if (pc == 0xfe3aU) return true;
    for (const auto &phase : phases) if (phase.site == pc) return true;
    return false;
}

bool run_character_update(const Character &character, FunctionContext &context,
                           FunctionResult &result)
{
    LegacyCharacterRuntime runtime(context);
    character.update(runtime);
    if (!runtime.result()) return false;
    result = *runtime.result();
    return true;
}

bool run_character_attacks(const Character &character, FunctionContext &context,
                            FunctionResult &result)
{
    LegacyCharacterRuntime runtime(context);
    if (character.update_attacks(runtime)) runtime.finish_attacks();
    if (!runtime.result()) return false;
    result = *runtime.result();
    return true;
}

} // namespace gain_ground::gameplay

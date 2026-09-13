#pragma once
#include "gground_functions.h"
#include "gain_ground/stage_function_registry.h"
#include <array>
#include <cstddef>

namespace gain_ground::translated {
FunctionResult cpu_b_state72_default_trap_return(FunctionContext &) noexcept;
FunctionResult cpu_b_irq_redirect_wait(FunctionContext &) noexcept;
}

namespace gain_ground::native_registry {
// Source-owned registration while fixture/catalog evidence is deferred.
// Existing state-72 opcode snapshot: 0x400 = 4e73 (RTE); vector 0x94 = 0x400.
// This is an additional implementation, not a captured fixture or parity pass.
inline constexpr FunctionContract kPendingCpuBTrapReturn{
    641U, 1U, 0x72U, false, true, 0x400U, 0x400U, 0x401U, 2U, 0U, 0U,
    "cpu-b", "72", "unverified", "retained-vector-and-opcode-source",
    "implementation-first", "implemented-but-unverified",
    "cpu_b_state72_default_trap_return",
    "native/src/translated/cpu_b_state72_default_trap_return.cpp",
    &translated::cpu_b_state72_default_trap_return};
static_assert(kPendingCpuBTrapReturn.id >= generated::kFunctions.size(),
    "Migrate the pending trap return when extending the generated inventory");

// Retained state-72 opcodes: 85a6 = 46fc2000, 85aa = 60fe.
// IRQ4 rewrites its exception-frame PC to this entry. No fixtures are claimed.
inline constexpr FunctionContract kPendingCpuBIrqRedirectWait{
    642U, 1U, 0x72U, false, true, 0x85a6U, 0x85a6U, 0x85abU, 6U, 0U, 0U,
    "cpu-b", "72", "unverified", "retained-irq-writer-and-opcode-source",
    "implementation-first", "implemented-but-unverified",
    "cpu_b_irq_redirect_wait",
    "native/src/translated/cpu_b_irq_redirect_wait.cpp",
    &translated::cpu_b_irq_redirect_wait};

// F178's retained F9BE BSR enters the same owner at F572. Register only that
// source-defined entry; this does not admit arbitrary addresses inside bodies.
inline constexpr FunctionContract kPendingCharacterResultEntry = [] {
    auto entry = generated::kFunctions[178U];
    entry.address = 0xf572U;
    entry.semantic_status = "implemented-but-unverified";
    entry.confidence = "retained-opcode-source";
    return entry;
}();

// F179's original selector branches also own RTS at F516 and F518. The retained
// fixture envelope ends at F515; using it for runtime IRQ resumption would drop
// those continuations. This source-owned override is not new fixture coverage.
inline constexpr FunctionContract kPendingCharacterStageDispatch = [] {
    auto entry = generated::kFunctions[179U];
    entry.body_max = 0xf519U;
    entry.body_bytes = 0x114U;
    entry.semantic_status = "implemented-but-unverified";
    entry.confidence = "retained-opcode-source";
    return entry;
}();

// Original 1B994 passes A6 (the next actor slot) in A5 to the special-stage
// initializer. The repaired PC-switch body also owns child-return resumption.
inline constexpr FunctionContract kPendingStageLoader = [] {
    auto entry = generated::kFunctions[323U];
    entry.semantic_status = "implemented-but-unverified";
    entry.confidence = "retained-opcode-source";
    return entry;
}();

// Original F201 call at 101D6 enters F203 after TAS, leaving the contacted
// object's mark byte unchanged. Register that exact entry, not a range alias.
inline constexpr FunctionContract kPendingUnmarkedCharacterHit = [] {
    auto entry = generated::kFunctions[203U];
    entry.address = 0x10276U;
    entry.semantic_status = "implemented-but-unverified";
    entry.confidence = "retained-opcode-source";
    return entry;
}();

// F175's omitted initials-entry helpers retain the original player's owner
// and native body. Only these retained BSR targets are admitted as entries.
inline constexpr auto kPendingInitialsHelpers = [] {
    std::array<FunctionContract, 5> entries{};
    constexpr std::array<std::uint32_t, 5> addresses{
        0xf2e6U, 0xf318U, 0xf33eU, 0xf35cU, 0xf38eU};
    for (std::size_t i = 0U; i < entries.size(); ++i) {
        entries[i] = generated::kFunctions[175U];
        entries[i].address = addresses[i];
        entries[i].semantic_status = "implemented-but-unverified";
        entries[i].confidence = "retained-opcode-source";
    }
    return entries;
}();

// Original drive-error branches enter after the normal drive-enable path.
// The body explicitly supports this entry and preserves the caller's error D0.
inline constexpr FunctionContract kPendingDriveErrorEntry = [] {
    auto entry = generated::kFunctions[23U];
    entry.address = 0x1990U;
    entry.semantic_status = "implemented-but-unverified";
    entry.confidence = "retained-opcode-source";
    return entry;
}();

inline const FunctionContract *find(std::uint8_t cpu, std::uint8_t state,
                                    std::uint32_t address) noexcept {
    if (kPendingDriveErrorEntry.cpu == cpu && kPendingDriveErrorEntry.state == state && kPendingDriveErrorEntry.address == address)
        return &kPendingDriveErrorEntry;
    const auto &stage_dispatch = kPendingCharacterStageDispatch;
    if (stage_dispatch.cpu == cpu && stage_dispatch.state == state && stage_dispatch.address == address)
        return &stage_dispatch;
    if (kPendingStageLoader.cpu == cpu && kPendingStageLoader.state == state && kPendingStageLoader.address == address)
        return &kPendingStageLoader;
    for (const auto &function : generated::kFunctions)
        if (function.cpu == cpu && function.state == state && function.address == address)
            return &function;
    const auto &pending = kPendingCpuBTrapReturn;
    if (pending.cpu == cpu && pending.state == state && pending.address == address)
        return &pending;
    const auto &redirect = kPendingCpuBIrqRedirectWait;
    if (redirect.cpu == cpu && redirect.state == state && redirect.address == address)
        return &redirect;
    const auto &character_result = kPendingCharacterResultEntry;
    if (character_result.cpu == cpu && character_result.state == state && character_result.address == address)
        return &character_result;
    const auto &unmarked_hit = kPendingUnmarkedCharacterHit;
    if (unmarked_hit.cpu == cpu && unmarked_hit.state == state && unmarked_hit.address == address)
        return &unmarked_hit;
    for (const auto &helper : kPendingInitialsHelpers)
        if (helper.cpu == cpu && helper.state == state && helper.address == address)
            return &helper;
    return stage_registry::find(cpu, state, address);
}
} // namespace gain_ground::native_registry

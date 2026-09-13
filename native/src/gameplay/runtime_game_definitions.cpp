#include "gain_ground/runtime_host.h"
#include "gain_ground/gameplay/game_definitions.h"

namespace gain_ground {
bool RuntimeHost::apply_level_definition()
{
    if (!assets_) return true;
    const auto bytes = region_bytes(2U);
    if (bytes.size() <= 0xc03U) return false;
    const auto index = static_cast<std::uint16_t>((bytes[0xc02U] << 8U) | bytes[0xc03U]);
    const auto *definition = gameplay::level_definition(index);
    if (!definition) return true; // Ending/title asset selectors are not levels.
    // Definitions replace immutable source tables, not live actors. Apply once
    // at the original stage initializer entry, never at a resumed child PC.
    // Default bytes come from the same original tables loaded from disk.
    for (const auto &patch : definition->data) {
        if (!load_region(2U, patch.address, patch.bytes)) {
            fail("Level definition exceeds original data storage", patch.address, 2U);
            return false;
        }
    }
    return true;
}

std::uint16_t RuntimeHost::definition_read_word(std::uint16_t region, std::uint32_t offset,
                                               std::uint16_t original) const noexcept
{
    if (!assets_ || !active_) return original;
    const auto pc = active_->registers.program_counter;
    // Remap CPU A's four per-stage resource selectors, preserving its request
    // latch, acknowledgement, and world-bank selection reads.
    // Absolute-short $8006 is $FFFF8006, mapped to share1 + $38006.
    if (active_->cpu == 0U && region == 3U && offset == 0x38006U && original != 0U &&
        (pc == 0x80426U || pc == 0x80448U || pc == 0x80488U || pc == 0x804baU)) {
        if (const auto *level = gameplay::level_definition(static_cast<std::uint16_t>(original - 1U)))
            return level->layout_asset_set;
    }
    // F322 indexes the matching per-stage palette stream. World graphics banks
    // remain unchanged, including F322's first-stage-of-world initialization
    // check at 182B0; the content generator restricts layouts to that bank.
    if (active_->cpu == 1U && region == 2U && offset == 0xc02U &&
        pc == 0x18276U) {
        if (const auto *level = gameplay::level_definition(original))
            return static_cast<std::uint16_t>(level->layout_asset_set - 1U);
    }
    return original;
}
} // namespace gain_ground

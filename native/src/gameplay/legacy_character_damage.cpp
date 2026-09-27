// Implemented but unverified. F203, its 10276 entry and F472 select the original
// contact-mark behavior; the interior entry does not mark the contacted object.
#include "legacy_gameplay_bridge.h"
#include "gain_ground/gameplay/character_damage.h"
#include "../translated/unverified_cpu_b_machine.h"

namespace gain_ground::gameplay {
namespace {
class LegacyDamage final : public CharacterDamage {
public:
    explicit LegacyDamage(FunctionContext &context)
        : host_(*context.host), registers_(context.registers),
          effects_{host_, registers_, 1U, 0x72U}, record_(registers_.address[5]) {}

    void mark_contact() override {
        // A protected hit marks the contacting object as spent, which removes an
        // enemy body. The test guard must not turn an invulnerable player into a
        // touch kill, so it leaves the object alone.
        if (host_.player_invulnerable()) return;
        const auto address = registers_.address[6] + 0x3fU;
        const auto previous = byte(address);
        byte(address, static_cast<std::uint8_t>(previous | 0x80U));
        effects_.logic(previous, 8U);
    }
    bool protected_from_hit() override {
        const auto timer = word(record_ + 0x48U);
        effects_.logic(timer, 16U);
        if (host_.player_invulnerable()) { effects_.logic(1U, 16U); return true; }
        return static_cast<std::int16_t>(timer) > 0;
    }
    void ignore_hit() override {
        registers_.status = static_cast<std::uint16_t>(registers_.status & ~0x1fU);
    }
    void defeat() override {
        word(record_ + 0x44U, 6U); effects_.logic(6U, 16U);
        const auto player = byte(record_ + 0x6dU);
        effects_.db(0U, player); effects_.logic(player, 8U);
        const auto previous = byte(0xc06U);
        const auto bit = static_cast<std::uint8_t>(1U << (player & 7U));
        registers_.status = static_cast<std::uint16_t>(
            (registers_.status & ~4U) | ((previous & bit) ? 0U : 4U));
        byte(0xc06U, static_cast<std::uint8_t>(previous | bit));
        const auto active = word(0xc10U);
        word(0xc10U, static_cast<std::uint16_t>(active - 1U));
        (void)effects_.sub(active, 1U, 16U);
        // Carry makes the original object scan stop after this hit.
        registers_.status = static_cast<std::uint16_t>((registers_.status & ~0x1fU) | 1U);
    }
    FunctionResult finish() {
        const auto high = word(registers_.address[7]);
        const auto low = word(registers_.address[7] + 2U);
        registers_.address[7] += 4U;
        registers_.program_counter = (static_cast<std::uint32_t>(high) << 16U) | low;
        return FunctionResult::complete(1U, registers_.program_counter);
    }

private:
    ExecutionHost &host_;
    CpuRegisters &registers_;
    translated::unverified::Machine effects_;
    std::uint32_t record_;

    std::uint16_t word(std::uint32_t address) {
        return host_.read_memory_word(2U, address, 0xffffU);
    }
    void word(std::uint32_t address, std::uint16_t value) {
        host_.write_memory_word(2U, address, value, 0xffffU);
    }
    std::uint8_t byte(std::uint32_t address) {
        const auto mask = static_cast<std::uint16_t>((address & 1U) ? 0xffU : 0xff00U);
        return static_cast<std::uint8_t>(host_.read_memory_word(2U, address & ~1U, mask)
            >> ((address & 1U) ? 0U : 8U));
    }
    void byte(std::uint32_t address, std::uint8_t value) {
        const auto mask = static_cast<std::uint16_t>((address & 1U) ? 0xffU : 0xff00U);
        const auto data = static_cast<std::uint16_t>(static_cast<unsigned>(value)
            << ((address & 1U) ? 0U : 8U));
        host_.write_memory_word(2U, address & ~1U, data, mask);
    }
};
} // namespace

bool run_character_damage(const Character &character, FunctionContext &context,
                           FunctionResult &result)
{
    const auto pc = context.registers.program_counter;
    const auto contact = pc == 0x10272U ? HitContact::mark_always
        : pc == 0x10276U ? HitContact::leave_unmarked : HitContact::mark_when_protected;
    LegacyDamage damage(context);
    character.take_hit(damage, contact);
    result = damage.finish();
    return true;
}

} // namespace gain_ground::gameplay

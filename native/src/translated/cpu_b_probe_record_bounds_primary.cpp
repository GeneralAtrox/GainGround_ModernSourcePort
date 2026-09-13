#include "gain_ground/contract_types.h"
#include "gain_ground/gameplay/enemy.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;

std::uint16_t rw(ExecutionHost &h, std::uint32_t a)
{ return h.read_memory_word(kRegion, a & 0x3ffffU, kMask); }
void ww(ExecutionHost &h, std::uint32_t a, std::uint16_t v)
{ h.write_memory_word(kRegion, a & 0x3ffffU, v, kMask); }
std::uint8_t rb(ExecutionHost &h, std::uint32_t a)
{
    const auto offset = a & 0x3ffffU;
    const bool odd = (offset & 1U) != 0U;
    const auto value = h.read_memory_word(kRegion, offset & ~1U,
        odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? value : value >> 8U);
}
void wb(ExecutionHost &h, std::uint32_t a, std::uint8_t v)
{
    const auto offset = a & 0x3ffffU;
    const bool odd = (offset & 1U) != 0U;
    h.write_memory_word(kRegion, offset & ~1U,
        static_cast<std::uint16_t>(v) << (odd ? 0U : 8U),
        odd ? 0x00ffU : 0xff00U);
}
void logicw(CpuRegisters &r, std::uint16_t v)
{
    std::uint16_t f = r.status & 0x10U;
    if ((v & 0x8000U) != 0U) f |= 8U;
    if (v == 0U) f |= 4U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
void logicb(CpuRegisters &r, std::uint8_t v)
{
    std::uint16_t f = r.status & 0x10U;
    if ((v & 0x80U) != 0U) f |= 8U;
    if (v == 0U) f |= 4U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
void cmpw(CpuRegisters &r, std::uint16_t d, std::uint16_t s)
{
    const auto v = static_cast<std::uint16_t>(d - s);
    std::uint16_t f = r.status & 0x10U;
    if (s > d) f |= 1U;
    if (((d ^ s) & (d ^ v) & 0x8000U) != 0U) f |= 2U;
    if (v == 0U) f |= 4U;
    if ((v & 0x8000U) != 0U) f |= 8U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
void cmpb(CpuRegisters &r, std::uint8_t d, std::uint8_t s)
{
    const auto v = static_cast<std::uint8_t>(d - s);
    std::uint16_t f = r.status & 0x10U;
    if (s > d) f |= 1U;
    if (((d ^ s) & (d ^ v) & 0x80U) != 0U) f |= 2U;
    if (v == 0U) f |= 4U;
    if ((v & 0x80U) != 0U) f |= 8U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
void addw(CpuRegisters &r, std::uint16_t d, std::uint16_t s,
    std::uint16_t v)
{
    std::uint16_t f = 0U;
    if (std::uint32_t(d) + s > 0xffffU) f |= 0x11U;
    if (((~(d ^ s)) & (d ^ v) & 0x8000U) != 0U) f |= 2U;
    if (v == 0U) f |= 4U;
    if ((v & 0x8000U) != 0U) f |= 8U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
void subw(CpuRegisters &r, std::uint16_t d, std::uint16_t s,
    std::uint16_t v)
{
    std::uint16_t f = 0U;
    if (s > d) f |= 0x11U;
    if (((d ^ s) & (d ^ v) & 0x8000U) != 0U) f |= 2U;
    if (v == 0U) f |= 4U;
    if ((v & 0x8000U) != 0U) f |= 8U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
void push(ExecutionHost &h, CpuRegisters &r, std::uint32_t v)
{
    r.address[7] -= 4U;
    ww(h, r.address[7], static_cast<std::uint16_t>(v >> 16U));
    ww(h, r.address[7] + 2U, static_cast<std::uint16_t>(v));
}
std::uint32_t pop(ExecutionHost &h, CpuRegisters &r)
{
    const auto v = (static_cast<std::uint32_t>(rw(h, r.address[7])) << 16U)
        | rw(h, r.address[7] + 2U);
    r.address[7] += 4U;
    return v;
}
FunctionResult child(FunctionContext &c, std::uint32_t id,
    std::uint32_t site, std::uint32_t target, std::uint32_t ret)
{
    push(*c.host, c.registers, ret);
    c.registers.program_counter = target;
    return c.host->call_function(id, 1U, 0x72U, 2U, site, target, c);
}
bool carry(const CpuRegisters &r) { return (r.status & 1U) != 0U; }
bool minus(const CpuRegisters &r) { return (r.status & 8U) != 0U; }
bool greater(const CpuRegisters &r)
{ return (r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U; }

class LegacyEnemyDamage final : public gameplay::EnemyDamage {
public:
    explicit LegacyEnemyDamage(FunctionContext &context) : h(*context.host), r(context.registers) {}
    void select_hit_award() override {
        r.data[0] = 0U; logicw(r, 0U);
        select_award(0x42U);
    }
    bool subtract_attack_damage() override {
        const auto damage = rw(h, r.address[5] + 0x3aU);
        r.data[1] = (r.data[1] & 0xffff0000U) | damage; logicw(r, damage);
        const auto old = rw(h, r.address[6] + 0x46U);
        const auto value = static_cast<std::uint16_t>(old - damage);
        ww(h, r.address[6] + 0x46U, value); subw(r, old, damage, value);
        return greater(r);
    }
    void select_defeat_award() override { select_award(0x44U); }
    void mark_defeated() override { mark(0x3fU); }
    void mark_secondary_contact() override { mark(0x3dU); }
private:
    ExecutionHost &h;
    CpuRegisters &r;
    void select_award(std::uint32_t offset) {
        const auto value = rw(h, r.address[6] + offset);
        r.data[0] = (r.data[0] & 0xffff0000U) | value; logicw(r, value);
    }
    void mark(std::uint32_t offset) {
        const auto old = rb(h, r.address[6] + offset);
        wb(h, r.address[6] + offset, static_cast<std::uint8_t>(old | 0x80U));
        logicb(r, old);
    }
};
} // namespace

FunctionResult cpu_b_probe_record_bounds_common(FunctionContext &c,
    bool secondary) noexcept
{
    if (c.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            c.registers.program_counter};
    auto &h = *c.host;
    auto &r = c.registers;

    r.address[3] = 0x6c00U;
    auto list_header = rw(h, r.address[3]);
    r.address[3] += 2U;
    logicw(r, list_header);
    if (!minus(r)) r.address[3] = 0x7002U;

    auto d0 = rw(h, r.address[5] + 0x1aU);
    std::uint16_t d7 = 0U;
    r.data[0] = (r.data[0] & 0xffff0000U) | d0;
    logicw(r, d0);
    cmpw(r, d0, 0x0200U);
    if (d0 >= 0x0200U) goto rejected;

    d7 = d0;
    r.data[7] = (r.data[7] & 0xffff0000U) | d7;
    logicw(r, d7);
    {
        const auto next = static_cast<std::uint16_t>(d7 + 0x20U);
        addw(r, d7, 0x20U, next);
        d7 = next;
        r.data[7] = (r.data[7] & 0xffff0000U) | d7;
    }
    {
        const auto next = static_cast<std::uint16_t>(d0 - 0x20U);
        subw(r, d0, 0x20U, next);
        d0 = next;
        r.data[0] = (r.data[0] & 0xffff0000U) | d0;
    }
    if (!minus(r)) {
        cmpw(r, d7, 0x01ffU);
        if (static_cast<std::int16_t>(d7) > 0x01ff) {
            d7 = 0x01ffU;
            r.data[7] = (r.data[7] & 0xffff0000U) | d7;
            logicw(r, d7);
        }
        const auto next = static_cast<std::uint16_t>(d7 - d0);
        subw(r, d7, d0, next);
        d7 = next;
        r.data[7] = (r.data[7] & 0xffff0000U) | d7;
        const auto doubled = static_cast<std::uint16_t>(d0 + d0);
        addw(r, d0, d0, doubled);
        d0 = doubled;
        r.data[0] = (r.data[0] & 0xffff0000U) | d0;
        r.address[3] = static_cast<std::uint32_t>(
            r.address[3] + static_cast<std::int16_t>(d0));
    }

    for (;;) {
        d0 = rw(h, r.address[3]);
        r.address[3] += 2U;
        r.data[0] = (r.data[0] & 0xffff0000U) | d0;
        logicw(r, d0);
        if (d0 != 0U) {
            r.address[6] = static_cast<std::uint32_t>(
                static_cast<std::int32_t>(static_cast<std::int16_t>(d0)));
            auto type = rb(h, r.address[6] + 0x0bU);
            cmpb(r, type, 4U);
            if (type == 4U) {
                auto z = child(c, 255U,
                    secondary ? 0x1292cU : 0x1287eU, 0x12a2aU,
                    secondary ? 0x12930U : 0x12882U);
                if (z.status != TranslationStatus::complete || z.control != 1U)
                    return z;
                if (carry(r)) {
                    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | 1U);
                    const auto target = pop(h, r);
                    r.program_counter = target;
                    return FunctionResult::complete(1U, target);
                }
            } else {
                type = rb(h, r.address[6] + 0x0bU);
                cmpb(r, type, 8U);
                if (static_cast<std::int8_t>(type) >= 8) {
                    type = rb(h, r.address[6] + 0x0bU);
                    cmpb(r, type, 10U);
                    if (static_cast<std::int8_t>(type) < 10) {
                        const auto flag = rb(h, r.address[6] + 0x3fU);
                        logicb(r, flag);
                        if (flag == 0U) {
                            auto z = child(c, 256U,
                                secondary ? 0x1294eU : 0x128a0U, 0x12a72U,
                                secondary ? 0x12952U : 0x128a4U);
                            if (z.status != TranslationStatus::complete || z.control != 1U)
                                return z;
                            if (carry(r)) goto matched;
                        }
                    } else {
                        const auto flag = rb(h, r.address[6] + 0x3fU);
                        logicb(r, flag);
                        if (flag == 0U) {
                            auto z = child(c, 255U,
                                secondary ? 0x1295cU : 0x128aeU, 0x12a2aU,
                                secondary ? 0x12960U : 0x128b2U);
                            if (z.status != TranslationStatus::complete || z.control != 1U)
                                return z;
                            if (carry(r)) goto matched;
                        }
                    }
                }
            }
        }
        d7 = static_cast<std::uint16_t>(d7 - 1U);
        r.data[7] = (r.data[7] & 0xffff0000U) | d7;
        if (d7 == 0xffffU) break;
    }
    goto rejected;

matched:
    {
        // Shared hit/defeat policy; template awards, health and projectile
        // damage still come from their original live records in the same order.
        LegacyEnemyDamage damage(c);
        gameplay::Enemy(gameplay::EnemyState{r.address[6]}).take_hit(damage, secondary);
    }
    r.address[4] = static_cast<std::uint32_t>(static_cast<std::int32_t>(
        static_cast<std::int16_t>(rw(h, r.address[5] + 0x36U))));
    r.address[4] = static_cast<std::uint32_t>(static_cast<std::int32_t>(
        static_cast<std::int16_t>(rw(h, r.address[4] + 0x60U))));
    {
        auto z = child(c, 286U,
            secondary ? 0x12986U : 0x128d4U, 0x15ebeU,
            secondary ? 0x1298cU : 0x128daU);
        if (z.status != TranslationStatus::complete || z.control != 1U)
            return z;
    }
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | 1U);
    {
        const auto target = pop(h, r);
        r.program_counter = target;
        return FunctionResult::complete(1U, target);
    }

rejected:
    r.status = static_cast<std::uint16_t>(r.status & ~0x1fU);
    {
        const auto target = pop(h, r);
        r.program_counter = target;
        return FunctionResult::complete(1U, target);
    }
}

FunctionResult cpu_b_probe_record_bounds_primary(FunctionContext &context) noexcept
{
    return cpu_b_probe_record_bounds_common(context, false);
}

} // namespace gain_ground::translated

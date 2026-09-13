#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

std::uint8_t read_byte(ExecutionHost &h, std::uint32_t a)
{
    const bool odd = (a & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(h.read_memory_word(
        kRegion, a & 0x0003fffeU, mask) >> (odd ? 0U : 8U));
}

void write_byte(ExecutionHost &h, std::uint32_t a, std::uint8_t v)
{
    const bool odd = (a & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    h.write_memory_word(kRegion, a & 0x0003fffeU,
        static_cast<std::uint16_t>(v) << (odd ? 0U : 8U), mask);
}

std::uint16_t read_word(ExecutionHost &h, std::uint32_t a)
{
    return h.read_memory_word(kRegion, a & 0x0003ffffU, kWordMask);
}

void write_word(ExecutionHost &h, std::uint32_t a, std::uint16_t v)
{
    h.write_memory_word(kRegion, a & 0x0003ffffU, v, kWordMask);
}

std::uint32_t read_long(ExecutionHost &h, std::uint32_t a)
{
    return (static_cast<std::uint32_t>(read_word(h, a)) << 16U)
        | read_word(h, a + 2U);
}

void logic_byte(CpuRegisters &r, std::uint8_t v)
{
    std::uint16_t f = r.status & 0x10U;
    if ((v & 0x80U) != 0U) f |= 0x08U;
    if (v == 0U) f |= 0x04U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}

void logic_word(CpuRegisters &r, std::uint16_t v)
{
    std::uint16_t f = r.status & 0x10U;
    if ((v & 0x8000U) != 0U) f |= 0x08U;
    if (v == 0U) f |= 0x04U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}

void sub_word(CpuRegisters &r, std::uint16_t l,
    std::uint16_t q, std::uint16_t v)
{
    std::uint16_t f{};
    if ((v & 0x8000U) != 0U) f |= 0x08U;
    if (v == 0U) f |= 0x04U;
    if (((l ^ q) & (l ^ v) & 0x8000U) != 0U) f |= 0x02U;
    if (l < q) f |= 0x11U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}

void set_z(CpuRegisters &r, bool z)
{
    r.status = static_cast<std::uint16_t>(
        (r.status & ~0x04U) | (z ? 0x04U : 0U));
}

void change_bit(ExecutionHost &h, CpuRegisters &r,
    std::uint32_t a, unsigned bit, bool set)
{
    const auto before = read_byte(h, a);
    const auto mask = static_cast<std::uint8_t>(1U << bit);
    const auto after = static_cast<std::uint8_t>(set
        ? before | mask
        : before & static_cast<std::uint8_t>(~mask));
    write_byte(h, a, after);
    set_z(r, (before & mask) == 0U);
}

void push_return(ExecutionHost &h, CpuRegisters &r, std::uint32_t v)
{
    r.address[7] -= 4U;
    write_word(h, r.address[7], static_cast<std::uint16_t>(v >> 16U));
    write_word(h, r.address[7] + 2U, static_cast<std::uint16_t>(v));
}

std::uint32_t pop_return(ExecutionHost &h, CpuRegisters &r)
{
    const auto v = read_long(h, r.address[7]);
    r.address[7] += 4U;
    return v;
}

FunctionResult tail(FunctionContext &c, std::uint32_t id,
    std::uint32_t site, std::uint32_t target)
{
    c.registers.program_counter = target;
    return c.host->call_function(id, 1U, 0x72U, 1U, site, target, c);
}

FunctionResult call(FunctionContext &c, std::uint32_t id,
    std::uint32_t site, std::uint32_t target, std::uint32_t continuation)
{
    push_return(*c.host, c.registers, continuation);
    c.registers.program_counter = target;
    return c.host->call_function(id, 1U, 0x72U, 2U, site, target, c);
}
} // namespace

FunctionResult cpu_b_record_mode_gate(FunctionContext &context) noexcept
{
    if (context.host == nullptr
        || context.registers.program_counter != 0x0001efdcU)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];
    const auto finish = [&]() -> FunctionResult {
        const auto target = pop_return(host, r);
        r.program_counter = target;
        return FunctionResult::complete(1U, target);
    };
    const auto completed = [](const FunctionResult &result) {
        return result.status == TranslationStatus::complete
            && result.control == 1U;
    };

    const auto gate = read_byte(host, base + 0x3eU);
    logic_byte(r, gate);
    if (gate != 0U) return finish();

    const auto mode = read_byte(host, base + 0x41U);
    const bool bit1 = (mode & 0x02U) != 0U;
    set_z(r, !bit1);
    if (bit1)
        return tail(context, 556U, 0x0001efeaU, 0x0001f02aU);

    const auto timer = read_word(host, base + 0x74U);
    logic_word(r, timer);
    if (static_cast<std::int16_t>(timer) > 0) {
        const auto before = read_word(host, base + 0x74U);
        const auto after = static_cast<std::uint16_t>(before - 1U);
        write_word(host, base + 0x74U, after);
        sub_word(r, before, 1U, after);
        return finish();
    }

    r.address[0] = read_long(host, base + 0x66U);
    const auto initial = read_byte(host, r.address[0] + 0x12U);
    write_byte(host, base + 0x72U, initial);
    logic_byte(r, initial);
    change_bit(host, r, base + 0x41U, 1U, true);
    change_bit(host, r, base + 0x41U, 0U, false);
    change_bit(host, r, base + 0x41U, 2U, true);
    write_word(host, base + 0x74U, 6U);
    logic_word(r, 6U);

    auto result = call(context, 350U,
        0x0001f01aU, 0x0001dad8U, 0x0001f01eU);
    if (!completed(result)) return result;

    const auto variant = read_byte(host, base + 0x60U);
    logic_byte(r, variant);
    if (variant == 0U)
        return tail(context, 606U, 0x0001f022U, 0x0001f07aU);

    result = call(context, 347U,
        0x0001f024U, 0x0001da58U, 0x0001f028U);
    if (!completed(result)) return result;
    return finish();
}

} // namespace gain_ground::translated

#include "gain_ground/contract_types.h"
#include "gground_functions.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kSourceRegion = 4U;
constexpr std::uint16_t kOutputRegion = 8U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kCpuMask = 0x0003ffffU;

std::uint16_t read_cpu(ExecutionHost &h, std::uint32_t a)
{
    return h.read_memory_word(kSharedRegion, a & kCpuMask, kWordMask);
}
void prefetch(ExecutionHost &h, std::uint32_t a)
{
    (void)read_cpu(h, a);
}
void prefetch_program(ExecutionHost &h, std::uint32_t a)
{
    (void)h.read_memory_word(kProgramRegion, a, kWordMask);
}
void write_cpu(ExecutionHost &h, std::uint32_t a, std::uint16_t v)
{
    h.write_memory_word(kSharedRegion, a & kCpuMask, v, kWordMask);
}
std::uint8_t read_source_byte(ExecutionHost &h, std::uint32_t a)
{
    const bool shared = a >= 0x00080000U && a < 0x000c0000U;
    const auto region = shared ? kSharedRegion
        : (a < 0x00100000U ? kProgramRegion : kSourceRegion);
    const auto off = shared ? (a & kCpuMask)
        : (a < 0x00100000U ? a : a - 0x00100000U);
    const bool odd = (off & 1U) != 0U;
    const auto v = h.read_memory_word(region, off & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U));
    return static_cast<std::uint8_t>(odd ? v : v >> 8U);
}
void write_output_byte(ExecutionHost &h, std::uint32_t a, std::uint8_t v)
{
    const auto off = a - 0x00280000U;
    const bool odd = (off & 1U) != 0U;
    h.write_memory_word(kOutputRegion, off & ~1U,
        static_cast<std::uint16_t>(v) << (odd ? 0U : 8U),
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U));
}
void logic(CpuRegisters &r, std::uint32_t v, std::uint32_t sign)
{
    std::uint16_t f = r.status & 0x0010U;
    if ((v & sign) != 0U) f |= 0x0008U;
    if (v == 0U) f |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}
void add_byte(CpuRegisters &r, unsigned reg)
{
    const auto a = static_cast<std::uint8_t>(r.data[reg]);
    const auto v = static_cast<std::uint8_t>(a + a);
    std::uint16_t f{};
    if (static_cast<unsigned>(a) + a > 0xffU) f |= 0x0011U;
    if (((~(a ^ a)) & (a ^ v) & 0x80U) != 0U) f |= 0x0002U;
    if ((v & 0x80U) != 0U) f |= 0x0008U;
    if (v == 0U) f |= 0x0004U;
    r.data[reg] = (r.data[reg] & 0xffffff00U) | v;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}
void roxl_byte_four(CpuRegisters &r)
{
    const auto a = static_cast<std::uint8_t>(r.data[2]);
    std::uint16_t ring = static_cast<std::uint16_t>(
        (static_cast<std::uint16_t>(a) << 1U) | ((r.status >> 4U) & 1U));
    ring = static_cast<std::uint16_t>(((ring << 4U) | (ring >> 5U)) & 0x01ffU);
    const auto v = static_cast<std::uint8_t>(ring >> 1U);
    std::uint16_t f = (ring & 1U) != 0U ? 0x0011U : 0U;
    if ((v & 0x80U) != 0U) f |= 0x0008U;
    if (v == 0U) f |= 0x0004U;
    r.data[2] = (r.data[2] & 0xffffff00U) | v;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}
void addx_byte(CpuRegisters &r)
{
    const auto a = static_cast<std::uint8_t>(r.data[2]);
    const auto x = static_cast<unsigned>((r.status >> 4U) & 1U);
    const auto sum = static_cast<unsigned>(a) + a + x;
    const auto v = static_cast<std::uint8_t>(sum);
    const bool prior_z = (r.status & 0x0004U) != 0U;
    std::uint16_t f{};
    if (sum > 0xffU) f |= 0x0011U;
    if (((~(a ^ a)) & (a ^ v) & 0x80U) != 0U) f |= 0x0002U;
    if ((v & 0x80U) != 0U) f |= 0x0008U;
    if (v == 0U && prior_z) f |= 0x0004U;
    r.data[2] = (r.data[2] & 0xffffff00U) | v;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}

FunctionResult interrupt(FunctionContext &c, std::uint32_t pc,
    std::uint32_t resume, std::uint8_t kind, std::uint32_t lookahead)
{
    auto &h = *c.host;
    auto &r = c.registers;
    const auto pending = h.consume_pending_interrupt(0U, 0xffU, pc);
    if (!pending.asserted || pending.level <= ((r.status >> 8U) & 7U))
        return FunctionResult::complete(0U, resume);
    if (pending.level < 3U || pending.level > 5U)
        return {TranslationStatus::contract_violation, 0U, pc};
    if (lookahead != 0U) prefetch(h, lookahead);

    const auto sr = r.status;
    r.address[7] -= 4U;
    write_cpu(h, r.address[7] + 2U, static_cast<std::uint16_t>(resume));
    r.address[7] -= 2U;
    write_cpu(h, r.address[7], sr);
    write_cpu(h, r.address[7] + 2U, static_cast<std::uint16_t>(resume >> 16U));
    r.status = static_cast<std::uint16_t>((sr & 0x38ffU) | 0x2000U
        | (static_cast<std::uint16_t>(pending.level) << 8U));

    const auto vector = static_cast<std::uint32_t>(0x0060U + pending.level * 4U);
    const auto target = static_cast<std::uint32_t>(0x00080030U + pending.level * 6U);
    const auto function_id = static_cast<std::uint32_t>(43U + pending.level);
    prefetch_program(h, vector);
    prefetch_program(h, vector + 2U);
    const auto stub = static_cast<std::uint32_t>(0x00000030U + pending.level * 6U);
    (void)read_cpu(h, stub);
    (void)read_cpu(h, stub + 2U);
    r.program_counter = target;
    const auto child = h.call_function(
        function_id, 0U, 0xffU, kind, pc, target, c);
    if (child.status != TranslationStatus::complete) return child;
    if (kind == 0U) {
        const auto trampoline = pending.level == 3U
            ? cpu_a_irq3_vector_trampoline(c)
            : (pending.level == 4U ? cpu_a_irq4_vector_trampoline(c)
                                   : cpu_a_irq5_vector_trampoline(c));
        if (trampoline.status != TranslationStatus::complete) return trampoline;
    }
    r.program_counter = resume;
    return FunctionResult::complete(1U, resume);
}
} // namespace

FunctionResult proven_static_cpu_a_plain_0008031a(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;

    r.data[4] = 3U;
    logic(r, r.data[4], 0x80000000U);
    r.address[0] += 3U;
    prefetch(h, 0x0008031eU);

    bool outer_320_prefetched = false;
    for (;;) {
        r.address[2] = r.address[0];
        if (!outer_320_prefetched) prefetch(h, 0x00080320U);
        outer_320_prefetched = false;
        prefetch(h, 0x00080322U);
        const auto d0 = read_source_byte(h, r.address[1]++);
        r.data[0] = (r.data[0] & 0xffffff00U) | d0;
        logic(r, d0, 0x80U);
        prefetch(h, 0x00080324U);
        const auto d1 = read_source_byte(h, r.address[1]++);
        r.data[1] = (r.data[1] & 0xffffff00U) | d1;
        logic(r, d1, 0x80U);
        const auto irq322 = interrupt(
            context, 0x00080322U, 0x00080324U, 0U, 0x00080326U);
        if (irq322.status != TranslationStatus::complete) return irq322;
        if (irq322.control == 0U) prefetch(h, 0x00080326U);
        r.data[3] = 7U;
        logic(r, r.data[3], 0x80000000U);

        bool pf328 = false;
        bool pf33a = false;
        for (;;) {
            r.data[2] = 0U;
            logic(r, 0U, 0x80000000U);
            if (!pf328) prefetch(h, 0x00080328U);
            pf328 = false;
            const auto irq326 = interrupt(
                context, 0x00080326U, 0x00080328U, 0U, 0x0008032aU);
            if (irq326.status != TranslationStatus::complete) return irq326;
            add_byte(r, 1U);
            if (irq326.control == 0U) prefetch(h, 0x0008032aU);
            const auto irq328 = interrupt(
                context, 0x00080328U, 0x0008032aU, 0U, 0x0008032cU);
            if (irq328.status != TranslationStatus::complete) return irq328;
            roxl_byte_four(r);
            if (irq328.control == 0U) prefetch(h, 0x0008032cU);
            const auto irq32a = interrupt(
                context, 0x0008032aU, 0x0008032cU, 0U, 0x0008032eU);
            if (irq32a.status != TranslationStatus::complete) return irq32a;
            add_byte(r, 0U);
            if (irq32a.control == 0U) prefetch(h, 0x0008032eU);
            const auto irq32c = interrupt(
                context, 0x0008032cU, 0x0008032eU, 0U, 0x00080330U);
            if (irq32c.status != TranslationStatus::complete) return irq32c;
            addx_byte(r);
            if (irq32c.control == 0U) prefetch(h, 0x00080330U);
            const auto irq32e = interrupt(
                context, 0x0008032eU, 0x00080330U, 0U, 0x00080332U);
            if (irq32e.status != TranslationStatus::complete) return irq32e;
            if (irq32e.control == 0U) prefetch(h, 0x00080332U);
            const auto out = static_cast<std::uint8_t>(r.data[2]);
            write_output_byte(h, r.address[0], out);
            logic(r, out, 0x80U);
            const auto irq330 = interrupt(
                context, 0x00080330U, 0x00080332U, 0U, 0x00080334U);
            if (irq330.status != TranslationStatus::complete) return irq330;
            r.address[0] += 4U;
            if (irq330.control == 0U) prefetch(h, 0x00080334U);
            const auto irq332 = interrupt(
                context, 0x00080332U, 0x00080334U, 0U, 0x00080336U);
            if (irq332.status != TranslationStatus::complete) return irq332;
            const auto count = static_cast<std::uint16_t>(r.data[3] - 1U);
            r.data[3] = (r.data[3] & 0xffff0000U) | count;
            if (irq332.control == 0U) prefetch(h, 0x00080336U);
            prefetch(h, 0x00080326U);
            if (count == 0xffffU) {
                prefetch(h, 0x00080338U);
                const auto irq334 = interrupt(
                    context, 0x00080334U, 0x00080338U, 1U, 0x0008033aU);
                if (irq334.status != TranslationStatus::complete) return irq334;
                pf33a = irq334.control != 0U;
                break;
            }
            const auto irq334 = interrupt(
                context, 0x00080334U, 0x00080326U, 1U, 0x00080328U);
            if (irq334.status != TranslationStatus::complete) return irq334;
            pf328 = irq334.control != 0U;
        }

        r.address[0] = r.address[2];
        if (!pf33a) prefetch(h, 0x0008033aU);
        r.address[0] -= 1U;
        prefetch(h, 0x0008033cU);
        const auto irq33a = interrupt(
            context, 0x0008033aU, 0x0008033cU, 0U, 0x0008033eU);
        if (irq33a.status != TranslationStatus::complete) return irq33a;
        const auto outer = static_cast<std::uint16_t>(r.data[4] - 1U);
        r.data[4] = (r.data[4] & 0xffff0000U) | outer;
        if (irq33a.control == 0U) prefetch(h, 0x0008033eU);
        prefetch(h, 0x0008031eU);
        const auto irq33c = interrupt(context, 0x0008033cU,
            outer == 0xffffU ? 0x00080340U : 0x0008031eU, 1U,
            outer == 0xffffU ? 0U : 0x00080320U);
        if (irq33c.status != TranslationStatus::complete) return irq33c;
        if (outer == 0xffffU) break;
        outer_320_prefetched = irq33c.control != 0U;
    }

    prefetch(h, 0x00080340U);
    prefetch(h, 0x00080342U);
    r.address[0] += 0x21U;
    prefetch(h, 0x00080344U);
    prefetch(h, 0x00080346U);
    const auto high = read_cpu(h, r.address[7]);
    const auto low = read_cpu(h, r.address[7] + 2U);
    r.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    prefetch(h, target);
    prefetch(h, target + 2U);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated

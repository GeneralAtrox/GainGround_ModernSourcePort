#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kTileRegion = 5U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_flags(CpuRegisters &r, std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t f = r.status & 0x0010U;
    if ((value & sign) != 0U) f |= 0x0008U;
    if (value == 0U) f |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}

void set_add_word_flags(CpuRegisters &r, std::uint16_t a, std::uint16_t b, std::uint16_t q)
{
    std::uint16_t f{};
    if ((q & 0x8000U) != 0U) f |= 0x0008U;
    if (q == 0U) f |= 0x0004U;
    if (((~(a ^ b)) & (a ^ q) & 0x8000U) != 0U) f |= 0x0002U;
    if (static_cast<std::uint32_t>(a) + b > 0xffffU) f |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}

void set_sub_byte_flags(CpuRegisters &r, std::uint8_t a, std::uint8_t b, std::uint8_t q)
{
    std::uint16_t f{};
    if ((q & 0x80U) != 0U) f |= 0x0008U;
    if (q == 0U) f |= 0x0004U;
    if (((a ^ b) & (a ^ q) & 0x80U) != 0U) f |= 0x0002U;
    if (b > a) f |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &h, std::uint32_t a)
{
    const auto hi = h.read_memory_word(kPrivateRegion, a, kWordMask);
    const auto lo = h.read_memory_word(kPrivateRegion, a + 2U, kWordMask);
    return (static_cast<std::uint32_t>(hi) << 16U) | lo;
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &h, std::uint32_t a)
{
    const bool odd = (a & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto word = h.read_memory_word(kPrivateRegion, a & ~1U, mask);
    return static_cast<std::uint8_t>(odd ? word : (word >> 8U));
}

void write_tile_word(ExecutionHost &h, std::uint32_t a, std::uint16_t value)
{
    h.write_memory_word(kTileRegion, a - 0x00200000U, value, kWordMask);
}

void write_tile_long(ExecutionHost &h, std::uint32_t a, std::uint32_t value)
{
    write_tile_word(h, a, static_cast<std::uint16_t>(value >> 16U));
    write_tile_word(h, a + 2U, static_cast<std::uint16_t>(value));
}

void clear_tile_long(ExecutionHost &h, CpuRegisters &r, std::uint32_t a)
{
    (void)h.read_memory_word(kTileRegion, a - 0x00200000U, kWordMask);
    (void)h.read_memory_word(kTileRegion, a - 0x00200000U + 2U, kWordMask);
    h.write_memory_word(kTileRegion, a - 0x00200000U + 2U, 0U, kWordMask);
    h.write_memory_word(kTileRegion, a - 0x00200000U, 0U, kWordMask);
    set_logic_flags(r, 0U, 0x80000000U);
}

void push_return(ExecutionHost &h, CpuRegisters &r, std::uint32_t pc)
{
    r.address[7] -= 4U;
    h.write_memory_word(kPrivateRegion, r.address[7], static_cast<std::uint16_t>(pc >> 16U), kWordMask);
    h.write_memory_word(kPrivateRegion, r.address[7] + 2U, static_cast<std::uint16_t>(pc), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &h, CpuRegisters &r)
{
    const auto pc = read_long(h, r.address[7]);
    r.address[7] += 4U;
    return pc;
}

[[nodiscard]] FunctionResult call_child(FunctionContext &c, std::uint32_t id,
    std::uint32_t callsite, std::uint32_t target, std::uint32_t return_pc)
{
    auto &h = *c.host;
    push_return(h, c.registers, return_pc);
    c.registers.program_counter = target;
    return h.call_function(id, 1U, 0x72U, 2U, callsite, target, c);
}

[[nodiscard]] bool child_returned(const FunctionResult &result)
{
    return result.status == TranslationStatus::complete && result.control == 1U;
}

void move_d0_word(CpuRegisters &r, std::uint16_t value)
{
    r.data[0] = (r.data[0] & 0xffff0000U) | value;
    set_logic_flags(r, value, 0x8000U);
}

void asl_d0_word(CpuRegisters &r, unsigned count)
{
    auto value = static_cast<std::uint16_t>(r.data[0]);
    bool overflow{};
    bool carry{};
    for (unsigned i = 0; i != count; ++i) {
        const bool old_sign = (value & 0x8000U) != 0U;
        carry = old_sign;
        value = static_cast<std::uint16_t>(value << 1U);
        overflow = overflow || (old_sign != ((value & 0x8000U) != 0U));
    }
    std::uint16_t f{};
    if ((value & 0x8000U) != 0U) f |= 0x0008U;
    if (value == 0U) f |= 0x0004U;
    if (overflow) f |= 0x0002U;
    if (carry) f |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
    r.data[0] = (r.data[0] & 0xffff0000U) | value;
}

void add_d0_word(CpuRegisters &r, std::uint16_t addend)
{
    const auto a = static_cast<std::uint16_t>(r.data[0]);
    const auto q = static_cast<std::uint16_t>(a + addend);
    set_add_word_flags(r, a, addend, q);
    r.data[0] = (r.data[0] & 0xffff0000U) | q;
}

void internal_incrementing_rows(FunctionContext &c, bool load_count)
{
    auto &h = *c.host;
    auto &r = c.registers;
    if (load_count) {
        const auto count = h.read_memory_word(kPrivateRegion, r.address[1], kWordMask);
        r.address[1] += 2U;
        r.data[2] = (r.data[2] & 0xffff0000U) | count;
        set_logic_flags(r, count, 0x8000U);
    }
    for (;;) {
        const auto value = h.read_memory_word(kPrivateRegion, r.address[1], kWordMask);
        r.address[1] += 2U;
        move_d0_word(r, value);
        write_tile_word(h, r.address[0], static_cast<std::uint16_t>(r.data[0]));
        set_logic_flags(r, static_cast<std::uint16_t>(r.data[0]), 0x8000U);
        add_d0_word(r, 1U);
        write_tile_word(h, r.address[0], static_cast<std::uint16_t>(r.data[0]));
        set_logic_flags(r, static_cast<std::uint16_t>(r.data[0]), 0x8000U);
        r.address[0] += 0x7eU;
        const auto counter = static_cast<std::uint16_t>(r.data[2] - 1U);
        r.data[2] = (r.data[2] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
    }
}
} // namespace

FunctionResult cpu_b_process_object_record(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;

    move_d0_word(r, static_cast<std::uint16_t>(r.data[5]));
    r.address[3] = 0x00007800U;
    asl_d0_word(r, 3U);
    r.address[3] += static_cast<std::int16_t>(r.data[0]);
    move_d0_word(r, static_cast<std::uint16_t>(r.data[5]));
    add_d0_word(r, 1U);
    const auto compared = static_cast<std::uint16_t>(r.data[0]);
    const auto compare_result = static_cast<std::uint16_t>(compared - 10U);
    std::uint16_t compare_flags = r.status & 0x0010U;
    if ((compare_result & 0x8000U) != 0U) compare_flags |= 0x0008U;
    if (compare_result == 0U) compare_flags |= 0x0004U;
    if (((compared ^ 10U) & (compared ^ compare_result) & 0x8000U) != 0U) compare_flags |= 0x0002U;
    if (10U > compared) compare_flags |= 0x0001U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x000fU) | compare_flags);
    if ((compare_result & 0x8000U) != 0U) {
        asl_d0_word(r, 2U);
        add_d0_word(r, 0x80c0U);
        r.address[0] = r.address[4];
        const auto child = call_child(context, 298U, 0x00009e22U, 0x000161eaU, 0x00009e28U);
        if (!child_returned(child)) return child;
    } else {
        const auto decimal = call_child(context, 299U, 0x00009e2aU, 0x00016234U, 0x00009e30U);
        if (!child_returned(decimal)) return decimal;
        r.data[2] = r.data[0];
        set_logic_flags(r, r.data[2], 0x80000000U);
        const auto old_d2 = r.data[2];
        r.data[2] = (old_d2 >> 8U) | (old_d2 << 24U);
        std::uint16_t f = r.status & 0x0010U;
        if ((r.data[2] & 0x80000000U) != 0U) f |= 0x0008U;
        if (r.data[2] == 0U) f |= 0x0004U;
        if ((old_d2 & 0x00000080U) != 0U) f |= 0x0001U;
        r.status = static_cast<std::uint16_t>((r.status & ~0x000fU) | f);
        r.data[3] = 1U;
        set_logic_flags(r, 1U, 0x80000000U);
        r.address[0] = r.address[4];
        const auto digits = call_child(context, 294U, 0x00009e38U, 0x000160c8U, 0x00009e3eU);
        if (digits.status != TranslationStatus::complete) return digits;
    }

    r.address[0] = r.address[4] + 0x200U;
    r.address[1] = r.address[3] + 4U;
    r.data[1] = 2U;
    set_logic_flags(r, 2U, 0x80000000U);
    for (;;) {
        r.data[0] = 0U;
        set_logic_flags(r, 0U, 0x80000000U);
        const auto byte = read_byte(h, r.address[1]++);
        r.data[0] = byte;
        set_logic_flags(r, byte, 0x80U);
        if (byte == 0U || byte == 0x20U) {
            r.data[0] = 0U;
            set_logic_flags(r, 0U, 0x80000000U);
        } else if (byte == 0x2eU) {
            move_d0_word(r, 0x8164U);
        } else {
            asl_d0_word(r, 2U);
            add_d0_word(r, 0x7fe4U);
        }
        const auto child = call_child(context, 298U, 0x00009e6aU, 0x000161eaU, 0x00009e70U);
        if (!child_returned(child)) return child;
        r.address[0] += 0x7eU;
        const auto counter = static_cast<std::uint16_t>(r.data[1] - 1U);
        r.data[1] = (r.data[1] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
    }

    r.address[0] = r.address[4] + 0x700U;
    r.data[1] = 0U;
    set_logic_flags(r, 0U, 0x80000000U);
    const auto numeric = read_byte(h, r.address[3] + 7U);
    r.data[1] = numeric;
    set_logic_flags(r, numeric, 0x80U);
    if (numeric < 0x28U) {
        const auto quotient = static_cast<std::uint16_t>(numeric / 10U);
        const auto remainder = static_cast<std::uint16_t>(numeric % 10U);
        r.data[1] = (static_cast<std::uint32_t>(remainder) << 16U) | quotient;
        set_logic_flags(r, quotient, 0x8000U);
        r.data[0] = r.data[1];
        set_logic_flags(r, r.data[0], 0x80000000U);
        asl_d0_word(r, 2U);
        add_d0_word(r, 0x80c4U);
        auto child = call_child(context, 298U, 0x00009e94U, 0x000161eaU, 0x00009e9aU);
        if (!child_returned(child)) return child;
        write_tile_long(h, r.address[0] + 0x7eU, 0x80ac80adU);
        set_logic_flags(r, 0x80ac80adU, 0x80000000U);
        r.address[0] += 0xfeU;
        r.data[1] = (r.data[1] << 16U) | (r.data[1] >> 16U);
        set_logic_flags(r, r.data[1], 0x80000000U);
        if (static_cast<std::uint16_t>(r.data[1]) != 9U) {
            move_d0_word(r, static_cast<std::uint16_t>(r.data[1]));
            asl_d0_word(r, 2U);
            add_d0_word(r, 0x80c4U);
            child = call_child(context, 298U, 0x00009eb6U, 0x000161eaU, 0x00009ebcU);
            if (!child_returned(child)) return child;
        } else {
            r.address[1] = 0x0000a100U;
            r.data[2] = 1U;
            set_logic_flags(r, 1U, 0x80000000U);
            push_return(h, r, 0x00009ecaU);
            internal_incrementing_rows(context, false);
            r.program_counter = pop_return(h, r);
        }
    } else {
        r.address[1] = 0x0000a104U;
        push_return(h, r, 0x00009ed6U);
        internal_incrementing_rows(context, true);
        r.program_counter = pop_return(h, r);
    }

    r.address[0] = r.address[4] + 0x0b00U;
    r.data[2] = read_long(h, r.address[3]);
    set_logic_flags(r, r.data[2], 0x80000000U);
    r.data[3] = 7U;
    set_logic_flags(r, 7U, 0x80000000U);
    r.data[4] = (r.data[4] & 0xffff0000U) | 0x0048U;
    set_logic_flags(r, 0x0048U, 0x8000U);
    for (;;) {
        const auto old_d2 = r.data[2];
        r.data[2] = (old_d2 << 4U) | (old_d2 >> 28U);
        std::uint16_t rotate_flags = r.status & 0x0010U;
        if ((r.data[2] & 0x80000000U) != 0U) rotate_flags |= 0x0008U;
        if (r.data[2] == 0U) rotate_flags |= 0x0004U;
        if ((old_d2 & 0x10000000U) != 0U) rotate_flags |= 0x0001U;
        r.status = static_cast<std::uint16_t>((r.status & ~0x000fU) | rotate_flags);
        move_d0_word(r, static_cast<std::uint16_t>(r.data[2]));
        const auto nibble = static_cast<std::uint16_t>(r.data[0] & 0x000fU);
        move_d0_word(r, nibble);
        bool zero_output{};
        if (nibble != 0U) {
            const bool old = (r.data[3] & 0x80000000U) != 0U;
            r.data[3] |= 0x80000000U;
            if (old) r.status = static_cast<std::uint16_t>(r.status & ~0x0004U);
            else r.status = static_cast<std::uint16_t>(r.status | 0x0004U);
        } else {
            const auto word = static_cast<std::uint16_t>(r.data[3]);
            set_logic_flags(r, word, 0x8000U);
            if (word != 0U) {
                set_logic_flags(r, r.data[3], 0x80000000U);
                if ((r.data[3] & 0x80000000U) == 0U) zero_output = true;
                else {
                    const bool old = (r.data[3] & 0x80000000U) != 0U;
                    r.data[3] |= 0x80000000U;
                    if (old) r.status = static_cast<std::uint16_t>(r.status & ~0x0004U);
                    else r.status = static_cast<std::uint16_t>(r.status | 0x0004U);
                }
            }
        }
        if (zero_output) {
            r.data[0] = 0U;
            set_logic_flags(r, 0U, 0x80000000U);
        } else {
            add_d0_word(r, 0x0030U);
            const auto before = static_cast<std::uint16_t>(r.data[0]);
            const auto doubled = static_cast<std::uint16_t>(before + before);
            set_add_word_flags(r, before, before, doubled);
            r.data[0] = (r.data[0] & 0xffff0000U) | doubled;
        }
        move_d0_word(r, static_cast<std::uint16_t>(r.data[0] | 0x8000U));
        write_tile_word(h, r.address[0], static_cast<std::uint16_t>(r.data[0]));
        r.address[0] += 2U;
        set_logic_flags(r, static_cast<std::uint16_t>(r.data[0]), 0x8000U);
        add_d0_word(r, 1U);
        write_tile_word(h, r.address[0], static_cast<std::uint16_t>(r.data[0]));
        set_logic_flags(r, static_cast<std::uint16_t>(r.data[0]), 0x8000U);
        r.address[0] += 0x7eU;
        const auto d4b = static_cast<std::uint8_t>(r.data[4]);
        const auto doubled_b = static_cast<std::uint8_t>(d4b + d4b);
        std::uint16_t byte_flags{};
        if ((doubled_b & 0x80U) != 0U) byte_flags |= 0x0008U;
        if (doubled_b == 0U) byte_flags |= 0x0004U;
        if (((~(d4b ^ d4b)) & (d4b ^ doubled_b) & 0x80U) != 0U) byte_flags |= 0x0002U;
        const bool byte_carry = static_cast<unsigned>(d4b) * 2U > 0xffU;
        if (byte_carry) byte_flags |= 0x0011U;
        r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | byte_flags);
        r.data[4] = (r.data[4] & 0xffffff00U) | doubled_b;
        if (byte_carry) {
            clear_tile_long(h, r, r.address[0]);
            const auto before = static_cast<std::uint8_t>(r.data[0]);
            const auto after = static_cast<std::uint8_t>(before - 1U);
            r.data[0] = (r.data[0] & 0xffffff00U) | after;
            set_sub_byte_flags(r, before, 1U, after);
            if (after != 0U) {
                write_tile_long(h, r.address[0], 0x80b280b3U);
                set_logic_flags(r, 0x80b280b3U, 0x80000000U);
            }
            r.address[0] += 0x80U;
        }
        const auto counter = static_cast<std::uint16_t>(r.data[3] - 1U);
        r.data[3] = (r.data[3] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
    }

    const auto return_address = pop_return(h, r);
    r.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated

#include "cpu_b_compute_record_deltas_detail.h"

namespace gain_ground::translated::cpu_b_compute_record_deltas_detail {

constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kStatusMask = 0x001fU;
constexpr std::uint16_t kExtendBit = 0x0010U;
constexpr std::uint16_t kZeroBit = 0x0004U;
constexpr std::uint16_t kOverflowBit = 0x0002U;
constexpr std::uint16_t kCarryBit = 0x0001U;

[[nodiscard]] bool get_x(CpuRegisters &r) { return (r.status & kExtendBit) != 0U; }
void set_x(CpuRegisters &r, bool v)
{
    if (v) r.status |= kExtendBit; else r.status &= static_cast<std::uint16_t>(~kExtendBit);
}

[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & 0x00ffffffU, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t data)
{
    host.write_memory_word(kRegion, address & 0x00ffffffU, data, kWordMask);
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto w = host.read_memory_word(kRegion, address & ~1U, mask);
    return static_cast<std::uint8_t>(odd ? (w & 0xffU) : (w >> 8U));
}

void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    write_word(host, r.address[7], static_cast<std::uint16_t>(value >> 16U));
    write_word(host, r.address[7] + 2U, static_cast<std::uint16_t>(value));
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto hi = read_word(host, r.address[7]);
    const auto lo = read_word(host, r.address[7] + 2U);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(hi) << 16U) | lo;
}

// Commit only N/Z/V/C; preserve current X.
void commit_nzvc(CpuRegisters &r, bool n, bool z, bool v, bool c)
{
    std::uint16_t f = r.status & kExtendBit;
    if (n) f |= kNegativeBit;
    if (z) f |= kZeroBit;
    if (v) f |= kOverflowBit;
    if (c) f |= kCarryBit;
    r.status = static_cast<std::uint16_t>((r.status & ~kStatusMask) | f);
}

// Commit full CCR including explicit X.
void commit_full(CpuRegisters &r, bool x, bool n, bool z, bool v, bool c)
{
    std::uint16_t f{};
    if (x) f |= kExtendBit;
    if (n) f |= kNegativeBit;
    if (z) f |= kZeroBit;
    if (v) f |= kOverflowBit;
    if (c) f |= kCarryBit;
    r.status = static_cast<std::uint16_t>((r.status & ~kStatusMask) | f);
}

void do_add_word(CpuRegisters &r, std::uint16_t src, std::uint16_t &dst_reg_low,
                 bool &x_flag)
{
    const std::uint32_t wide =
        static_cast<std::uint32_t>(dst_reg_low) + src;
    const auto result = static_cast<std::uint16_t>(wide);
    const bool carry = wide > 0xffffU;
    const bool ovf = ((~(dst_reg_low ^ src)) & (dst_reg_low ^ result) & 0x8000U) != 0U;
    dst_reg_low = result;
    x_flag = carry;
    commit_nzvc(r, (result & 0x8000U) != 0U, result == 0U, ovf, carry);
}

void do_sub_word(CpuRegisters &r, std::uint16_t src, std::uint16_t &dst_reg_low,
                 bool &x_flag)
{
    const auto result = static_cast<std::uint16_t>(dst_reg_low - src);
    const bool borrow = dst_reg_low < src;
    const bool ovf = ((dst_reg_low ^ src) & (dst_reg_low ^ result) & 0x8000U) != 0U;
    dst_reg_low = result;
    x_flag = borrow;
    commit_nzvc(r, (result & 0x8000U) != 0U, result == 0U, ovf, borrow);
}

std::optional<FunctionResult> prepare_phase(FunctionContext &context,
    std::uint32_t a5)
{
    auto &host = *context.host;
    auto &r = context.registers;
// ---- 0x1fc88: movea.l (0x66,a5),a0 ----
{
    const auto hi = read_word(host, a5 + 0x66U);
    const auto lo = read_word(host, a5 + 0x68U);
    r.address[0] = (static_cast<std::uint32_t>(hi) << 16U) | lo;
}

// ---- 0x1fc8c: move.w (0x18,a0),d0w ----
// Upper word of d0 preserved.
{
    const auto v = read_word(host, r.address[0] + 0x18U);
    r.data[0] = (r.data[0] & 0xffff0000U) | v;
}

// ---- 0x1fc90: add.w d0,d0 (d0w *= 2); sets N/Z/V/C, X preserved ----
{
    auto lo = static_cast<std::uint16_t>(r.data[0] & 0xffffU);
    bool xf = get_x(r);
    do_add_word(r, lo, lo, xf);
    set_x(r, xf);
    r.data[0] = (r.data[0] & 0xffff0000U) | lo;
}

// ---- 0x1fc92: move.w d0,d1w (upper word of d1 preserved); N/Z ----
{
    const auto lo = static_cast<std::uint16_t>(r.data[0] & 0xffffU);
    r.data[1] = (r.data[1] & 0xffff0000U) | lo;
    commit_nzvc(r, (lo & 0x8000U) != 0U, lo == 0U, false, false);
}

// ---- 0x1fc94: asl.w #4,d1w ----
// ASL.W sets X=C=last bit shifted out (= bit12 of original for #4).
// V is set if the sign changes during any constituent single-bit shift.
{
    auto lo = static_cast<std::uint16_t>(r.data[1] & 0xffffU);
    bool carry{};
    bool overflow{};
    for (unsigned i = 0; i < 4U; ++i) {
        const bool msb = (lo & 0x8000U) != 0U;
        const auto shifted = static_cast<std::uint16_t>(lo << 1U);
        overflow = overflow || ((lo ^ shifted) & 0x8000U) != 0U;
        carry = msb;
        lo = shifted;
    }
    r.data[1] = (r.data[1] & 0xffff0000U) | lo;
    commit_full(r, carry, (lo & 0x8000U) != 0U, lo == 0U,
        overflow, carry);
}

// ---- 0x1fc96: add.w d0,d1w ----
{
    auto lo = static_cast<std::uint16_t>(r.data[1] & 0xffffU);
    const auto src = static_cast<std::uint16_t>(r.data[0] & 0xffffU);
    bool xf = get_x(r);
    do_add_word(r, src, lo, xf);
    set_x(r, xf);
    r.data[1] = (r.data[1] & 0xffff0000U) | lo;
}
// d1w now contains mode * 34 = byte offset into the mode table.

// ---- 0x1fc98: lea 0x002a6e0.l,a0 ----
r.address[0] = 0x002a6e0U;

// ---- 0x1fc9e: adda.w #2,a0 (sign-extends #2 to $0002) ----
r.address[0] = (r.address[0] + 0x0002U) & 0x00ffffffU;

// ---- 0x1fca2: adda.w d1w,a0 (sign-extends d1w to 32-bit before adding) ----
r.address[0] = (r.address[0]
    + static_cast<std::uint32_t>(static_cast<std::int16_t>(
        static_cast<std::uint16_t>(r.data[1] & 0xffffU))))
    & 0x00ffffffU;

// ---- 0x1fca4..b3: band select ----
// d0w = ((word[a5+0x5c] + 0x80) & 0x700) >> 6
// No CCR tracking needed for address computation.
{
    const std::uint16_t band = static_cast<std::uint16_t>(
        ((read_word(host, a5 + 0x5cU) + 0x0080U) & 0x0700U) >> 6U);
    r.address[0] = (r.address[0] + band) & 0x00ffffffU;
}

// ---- Phase 2: delta computation ----

// ---- 0x1fcb4: move.w (0x6a,a5),d0w (upper preserved) ----
r.data[0] = (r.data[0] & 0xffff0000U)
    | read_word(host, a5 + 0x6aU);

// ---- 0x1fcb8: sub.w (0x12,a5),d0w ----
{
    const auto src = read_word(host, a5 + 0x12U);
    auto lo = static_cast<std::uint16_t>(r.data[0] & 0xffffU);
    bool xf = get_x(r);
    do_sub_word(r, src, lo, xf);
    set_x(r, xf);
    r.data[0] = (r.data[0] & 0xffff0000U) | lo;
}

// ---- 0x1fcbc: sub.w (a0)+,d0w (read then post-increment by 2) ----
{
    const auto tbl = read_word(host, r.address[0]);
    r.address[0] = (r.address[0] + 2U) & 0x00ffffffU;
    auto lo = static_cast<std::uint16_t>(r.data[0] & 0xffffU);
    bool xf = get_x(r);
    do_sub_word(r, tbl, lo, xf);
    set_x(r, xf);
    r.data[0] = (r.data[0] & 0xffff0000U) | lo;
}

// ---- 0x1fcbe: move.w (0x6c,a5),d1w (upper preserved) ----
r.data[1] = (r.data[1] & 0xffff0000U)
    | read_word(host, a5 + 0x6cU);

// ---- 0x1fcc2: sub.w (0x1a,a5),d1w ----
{
    const auto src = read_word(host, a5 + 0x1aU);
    auto lo = static_cast<std::uint16_t>(r.data[1] & 0xffffU);
    bool xf = get_x(r);
    do_sub_word(r, src, lo, xf);
    set_x(r, xf);
    r.data[1] = (r.data[1] & 0xffff0000U) | lo;
}

// ---- 0x1fcc6: sub.w (a0)+,d1w (read then post-increment by 2) ----
{
    const auto tbl = read_word(host, r.address[0]);
    r.address[0] = (r.address[0] + 2U) & 0x00ffffffU;
    auto lo = static_cast<std::uint16_t>(r.data[1] & 0xffffU);
    bool xf = get_x(r);
    do_sub_word(r, tbl, lo, xf);
    set_x(r, xf);
    r.data[1] = (r.data[1] & 0xffff0000U) | lo;
}

// ---- 0x1fcc8: jsr 0x1631a → fn304 apply_signed_lookup_result ----
push_return(host, r, 0x0001fcceU);
r.program_counter = 0x0001631aU;
{
    const auto child = host.call_function(
        304U, 1U, 0x72U, 2U, 0x0001fcc8U, 0x0001631aU, context);
    // Function 304 has an authoritative JMP tail into function 301, which
    // reports control 3 after returning through this call's continuation.
    if (child.status != TranslationStatus::complete
        || (child.control != 1U && child.control != 3U))
        return child;
}

    return std::nullopt;
}

} // namespace gain_ground::translated::cpu_b_compute_record_deltas_detail

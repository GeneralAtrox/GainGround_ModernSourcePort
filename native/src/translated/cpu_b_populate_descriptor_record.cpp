#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivate = 2U;
constexpr std::uint16_t kMask = 0xffffU;
struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };
ByteLocation byte_location(std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U, static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}
std::uint8_t read_byte(ExecutionHost &h, std::uint32_t address)
{
    const auto b = byte_location(address);
    return static_cast<std::uint8_t>(h.read_memory_word(kPrivate, b.offset, b.mask) >> b.shift);
}
void write_byte(ExecutionHost &h, std::uint32_t address, std::uint8_t value)
{
    const auto b = byte_location(address);
    h.write_memory_word(kPrivate, b.offset, static_cast<std::uint16_t>(value) << b.shift, b.mask);
}
void set_logic(CpuRegisters &r, std::uint32_t value, std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x10U;
    if ((value & sign) != 0U) flags |= 8U;
    if ((value & mask) == 0U) flags |= 4U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | flags);
}
void set_add_word(CpuRegisters &r, std::uint16_t l, std::uint16_t q, std::uint16_t v)
{
    const auto wide = static_cast<std::uint32_t>(l) + q;
    std::uint16_t f{};
    if (v & 0x8000U) f |= 8U;
    if (v == 0U) f |= 4U;
    if (((~(l ^ q)) & (l ^ v) & 0x8000U) != 0U) f |= 2U;
    if (wide > 0xffffU) f |= 0x11U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
void set_sub_word(CpuRegisters &r, std::uint16_t l, std::uint16_t q, std::uint16_t v)
{
    std::uint16_t f{};
    if (v & 0x8000U) f |= 8U;
    if (v == 0U) f |= 4U;
    if (((l ^ q) & (l ^ v) & 0x8000U) != 0U) f |= 2U;
    if (q > l) f |= 0x11U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
std::uint32_t read_long(ExecutionHost &h, std::uint32_t address)
{
    const auto hi = h.read_memory_word(kPrivate, address, kMask);
    const auto lo = h.read_memory_word(kPrivate, address + 2U, kMask);
    return (static_cast<std::uint32_t>(hi) << 16U) | lo;
}
void write_long(ExecutionHost &h, std::uint32_t address, std::uint32_t value)
{
    h.write_memory_word(kPrivate, address, static_cast<std::uint16_t>(value >> 16U), kMask);
    h.write_memory_word(kPrivate, address + 2U, static_cast<std::uint16_t>(value), kMask);
}
void bit_change(ExecutionHost &h, CpuRegisters &r, std::uint32_t address, unsigned bit, bool set)
{
    const auto old = read_byte(h, address);
    if ((old & (1U << bit)) != 0U) r.status &= static_cast<std::uint16_t>(~4U);
    else r.status |= 4U;
    const auto value = static_cast<std::uint8_t>(set ? old | (1U << bit) : old & ~(1U << bit));
    write_byte(h, address, value);
}
std::uint32_t pop_return(ExecutionHost &h, CpuRegisters &r)
{
    const auto value = read_long(h, r.address[7]); r.address[7] += 4U; return value;
}
} // namespace

FunctionResult cpu_b_populate_descriptor_record(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &h = *context.host; auto &r = context.registers;
    auto &a0 = r.address[0]; auto &a1 = r.address[1]; auto &a6 = r.address[6];

    const auto move_word_to = [&](std::uint32_t destination) {
        const auto v = h.read_memory_word(kPrivate, a0, kMask); a0 += 2U;
        h.write_memory_word(kPrivate, destination, v, kMask); set_logic(r, v, 0x8000U, 0xffffU); return v;
    };
    const auto move_long_from_a0 = [&](std::uint32_t destination) {
        const auto v = read_long(h, a0); a0 += 4U; write_long(h, destination, v); set_logic(r, v, 0x80000000U, 0xffffffffU); return v;
    };
    const auto move_long_from_a1 = [&](std::uint32_t destination) {
        const auto v = read_long(h, a1); a1 += 4U; write_long(h, destination, v); set_logic(r, v, 0x80000000U, 0xffffffffU); return v;
    };

    h.write_memory_word(kPrivate, a6, 0x8000U, kMask); set_logic(r, 0x8000U, 0x8000U, 0xffffU);
    a1 = read_long(h, a0); a0 += 4U;
    move_long_from_a0(a6 + 2U);
    auto d0 = move_word_to(a6 + 0x12U);
    for (const auto o : {0x2aU, 0x2cU, 0x62U}) { h.write_memory_word(kPrivate, a6 + o, d0, kMask); set_logic(r,d0,0x8000U,0xffffU); }
    d0 = move_word_to(a6 + 0x16U);
    for (const auto o : {0x2eU, 0x30U}) { h.write_memory_word(kPrivate, a6 + o, d0, kMask); set_logic(r,d0,0x8000U,0xffffU); }
    d0 = move_word_to(a6 + 0x1aU);
    for (const auto o : {0x32U, 0x34U, 0x64U}) { h.write_memory_word(kPrivate, a6 + o, d0, kMask); set_logic(r,d0,0x8000U,0xffffU); }
    d0 = h.read_memory_word(kPrivate, a0, kMask); a0 += 2U; r.data[0]=(r.data[0]&0xffff0000U)|d0; set_logic(r,d0,0x8000U,0xffffU);
    write_byte(h,a6+0x36U,static_cast<std::uint8_t>(d0)); set_logic(r,d0,0x80U,0xffU);
    write_byte(h,a6+0x37U,static_cast<std::uint8_t>(d0)); set_logic(r,d0,0x80U,0xffU);
    move_word_to(a6 + 0x5eU); move_word_to(a6 + 0x60U);
    move_long_from_a0(a6 + 0x66U); move_long_from_a0(a6 + 0x6eU);
    h.write_memory_word(kPrivate,a6+0x5cU,0x0200U,kMask); set_logic(r,0x0200U,0x8000U,0xffffU);
    move_long_from_a1(a6+0x48U); move_long_from_a1(a6+0x50U);
    auto b=read_byte(h,a1++); write_byte(h,a6+0x38U,b); set_logic(r,b,0x80U,0xffU);
    b=read_byte(h,a1++); write_byte(h,a6+0x3cU,b); set_logic(r,b,0x80U,0xffU);
    auto w=h.read_memory_word(kPrivate,a1,kMask); a1+=2U; h.write_memory_word(kPrivate,a6+0x42U,w,kMask); set_logic(r,w,0x8000U,0xffffU);
    w=h.read_memory_word(kPrivate,a1,kMask); a1+=2U; h.write_memory_word(kPrivate,a6+0x44U,w,kMask); set_logic(r,w,0x8000U,0xffffU);
    r.data[1]=0U; set_logic(r,0U,0x80000000U,0xffffffffU);
    b=read_byte(h,a1++); r.data[1]=(r.data[1]&0xffffff00U)|b; set_logic(r,b,0x80U,0xffU);
    w=static_cast<std::uint16_t>(r.data[1]); h.write_memory_word(kPrivate,a6+0x46U,w,kMask); set_logic(r,w,0x8000U,0xffffU);
    h.write_memory_word(kPrivate,a6+0x76U,w,kMask); set_logic(r,w,0x8000U,0xffffU);
    b=read_byte(h,a1++); write_byte(h,a6+0x0bU,b); set_logic(r,b,0x80U,0xffU);
    w=static_cast<std::uint16_t>(r.data[6]); h.write_memory_word(kPrivate,a6+0x54U,w,kMask); set_logic(r,w,0x8000U,0xffffU);
    h.write_memory_word(kPrivate,a6+0x56U,0U,kMask); set_logic(r,0U,0x8000U,0xffffU);
    bit_change(h,r,a6+0x40U,2U,true); bit_change(h,r,a6+0x40U,3U,true); bit_change(h,r,a6+0x40U,1U,false);
    r.data[1]=(r.data[1]&0xffff0000U)|static_cast<std::uint16_t>(a6); set_logic(r,static_cast<std::uint16_t>(a6),0x8000U,0xffffU);
    b=read_byte(h,a6+0x38U); r.data[0]=(r.data[0]&0xffffff00U)|b; set_logic(r,b,0x80U,0xffU);
    auto d0w=static_cast<std::uint16_t>(r.data[0]); auto reduced=static_cast<std::uint16_t>(d0w-2U);
    r.data[0]=(r.data[0]&0xffff0000U)|reduced; set_sub_word(r,d0w,2U,reduced);
    if ((reduced & 0x8000U)==0U) {
        r.data[2]=6U; set_logic(r,6U,0x80000000U,0xffffffffU);
        for (;;) {
            a6=static_cast<std::uint32_t>(static_cast<std::int64_t>(a6)+0x80);
            h.write_memory_word(kPrivate,a6,0x8000U,kMask); set_logic(r,0x8000U,0x8000U,0xffffU);
            w=static_cast<std::uint16_t>(r.data[1]); h.write_memory_word(kPrivate,a6+0x36U,w,kMask); set_logic(r,w,0x8000U,0xffffU);
            write_byte(h,a6+0x0bU,0U); set_logic(r,0U,0x80U,0xffU);
            write_long(h,a6+2U,0x0001ef62U); set_logic(r,0x0001ef62U,0x80000000U,0xffffffffU);
            w=static_cast<std::uint16_t>(r.data[2]); h.write_memory_word(kPrivate,a6+0x38U,w,kMask); set_logic(r,w,0x8000U,0xffffU);
            const auto next=static_cast<std::uint16_t>(w+6U); r.data[2]=(r.data[2]&0xffff0000U)|next; set_add_word(r,w,6U,next);
            reduced=static_cast<std::uint16_t>(reduced-1U); r.data[0]=(r.data[0]&0xffff0000U)|reduced;
            if (reduced==0xffffU) break;
        }
    }
    const auto target=pop_return(h,r); r.program_counter=target; return FunctionResult::complete(1U,target);
}

} // namespace gain_ground::translated

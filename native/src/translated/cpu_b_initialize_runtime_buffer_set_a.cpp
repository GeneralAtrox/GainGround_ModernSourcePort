#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic(CpuRegisters &r, std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &h, CpuRegisters &r, std::uint32_t pc)
{
    r.address[7] -= 4U;
    h.write_memory_word(kPrivateRegion, r.address[7], static_cast<std::uint16_t>(pc >> 16U), kWordMask);
    h.write_memory_word(kPrivateRegion, r.address[7] + 2U, static_cast<std::uint16_t>(pc), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &h, CpuRegisters &r)
{
    const auto high = h.read_memory_word(kPrivateRegion, r.address[7], kWordMask);
    const auto low = h.read_memory_word(kPrivateRegion, r.address[7] + 2U, kWordMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void clear_long(ExecutionHost &h, CpuRegisters &r, std::uint32_t address)
{
    (void)h.read_memory_word(kPrivateRegion, address, kWordMask);
    (void)h.read_memory_word(kPrivateRegion, address + 2U, kWordMask);
    h.write_memory_word(kPrivateRegion, address + 2U, 0U, kWordMask);
    h.write_memory_word(kPrivateRegion, address, 0U, kWordMask);
    set_logic(r, 0U, 0x80000000U);
}

void clear_word(ExecutionHost &h, CpuRegisters &r, std::uint32_t address)
{
    (void)h.read_memory_word(kPrivateRegion, address, kWordMask);
    h.write_memory_word(kPrivateRegion, address, 0U, kWordMask);
    set_logic(r, 0U, 0x8000U);
}

void store_true_byte(ExecutionHost &h, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    (void)h.read_memory_word(kPrivateRegion, address & ~1U, mask);
    h.write_memory_word(kPrivateRegion, address & ~1U, mask, mask);
}
} // namespace

FunctionResult cpu_b_initialize_runtime_buffer_set_a(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;
    const auto entry_pc = r.program_counter;

    if (entry_pc == 0x0000a5aeU) {
        const auto return_address = pop_return(h, r);
        r.program_counter = return_address;
        return FunctionResult::complete(1U, return_address);
    }

    h.write_memory_word(kPrivateRegion, 0x00000834U, 3U, kWordMask);
    set_logic(r, 3U, 0x8000U);
    // Absolute-short 0x8006 sign-extends to 0xffff8006 and maps through the
    // dual-CPU shared window to captured region 3 offset 0x38006.
    h.write_memory_word(kSharedRegion, 0x00038006U, 1U, kWordMask);
    set_logic(r, 1U, 0x8000U);

    const auto original_820 = h.read_memory_word(kPrivateRegion, 0x00000820U, 0xff00U);
    h.write_memory_word(kPrivateRegion, 0x00000820U,
        static_cast<std::uint16_t>(original_820 | 0x0800U), 0xff00U);
    if ((original_820 & 0x0800U) == 0U)
        r.status = static_cast<std::uint16_t>(r.status | 0x0004U);
    else
        r.status = static_cast<std::uint16_t>(r.status & ~0x0004U);

    h.write_memory_word(kPrivateRegion, 0x00000832U, 0x7f00U, 0xff00U);
    set_logic(r, 0x7fU, 0x80U);
    store_true_byte(h, 0x00000411U);

    r.address[0] = 0x00000c00U;
    r.data[0] = (r.data[0] & 0xffff0000U) | 0x007fU;
    set_logic(r, 0x007fU, 0x8000U);
    for (;;) {
        clear_long(h, r, r.address[0]);
        r.address[0] += 4U;
        const auto counter = static_cast<std::uint16_t>(r.data[0] - 1U);
        r.data[0] = (r.data[0] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
    }
    clear_word(h, r, 0x00000e84U);
    clear_word(h, r, 0x00001084U);
    clear_word(h, r, 0x00001284U);
    store_true_byte(h, 0x00000c18U);
    r.address[1] = 0x00024836U;

    push_return(h, r, 0x0000a5aeU);
    r.program_counter = 0x00008872U;
    const auto child = h.call_function(126U, 1U, 0x72U, 2U,
        0x0000a5a8U, 0x00008872U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;
    const auto return_address = pop_return(h, r);
    r.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated

#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto word = host.read_memory_word(
        kRegion, address & ~1U, odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    host.write_memory_word(kRegion, address & ~1U,
        static_cast<std::uint16_t>(odd ? value : value << 8U),
        odd ? 0x00ffU : 0xff00U);
}

void set_logic_flags(CpuRegisters &r, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &r, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (static_cast<std::uint32_t>(left) + right > 0xffffU) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_sub_word_flags(CpuRegisters &r, std::uint16_t left,
    std::uint16_t right, std::uint16_t result, bool update_extend)
{
    std::uint16_t flags = update_extend ? 0U : (r.status & 0x0010U);
    if (left < right) flags |= update_extend ? 0x0011U : 0x0001U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto high = host.read_memory_word(kRegion, r.address[7], kWordMask);
    const auto low = host.read_memory_word(kRegion, r.address[7] + 2U, kWordMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_record_block_pointer_setup_variant(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];
    r.address[4] = 0x00003409U;

    r.data[0] = 0U;
    set_logic_flags(r, 0U, 0x80000000U, 0xffffffffU);
    auto selector = read_byte(host, base + 0x61U);
    r.data[0] = selector;
    set_logic_flags(r, selector, 0x80U, 0xffU);

    if (selector != 0U) {
        r.data[1] = 2U;
        set_logic_flags(r, 2U, 0x80000000U, 0xffffffffU);
        for (;;) {
            const auto occupied = read_byte(host,
                r.address[4] + static_cast<std::int16_t>(
                    static_cast<std::uint16_t>(r.data[0])));
            set_logic_flags(r, occupied, 0x80U, 0xffU);
            if (occupied != 0U)
                break;

            auto old = static_cast<std::uint16_t>(r.data[0]);
            selector = static_cast<std::uint16_t>(old + 1U);
            r.data[0] = (r.data[0] & 0xffff0000U) | selector;
            set_add_word_flags(r, old, 1U, selector);

            const auto compared = static_cast<std::uint16_t>(selector - 4U);
            set_sub_word_flags(r, selector, 4U, compared, false);
            const bool less_than = ((r.status & 0x0008U) != 0U)
                != ((r.status & 0x0002U) != 0U);
            if (!less_than) {
                old = selector;
                selector = static_cast<std::uint16_t>(old + 1U);
                r.data[0] = (r.data[0] & 0xffff0000U) | selector;
                set_add_word_flags(r, old, 1U, selector);
            }

            selector = static_cast<std::uint16_t>(selector & 3U);
            r.data[0] = (r.data[0] & 0xffff0000U) | selector;
            set_logic_flags(r, selector, 0x8000U, 0xffffU);

            auto counter = static_cast<std::uint16_t>(r.data[1]);
            counter = static_cast<std::uint16_t>(counter - 1U);
            r.data[1] = (r.data[1] & 0xffff0000U) | counter;
            if (counter == 0xffffU) {
                r.data[0] = 0U;
                set_logic_flags(r, 0U, 0x80000000U, 0xffffffffU);
                selector = 0U;
                break;
            }
        }
    }

    write_byte(host, base + 0x60U, static_cast<std::uint8_t>(selector));
    set_logic_flags(r, selector, 0x80U, 0xffU);
    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated

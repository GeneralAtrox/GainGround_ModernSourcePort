#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kTileRegion = 5U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kFlagC = 0x0001U;
constexpr std::uint16_t kFlagV = 0x0002U;
constexpr std::uint16_t kFlagZ = 0x0004U;
constexpr std::uint16_t kFlagN = 0x0008U;
constexpr std::uint16_t kFlagX = 0x0010U;

void set_logic_word(CpuRegisters &r, std::uint16_t value)
{
    std::uint16_t flags = r.status & kFlagX;
    if ((value & 0x8000U) != 0U) flags |= kFlagN;
    if (value == 0U) flags |= kFlagZ;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_logic_long(CpuRegisters &r, std::uint32_t value)
{
    std::uint16_t flags = r.status & kFlagX;
    if ((value & 0x80000000U) != 0U) flags |= kFlagN;
    if (value == 0U) flags |= kFlagZ;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_compare_word(CpuRegisters &r, std::uint16_t left,
                      std::uint16_t right)
{
    const auto result = static_cast<std::uint16_t>(left - right);
    std::uint16_t flags = r.status & kFlagX;
    if (left < right) flags |= kFlagC;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U)
        flags |= kFlagV;
    if ((result & 0x8000U) != 0U) flags |= kFlagN;
    if (result == 0U) flags |= kFlagZ;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_subtract_word(CpuRegisters &r, std::uint16_t left,
                       std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (left < right) flags |= kFlagC | kFlagX;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U)
        flags |= kFlagV;
    if ((result & 0x8000U) != 0U) flags |= kFlagN;
    if (result == 0U) flags |= kFlagZ;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

std::uint16_t arithmetic_shift_left_word_2(
    CpuRegisters &r, std::uint16_t value)
{
    const bool sign0 = (value & 0x8000U) != 0U;
    const auto once = static_cast<std::uint16_t>(value << 1U);
    const bool sign1 = (once & 0x8000U) != 0U;
    const bool carry = sign1;
    const auto result = static_cast<std::uint16_t>(once << 1U);
    const bool sign2 = (result & 0x8000U) != 0U;

    std::uint16_t flags{};
    if (carry) flags |= kFlagC | kFlagX;
    if (sign0 != sign1 || sign1 != sign2) flags |= kFlagV;
    if (sign2) flags |= kFlagN;
    if (result == 0U) flags |= kFlagZ;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
    return result;
}

[[nodiscard]] std::uint32_t read_long(
    ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(
        kPrivateRegion, address, kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

[[nodiscard]] FunctionResult finish_return(
    ExecutionHost &host, CpuRegisters &r)
{
    const auto target = read_long(host, r.address[7]);
    r.address[7] += 4U;
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0000b77a(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    if (r.program_counter != 0x0000b77aU)
        return {TranslationStatus::contract_violation, 0U,
            r.program_counter};

    const auto selector = host.read_memory_word(
        kPrivateRegion, r.address[5] + 0x0cU, kWordMask);
    r.data[0] = (r.data[0] & 0xffff0000U) | selector;
    set_logic_word(r, selector);
    set_compare_word(r, selector, 1U);

    if (selector == 1U) {
        r.address[0] = 0x0020460cU;
        r.address[2] = 0x0000c5ccU;
        r.data[2] = 0x00000017U;
        set_logic_long(r, r.data[2]);

        for (;;) {
            r.address[1] = r.address[0];
            r.data[1] = 0x00000011U;
            set_logic_long(r, r.data[1]);
            for (;;) {
                const auto value = host.read_memory_word(
                    kPrivateRegion, r.address[2], kWordMask);
                r.address[2] += 2U;
                host.write_memory_word(kTileRegion,
                    r.address[1] - 0x00200000U, value, kWordMask);
                r.address[1] += 2U;
                set_logic_word(r, value);

                const auto count = static_cast<std::uint16_t>(r.data[1] - 1U);
                r.data[1] = (r.data[1] & 0xffff0000U) | count;
                if (count == 0xffffU) break;
            }

            r.address[0] += 0x80U;
            const auto count = static_cast<std::uint16_t>(r.data[2] - 1U);
            r.data[2] = (r.data[2] & 0xffff0000U) | count;
            if (count == 0xffffU) break;
        }
        return finish_return(host, r);
    }

    const auto before_subtract = static_cast<std::uint16_t>(r.data[0]);
    const auto after_subtract =
        static_cast<std::uint16_t>(before_subtract - 2U);
    r.data[0] = (r.data[0] & 0xffff0000U) | after_subtract;
    set_subtract_word(r, before_subtract, 2U, after_subtract);

    const auto table_offset = arithmetic_shift_left_word_2(r, after_subtract);
    r.data[0] = (r.data[0] & 0xffff0000U) | table_offset;
    r.address[0] = 0x0000c92cU;
    const auto signed_offset = static_cast<std::int16_t>(table_offset);
    r.address[2] = read_long(host,
        r.address[0] + static_cast<std::uint32_t>(
            static_cast<std::int32_t>(signed_offset)));

    const auto first_word = host.read_memory_word(
        kPrivateRegion, r.address[2], kWordMask);
    r.data[0] = (r.data[0] & 0xffff0000U) | first_word;
    set_logic_word(r, first_word);
    if ((first_word & 0x8000U) != 0U)
        return finish_return(host, r);

    r.address[0] = 0x00204250U;
    r.data[2] = 0x0000000fU;
    set_logic_long(r, r.data[2]);
    for (;;) {
        r.address[1] = r.address[0];
        r.data[1] = 0x0000000fU;
        set_logic_long(r, r.data[1]);
        for (;;) {
            set_logic_word(r, static_cast<std::uint16_t>(r.data[0]));
            if ((r.status & kFlagZ) != 0U) {
                host.write_memory_word(kTileRegion,
                    r.address[1] - 0x00200000U,
                    static_cast<std::uint16_t>(r.data[0]), kWordMask);
                r.address[1] += 2U;
                set_logic_word(r, static_cast<std::uint16_t>(r.data[0]));
            } else {
                const auto value = host.read_memory_word(
                    kPrivateRegion, r.address[2], kWordMask);
                r.address[2] += 2U;
                host.write_memory_word(kTileRegion,
                    r.address[1] - 0x00200000U, value, kWordMask);
                r.address[1] += 2U;
                set_logic_word(r, value);
            }

            const auto count = static_cast<std::uint16_t>(r.data[1] - 1U);
            r.data[1] = (r.data[1] & 0xffff0000U) | count;
            if (count == 0xffffU) break;
        }

        r.address[0] += 0x80U;
        const auto count = static_cast<std::uint16_t>(r.data[2] - 1U);
        r.data[2] = (r.data[2] & 0xffff0000U) | count;
        if (count == 0xffffU) break;
    }
    return finish_return(host, r);
}

} // namespace gain_ground::translated

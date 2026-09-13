#include "gain_ground/contract_types.h"

#include <cstddef>
#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address, kWordMask);
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto word = host.read_memory_word(kRegion, address & ~1U,
        odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

void set_data_word(CpuRegisters &r, std::size_t index, std::uint16_t value)
{
    r.data[index] = (r.data[index] & 0xffff0000U) | value;
}

void set_logic_flags(CpuRegisters &r, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_sub_word_flags(CpuRegisters &r, std::uint16_t left,
    std::uint16_t right, std::uint16_t result, bool preserve_x)
{
    const auto old_x = static_cast<std::uint16_t>(r.status & 0x0010U);
    std::uint16_t flags{};
    if (left < right) flags |= 0x0001U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (!preserve_x && left < right) flags |= 0x0010U;
    if (preserve_x) flags |= old_x;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_sub_byte_flags(CpuRegisters &r, std::uint8_t left,
    std::uint8_t right, std::uint8_t result)
{
    const auto old_x = static_cast<std::uint16_t>(r.status & 0x0010U);
    std::uint16_t flags{};
    if (left < right) flags |= 0x0001U;
    if (((left ^ right) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    r.status = static_cast<std::uint16_t>(
        (r.status & ~0x001fU) | flags | old_x);
}

void set_add_word_flags(CpuRegisters &r, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (static_cast<std::uint32_t>(left) + right > 0xffffU) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

[[nodiscard]] bool signed_greater(const CpuRegisters &r)
{
    const bool negative = (r.status & 0x0008U) != 0U;
    const bool overflow = (r.status & 0x0002U) != 0U;
    return (r.status & 0x0004U) == 0U && negative == overflow;
}

[[nodiscard]] bool signed_less_equal(const CpuRegisters &r)
{
    const bool negative = (r.status & 0x0008U) != 0U;
    const bool overflow = (r.status & 0x0002U) != 0U;
    return (r.status & 0x0004U) != 0U || negative != overflow;
}

[[nodiscard]] FunctionResult return_from_function(
    ExecutionHost &host, CpuRegisters &r)
{
    const auto high = read_word(host, r.address[7]);
    const auto low = read_word(host, r.address[7] + 2U);
    r.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_find_record_containing_bounds(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];

    r.address[3] = 0x00006c00U;
    auto probe = read_word(host, r.address[3]);
    r.address[3] += 2U;
    set_logic_flags(r, probe, 0x8000U, 0xffffU);
    if ((r.status & 0x0008U) == 0U)
        r.address[3] = 0x00007002U;

    auto d0 = read_word(host, base + 0x1aU);
    set_data_word(r, 0U, d0);
    set_logic_flags(r, d0, 0x8000U, 0xffffU);
    auto result = static_cast<std::uint16_t>(d0 - 0x0200U);
    set_sub_word_flags(r, d0, 0x0200U, result, true);
    if ((r.status & 0x0001U) == 0U)
        return return_from_function(host, r);

    auto d7 = d0;
    set_data_word(r, 7U, d7);
    set_logic_flags(r, d7, 0x8000U, 0xffffU);
    result = static_cast<std::uint16_t>(d7 + 0x0020U);
    set_add_word_flags(r, d7, 0x0020U, result);
    d7 = result;
    set_data_word(r, 7U, d7);
    result = static_cast<std::uint16_t>(d0 - 0x0020U);
    set_sub_word_flags(r, d0, 0x0020U, result, false);
    d0 = result;
    set_data_word(r, 0U, d0);

    if ((r.status & 0x0008U) == 0U) {
        result = static_cast<std::uint16_t>(d7 - 0x01ffU);
        set_sub_word_flags(r, d7, 0x01ffU, result, true);
        if (!signed_less_equal(r)) {
            d7 = 0x01ffU;
            set_data_word(r, 7U, d7);
            set_logic_flags(r, d7, 0x8000U, 0xffffU);
        }
        result = static_cast<std::uint16_t>(d7 - d0);
        set_sub_word_flags(r, d7, d0, result, false);
        d7 = result;
        set_data_word(r, 7U, d7);
        result = static_cast<std::uint16_t>(d0 + d0);
        set_add_word_flags(r, d0, d0, result);
        d0 = result;
        set_data_word(r, 0U, d0);
        r.address[3] = static_cast<std::uint32_t>(
            r.address[3] + static_cast<std::int16_t>(d0));
    }

    for (;;) {
        d0 = read_word(host, r.address[3]);
        r.address[3] += 2U;
        set_data_word(r, 0U, d0);
        set_logic_flags(r, d0, 0x8000U, 0xffffU);

        bool reject = d0 == 0U;
        if (!reject) {
            r.address[6] = static_cast<std::uint32_t>(
                static_cast<std::int32_t>(static_cast<std::int16_t>(d0)));
            const auto selector = read_byte(host, r.address[6] + 0x0bU);
            const auto byte_result = static_cast<std::uint8_t>(selector - 4U);
            set_sub_byte_flags(r, selector, 4U, byte_result);
            reject = byte_result != 0U;

            if (!reject) {
                const auto compare = [&](std::uint32_t left_address,
                                         std::uint32_t right_address,
                                         bool branch_on_greater) {
                    d0 = read_word(host, left_address);
                    set_data_word(r, 0U, d0);
                    set_logic_flags(r, d0, 0x8000U, 0xffffU);
                    const auto right = read_word(host, right_address);
                    const auto difference = static_cast<std::uint16_t>(d0 - right);
                    set_sub_word_flags(r, d0, right, difference, true);
                    return branch_on_greater
                        ? signed_greater(r)
                        : (r.status & 0x0008U) != 0U;
                };

                reject = compare(base + 0x2cU, r.address[6] + 0x2aU, false)
                    || compare(base + 0x2aU, r.address[6] + 0x2cU, true)
                    || compare(base + 0x30U, r.address[6] + 0x2eU, false)
                    || compare(base + 0x2eU, r.address[6] + 0x30U, true)
                    || compare(base + 0x34U, r.address[6] + 0x32U, false)
                    || compare(base + 0x32U, r.address[6] + 0x34U, true);

                if (!reject) {
                    r.status = static_cast<std::uint16_t>(
                        (r.status & ~0x001fU) | 0x0001U);
                    return return_from_function(host, r);
                }
            }
        }

        r.status = static_cast<std::uint16_t>(r.status & ~0x001fU);
        d7 = static_cast<std::uint16_t>(d7 - 1U);
        set_data_word(r, 7U, d7);
        if (d7 == 0xffffU) {
            r.status = static_cast<std::uint16_t>(r.status & ~0x001fU);
            return return_from_function(host, r);
        }
    }
}

} // namespace gain_ground::translated

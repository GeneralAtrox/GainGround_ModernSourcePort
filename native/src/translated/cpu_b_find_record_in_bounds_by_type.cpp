#include "gain_ground/contract_types.h"

#include <cstddef>
#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address, kMask);
}

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto word = host.read_memory_word(
        kRegion, address & ~1U, odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

void put_word(CpuRegisters &r, std::size_t index, std::uint16_t value)
{
    r.data[index] = (r.data[index] & 0xffff0000U) | value;
}

void logic_flags(CpuRegisters &r, std::uint32_t value,
                 std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void add_flags(CpuRegisters &r, std::uint16_t left,
               std::uint16_t right, std::uint16_t result)
{
    const bool carry = static_cast<std::uint32_t>(left) + right > 0xffffU;
    const bool overflow =
        ((~(left ^ right) & (left ^ result)) & 0x8000U) != 0U;
    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void sub_flags(CpuRegisters &r, std::uint16_t left,
               std::uint16_t right, std::uint16_t result, bool compare)
{
    const auto old_x = static_cast<std::uint16_t>(r.status & 0x0010U);
    const bool borrow = left < right;
    const bool overflow = ((left ^ right) & (left ^ result) & 0x8000U) != 0U;
    std::uint16_t flags = compare ? old_x : (borrow ? 0x0010U : 0U);
    if (borrow) flags |= 0x0001U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void asl_two_flags(CpuRegisters &r, std::uint16_t original,
                   std::uint16_t result)
{
    const bool carry = (original & 0x4000U) != 0U;
    const bool overflow = ((original ^ (original << 1U)) & 0x8000U) != 0U
        || (((original << 1U) ^ (original << 2U)) & 0x8000U) != 0U;
    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

bool signed_greater(const CpuRegisters &r)
{
    const bool n = (r.status & 0x0008U) != 0U;
    const bool v = (r.status & 0x0002U) != 0U;
    return (r.status & 0x0004U) == 0U && n == v;
}

bool signed_less_equal(const CpuRegisters &r)
{
    const bool n = (r.status & 0x0008U) != 0U;
    const bool v = (r.status & 0x0002U) != 0U;
    return (r.status & 0x0004U) != 0U || n != v;
}

FunctionResult return_from_function(ExecutionHost &host, CpuRegisters &r)
{
    const auto high = read_word(host, r.address[7]);
    const auto low = read_word(host, r.address[7] + 2U);
    r.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_find_record_in_bounds_by_type(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto bounds = r.address[2];

    r.address[3] = 0x00006c00U;
    const auto initial = read_word(host, r.address[3]);
    r.address[3] += 2U;
    logic_flags(r, initial, 0x8000U, 0xffffU);
    if ((r.status & 0x0008U) == 0U) r.address[3] = 0x00007002U;

    auto d0 = read_word(host, bounds + 2U);
    put_word(r, 0U, d0);
    logic_flags(r, d0, 0x8000U, 0xffffU);
    auto d7 = d0;
    put_word(r, 7U, d7);
    logic_flags(r, d7, 0x8000U, 0xffffU);

    auto result = static_cast<std::uint16_t>(d7 + 0x20U);
    add_flags(r, d7, 0x20U, result);
    d7 = result;
    put_word(r, 7U, d7);
    result = static_cast<std::uint16_t>(d0 - 0x20U);
    sub_flags(r, d0, 0x20U, result, false);
    d0 = result;
    put_word(r, 0U, d0);

    if ((r.status & 0x0008U) == 0U) {
        result = static_cast<std::uint16_t>(d7 - 0x01ffU);
        sub_flags(r, d7, 0x01ffU, result, true);
        if (!signed_less_equal(r)) {
            d7 = 0x01ffU;
            put_word(r, 7U, d7);
            logic_flags(r, d7, 0x8000U, 0xffffU);
        }
        result = static_cast<std::uint16_t>(d7 - d0);
        sub_flags(r, d7, d0, result, false);
        d7 = result;
        put_word(r, 7U, d7);
        result = static_cast<std::uint16_t>(d0 + d0);
        add_flags(r, d0, d0, result);
        d0 = result;
        put_word(r, 0U, d0);
        r.address[3] = static_cast<std::uint32_t>(
            r.address[3] + static_cast<std::int16_t>(d0));
    }

    for (;;) {
        d0 = read_word(host, r.address[3]);
        r.address[3] += 2U;
        put_word(r, 0U, d0);
        logic_flags(r, d0, 0x8000U, 0xffffU);
        bool reject = d0 == 0U;

        if (!reject) {
            r.address[6] = static_cast<std::uint32_t>(
                static_cast<std::int32_t>(static_cast<std::int16_t>(d0)));
            r.data[0] = 0U;
            logic_flags(r, 0U, 0x80000000U, 0xffffffffU);
            const auto selector = read_byte(host, r.address[6] + 0x0bU);
            r.data[0] = selector;
            logic_flags(r, selector, 0x80U, 0xffU);
            const auto table_offset = static_cast<std::uint16_t>(selector << 2U);
            put_word(r, 0U, table_offset);
            asl_two_flags(r, selector, table_offset);

            // 239B0 holds twelve BRA.W entries: types 1, 4, 8, 9, 10 and 11
            // take the bounds test at 239E0; the others skip the record.
            switch (table_offset) {
            case 4U:
            case 16U:
            case 32U:
            case 36U:
            case 40U:
            case 44U:
                reject = false;
                break;
            case 0U:
            case 8U:
            case 12U:
            case 20U:
            case 24U:
            case 28U:
                reject = true;
                break;
            default:
                return {TranslationStatus::contract_violation, 0U, 0x000239acU};
            }

            if (!reject) {
                auto d1 = static_cast<std::uint16_t>(0xffd0U);
                auto d2 = static_cast<std::uint16_t>(0x0030U);
                auto d3 = static_cast<std::uint16_t>(0xffd0U);
                auto d4 = static_cast<std::uint16_t>(0x0030U);
                put_word(r, 1U, d1); logic_flags(r, d1, 0x8000U, 0xffffU);
                put_word(r, 2U, d2); logic_flags(r, d2, 0x8000U, 0xffffU);
                put_word(r, 3U, d3); logic_flags(r, d3, 0x8000U, 0xffffU);
                put_word(r, 4U, d4); logic_flags(r, d4, 0x8000U, 0xffffU);

                d0 = read_word(host, bounds);
                put_word(r, 0U, d0); logic_flags(r, d0, 0x8000U, 0xffffU);
                result = static_cast<std::uint16_t>(d1 + d0);
                add_flags(r, d1, d0, result); d1 = result; put_word(r, 1U, d1);
                result = static_cast<std::uint16_t>(d2 + d0);
                add_flags(r, d2, d0, result); d2 = result; put_word(r, 2U, d2);

                d0 = read_word(host, bounds + 2U);
                put_word(r, 0U, d0); logic_flags(r, d0, 0x8000U, 0xffffU);
                result = static_cast<std::uint16_t>(d3 + d0);
                add_flags(r, d3, d0, result); d3 = result; put_word(r, 3U, d3);
                result = static_cast<std::uint16_t>(d4 + d0);
                add_flags(r, d4, d0, result); d4 = result; put_word(r, 4U, d4);

                d0 = read_word(host, r.address[6] + 0x12U);
                put_word(r, 0U, d0); logic_flags(r, d0, 0x8000U, 0xffffU);
                result = static_cast<std::uint16_t>(d0 - d2);
                sub_flags(r, d0, d2, result, true);
                reject = signed_greater(r);
                if (!reject) {
                    result = static_cast<std::uint16_t>(d0 - d1);
                    sub_flags(r, d0, d1, result, true);
                    reject = (r.status & 0x0008U) != 0U;
                }
                if (!reject) {
                    d0 = read_word(host, r.address[6] + 0x1aU);
                    put_word(r, 0U, d0); logic_flags(r, d0, 0x8000U, 0xffffU);
                    result = static_cast<std::uint16_t>(d0 - d4);
                    sub_flags(r, d0, d4, result, true);
                    reject = signed_greater(r);
                    if (!reject) {
                        result = static_cast<std::uint16_t>(d0 - d3);
                        sub_flags(r, d0, d3, result, true);
                        reject = (r.status & 0x0008U) != 0U;
                    }
                }
                if (!reject) {
                    r.status = static_cast<std::uint16_t>(
                        (r.status & ~0x001fU) | 0x0001U);
                    return return_from_function(host, r);
                }
            }
        }

        d7 = static_cast<std::uint16_t>(d7 - 1U);
        put_word(r, 7U, d7);
        if (d7 == 0xffffU) {
            r.status = static_cast<std::uint16_t>(r.status & ~0x001fU);
            return return_from_function(host, r);
        }
    }
}

} // namespace gain_ground::translated

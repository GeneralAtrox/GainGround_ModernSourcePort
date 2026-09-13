#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kPrivateMask = 0x0003ffffU;

struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

[[nodiscard]] constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {(address & kPrivateMask) & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        address <= kPrivateMask ? kPrivateRegion : kSharedRegion,
        location.offset, location.mask) >> location.shift);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kPrivateRegion,
        address & kPrivateMask, kWordMask);
    const auto low = host.read_memory_word(kPrivateRegion,
        (address + 2U) & kPrivateMask, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kPrivateRegion, (address + 2U) & kPrivateMask,
        static_cast<std::uint16_t>(value), kWordMask);
    host.write_memory_word(kPrivateRegion, address & kPrivateMask,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
}

void set_logic_flags(CpuRegisters &r, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_add_flags(CpuRegisters &r, std::uint32_t left,
    std::uint32_t right, std::uint32_t result, std::uint32_t sign,
    std::uint64_t modulus)
{
    const auto mask = static_cast<std::uint32_t>(modulus - 1U);
    std::uint16_t flags{};
    if (static_cast<std::uint64_t>(left) + right >= modulus) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & sign) != 0U) flags |= 0x0002U;
    if ((result & sign) != 0U) flags |= 0x0008U;
    if ((result & mask) == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_sub_word_flags(CpuRegisters &r,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (left < right) flags |= 0x0011U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_compare_word_flags(CpuRegisters &r,
    std::uint16_t destination, std::uint16_t source)
{
    const auto extend = static_cast<std::uint16_t>(r.status & 0x0010U);
    set_sub_word_flags(r, destination, source,
        static_cast<std::uint16_t>(destination - source));
    r.status = static_cast<std::uint16_t>(
        (r.status & ~0x0010U) | extend);
}

void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    host.write_memory_word(kPrivateRegion, r.address[7] & kPrivateMask,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, (r.address[7] + 2U) & kPrivateMask,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] FunctionResult finish(FunctionContext &context)
{
    auto &r = context.registers;
    const auto target = read_long(*context.host, r.address[7]);
    r.address[7] += 4U;
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

void clear_pending(ExecutionHost &host, CpuRegisters &r, std::uint32_t address)
{
    (void)read_long(host, address);
    write_long(host, address, 0U);
    set_logic_flags(r, 0U, 0x80000000U, 0xffffffffU);
}

[[nodiscard]] bool bit_zero(CpuRegisters &r, std::uint8_t value)
{
    const bool set = (value & 1U) != 0U;
    r.status = static_cast<std::uint16_t>(
        (r.status & ~0x0004U) | (set ? 0U : 0x0004U));
    return set;
}
} // namespace

FunctionResult cpu_b_advance_two_pending_offsets_with_bounds(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;
    if (r.program_counter != 0x000102aaU)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    FunctionResult character_result;
    if (host.run_character_collision(context, character_result)) return character_result;
    const auto base = r.address[5];

    auto pending_y = read_long(host, base + 0x26U);
    r.data[1] = pending_y;
    set_logic_flags(r, pending_y, 0x80000000U, 0xffffffffU);
    if (pending_y != 0U) {
        bool reject_y = false;
        auto d1 = pending_y;
        if (static_cast<std::int32_t>(pending_y) >= 0) {
            const auto position_y = read_long(host, base + 0x1aU);
            const auto sum = d1 + position_y;
            set_add_flags(r, d1, position_y, sum,
                0x80000000U, 0x100000000ULL);
            d1 = sum;
            d1 = (d1 << 16U) | (d1 >> 16U);
            set_logic_flags(r, d1, 0x80000000U, 0xffffffffU);
            auto word = static_cast<std::uint16_t>(d1);
            const auto next = static_cast<std::uint16_t>(word + 6U);
            set_add_flags(r, word, 6U, next, 0x8000U, 0x10000U);
            word = next;
            r.data[1] = (d1 & 0xffff0000U) | word;
            set_compare_word_flags(r, word, 0x01bfU);
            reject_y = static_cast<std::int16_t>(word) > 0x01bf;
        } else {
            const auto position_y = read_long(host, base + 0x1aU);
            const auto sum = d1 + position_y;
            set_add_flags(r, d1, position_y, sum,
                0x80000000U, 0x100000000ULL);
            d1 = (sum << 16U) | (sum >> 16U);
            set_logic_flags(r, d1, 0x80000000U, 0xffffffffU);
            auto word = static_cast<std::uint16_t>(d1);
            const auto next = static_cast<std::uint16_t>(word - 5U);
            set_sub_word_flags(r, word, 5U, next);
            word = next;
            r.data[1] = (d1 & 0xffff0000U) | word;
            set_compare_word_flags(r, word, 8U);
            reject_y = static_cast<std::int16_t>(word) < 8;
        }

        if (!reject_y) {
            auto d0 = host.read_memory_word(
                kPrivateRegion, base + 0x12U, kWordMask);
            r.data[0] = (r.data[0] & 0xffff0000U) | d0;
            set_logic_flags(r, d0, 0x8000U, 0xffffU);
            auto next = static_cast<std::uint16_t>(d0 - 7U);
            set_sub_word_flags(r, d0, 7U, next);
            d0 = next;
            r.data[0] = (r.data[0] & 0xffff0000U) | d0;
            set_compare_word_flags(r, d0, 4U);
            reject_y = static_cast<std::int16_t>(d0) < 4;
            if (!reject_y) {
                push_return(host, r, 0x000102e2U);
                r.program_counter = 0x00015edeU;
                auto child = host.call_function(287U, 1U, 0x72U, 2U,
                    0x000102dcU, 0x00015edeU, context);
                if (child.status != TranslationStatus::complete || child.control != 1U)
                    return child;
                reject_y = bit_zero(r, read_byte(host, r.address[0]));
            }
            if (!reject_y) {
                d0 = host.read_memory_word(
                    kPrivateRegion, base + 0x12U, kWordMask);
                r.data[0] = (r.data[0] & 0xffff0000U) | d0;
                set_logic_flags(r, d0, 0x8000U, 0xffffU);
                next = static_cast<std::uint16_t>(d0 + 6U);
                set_add_flags(r, d0, 6U, next, 0x8000U, 0x10000U);
                d0 = next;
                r.data[0] = (r.data[0] & 0xffff0000U) | d0;
                set_compare_word_flags(r, d0, 0x017cU);
                reject_y = static_cast<std::int16_t>(d0) > 0x017c;
            }
            if (!reject_y) {
                push_return(host, r, 0x000102faU);
                r.program_counter = 0x00015edeU;
                const auto child = host.call_function(287U, 1U, 0x72U, 2U,
                    0x000102f4U, 0x00015edeU, context);
                if (child.status != TranslationStatus::complete || child.control != 1U)
                    return child;
                reject_y = bit_zero(r, read_byte(host, r.address[0]));
            }
        }

        if (reject_y) {
            clear_pending(host, r, base + 0x26U);
        } else {
            pending_y = read_long(host, base + 0x26U);
            r.data[0] = pending_y;
            set_logic_flags(r, pending_y, 0x80000000U, 0xffffffffU);
            const auto position = read_long(host, base + 0x1aU);
            const auto result = position + pending_y;
            write_long(host, base + 0x1aU, result);
            set_add_flags(r, position, pending_y, result,
                0x80000000U, 0x100000000ULL);
        }
    }

    auto pending_x = read_long(host, base + 0x1eU);
    r.data[0] = pending_x;
    set_logic_flags(r, pending_x, 0x80000000U, 0xffffffffU);
    if (pending_x == 0U) return finish(context);

    bool reject_x = false;
    auto d0 = pending_x;
    if (static_cast<std::int32_t>(pending_x) >= 0) {
        const auto position_x = read_long(host, base + 0x12U);
        const auto sum = d0 + position_x;
        set_add_flags(r, d0, position_x, sum,
            0x80000000U, 0x100000000ULL);
        d0 = (sum << 16U) | (sum >> 16U);
        set_logic_flags(r, d0, 0x80000000U, 0xffffffffU);
        auto word = static_cast<std::uint16_t>(d0);
        const auto next = static_cast<std::uint16_t>(word + 6U);
        set_add_flags(r, word, 6U, next, 0x8000U, 0x10000U);
        word = next;
        r.data[0] = (d0 & 0xffff0000U) | word;
        set_compare_word_flags(r, word, 0x017cU);
        reject_x = static_cast<std::int16_t>(word) > 0x017c;
    } else {
        const auto position_x = read_long(host, base + 0x12U);
        const auto sum = d0 + position_x;
        set_add_flags(r, d0, position_x, sum,
            0x80000000U, 0x100000000ULL);
        d0 = (sum << 16U) | (sum >> 16U);
        set_logic_flags(r, d0, 0x80000000U, 0xffffffffU);
        auto word = static_cast<std::uint16_t>(d0);
        const auto next = static_cast<std::uint16_t>(word - 7U);
        set_sub_word_flags(r, word, 7U, next);
        word = next;
        r.data[0] = (d0 & 0xffff0000U) | word;
        set_compare_word_flags(r, word, 4U);
        reject_x = static_cast<std::int16_t>(word) < 4;
    }

    if (!reject_x) {
        auto d1 = host.read_memory_word(
            kPrivateRegion, base + 0x1aU, kWordMask);
        r.data[1] = (r.data[1] & 0xffff0000U) | d1;
        set_logic_flags(r, d1, 0x8000U, 0xffffU);
        auto next = static_cast<std::uint16_t>(d1 + 6U);
        set_add_flags(r, d1, 6U, next, 0x8000U, 0x10000U);
        d1 = next;
        r.data[1] = (r.data[1] & 0xffff0000U) | d1;
        set_compare_word_flags(r, d1, 0x01bfU);
        reject_x = static_cast<std::int16_t>(d1) > 0x01bf;
        if (!reject_x) {
            push_return(host, r, 0x00010346U);
            r.program_counter = 0x00015edeU;
            auto child = host.call_function(287U, 1U, 0x72U, 2U,
                0x00010340U, 0x00015edeU, context);
            if (child.status != TranslationStatus::complete || child.control != 1U)
                return child;
            reject_x = bit_zero(r, read_byte(host, r.address[0]));
        }
        if (!reject_x) {
            d1 = host.read_memory_word(
                kPrivateRegion, base + 0x1aU, kWordMask);
            r.data[1] = (r.data[1] & 0xffff0000U) | d1;
            set_logic_flags(r, d1, 0x8000U, 0xffffU);
            next = static_cast<std::uint16_t>(d1 - 5U);
            set_sub_word_flags(r, d1, 5U, next);
            d1 = next;
            r.data[1] = (r.data[1] & 0xffff0000U) | d1;
            set_compare_word_flags(r, d1, 8U);
            reject_x = static_cast<std::int16_t>(d1) < 8;
        }
        if (!reject_x) {
            push_return(host, r, 0x0001035eU);
            r.program_counter = 0x00015edeU;
            const auto child = host.call_function(287U, 1U, 0x72U, 2U,
                0x00010358U, 0x00015edeU, context);
            if (child.status != TranslationStatus::complete || child.control != 1U)
                return child;
            reject_x = bit_zero(r, read_byte(host, r.address[0]));
        }
    }

    if (reject_x) {
        clear_pending(host, r, base + 0x1eU);
    } else {
        pending_x = read_long(host, base + 0x1eU);
        r.data[0] = pending_x;
        set_logic_flags(r, pending_x, 0x80000000U, 0xffffffffU);
        const auto position = read_long(host, base + 0x12U);
        const auto result = position + pending_x;
        write_long(host, base + 0x12U, result);
        set_add_flags(r, position, pending_x, result,
            0x80000000U, 0x100000000ULL);
    }
    return finish(context);
}

} // namespace gain_ground::translated

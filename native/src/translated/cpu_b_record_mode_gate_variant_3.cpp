#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

struct ByteLocation {
    std::uint32_t address;
    std::uint16_t mask;
    unsigned shift;
};

ByteLocation locate_byte(std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto byte = locate_byte(address);
    return static_cast<std::uint8_t>(
        host.read_memory_word(kRegion, byte.address, byte.mask) >> byte.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto byte = locate_byte(address);
    host.write_memory_word(kRegion, byte.address,
        static_cast<std::uint16_t>(value) << byte.shift, byte.mask);
}

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address, value, kWordMask);
}

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
}

void logic(CpuRegisters &r, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x10U;
    if ((value & sign) != 0U) flags |= 8U;
    if ((value & mask) == 0U) flags |= 4U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | flags);
}

void subtract_word(CpuRegisters &r,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 8U;
    if (result == 0U) flags |= 4U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 2U;
    if (left < right) flags |= 0x11U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | flags);
}

void subtract_byte(CpuRegisters &r,
    std::uint8_t left, std::uint8_t right, std::uint8_t result)
{
    std::uint16_t flags{};
    if ((result & 0x80U) != 0U) flags |= 8U;
    if (result == 0U) flags |= 4U;
    if (((left ^ right) & (left ^ result) & 0x80U) != 0U) flags |= 2U;
    if (left < right) flags |= 0x11U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | flags);
}

void compare_word(CpuRegisters &r,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    const auto extend = static_cast<std::uint16_t>(r.status & 0x10U);
    subtract_word(r, left, right, result);
    r.status = static_cast<std::uint16_t>((r.status & ~0x10U) | extend);
}

void add_word(CpuRegisters &r,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 8U;
    if (result == 0U) flags |= 4U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 2U;
    if (static_cast<std::uint32_t>(left) + right > 0xffffU) flags |= 0x11U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | flags);
}

void set_word(CpuRegisters &r, unsigned index, std::uint16_t value)
{
    r.data[index] = (r.data[index] & 0xffff0000U) | value;
}

void bit_zero(CpuRegisters &r, bool zero)
{
    r.status = static_cast<std::uint16_t>((r.status & ~4U) | (zero ? 4U : 0U));
}

void change_bit(ExecutionHost &host, CpuRegisters &r,
    std::uint32_t address, unsigned bit, bool set)
{
    const auto before = read_byte(host, address);
    const auto mask = static_cast<std::uint8_t>(1U << bit);
    write_byte(host, address, static_cast<std::uint8_t>(
        set ? before | mask : before & static_cast<std::uint8_t>(~mask)));
    bit_zero(r, (before & mask) == 0U);
}

FunctionResult call(FunctionContext &context, std::uint32_t id,
    std::uint32_t site, std::uint32_t target, std::uint32_t continuation)
{
    auto &r = context.registers;
    r.address[7] -= 4U;
    write_long(*context.host, r.address[7], continuation);
    r.program_counter = target;
    return context.host->call_function(id, 1U, 0x72U, 2U, site, target, context);
}

bool complete(const FunctionResult &result)
{
    return result.status == TranslationStatus::complete && result.control == 1U;
}
} // namespace

FunctionResult cpu_b_record_mode_gate_variant_3(FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x0001f3a8U)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];
    const auto finish = [&]() {
        const auto target = read_long(host, r.address[7]);
        r.address[7] += 4U;
        r.program_counter = target;
        return FunctionResult::complete(1U, target);
    };

    auto byte = read_byte(host, base + 0x3eU);
    logic(r, byte, 0x80U, 0xffU);
    if (byte != 0U) return finish();

    byte = read_byte(host, base + 0x41U);
    bit_zero(r, (byte & 2U) == 0U);
    if ((byte & 2U) == 0U) {
        const auto timer = read_word(host, base + 0x74U);
        logic(r, timer, 0x8000U, 0xffffU);
        if (timer != 0U) {
            static_cast<void>(read_word(host, base + 0x74U));
            const auto next = static_cast<std::uint16_t>(timer - 1U);
            write_word(host, base + 0x74U, next);
            subtract_word(r, timer, 1U, next);
            return finish();
        }
        auto child = call(context, 374U, 0x0001f3c6U, 0x0001fae6U, 0x0001f3caU);
        if (!complete(child)) return child;
        if ((r.status & 1U) == 0U) return finish();
        r.address[0] = read_long(host, base + 0x66U);
        byte = read_byte(host, r.address[0] + 0x12U);
        write_byte(host, base + 0x72U, byte);
        logic(r, byte, 0x80U, 0xffU);
        change_bit(host, r, base + 0x41U, 1U, true);
        change_bit(host, r, base + 0x41U, 0U, false);
        change_bit(host, r, base + 0x41U, 2U, true);
        write_word(host, base + 0x74U, 6U);
        logic(r, 6U, 0x8000U, 0xffffU);
        child = call(context, 347U, 0x0001f3f0U, 0x0001da58U, 0x0001f3f4U);
        if (!complete(child)) return child;
        return finish();
    }

    byte = read_byte(host, base + 0x41U);
    bit_zero(r, (byte & 4U) == 0U);
    if ((byte & 4U) == 0U) {
        const auto timer = read_word(host, base + 0x74U);
        logic(r, timer, 0x8000U, 0xffffU);
        if (timer != 0U) {
            static_cast<void>(read_word(host, base + 0x74U));
            const auto next = static_cast<std::uint16_t>(timer - 1U);
            write_word(host, base + 0x74U, next);
            subtract_word(r, timer, 1U, next);
            return finish();
        }
        change_bit(host, r, base + 0x41U, 1U, true);
        change_bit(host, r, base + 0x41U, 0U, false);
        change_bit(host, r, base + 0x41U, 2U, true);
        write_word(host, base + 0x74U, 6U);
        logic(r, 6U, 0x8000U, 0xffffU);
        return finish();
    }

    auto timer = read_word(host, base + 0x74U);
    set_word(r, 0U, timer);
    logic(r, timer, 0x8000U, 0xffffU);
    auto next = static_cast<std::uint16_t>(timer - 1U);
    set_word(r, 0U, next);
    subtract_word(r, timer, 1U, next);
    write_word(host, base + 0x74U, next);
    logic(r, next, 0x8000U, 0xffffU);
    const auto comparison = static_cast<std::uint16_t>(next - 2U);
    compare_word(r, next, 2U, comparison);
    if (next != 2U) {
        logic(r, static_cast<std::uint16_t>(r.data[0]), 0x8000U, 0xffffU);
        if (static_cast<std::uint16_t>(r.data[0]) != 0U) return finish();
        byte = read_byte(host, base + 0x72U);
        logic(r, byte, 0x80U, 0xffU);
        if (byte != 0U) {
            static_cast<void>(read_byte(host, base + 0x72U));
            const auto decremented = static_cast<std::uint8_t>(byte - 1U);
            write_byte(host, base + 0x72U, decremented);
            subtract_byte(r, byte, 1U, decremented);
            if (decremented != 0U) {
                r.address[0] = read_long(host, base + 0x66U);
                r.data[0] = 0U;
                logic(r, 0U, 0x80000000U, 0xffffffffU);
                byte = read_byte(host, r.address[0] + 0x13U);
                r.data[0] = byte;
                logic(r, byte, 0x80U, 0xffU);
                write_word(host, base + 0x74U, static_cast<std::uint16_t>(r.data[0]));
                logic(r, static_cast<std::uint16_t>(r.data[0]), 0x8000U, 0xffffU);
                change_bit(host, r, base + 0x41U, 1U, true);
                change_bit(host, r, base + 0x41U, 0U, true);
                change_bit(host, r, base + 0x41U, 2U, false);
                return finish();
            }
        }
        r.address[0] = read_long(host, base + 0x66U);
        auto child = call(context, 384U, 0x0001f44aU, 0x000204c0U, 0x0001f44eU);
        if (!complete(child)) return child;
        change_bit(host, r, base + 0x41U, 1U, false);
        change_bit(host, r, base + 0x41U, 0U, true);
        change_bit(host, r, base + 0x41U, 2U, true);
        return finish();
    }

    auto child = call(context, 347U, 0x0001f484U, 0x0001da58U, 0x0001f488U);
    if (!complete(child)) return child;
    auto word = read_word(host, base + 0x5cU);
    set_word(r, 0U, word);
    logic(r, word, 0x8000U, 0xffffU);
    next = static_cast<std::uint16_t>(word + 0x0200U);
    set_word(r, 0U, next);
    add_word(r, word, 0x0200U, next);
    word = static_cast<std::uint16_t>(next & 0x0400U);
    set_word(r, 0U, word);
    logic(r, word, 0x8000U, 0xffffU);
    write_word(host, base + 0x5cU, word);
    logic(r, word, 0x8000U, 0xffffU);
    word = read_word(host, base + 0x1aU);
    write_word(host, base + 0x6cU, word);
    logic(r, word, 0x8000U, 0xffffU);

    child = call(context, 382U, 0x0001f49eU, 0x000203c2U, 0x0001f4a2U);
    if (!complete(child)) return child;
    logic(r, static_cast<std::uint16_t>(r.data[0]), 0x8000U, 0xffffU);
    if ((r.data[0] & 0x8000U) != 0U) return finish();
    child = call(context, 369U, 0x0001f4a6U, 0x0001f106U, 0x0001f4aaU);
    if (!complete(child)) return child;
    child = call(context, 376U, 0x0001f4aaU, 0x0001fb84U, 0x0001f4aeU);
    if (!complete(child)) return child;
    r.address[3] = r.address[6];
    write_word(host, r.address[6], 0U);
    logic(r, 0U, 0x8000U, 0xffffU);
    r.address[0] = read_long(host, base + 0x66U);
    word = read_word(host, base + 0x5cU);
    set_word(r, 5U, word);
    logic(r, word, 0x8000U, 0xffffU);
    write_word(host, r.address[6] + 0x3aU, word);
    logic(r, word, 0x8000U, 0xffffU);
    r.data[6] = 0U;
    logic(r, 0U, 0x80000000U, 0xffffffffU);
    r.data[7] = 0U;
    logic(r, 0U, 0x80000000U, 0xffffffffU);
    byte = read_byte(host, r.address[0] + 0x14U);
    r.data[6] = byte;
    logic(r, byte, 0x80U, 0xffU);
    byte = read_byte(host, r.address[0] + 0x15U);
    r.data[7] = byte;
    logic(r, byte, 0x80U, 0xffU);
    const auto shifted = static_cast<std::uint16_t>(r.data[7] << 4U);
    const bool carry = (r.data[7] & 0x1000U) != 0U;
    set_word(r, 7U, shifted);
    logic(r, shifted, 0x8000U, 0xffffU);
    r.status = static_cast<std::uint16_t>(
        (r.status & ~0x11U) | (carry ? 0x11U : 0U));
    word = static_cast<std::uint16_t>(r.data[6]);
    next = static_cast<std::uint16_t>(word - 1U);
    set_word(r, 6U, next);
    subtract_word(r, word, 1U, next);
    if (next != 0U) {
        byte = static_cast<std::uint8_t>(r.data[6]);
        r.data[4] = (r.data[4] & 0xffffff00U) | byte;
        logic(r, byte, 0x80U, 0xffU);
        const bool last_shifted = (byte & 2U) != 0U;
        byte = static_cast<std::uint8_t>(byte >> 2U);
        r.data[4] = (r.data[4] & 0xffffff00U) | byte;
        logic(r, byte, 0x80U, 0xffU);
        r.status = static_cast<std::uint16_t>(
            (r.status & ~0x11U) | (last_shifted ? 0x11U : 0U));
        do {
            const auto before = static_cast<std::uint16_t>(r.data[5]);
            const auto amount = static_cast<std::uint16_t>(r.data[7]);
            const auto result = static_cast<std::uint16_t>(before - amount);
            set_word(r, 5U, result);
            subtract_word(r, before, amount, result);
            const auto counter = static_cast<std::uint16_t>(r.data[4] - 1U);
            set_word(r, 4U, counter);
            if (counter == 0xffffU) break;
        } while (true);
        r.data[6] = (r.data[6] << 16U) | (r.data[6] >> 16U);
        logic(r, r.data[6], 0x80000000U, 0xffffffffU);
        word = static_cast<std::uint16_t>(r.data[7]);
        set_word(r, 6U, word);
        logic(r, word, 0x8000U, 0xffffU);
        r.data[6] = (r.data[6] << 16U) | (r.data[6] >> 16U);
        logic(r, r.data[6], 0x80000000U, 0xffffffffU);
    }

    for (;;) {
        word = static_cast<std::uint16_t>(r.data[5] & 0x07ffU);
        set_word(r, 5U, word);
        logic(r, word, 0x8000U, 0xffffU);
        child = call(context, 382U, 0x0001f4e6U, 0x000203c2U, 0x0001f4eaU);
        if (!complete(child)) return child;
        logic(r, static_cast<std::uint16_t>(r.data[0]), 0x8000U, 0xffffU);
        if ((r.data[0] & 0x8000U) != 0U) return finish();
        child = call(context, 369U, 0x0001f4eeU, 0x0001f106U, 0x0001f4f2U);
        if (!complete(child)) return child;
        child = call(context, 370U, 0x0001f4f2U, 0x0001f146U, 0x0001f4f6U);
        if (!complete(child)) return child;
        const auto pointer = read_long(host, r.address[3] + 0x22U);
        write_long(host, r.address[6] + 0x22U, pointer);
        logic(r, pointer, 0x80000000U, 0xffffffffU);
        write_long(host, r.address[6] + 0x50U, 0x00002000U);
        logic(r, 0x00002000U, 0x80000000U, 0xffffffffU);
        r.address[4] = r.address[5];
        r.address[5] = r.address[6];
        child = call(context, 388U, 0x0001f508U, 0x000207c2U, 0x0001f50eU);
        if (!complete(child)) return child;
        r.address[5] = r.address[4];
        r.data[6] = (r.data[6] << 16U) | (r.data[6] >> 16U);
        logic(r, r.data[6], 0x80000000U, 0xffffffffU);
        word = static_cast<std::uint16_t>(r.data[5]);
        const auto increment = static_cast<std::uint16_t>(r.data[6]);
        next = static_cast<std::uint16_t>(word + increment);
        set_word(r, 5U, next);
        add_word(r, word, increment, next);
        r.data[6] = (r.data[6] << 16U) | (r.data[6] >> 16U);
        logic(r, r.data[6], 0x80000000U, 0xffffffffU);
        const auto counter = static_cast<std::uint16_t>(r.data[6] - 1U);
        set_word(r, 6U, counter);
        if (counter == 0xffffU) return finish();
    }
}

} // namespace gain_ground::translated

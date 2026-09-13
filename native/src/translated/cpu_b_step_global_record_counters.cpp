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

[[nodiscard]] constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    return {address & ~1U,
        static_cast<std::uint16_t>((address & 1U) != 0U ? 0x00ffU : 0xff00U),
        (address & 1U) != 0U ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, location.address, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kRegion, location.address,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kRegion, address,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, address + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

void set_logic_flags(CpuRegisters &r, std::uint32_t value, std::uint32_t sign_bit)
{
    std::uint16_t flags = static_cast<std::uint16_t>(r.status & 0x0010U);
    if ((value & sign_bit) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_sub_word_flags(CpuRegisters &r, std::uint16_t destination,
    std::uint16_t source, std::uint16_t result)
{
    const bool destination_negative = (destination & 0x8000U) != 0U;
    const bool source_negative = (source & 0x8000U) != 0U;
    const bool result_negative = (result & 0x8000U) != 0U;
    std::uint16_t flags{};
    if (result_negative) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (destination_negative != source_negative && result_negative != destination_negative)
        flags |= 0x0002U;
    if (destination < source) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint16_t subtract_word(
    CpuRegisters &r, std::uint16_t destination, std::uint16_t source)
{
    const auto result = static_cast<std::uint16_t>(destination - source);
    set_sub_word_flags(r, destination, source, result);
    return result;
}

[[nodiscard]] std::uint8_t sbcd(
    CpuRegisters &r, std::uint8_t destination, std::uint8_t source)
{
    const int extend = (r.status & 0x0010U) != 0U ? 1 : 0;
    int result = static_cast<int>(destination) - static_cast<int>(source) - extend;
    if (static_cast<int>(destination & 0x0fU)
        - static_cast<int>(source & 0x0fU) - extend < 0)
        result -= 6;
    const bool borrow = result < 0;
    if (borrow) result -= 0x60;
    const auto value = static_cast<std::uint8_t>(result);

    std::uint16_t flags = static_cast<std::uint16_t>(r.status & 0x0004U);
    if (borrow) flags |= 0x0011U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value != 0U) flags &= static_cast<std::uint16_t>(~0x0004U);
    const auto binary = static_cast<std::uint8_t>(destination - source - extend);
    if (((destination ^ source) & (destination ^ binary) & 0x80U) != 0U)
        flags |= 0x0002U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
    return value;
}

void push_long(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    write_long(host, r.address[7], value);
}

void call(ExecutionHost &host, FunctionContext &context, std::uint32_t id,
    std::uint32_t callsite, std::uint32_t target, std::uint32_t return_pc)
{
    push_long(host, context.registers, return_pc);
    context.registers.program_counter = target;
    (void)host.call_function(id, 1U, 0x72U, 2U, callsite, target, context);
}
} // namespace

FunctionResult cpu_b_step_global_record_counters(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;

    const auto skip_counters = read_byte(host, 0x00000d1bU);
    set_logic_flags(r, skip_counters, 0x80U);
    if (skip_counters == 0U) {
        r.address[0] = 0x00000c08U;
        auto value = host.read_memory_word(kRegion, r.address[0], kWordMask);
        value = subtract_word(r, value, 1U);
        host.write_memory_word(kRegion, r.address[0], value, kWordMask);

        if (static_cast<std::int16_t>(value) <= 0) {
            const auto reload = host.read_memory_word(kRegion, r.address[0] + 6U, kWordMask);
            host.write_memory_word(kRegion, r.address[0], reload, kWordMask);
            r.address[0] += 2U;
            set_logic_flags(r, reload, 0x8000U);

            value = host.read_memory_word(kRegion, r.address[0], kWordMask);
            value = subtract_word(r, value, 1U);
            host.write_memory_word(kRegion, r.address[0], value, kWordMask);
            if (static_cast<std::int16_t>(value) <= 0) {
                (void)read_long(host, r.address[0]);
                host.write_memory_word(kRegion, r.address[0] + 2U, 0U, kWordMask);
                host.write_memory_word(kRegion, r.address[0], 0U, kWordMask);
                set_logic_flags(r, 0U, 0x80000000U);
            } else {
                auto test = host.read_memory_word(kRegion, r.address[0], kWordMask);
                r.data[0] = (r.data[0] & 0xffff0000U) | test;
                set_logic_flags(r, test, 0x8000U);
                test = subtract_word(r, test, 10U);
                r.data[0] = (r.data[0] & 0xffff0000U) | test;

                bool update_status = static_cast<std::int16_t>(test) <= 0;
                if (!update_status) {
                    r.data[1] = 3U;
                    set_logic_flags(r, 3U, 0x80000000U);
                    do {
                        test = subtract_word(r, test, 5U);
                        r.data[0] = (r.data[0] & 0xffff0000U) | test;
                        if (test == 0U) {
                            update_status = true;
                            break;
                        }
                        auto counter = static_cast<std::uint16_t>(r.data[1]);
                        counter = static_cast<std::uint16_t>(counter - 1U);
                        r.data[1] = (r.data[1] & 0xffff0000U) | counter;
                        if (counter == 0xffffU)
                            break;
                    } while (true);
                }

                if (update_status) {
                    const auto player_count = read_byte(host, 0x00000821U);
                    set_logic_flags(r, player_count, 0x80U);
                    if (player_count != 0U) {
                        host.write_memory_word(kRegion, r.address[5] + 0x24U, 10U, kWordMask);
                        set_logic_flags(r, 10U, 0x8000U);
                        r.address[1] = 0x0000745cU;
                        for (const std::uint16_t word : {0x0080U, 0x0048U, 0x0803U}) {
                            host.write_memory_word(kRegion, r.address[1], word, kWordMask);
                            r.address[1] += 2U;
                            set_logic_flags(r, word, 0x8000U);
                        }
                        write_long(host, r.address[1], 0x0000e06eU);
                        r.address[1] += 4U;
                        set_logic_flags(r, 0x0000e06eU, 0x80000000U);
                    }
                }

                r.address[0] += 4U;
                r.address[1] = 0x00000826U;
                host.write_memory_word(kRegion, r.address[1], 1U, kWordMask);
                r.address[1] += 2U;
                set_logic_flags(r, 1U, 0x8000U);
                r.status = static_cast<std::uint16_t>(r.status & ~0x001fU);
                for (unsigned count = 0; count != 2U; ++count) {
                    --r.address[1];
                    --r.address[0];
                    const auto source = read_byte(host, r.address[1]);
                    const auto destination = read_byte(host, r.address[0]);
                    write_byte(host, r.address[0], sbcd(r, destination, source));
                }
            }
            call(host, context, 145U, 0x0000ddecU, 0x0000a85cU, 0x0000ddf2U);
        }
    } else {
        call(host, context, 145U, 0x0000ddecU, 0x0000a85cU, 0x0000ddf2U);
    }

    call(host, context, 146U, 0x0000ddf2U, 0x0000a86cU, 0x0000ddf8U);
    const auto return_address = read_long(host, r.address[7]);
    r.address[7] += 4U;
    r.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated

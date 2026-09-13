#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address, kMask);
}

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto value = host.read_memory_word(kRegion, address & ~1U,
        odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? value : value >> 8U);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address, value, kMask);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    host.write_memory_word(kRegion, address & ~1U,
        static_cast<std::uint16_t>(value) << (odd ? 0U : 8U),
        odd ? 0x00ffU : 0xff00U);
}

void set_logic_word(CpuRegisters &r, std::uint16_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_logic_byte(CpuRegisters &r, std::uint8_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void clear_word(ExecutionHost &host, CpuRegisters &r, std::uint32_t address)
{
    (void)read_word(host, address);
    write_word(host, address, 0U);
    set_logic_word(r, 0U);
}

void move_parent_word(ExecutionHost &host, CpuRegisters &r,
    std::uint32_t source, std::uint32_t destination)
{
    const auto value = read_word(host, source);
    r.data[0] = (r.data[0] & 0xffff0000U) | value;
    set_logic_word(r, value);
    write_word(host, destination, value);
    set_logic_word(r, value);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto target = read_long(host, r.address[7]);
    r.address[7] += 4U;
    return target;
}
} // namespace

FunctionResult cpu_b_initialize_record_positions_from_parent(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;

    r.address[7] -= 2U;
    write_word(host, r.address[7], static_cast<std::uint16_t>(r.address[6]));
    set_logic_word(r, static_cast<std::uint16_t>(r.address[6]));
    const auto initial_count = read_word(host, r.address[3] + 6U);
    r.data[1] = (r.data[1] & 0xffff0000U) | initial_count;
    set_logic_word(r, initial_count);

    for (;;) {
        const auto flags = read_byte(host, r.address[6]);
        write_byte(host, r.address[6], static_cast<std::uint8_t>(flags | 0x80U));
        if ((flags & 0x80U) == 0U)
            r.status = static_cast<std::uint16_t>(r.status | 0x0004U);
        else
            r.status = static_cast<std::uint16_t>(r.status & ~0x0004U);

        move_parent_word(host, r, r.address[5] + 0x12U, r.address[6] + 0x2aU);
        write_word(host, r.address[6] + 0x2cU,
            static_cast<std::uint16_t>(r.data[0]));
        set_logic_word(r, static_cast<std::uint16_t>(r.data[0]));
        write_word(host, r.address[6] + 0x12U,
            static_cast<std::uint16_t>(r.data[0]));
        set_logic_word(r, static_cast<std::uint16_t>(r.data[0]));
        clear_word(host, r, r.address[6] + 0x14U);

        move_parent_word(host, r, r.address[5] + 0x16U, r.address[6] + 0x2eU);
        write_word(host, r.address[6] + 0x30U,
            static_cast<std::uint16_t>(r.data[0]));
        set_logic_word(r, static_cast<std::uint16_t>(r.data[0]));
        write_word(host, r.address[6] + 0x16U,
            static_cast<std::uint16_t>(r.data[0]));
        set_logic_word(r, static_cast<std::uint16_t>(r.data[0]));
        clear_word(host, r, r.address[6] + 0x18U);

        move_parent_word(host, r, r.address[5] + 0x1aU, r.address[6] + 0x32U);
        write_word(host, r.address[6] + 0x34U,
            static_cast<std::uint16_t>(r.data[0]));
        set_logic_word(r, static_cast<std::uint16_t>(r.data[0]));
        write_word(host, r.address[6] + 0x1aU,
            static_cast<std::uint16_t>(r.data[0]));
        set_logic_word(r, static_cast<std::uint16_t>(r.data[0]));
        clear_word(host, r, r.address[6] + 0x1cU);
        clear_word(host, r, r.address[6] + 0x46U);

        do {
            r.address[6] += 0x80U;
            const auto next_flags = read_byte(host, r.address[6]);
            set_logic_byte(r, next_flags);
            if ((next_flags & 0x80U) == 0U) break;
        } while (true);

        const auto count = static_cast<std::uint16_t>(r.data[1] - 1U);
        r.data[1] = (r.data[1] & 0xffff0000U) | count;
        if (count == 0xffffU) break;
    }

    const auto saved_a6 = read_word(host, r.address[7]);
    r.address[7] += 2U;
    r.address[6] = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(static_cast<std::int16_t>(saved_a6)));
    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated

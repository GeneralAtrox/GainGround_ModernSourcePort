#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

[[nodiscard]] constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

void set_logic_byte_flags(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_add_byte_flags(CpuRegisters &registers,
    std::uint8_t left, std::uint8_t right, std::uint8_t result)
{
    std::uint16_t flags{};
    if (static_cast<unsigned>(left) + right > 0xffU) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_compare_byte_flags(CpuRegisters &registers,
    std::uint8_t left, std::uint8_t right, std::uint8_t result)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if (left < right) flags |= 0x0001U;
    if (((left ^ right) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_bit_test_zero(CpuRegisters &registers, bool bit_was_set)
{
    if (bit_was_set)
        registers.status = static_cast<std::uint16_t>(registers.status & ~0x0004U);
    else
        registers.status = static_cast<std::uint16_t>(registers.status | 0x0004U);
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kRegion, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(kRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(kRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_toggle_record_phase_and_load_step(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    const auto tas_address = registers.address[5] + 0x3fU;
    const auto tas_value = read_byte(host, tas_address);
    write_byte(host, tas_address, static_cast<std::uint8_t>(tas_value | 0x80U));
    set_logic_byte_flags(registers, tas_value);

    const auto toggle_address = registers.address[5] + 0x3cU;
    const auto toggle_value = read_byte(host, toggle_address);
    write_byte(host, toggle_address, static_cast<std::uint8_t>(toggle_value ^ 0x01U));
    const bool bit_was_set = (toggle_value & 0x01U) != 0U;
    set_bit_test_zero(registers, bit_was_set);

    if (!bit_was_set) {
        const auto phase_address = registers.address[5] + 0x3dU;
        const auto old_phase = read_byte(host, phase_address);
        const auto phase = static_cast<std::uint8_t>(old_phase + 4U);
        write_byte(host, phase_address, phase);
        set_add_byte_flags(registers, old_phase, 4U, phase);

        const auto compared_phase = read_byte(host, phase_address);
        set_compare_byte_flags(registers, compared_phase, 0x20U,
            static_cast<std::uint8_t>(compared_phase - 0x20U));
        if (compared_phase >= 0x20U) {
            (void)read_byte(host, registers.address[5]);
            write_byte(host, registers.address[5], 0U);
            set_logic_byte_flags(registers, 0U);
            const auto target = pop_return(host, registers);
            registers.program_counter = target;
            return FunctionResult::complete(1U, target);
        }

        registers.data[0] = 0U;
        set_logic_byte_flags(registers, 0U);
        registers.data[0] = read_byte(host, phase_address);
        set_logic_byte_flags(registers, static_cast<std::uint8_t>(registers.data[0]));
        registers.address[0] = 0x0001360aU
            + static_cast<std::uint32_t>(static_cast<std::int16_t>(registers.data[0]));

        const auto step = host.read_memory_word(kRegion, registers.address[0], kWordMask);
        registers.address[0] += 2U;
        host.write_memory_word(kRegion, registers.address[5] + 6U, step, kWordMask);
        set_logic_byte_flags(registers, static_cast<std::uint8_t>(step >> 8U));
        if ((step & 0x8000U) != 0U) registers.status |= 0x0008U;
        if (step == 0U) registers.status |= 0x0004U;
        else registers.status &= static_cast<std::uint16_t>(~0x0004U);

        const auto first = read_byte(host, registers.address[0]);
        ++registers.address[0];
        write_byte(host, registers.address[5] + 1U, first);
        set_logic_byte_flags(registers, first);
        const auto second = read_byte(host, registers.address[0]);
        ++registers.address[0];
        write_byte(host, registers.address[5] + 9U, second);
        set_logic_byte_flags(registers, second);
    }

    push_return(host, registers, 0x00012de8U);
    registers.program_counter = 0x00015d24U;
    auto child = host.call_function(280U, 1U, 0x72U, 2U,
        0x00012de2U, 0x00015d24U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    push_return(host, registers, 0x00012deeU);
    registers.program_counter = 0x00015df2U;
    child = host.call_function(282U, 1U, 0x72U, 2U,
        0x00012de8U, 0x00015df2U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated

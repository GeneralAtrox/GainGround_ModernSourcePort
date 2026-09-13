#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
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
        kPrivateRegion, location.offset, location.mask) >> location.shift);
}

void set_logic_byte(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_compare_word(CpuRegisters &registers,
    std::uint16_t destination, std::uint16_t source)
{
    const auto result = static_cast<std::uint16_t>(destination - source);
    std::uint16_t flags = registers.status & 0x0010U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (destination < source) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_byte(CpuRegisters &registers,
    std::uint8_t destination, std::uint8_t source, std::uint8_t result)
{
    std::uint16_t flags = 0U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(destination ^ source)) & (destination ^ result) & 0x80U) != 0U)
        flags |= 0x0002U;
    if (static_cast<std::uint16_t>(destination) + source > 0xffU)
        flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kPrivateRegion, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(
        kPrivateRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_object_callback_fa92(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    const auto d7_byte = static_cast<std::uint8_t>(registers.data[7]);
    set_logic_byte(registers, d7_byte);
    if (d7_byte != 0U) {
        const auto mode = host.read_memory_word(
            kPrivateRegion, 0x00000c00U, kWordMask);
        set_compare_word(registers, mode, 3U);
        if (mode < 3U) {
            const auto pointer_word = host.read_memory_word(
                kPrivateRegion, registers.address[5] + 0x6aU, kWordMask);
            registers.address[0] = static_cast<std::uint32_t>(
                static_cast<std::int32_t>(static_cast<std::int16_t>(pointer_word)));

            const auto record_flags = read_byte(host, registers.address[0] + 1U);
            registers.data[0] = (registers.data[0] & 0xffffff00U) | record_flags;
            set_logic_byte(registers, record_flags);
            const auto selected = static_cast<std::uint8_t>(record_flags & 0x06U);
            registers.data[0] = (registers.data[0] & 0xffffff00U) | selected;
            set_logic_byte(registers, selected);

            if (selected != 0U) {
                push_return(host, registers, 0x0000fab0U);
                registers.program_counter = 0x0000fac4U;
                const auto child = host.call_function(184U, 1U, 0x72U, 2U,
                    0x0000faacU, 0x0000fac4U, context);
                if (child.status != TranslationStatus::complete || child.control != 1U)
                    return child;

                const auto source = read_byte(host, registers.address[5] + 0x6dU);
                registers.data[0] = (registers.data[0] & 0xffffff00U) | source;
                set_logic_byte(registers, source);

                const auto doubled = static_cast<std::uint8_t>(source + source);
                set_add_byte(registers, source, source, doubled);
                registers.data[0] = (registers.data[0] & 0xffffff00U) | doubled;

                const auto output = static_cast<std::uint8_t>(doubled + 1U);
                set_add_byte(registers, doubled, 1U, output);
                registers.data[0] = (registers.data[0] & 0xffffff00U) | output;

                host.write_hardware(2U, 1U, 0x72U, 0x0000fab8U,
                    0x00d00034U, static_cast<std::uint16_t>(output) * 0x0101U,
                    0x00ffU);
                set_logic_byte(registers, output);

                registers.status = static_cast<std::uint16_t>(
                    (registers.status & 0xffe0U) | 0x0001U);
            }
        }
    }

    const auto return_address = pop_return(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated

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

[[nodiscard]] ByteLocation locate_byte(std::uint32_t address) noexcept
{
    address &= 0x00ffffffU;
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U),
        odd ? 0U : 8U};
}

[[nodiscard]] std::uint16_t read_word(
    ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & 0x00ffffffU, kWordMask);
}

[[nodiscard]] std::uint8_t read_workspace_byte(
    ExecutionHost &host, std::uint32_t address)
{
    const auto byte = locate_byte(address & 0x0003ffffU);
    return static_cast<std::uint8_t>(host.read_memory_word(
        3U, byte.address, byte.mask) >> byte.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address,
    std::uint8_t value)
{
    const auto byte = locate_byte(address);
    host.write_memory_word(kRegion, byte.address,
        static_cast<std::uint16_t>(value) << byte.shift, byte.mask);
}

void logic_byte(CpuRegisters &registers, std::uint8_t value) noexcept
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void logic_word(CpuRegisters &registers, std::uint16_t value) noexcept
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void compare_word(CpuRegisters &registers, std::uint16_t destination,
    std::uint16_t source) noexcept
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

void add_word(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result) noexcept
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (static_cast<std::uint32_t>(left) + right > 0xffffU)
        flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] bool negative(const CpuRegisters &registers) noexcept
{
    return (registers.status & 0x0008U) != 0U;
}

[[nodiscard]] bool greater_than(const CpuRegisters &registers) noexcept
{
    return (registers.status & 0x0004U) == 0U
        && ((registers.status & 0x0008U) != 0U)
            == ((registers.status & 0x0002U) != 0U);
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kRegion, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = read_word(host, registers.address[7]);
    const auto low = read_word(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

[[nodiscard]] FunctionResult call_probe(FunctionContext &context,
    std::uint32_t callsite, std::uint32_t continuation)
{
    push_return(*context.host, context.registers, continuation);
    context.registers.program_counter = 0x00015edeU;
    return context.host->call_function(287U, 1U, 0x72U, 2U,
        callsite, 0x00015edeU, context);
}

[[nodiscard]] bool complete(const FunctionResult &result) noexcept
{
    return result.status == TranslationStatus::complete
        && result.control == 1U;
}
} // namespace

FunctionResult cpu_b_check_record_position_window_y(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr
        || context.registers.program_counter != 0x00012c3cU)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto record = registers.address[5];
    const auto reject = [&]() -> FunctionResult {
        registers.address[7] += 4U;
        const auto byte = locate_byte(record);
        (void)host.read_memory_word(kRegion, byte.address, byte.mask);
        write_byte(host, record, 0U);
        logic_byte(registers, 0U);
        const auto target = pop_return(host, registers);
        registers.program_counter = target;
        return FunctionResult::complete(8U, target);
    };

    auto value = read_word(host, record + 0x12U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    logic_word(registers, value);
    compare_word(registers, value, 0xffe0U);
    if (negative(registers)) return reject();
    compare_word(registers, value, 0x01a0U);
    if (greater_than(registers)) return reject();

    value = read_word(host, record + 0x1aU);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    logic_word(registers, value);
    compare_word(registers, value, 0xffe0U);
    if (negative(registers)) return reject();
    compare_word(registers, value, 0x0210U);
    if (!negative(registers)) return reject();

    value = read_word(host, record + 0x2aU);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    logic_word(registers, value);
    value = read_word(host, record + 0x32U);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | value;
    logic_word(registers, value);
    value = read_word(host, record + 0x2eU);
    registers.data[2] = (registers.data[2] & 0xffff0000U) | value;
    logic_word(registers, value);
    registers.data[4] = 0U;
    logic_word(registers, 0U);

    auto doubled = static_cast<std::uint16_t>(
        static_cast<std::uint16_t>(registers.data[2]) * 2U);
    add_word(registers, static_cast<std::uint16_t>(registers.data[2]),
        static_cast<std::uint16_t>(registers.data[2]), doubled);
    registers.data[2] = (registers.data[2] & 0xffff0000U) | doubled;

    auto probe = call_probe(context, 0x00012c72U, 0x00012c78U);
    if (!complete(probe)) return probe;
    auto sample = read_workspace_byte(host, registers.address[0]);
    registers.data[3] = (registers.data[3] & 0xffffff00U) | sample;
    logic_byte(registers, sample);
    auto old_low = static_cast<std::uint8_t>(registers.data[4]);
    auto new_low = static_cast<std::uint8_t>(old_low | sample);
    registers.data[4] = (registers.data[4] & 0xffffff00U) | new_low;
    logic_byte(registers, new_low);
    auto extended = static_cast<std::uint16_t>(static_cast<std::int16_t>(
        static_cast<std::int8_t>(sample)));
    registers.data[3] = (registers.data[3] & 0xffff0000U) | extended;
    logic_word(registers, extended);
    compare_word(registers, static_cast<std::uint16_t>(registers.data[2]), extended);
    if (negative(registers)) {
        const auto before = static_cast<std::uint16_t>(registers.data[4]);
        const auto after = static_cast<std::uint16_t>(before + 0x0100U);
        registers.data[4] = (registers.data[4] & 0xffff0000U) | after;
        add_word(registers, before, 0x0100U, after);
    }

    registers.data[1] = (registers.data[1] & 0xffff0000U)
        | read_word(host, record + 0x34U);
    logic_word(registers, static_cast<std::uint16_t>(registers.data[1]));
    probe = call_probe(context, 0x00012c8aU, 0x00012c90U);
    if (!complete(probe)) return probe;
    sample = read_workspace_byte(host, registers.address[0]);
    registers.data[3] = (registers.data[3] & 0xffffff00U) | sample;
    logic_byte(registers, sample);
    old_low = static_cast<std::uint8_t>(registers.data[4]);
    new_low = static_cast<std::uint8_t>(old_low | sample);
    registers.data[4] = (registers.data[4] & 0xffffff00U) | new_low;
    logic_byte(registers, new_low);
    extended = static_cast<std::uint16_t>(static_cast<std::int16_t>(
        static_cast<std::int8_t>(sample)));
    registers.data[3] = (registers.data[3] & 0xffff0000U) | extended;
    logic_word(registers, extended);
    compare_word(registers, static_cast<std::uint16_t>(registers.data[2]), extended);
    if (negative(registers)) {
        const auto before = static_cast<std::uint16_t>(registers.data[4]);
        const auto after = static_cast<std::uint16_t>(before + 0x0100U);
        registers.data[4] = (registers.data[4] & 0xffff0000U) | after;
        add_word(registers, before, 0x0100U, after);
    }

    registers.data[0] = (registers.data[0] & 0xffff0000U)
        | read_word(host, record + 0x2cU);
    logic_word(registers, static_cast<std::uint16_t>(registers.data[0]));
    probe = call_probe(context, 0x00012ca2U, 0x00012ca8U);
    if (!complete(probe)) return probe;
    sample = read_workspace_byte(host, registers.address[0]);
    registers.data[3] = (registers.data[3] & 0xffffff00U) | sample;
    logic_byte(registers, sample);
    old_low = static_cast<std::uint8_t>(registers.data[4]);
    new_low = static_cast<std::uint8_t>(old_low | sample);
    registers.data[4] = (registers.data[4] & 0xffffff00U) | new_low;
    logic_byte(registers, new_low);
    extended = static_cast<std::uint16_t>(static_cast<std::int16_t>(
        static_cast<std::int8_t>(sample)));
    registers.data[3] = (registers.data[3] & 0xffff0000U) | extended;
    logic_word(registers, extended);
    compare_word(registers, static_cast<std::uint16_t>(registers.data[2]), extended);
    if (negative(registers)) {
        const auto before = static_cast<std::uint16_t>(registers.data[4]);
        const auto after = static_cast<std::uint16_t>(before + 0x0100U);
        registers.data[4] = (registers.data[4] & 0xffff0000U) | after;
        add_word(registers, before, 0x0100U, after);
    }

    registers.data[1] = (registers.data[1] & 0xffff0000U)
        | read_word(host, record + 0x32U);
    logic_word(registers, static_cast<std::uint16_t>(registers.data[1]));
    probe = call_probe(context, 0x00012cbaU, 0x00012cc0U);
    if (!complete(probe)) return probe;
    sample = read_workspace_byte(host, registers.address[0]);
    registers.data[3] = (registers.data[3] & 0xffffff00U) | sample;
    logic_byte(registers, sample);
    old_low = static_cast<std::uint8_t>(registers.data[4]);
    new_low = static_cast<std::uint8_t>(old_low | sample);
    registers.data[4] = (registers.data[4] & 0xffffff00U) | new_low;
    logic_byte(registers, new_low);
    extended = static_cast<std::uint16_t>(static_cast<std::int16_t>(
        static_cast<std::int8_t>(sample)));
    registers.data[3] = (registers.data[3] & 0xffff0000U) | extended;
    logic_word(registers, extended);
    compare_word(registers, static_cast<std::uint16_t>(registers.data[2]), extended);
    if (negative(registers)) {
        const auto before = static_cast<std::uint16_t>(registers.data[4]);
        const auto after = static_cast<std::uint16_t>(before + 0x0100U);
        registers.data[4] = (registers.data[4] & 0xffff0000U) | after;
        add_word(registers, before, 0x0100U, after);
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated

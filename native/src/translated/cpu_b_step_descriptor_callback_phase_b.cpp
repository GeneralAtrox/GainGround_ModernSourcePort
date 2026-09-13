#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

struct ByteLocation {
    std::uint32_t offset;
    std::uint16_t mask;
    unsigned shift;
};

[[nodiscard]] constexpr ByteLocation locate(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U),
        odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(
    ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address,
    std::uint8_t value)
{
    const auto location = locate(address);
    host.write_memory_word(kRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift,
        location.mask);
}

[[nodiscard]] std::uint16_t read_word(
    ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & 0x00ffffffU, kWordMask);
}

[[nodiscard]] std::uint32_t read_long(
    ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

void logic(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask) noexcept
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
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

void add_byte(CpuRegisters &registers, std::uint8_t left,
    std::uint8_t right, std::uint8_t result) noexcept
{
    std::uint16_t flags{};
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x80U) != 0U)
        flags |= 0x0002U;
    if (static_cast<std::uint16_t>(left) + right > 0xffU)
        flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void sub_byte(CpuRegisters &registers, std::uint8_t left,
    std::uint8_t right, std::uint8_t result, bool update_extend) noexcept
{
    std::uint16_t flags = update_extend ? 0U : (registers.status & 0x0010U);
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x80U) != 0U)
        flags |= 0x0002U;
    if (right > left) flags |= update_extend ? 0x0011U : 0x0001U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void asl_word(CpuRegisters &registers, unsigned count) noexcept
{
    auto value = static_cast<std::uint16_t>(registers.data[0]);
    bool overflow{};
    bool carry{};
    for (unsigned index = 0; index != count; ++index) {
        const bool old_sign = (value & 0x8000U) != 0U;
        carry = old_sign;
        value = static_cast<std::uint16_t>(value << 1U);
        overflow = overflow || (old_sign != ((value & 0x8000U) != 0U));
    }
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    std::uint16_t flags{};
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    if (carry) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
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
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return target;
}

[[nodiscard]] FunctionResult call_child(FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite,
    std::uint32_t target, std::uint32_t continuation)
{
    push_return(*context.host, context.registers, continuation);
    context.registers.program_counter = target;
    return context.host->call_function(function_id, 1U, 0x72U, 2U,
        callsite, target, context);
}

[[nodiscard]] bool complete(const FunctionResult &result) noexcept
{
    return result.status == TranslationStatus::complete
        && result.control == 1U;
}
} // namespace

FunctionResult cpu_b_step_descriptor_callback_phase_b(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    FunctionResult character_result;
    if (host.run_character_attack_phase(context, character_result)) return character_result;
    if (registers.program_counter != 0x00010cc0U)
        return {TranslationStatus::contract_violation, 0U, registers.program_counter};
    const auto record = registers.address[5];

    auto d0 = read_word(host, record + 0x3aU);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    logic(registers, d0, 0x8000U, 0xffffU);
    registers.data[7] = (registers.data[7] & 0xffff0000U) | d0;
    logic(registers, d0, 0x8000U, 0xffffU);

    auto result_word = static_cast<std::uint16_t>(d0 + d0);
    add_word(registers, d0, d0, result_word);
    d0 = result_word;
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    registers.data[1] = (registers.data[1] & 0xffff0000U) | d0;
    logic(registers, d0, 0x8000U, 0xffffU);

    result_word = static_cast<std::uint16_t>(d0 + d0);
    add_word(registers, d0, d0, result_word);
    d0 = result_word;
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    auto d1 = static_cast<std::uint16_t>(registers.data[1]);
    result_word = static_cast<std::uint16_t>(d1 + d0);
    add_word(registers, d1, d0, result_word);
    d1 = result_word;
    registers.data[1] = (registers.data[1] & 0xffff0000U) | d1;

    asl_word(registers, 3U);
    d0 = static_cast<std::uint16_t>(registers.data[0]);
    result_word = static_cast<std::uint16_t>(d0 + d1);
    add_word(registers, d0, d1, result_word);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | result_word;
    registers.address[3] = static_cast<std::uint32_t>(0x00011744U
        + static_cast<std::int32_t>(static_cast<std::int16_t>(result_word)));

    auto child = call_child(context, 225U, 0x00010cd8U,
        0x00010d34U, 0x00010cdcU);
    if (!complete(child)) return child;

    auto timer = read_byte(host, record + 0x41U);
    logic(registers, timer, 0x80U, 0xffU);
    if (timer != 0U) {
        const auto timer_operand = read_byte(host, record + 0x41U);
        const auto decremented = static_cast<std::uint8_t>(timer_operand - 1U);
        write_byte(host, record + 0x41U, decremented);
        sub_byte(registers, timer_operand, 1U, decremented, true);
    }

    const auto mode = read_byte(host, record + 0x3eU);
    sub_byte(registers, mode, 2U,
        static_cast<std::uint8_t>(mode - 2U), false);
    if (mode != 2U) {
        const auto target = pop_return(host, registers);
        registers.program_counter = target;
        return FunctionResult::complete(1U, target);
    }

    auto counter = read_byte(host, record + 0x3cU);
    const auto incremented_counter = static_cast<std::uint8_t>(counter + 1U);
    write_byte(host, record + 0x3cU, incremented_counter);
    add_byte(registers, counter, 1U, incremented_counter);
    counter = incremented_counter;

    registers.address[0] = 0x0001154eU;
    const auto selector = static_cast<std::int16_t>(registers.data[7]);
    auto d0_byte = read_byte(host, static_cast<std::uint32_t>(
        registers.address[0] + static_cast<std::int32_t>(selector)));
    registers.data[0] = (registers.data[0] & 0xffffff00U) | d0_byte;
    logic(registers, d0_byte, 0x80U, 0xffU);
    const auto compared_counter = read_byte(host, record + 0x3cU);
    sub_byte(registers, d0_byte, compared_counter,
        static_cast<std::uint8_t>(d0_byte - compared_counter), false);
    const bool greater = (registers.status & 0x0004U) == 0U
        && ((registers.status & 0x0008U) != 0U)
            == ((registers.status & 0x0002U) != 0U);
    if (greater) {
        const auto target = pop_return(host, registers);
        registers.program_counter = target;
        return FunctionResult::complete(1U, target);
    }

    (void)read_byte(host, record + 0x3cU);
    write_byte(host, record + 0x3cU, 0U);
    logic(registers, 0U, 0x80U, 0xffU);
    auto phase = read_byte(host, record + 0x3dU);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | phase;
    logic(registers, phase, 0x80U, 0xffU);
    const auto incremented_phase = static_cast<std::uint8_t>(phase + 1U);
    add_byte(registers, phase, 1U, incremented_phase);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | incremented_phase;
    write_byte(host, record + 0x3dU, incremented_phase);
    logic(registers, incremented_phase, 0x80U, 0xffU);
    sub_byte(registers, incremented_phase, 1U,
        static_cast<std::uint8_t>(incremented_phase - 1U), false);
    if (incremented_phase == 1U) {
        child = call_child(context, 230U, 0x00010d14U,
            0x00010e60U, 0x00010d1aU);
        if (!complete(child)) return child;
        d0_byte = read_byte(host, record + 0x3dU);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | d0_byte;
        logic(registers, d0_byte, 0x80U, 0xffU);
    } else {
        d0_byte = incremented_phase;
    }

    registers.address[0] = 0x0001156eU;
    const auto limit = read_byte(host, static_cast<std::uint32_t>(
        registers.address[0] + static_cast<std::int32_t>(selector)));
    sub_byte(registers, d0_byte, limit,
        static_cast<std::uint8_t>(d0_byte - limit), false);
    if (d0_byte >= limit) {
        (void)read_byte(host, record + 0x3eU);
        write_byte(host, record + 0x3eU, 0U);
        logic(registers, 0U, 0x80U, 0xffU);
        write_byte(host, record + 0x3dU, 1U);
        logic(registers, 1U, 0x80U, 0xffU);
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated

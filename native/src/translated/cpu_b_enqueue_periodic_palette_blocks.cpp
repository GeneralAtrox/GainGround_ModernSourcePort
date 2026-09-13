#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint16_t read_word(
    ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & 0x00ffffffU, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address,
    std::uint16_t value)
{
    host.write_memory_word(kRegion, address & 0x00ffffffU, value, kWordMask);
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

void sub_word(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result, bool update_extend) noexcept
{
    std::uint16_t flags = update_extend ? 0U : (registers.status & 0x0010U);
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (right > left) flags |= update_extend ? 0x0011U : 0x0001U;
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
    write_word(host, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U));
    write_word(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value));
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return target;
}

[[nodiscard]] bool signed_greater(const CpuRegisters &registers) noexcept
{
    return (registers.status & 0x0004U) == 0U
        && ((registers.status & 0x0008U) != 0U)
            == ((registers.status & 0x0002U) != 0U);
}

[[nodiscard]] FunctionResult finish(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_enqueue_periodic_palette_blocks(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr
        || context.registers.program_counter != 0x0000ddfaU)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto record = registers.address[5];

    const auto mode = read_word(host, 0x00000c00U);
    sub_word(registers, mode, 3U,
        static_cast<std::uint16_t>(mode - 3U), false);
    if (mode != 3U) return finish(host, registers);

    const auto old_timer = read_word(host, record + 0x28U);
    const auto timer = static_cast<std::uint16_t>(old_timer - 1U);
    write_word(host, record + 0x28U, timer);
    sub_word(registers, old_timer, 1U, timer, true);
    if (signed_greater(registers)) return finish(host, registers);

    write_word(host, record + 0x28U, 4U);
    logic(registers, 4U, 0x8000U, 0xffffU);
    registers.address[1] = 0x000255a4U;

    auto d0 = read_word(host, 0x00000c02U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    logic(registers, d0, 0x8000U, 0xffffU);
    auto result_word = static_cast<std::uint16_t>(d0 - 0x001eU);
    sub_word(registers, d0, 0x001eU, result_word, true);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | result_word;
    asl_word(registers, 2U);
    const auto index = static_cast<std::int16_t>(registers.data[0]);
    registers.address[1] = read_long(host, static_cast<std::uint32_t>(
        registers.address[1] + static_cast<std::int32_t>(index)));

    auto d2 = read_word(host, registers.address[1]);
    registers.address[1] += 2U;
    registers.data[2] = (registers.data[2] & 0xffff0000U) | d2;
    logic(registers, d2, 0x8000U, 0xffffU);
    if (d2 == 0U) return finish(host, registers);

    d0 = read_word(host, registers.address[1]);
    registers.address[1] += 2U;
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    logic(registers, d0, 0x8000U, 0xffffU);

    const auto old_counter = read_word(host, record + 0x2aU);
    const auto counter = static_cast<std::uint16_t>(old_counter + 1U);
    write_word(host, record + 0x2aU, counter);
    add_word(registers, old_counter, 1U, counter);
    const auto compared_counter = read_word(host, record + 0x2aU);
    sub_word(registers, d0, compared_counter,
        static_cast<std::uint16_t>(d0 - compared_counter), false);
    if (!signed_greater(registers)) {
        (void)read_word(host, record + 0x2aU);
        write_word(host, record + 0x2aU, 0U);
        logic(registers, 0U, 0x8000U, 0xffffU);
    }

    d0 = read_word(host, record + 0x2aU);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    logic(registers, d0, 0x8000U, 0xffffU);
    const auto product = static_cast<std::uint32_t>(d0) * d2;
    registers.data[0] = product;
    logic(registers, product, 0x80000000U, 0xffffffffU);
    registers.address[1] = static_cast<std::uint32_t>(registers.address[1]
        + static_cast<std::int32_t>(static_cast<std::int16_t>(product)));

    do {
        d0 = 0U;
        registers.data[0] = read_long(host, registers.address[1]);
        registers.address[1] += 4U;
        logic(registers, registers.data[0], 0x80000000U, 0xffffffffU);

        push_return(host, registers, 0x0000de46U);
        registers.program_counter = 0x00015e4eU;
        const auto child = host.call_function(284U, 1U, 0x72U, 2U,
            0x0000de40U, 0x00015e4eU, context);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;

        const auto next_d2 = static_cast<std::uint16_t>(d2 - 4U);
        sub_word(registers, d2, 4U, next_d2, true);
        d2 = next_d2;
        registers.data[2] = (registers.data[2] & 0xffff0000U) | d2;
    } while (signed_greater(registers));

    return finish(host, registers);
}

} // namespace gain_ground::translated

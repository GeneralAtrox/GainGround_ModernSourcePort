#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgram = 1U;
constexpr std::uint16_t kMain = 2U;
constexpr std::uint16_t kShared = 3U;
constexpr std::uint16_t kWord = 0xffffU;
constexpr std::uint16_t kLowByte = 0x00ffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void pf(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgram, address, kWord);
}

std::uint16_t read_shared_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kShared, address & kSharedMask, kWord);
}

void write_shared_word(ExecutionHost &host, std::uint32_t address,
    std::uint16_t value)
{
    host.write_memory_word(kShared, address & kSharedMask, value, kWord);
}

std::uint32_t read_shared_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_shared_word(host, address)) << 16U)
        | read_shared_word(host, address + 2U);
}

void write_shared_long(ExecutionHost &host, std::uint32_t address,
    std::uint32_t value)
{
    write_shared_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_shared_word(host, address + 2U, static_cast<std::uint16_t>(value));
}

std::uint8_t read_main_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto value = host.read_memory_word(kMain,
        (address & kSharedMask) & ~1U, odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? value : value >> 8U);
}

void write_shared_byte(ExecutionHost &host, std::uint32_t address,
    std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    host.write_memory_word(kShared, (address & kSharedMask) & ~1U,
        static_cast<std::uint16_t>(value) << (odd ? 0U : 8U),
        odd ? 0x00ffU : 0xff00U);
}

std::uint8_t read_hardware_byte(FunctionContext &context, std::uint32_t pc,
    std::uint32_t address)
{
    return static_cast<std::uint8_t>(context.host->read_hardware(
        1U, context.cpu, context.state, pc, address & ~1U, kLowByte));
}

void write_hardware_byte(FunctionContext &context, std::uint32_t pc,
    std::uint32_t address, std::uint8_t value)
{
    context.host->write_hardware(2U, context.cpu, context.state, pc,
        address & ~1U, static_cast<std::uint16_t>(value) * 0x0101U,
        kLowByte);
}

void logic_byte(CpuRegisters &registers, std::uint8_t value)
{
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x000fU)
        | (value == 0U ? 0x0004U : 0U)
        | ((value & 0x80U) != 0U ? 0x0008U : 0U));
}

void logic_word(CpuRegisters &registers, std::uint16_t value)
{
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x000fU)
        | (value == 0U ? 0x0004U : 0U)
        | ((value & 0x8000U) != 0U ? 0x0008U : 0U));
}

void logic_long(CpuRegisters &registers, std::uint32_t value)
{
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x000fU)
        | (value == 0U ? 0x0004U : 0U)
        | ((value & 0x80000000U) != 0U ? 0x0008U : 0U));
}

void compare_byte(CpuRegisters &registers, std::uint8_t destination,
    std::uint8_t source)
{
    const auto result = static_cast<std::uint8_t>(destination - source);
    const bool borrow = destination < source;
    const bool overflow = (((destination ^ source) & (destination ^ result))
        & 0x80U) != 0U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x000fU)
        | (result == 0U ? 0x0004U : 0U)
        | ((result & 0x80U) != 0U ? 0x0008U : 0U)
        | (overflow ? 0x0002U : 0U)
        | (borrow ? 0x0001U : 0U));
}

void bit_test(CpuRegisters &registers, bool set)
{
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
        | (set ? 0U : 0x0004U));
}

void subtract_quick_word(CpuRegisters &registers, std::uint32_t index,
    std::uint16_t amount)
{
    const auto before = static_cast<std::uint16_t>(registers.data[index]);
    const auto result = static_cast<std::uint16_t>(before - amount);
    const bool borrow = before < amount;
    const bool overflow = (((before ^ amount) & (before ^ result))
        & 0x8000U) != 0U;
    registers.data[index] = (registers.data[index] & 0xffff0000U) | result;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU)
        | (result == 0U ? 0x0004U : 0U)
        | ((result & 0x8000U) != 0U ? 0x0008U : 0U)
        | (overflow ? 0x0002U : 0U)
        | (borrow ? 0x0011U : 0U));
}

void push_long(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_shared_long(host, registers.address[7], value);
}

FunctionResult call_child(FunctionContext &context, std::uint32_t function_id,
    std::uint32_t callsite, std::uint32_t target, std::uint32_t return_pc)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    push_long(host, registers, return_pc);
    pf(host, target);
    pf(host, target + 2U);
    registers.program_counter = target;
    return host.call_function(function_id, 0U, 0xffU, 2U,
        callsite, target, context);
}

FunctionResult return_from_subroutine(FunctionContext &context)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto target = read_shared_long(host, registers.address[7]);
    registers.address[7] += 4U;
    pf(host, target);
    pf(host, target + 2U);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

FunctionResult drive_absent_recovery(FunctionContext &context)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    pf(host, 0x0000195eU);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | 1U;
    logic_word(registers, 1U);
    pf(host, 0x00001960U);
    pf(host, 0x00001962U);
    pf(host, 0x00001990U);
    pf(host, 0x00001992U);
    registers.program_counter = 0x00001990U;
    return host.call_function(23U, 0U, 0xffU, 1U,
        0x00001960U, 0x00001990U, context);
}

void wait_fdc_bit0_clear(FunctionContext &context, std::uint32_t test_pc,
    std::uint32_t branch_pc)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    for (;;) {
        pf(host, test_pc + 2U);
        pf(host, test_pc + 4U);
        pf(host, branch_pc);
        const auto value = read_hardware_byte(context, test_pc,
            registers.address[5]);
        bit_test(registers, (value & 1U) != 0U);
        if ((value & 1U) == 0U)
            return;
        pf(host, test_pc);
    }
}
} // namespace

FunctionResult cpu_a_fdc_transfer_verified_buffer_stage(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr
        || context.registers.program_counter != 0x0000113eU) {
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    }

    auto &host = *context.host;
    auto &registers = context.registers;

    pf(host, 0x00001142U);
    pf(host, 0x00001144U);
    auto value = read_hardware_byte(context, 0x0000113eU, 0x00b00005U);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | value;
    logic_byte(registers, value);

    value = static_cast<std::uint8_t>(~value);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | value;
    logic_byte(registers, value);
    pf(host, 0x00001146U);
    pf(host, 0x00001148U);
    pf(host, 0x0000114aU);
    pf(host, 0x0000114cU);
    write_hardware_byte(context, 0x00001146U, 0x00b00005U, value);

    pf(host, 0x0000114eU);
    registers.address[7] -= 4U;
    auto stack_probe = read_shared_long(host, registers.address[7]);
    logic_long(registers, stack_probe);
    pf(host, 0x00001150U);
    stack_probe = read_shared_long(host, registers.address[7]);
    registers.address[7] += 4U;
    logic_long(registers, stack_probe);

    pf(host, 0x00001152U);
    pf(host, 0x00001154U);
    pf(host, 0x00001156U);
    const auto compare = read_hardware_byte(context, 0x00001150U,
        0x00b00005U);
    compare_byte(registers, static_cast<std::uint8_t>(registers.data[0]),
        compare);
    pf(host, 0x00001158U);
    if ((registers.status & 0x0004U) == 0U)
        return {TranslationStatus::contract_violation, 0U, 0x00001156U};

    pf(host, 0x0000115aU);
    pf(host, 0x0000115cU);
    pf(host, 0x0000115eU);
    registers.address[5] = 0x00b00001U;
    pf(host, 0x00001160U);
    registers.data[4] = registers.data[1];
    logic_long(registers, registers.data[4]);
    pf(host, 0x00001162U);
    pf(host, 0x00001164U);
    pf(host, 0x00001166U);
    pf(host, 0x00001168U);
    const auto drive = read_hardware_byte(context, 0x00001162U,
        registers.address[5] + 8U);
    bit_test(registers, (drive & 0x10U) != 0U);
    pf(host, 0x0000116aU);
    if ((drive & 0x10U) == 0U)
        return drive_absent_recovery(context);

    pf(host, 0x0000116cU);
    wait_fdc_bit0_clear(context, 0x0000116cU, 0x00001172U);
    pf(host, 0x00001174U);
    pf(host, 0x00001176U);
    pf(host, 0x00001178U);
    pf(host, 0x0000117aU);
    write_hardware_byte(context, 0x00001174U, registers.address[5], 0xd0U);
    logic_byte(registers, 0xd0U);

    wait_fdc_bit0_clear(context, 0x0000117aU, 0x00001180U);
    pf(host, 0x00001182U);
    pf(host, 0x00001184U);
    pf(host, 0x00001186U);
    pf(host, 0x00001188U);
    write_hardware_byte(context, 0x00001182U,
        registers.address[5] + 6U, 0xc0U);
    logic_byte(registers, 0xc0U);
    pf(host, 0x0000118aU);
    pf(host, 0x0000118cU);
    pf(host, 0x0000118eU);
    write_hardware_byte(context, 0x00001188U, registers.address[5], 0xfeU);
    logic_byte(registers, 0xfeU);

    wait_fdc_bit0_clear(context, 0x0000118eU, 0x00001194U);
    pf(host, 0x00001196U);
    pf(host, 0x00001198U);
    pf(host, 0x0000119aU);
    pf(host, 0x0000119cU);
    write_hardware_byte(context, 0x00001196U,
        registers.address[5] + 6U, 0x8aU);
    logic_byte(registers, 0x8aU);
    pf(host, 0x0000119eU);
    pf(host, 0x000011a0U);
    pf(host, 0x000011a2U);
    write_hardware_byte(context, 0x0000119cU, registers.address[5], 0xfdU);
    logic_byte(registers, 0xfdU);

    registers.address[2] = registers.address[0];
    pf(host, 0x000011a4U);
    registers.data[0] = 0U;
    logic_long(registers, 0U);
    pf(host, 0x000011a6U);
    pf(host, 0x000011a8U);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | 0x0b3fU;
    logic_word(registers, 0x0b3fU);
    pf(host, 0x000011aaU);

    for (;;) {
        pf(host, 0x000011acU);
        write_shared_long(host, registers.address[0], registers.data[0]);
        registers.address[0] += 4U;
        logic_long(registers, registers.data[0]);
        pf(host, 0x000011aeU);
        const auto counter = static_cast<std::uint16_t>(registers.data[1] - 1U);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | counter;
        pf(host, 0x000011aaU);
        if (counter == 0xffffU)
            break;
    }

    pf(host, 0x000011b0U);
    registers.address[0] = registers.address[2];
    pf(host, 0x000011b2U);
    registers.data[1] = registers.data[4];
    logic_long(registers, registers.data[1]);
    pf(host, 0x000011b4U);
    subtract_quick_word(registers, 1U, 1U);
    pf(host, 0x000011b6U);

    for (;;) {
        pf(host, 0x000011b8U);
        const auto byte = read_main_byte(host, registers.address[1]);
        ++registers.address[1];
        write_shared_byte(host, registers.address[0], byte);
        ++registers.address[0];
        logic_byte(registers, byte);
        pf(host, 0x000011baU);
        const auto counter = static_cast<std::uint16_t>(registers.data[1] - 1U);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | counter;
        pf(host, 0x000011b6U);
        if (counter == 0xffffU)
            break;
    }

    pf(host, 0x000011bcU);
    pf(host, 0x000011beU);
    auto child = call_child(context, 14U, 0x000011bcU,
        0x00001284U, 0x000011c0U);
    if (child.status != TranslationStatus::complete)
        return child;

    registers.data[7] = 0U;
    logic_long(registers, 0U);
    pf(host, 0x000011c4U);
    child = call_child(context, 17U, 0x000011c2U,
        0x00001710U, 0x000011c6U);
    if (child.status != TranslationStatus::complete)
        return child;
    if ((registers.status & 1U) != 0U)
        return {TranslationStatus::contract_violation, 0U, 0x000011c6U};

    pf(host, 0x000011caU);
    child = call_child(context, 16U, 0x000011c8U,
        0x000016a0U, 0x000011ccU);
    if (child.status != TranslationStatus::complete)
        return child;
    if ((registers.status & 1U) == 0U)
        pf(host, 0x000011d2U);
    else
        return {TranslationStatus::contract_violation, 0U, 0x000011ccU};

    registers.data[7] = 1U;
    logic_long(registers, 1U);
    pf(host, 0x000011d4U);
    pf(host, 0x000011d6U);
    child = call_child(context, 17U, 0x000011d4U,
        0x00001710U, 0x000011d8U);
    if (child.status != TranslationStatus::complete)
        return child;
    if ((registers.status & 1U) != 0U)
        return {TranslationStatus::contract_violation, 0U, 0x000011d8U};

    pf(host, 0x000011dcU);
    child = call_child(context, 16U, 0x000011daU,
        0x000016a0U, 0x000011deU);
    if (child.status != TranslationStatus::complete)
        return child;
    if ((registers.status & 1U) != 0U)
        return {TranslationStatus::contract_violation, 0U, 0x000011deU};

    pf(host, 0x000011e4U);
    pf(host, 0x000011e6U);
    return return_from_subroutine(context);
}

} // namespace gain_ground::translated

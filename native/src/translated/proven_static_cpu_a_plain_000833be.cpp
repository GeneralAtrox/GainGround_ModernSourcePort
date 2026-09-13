#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & kAddressMask, kWordMask);
}

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto offset = address & kAddressMask;
    const bool odd = (offset & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto word = host.read_memory_word(kRegion, offset & ~1U, mask);
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto offset = address & kAddressMask;
    const bool odd = (offset & 1U) != 0U;
    host.write_memory_word(kRegion, offset & ~1U,
        static_cast<std::uint16_t>(odd ? value : static_cast<unsigned>(value) << 8U),
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U));
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kRegion, address & kAddressMask,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, (address + 2U) & kAddressMask,
        static_cast<std::uint16_t>(value), kWordMask);
}

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)read_word(host, address);
}

void set_logic_flags(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t sign)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_bit_test_flags(CpuRegisters &registers, bool set)
{
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x0004U) | (set ? 0U : 0x0004U));
}

void set_add_flags(CpuRegisters &registers, std::uint32_t left,
    std::uint32_t right, std::uint32_t result, std::uint32_t mask,
    std::uint32_t sign)
{
    std::uint16_t flags{};
    if (left + right > mask) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & sign) != 0U) flags |= 0x0002U;
    if ((result & sign) != 0U) flags |= 0x0008U;
    if ((result & mask) == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_sub_byte_flags(CpuRegisters &registers, std::uint8_t left,
    std::uint8_t right, std::uint8_t result, bool alter_extend)
{
    const bool borrow = right > left;
    std::uint16_t flags = alter_extend ? 0U : registers.status & 0x0010U;
    if (borrow) flags |= static_cast<std::uint16_t>(alter_extend ? 0x0011U : 0x0001U);
    if (((left ^ right) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_lsl_word_flags(CpuRegisters &registers, std::uint16_t original,
    std::uint16_t result, unsigned count)
{
    const bool carry = (original & (0x10000U >> count)) != 0U;
    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t return_address)
{
    registers.address[7] -= 4U;
    write_long(host, registers.address[7], return_address);
}

FunctionResult return_from_subroutine(ExecutionHost &host,
    CpuRegisters &registers)
{
    const auto stack = registers.address[7] & kAddressMask;
    const auto target = (static_cast<std::uint32_t>(read_word(host, stack)) << 16U)
        | read_word(host, stack + 2U);
    registers.address[7] += 4U;
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

FunctionResult outer_return(ExecutionHost &host, CpuRegisters &registers)
{
    prefetch(host, 0x000834aeU);
    prefetch(host, 0x000834b0U);
    return return_from_subroutine(host, registers);
}
} // namespace

FunctionResult proven_static_cpu_a_plain_000833be(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto channel = registers.address[3];

    prefetch(host, 0x000833c2U);
    prefetch(host, 0x000833c4U);
    const auto mode = read_byte(host, channel + 1U);
    const bool early_return = (mode & 0x10U) != 0U;
    set_bit_test_flags(registers, early_return);
    prefetch(host, 0x000833c6U);
    if (early_return) return outer_return(host, registers);

    prefetch(host, 0x000833c8U);
    registers.data[0] = 0U;
    set_logic_flags(registers, 0U, 0x80000000U);
    prefetch(host, 0x000833caU);
    prefetch(host, 0x000833ccU);
    prefetch(host, 0x000833ceU);
    auto value = read_byte(host, channel + 9U);
    registers.data[0] = value;
    set_logic_flags(registers, value, 0x80U);
    const auto selector = static_cast<std::uint8_t>(value - 1U);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | selector;
    set_sub_byte_flags(registers, value, 1U, selector, true);
    prefetch(host, 0x000833d0U);
    const auto doubled = static_cast<std::uint16_t>(selector + selector);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | doubled;
    set_add_flags(registers, selector, selector, doubled, 0xffffU, 0x8000U);
    prefetch(host, 0x000833d2U);
    registers.data[1] = 8U;
    set_logic_flags(registers, 8U, 0x80000000U);
    prefetch(host, 0x000833d4U);
    prefetch(host, 0x000833d6U);
    push_return(host, registers, 0x000833d8U);
    prefetch(host, 0x0008413eU);
    prefetch(host, 0x00084140U);
    registers.program_counter = 0x0008413eU;
    const auto resolved = host.call_function(89U, 0U, 0xffU, 2U,
        0x000833d4U, 0x0008413eU, context);
    if (resolved.status != TranslationStatus::complete) return resolved;

    registers.address[1] = registers.address[0];

    for (;;) {
        registers.data[0] = 0U;
        set_logic_flags(registers, 0U, 0x80000000U);
        prefetch(host, 0x000833dcU);
        prefetch(host, 0x000833deU);
        prefetch(host, 0x000833e0U);
        const auto index = read_byte(host, channel + 0x14U);
        registers.data[0] = index;
        set_logic_flags(registers, index, 0x80U);
        prefetch(host, 0x000833e2U);
        const auto next_index = static_cast<std::uint8_t>(index + 1U);
        prefetch(host, 0x000833e4U);
        (void)read_byte(host, channel + 0x14U);
        prefetch(host, 0x000833e6U);
        write_byte(host, channel + 0x14U, next_index);
        set_add_flags(registers, index, 1U, next_index, 0xffU, 0x80U);
        registers.data[1] = 0U;
        set_logic_flags(registers, 0U, 0x80000000U);
        prefetch(host, 0x000833e8U);
        prefetch(host, 0x000833eaU);
        const auto command = read_byte(host, registers.address[1]
            + static_cast<std::int16_t>(static_cast<std::uint16_t>(registers.data[0])));
        registers.data[1] = command;
        set_logic_flags(registers, command, 0x80U);

        prefetch(host, 0x000833ecU);
        set_sub_byte_flags(registers, command, 0x80U,
            static_cast<std::uint8_t>(command - 0x80U), false);
        prefetch(host, 0x000833eeU);
        prefetch(host, 0x000833f0U);
        if (command < 0x80U) break;

        prefetch(host, 0x000833f2U);
        prefetch(host, 0x000833f4U);
        if (command == 0x80U) {
            prefetch(host, 0x00083410U);
            prefetch(host, 0x00083412U);
            prefetch(host, 0x00083414U);
            prefetch(host, 0x00083416U);
            write_byte(host, channel + 9U, 0U);
            set_logic_flags(registers, 0U, 0x80U);
            prefetch(host, 0x00083418U);
            prefetch(host, 0x0008341aU);
            prefetch(host, 0x0008341cU);
            write_byte(host, channel + 0x14U, 0U);
            set_logic_flags(registers, 0U, 0x80U);
            prefetch(host, 0x0008341eU);
            return return_from_subroutine(host, registers);
        }

        prefetch(host, 0x000833f6U);
        prefetch(host, 0x000833f8U);
        set_sub_byte_flags(registers, command, 0x83U,
            static_cast<std::uint8_t>(command - 0x83U), false);
        prefetch(host, 0x000833faU);
        prefetch(host, 0x000833fcU);
        if (command > 0x83U) break;
        prefetch(host, 0x000833feU);
        prefetch(host, 0x00083400U);
        if (command == 0x83U) {
            prefetch(host, 0x00083416U);
            prefetch(host, 0x00083418U);
            prefetch(host, 0x0008341aU);
            prefetch(host, 0x0008341cU);
            write_byte(host, channel + 0x14U, 0U);
            set_logic_flags(registers, 0U, 0x80U);
            prefetch(host, 0x0008341eU);
            return return_from_subroutine(host, registers);
        }

        prefetch(host, 0x00083402U);
        prefetch(host, 0x00083404U);
        set_sub_byte_flags(registers, command, 0x81U,
            static_cast<std::uint8_t>(command - 0x81U), false);
        prefetch(host, 0x00083406U);
        prefetch(host, 0x00083408U);
        if (command != 0x81U) {
            prefetch(host, 0x0008340aU);
            prefetch(host, 0x0008340cU);
            write_byte(host, channel + 0x14U, index);
            set_sub_byte_flags(registers, next_index, 1U, index, true);
            prefetch(host, 0x0008340eU);
            prefetch(host, 0x00083410U);
            return return_from_subroutine(host, registers);
        }

        prefetch(host, 0x0008341eU);
        prefetch(host, 0x00083420U);
        const auto high = read_byte(host, registers.address[1]++);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | high;
        set_logic_flags(registers, high, 0x80U);
        auto displacement = static_cast<std::uint16_t>(high) << 8U;
        registers.data[0] = (registers.data[0] & 0xffff0000U) | displacement;
        set_lsl_word_flags(registers, high, displacement, 8U);
        prefetch(host, 0x00083422U);
        prefetch(host, 0x00083424U);
        const auto low = read_byte(host, registers.address[1]++);
        displacement = static_cast<std::uint16_t>((displacement & 0xff00U) | low);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | displacement;
        set_logic_flags(registers, low, 0x80U);
        registers.address[1] = static_cast<std::uint32_t>(
            registers.address[1] + static_cast<std::int16_t>(displacement));
        prefetch(host, 0x00083426U);
        prefetch(host, 0x00083428U);
        prefetch(host, 0x0008342aU);
        prefetch(host, 0x0008342cU);
        write_byte(host, channel + 0x14U, 0U);
        set_logic_flags(registers, 0U, 0x80U);
        prefetch(host, 0x0008342eU);
        prefetch(host, 0x000833daU);
    }

    prefetch(host, 0x0008342eU);
    prefetch(host, 0x00083430U);
    prefetch(host, 0x00083432U);
    prefetch(host, 0x00083434U);
    const auto flags = read_byte(host, channel);
    const bool suppress = (flags & 0x04U) != 0U;
    set_bit_test_flags(registers, suppress);
    prefetch(host, 0x00083436U);
    if (suppress) return outer_return(host, registers);

    prefetch(host, 0x00083438U);
    const auto command = static_cast<std::uint8_t>(registers.data[1]);
    set_logic_flags(registers, command, 0x80U);
    prefetch(host, 0x0008343aU);
    prefetch(host, 0x0008343cU);
    auto pitch_delta = static_cast<std::uint16_t>(registers.data[1]);
    if ((command & 0x80U) != 0U) {
        prefetch(host, 0x0008343eU);
        prefetch(host, 0x00083440U);
        pitch_delta = static_cast<std::uint16_t>(pitch_delta | 0xff00U);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | pitch_delta;
        set_logic_flags(registers, pitch_delta, 0x8000U);
    }
    prefetch(host, 0x00083442U);
    const auto shifted = static_cast<std::uint16_t>(pitch_delta << 2U);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | shifted;
    set_lsl_word_flags(registers, pitch_delta, shifted, 2U);
    prefetch(host, 0x00083444U);
    prefetch(host, 0x00083446U);
    prefetch(host, 0x00083448U);
    const auto base_pitch = read_word(host, channel + 0x10U);
    const auto final_pitch = static_cast<std::uint16_t>(shifted + base_pitch);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | final_pitch;
    set_add_flags(registers, shifted, base_pitch, final_pitch, 0xffffU, 0x8000U);
    prefetch(host, 0x0008344aU);
    prefetch(host, 0x0008419cU);
    prefetch(host, 0x0008419eU);
    registers.program_counter = 0x0008419cU;
    return host.call_function(93U, 0U, 0xffU, 1U,
        0x00083448U, 0x0008419cU, context);
}

} // namespace gain_ground::translated

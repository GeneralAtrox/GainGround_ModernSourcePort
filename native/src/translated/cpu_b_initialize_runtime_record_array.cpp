#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

struct ByteLane {
    std::uint32_t offset;
    std::uint16_t mask;
    unsigned shift;
};

ByteLane byte_lane(std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U, static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto lane = byte_lane(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, lane.offset, lane.mask) >> lane.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto lane = byte_lane(address);
    host.write_memory_word(kRegion, lane.offset,
        static_cast<std::uint16_t>(value) << lane.shift, lane.mask);
}

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kRegion, address, static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, address + 2U, static_cast<std::uint16_t>(value), kWordMask);
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_long(host, registers.address[7], value);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto value = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return value;
}

void set_logic_flags(CpuRegisters &registers, std::uint32_t value,
                     std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_btst(CpuRegisters &registers, bool bit_set)
{
    if (bit_set) registers.status = static_cast<std::uint16_t>(registers.status & ~0x0004U);
    else registers.status = static_cast<std::uint16_t>(registers.status | 0x0004U);
}

void set_add_word(CpuRegisters &registers, std::uint16_t left,
                  std::uint16_t right, std::uint16_t result)
{
    const auto wide = static_cast<std::uint32_t>(left) + right;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (wide > 0xffffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_sub_word(CpuRegisters &registers, std::uint16_t left,
                  std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (right > left) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

FunctionResult call_child(FunctionContext &context, std::uint32_t id,
                          std::uint32_t callsite, std::uint32_t target,
                          std::uint32_t return_address)
{
    auto &registers = context.registers;
    push_return(*context.host, registers, return_address);
    registers.program_counter = target;
    return context.host->call_function(id, 1U, 0x72U, 2U, callsite, target, context);
}
} // namespace

FunctionResult cpu_b_initialize_runtime_record_array(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;

    registers.address[0] = 0x00000d00U;
    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x003fU;
    set_logic_flags(registers, 0x003fU, 0x8000U, 0xffffU);
    auto d0 = std::uint16_t{0x003fU};
    for (;;) {
        (void)read_long(host, registers.address[0]);
        host.write_memory_word(kRegion, registers.address[0] + 2U, 0U, kWordMask);
        host.write_memory_word(kRegion, registers.address[0], 0U, kWordMask);
        registers.address[0] += 4U;
        set_logic_flags(registers, 0U, 0x80000000U, 0xffffffffU);
        d0 = static_cast<std::uint16_t>(d0 - 1U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
        if (d0 == 0xffffU) break;
    }

    registers.address[0] = 0x00000c08U;
    registers.address[1] = 0x0000ab98U;
    const auto input_403 = read_byte(host, 0x00000403U);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | input_403;
    set_logic_flags(registers, input_403, 0x80U, 0xffU);
    d0 = static_cast<std::uint16_t>(registers.data[0]) & 0x0060U;
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    set_logic_flags(registers, d0, 0x8000U, 0xffffU);
    const auto shifted = static_cast<std::uint16_t>(d0 >> 3U);
    const bool shift_carry = (d0 & 0x0004U) != 0U;
    d0 = shifted;
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    std::uint16_t shift_flags{};
    if (d0 == 0U) shift_flags |= 0x0004U;
    if (shift_carry) shift_flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | shift_flags);
    host.write_memory_word(kRegion, registers.address[0], 0x001eU, kWordMask);
    registers.address[0] += 2U;
    set_logic_flags(registers, 0x001eU, 0x8000U, 0xffffU);
    const auto selected_long = read_long(host, static_cast<std::uint32_t>(
        static_cast<std::int64_t>(registers.address[1]) + static_cast<std::int16_t>(d0)));
    write_long(host, registers.address[0], selected_long);
    registers.address[0] += 4U;
    set_logic_flags(registers, selected_long, 0x80000000U, 0xffffffffU);
    host.write_memory_word(kRegion, registers.address[0], 0x001eU, kWordMask);
    set_logic_flags(registers, 0x001eU, 0x8000U, 0xffffU);
    const auto input_403_btst = read_byte(host, 0x00000403U);
    set_btst(registers, (input_403_btst & 0x80U) != 0U);
    if ((input_403_btst & 0x80U) != 0U) {
        host.write_memory_word(kRegion, registers.address[0], 0x0018U, kWordMask);
        set_logic_flags(registers, 0x0018U, 0x8000U, 0xffffU);
    }

    host.write_memory_word(kRegion, 0x00000d00U, 0x000fU, kWordMask);
    set_logic_flags(registers, 0x000fU, 0x8000U, 0xffffU);
    write_long(host, 0x00000d02U, 0x00005000U);
    set_logic_flags(registers, 0x00005000U, 0x80000000U, 0xffffffffU);
    registers.address[0] = 0x0000abb0U;
    d0 = host.read_memory_word(kRegion, 0x00000c02U, kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
    set_logic_flags(registers, d0, 0x8000U, 0xffffU);
    const auto selected_byte = read_byte(host, static_cast<std::uint32_t>(
        static_cast<std::int64_t>(registers.address[0]) + static_cast<std::int16_t>(d0)));
    write_byte(host, 0x00000d0eU, selected_byte);
    set_logic_flags(registers, selected_byte, 0x80U, 0xffU);

    registers.address[0] = 0x00001600U;
    registers.data[0] = 4U;
    set_logic_flags(registers, 4U, 0x80000000U, 0xffffffffU);
    d0 = 4U;
    auto d1 = host.read_memory_word(kRegion, 0x00024836U, kWordMask);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | d1;
    set_logic_flags(registers, d1, 0x8000U, 0xffffU);
    auto next = static_cast<std::uint16_t>(d1 - 5U);
    set_sub_word(registers, d1, 5U, next);
    d1 = next;
    registers.data[1] = (registers.data[1] & 0xffff0000U) | d1;
    for (;;) {
        (void)host.read_memory_word(kRegion, registers.address[0], kWordMask);
        host.write_memory_word(kRegion, registers.address[0], 0U, kWordMask);
        set_logic_flags(registers, 0U, 0x8000U, 0xffffU);
        write_byte(host, registers.address[0] + 0x0aU, static_cast<std::uint8_t>(d0));
        set_logic_flags(registers, static_cast<std::uint8_t>(d0), 0x80U, 0xffU);
        next = static_cast<std::uint16_t>(d0 + 1U);
        set_add_word(registers, d0, 1U, next);
        d0 = next;
        registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
        registers.address[0] += 0x80U;
        d1 = static_cast<std::uint16_t>(d1 - 1U);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | d1;
        if (d1 == 0xffffU) break;
    }

    registers.address[1] = 0x00001480U;
    registers.address[0] = 0x00001600U;
    registers.data[2] = 2U;
    set_logic_flags(registers, 2U, 0x80000000U, 0xffffffffU);
    auto d2 = std::uint16_t{2U};
    for (;;) {
        registers.data[1] = 0U;
        set_logic_flags(registers, 0U, 0x80000000U, 0xffffffffU);
        d1 = 0U;
        registers.data[0] = 2U;
        set_logic_flags(registers, 2U, 0x80000000U, 0xffffffffU);
        d0 = 2U;
        for (;;) {
            write_long(host, registers.address[0] + 2U, 0x0001045cU);
            set_logic_flags(registers, 0x0001045cU, 0x80000000U, 0xffffffffU);
            write_byte(host, registers.address[0] + 0x0bU, 0U);
            set_logic_flags(registers, 0U, 0x80U, 0xffU);
            host.write_memory_word(kRegion, registers.address[0] + 0x36U,
                static_cast<std::uint16_t>(registers.address[1]), kWordMask);
            set_logic_flags(registers, static_cast<std::uint16_t>(registers.address[1]),
                            0x8000U, 0xffffU);
            host.write_memory_word(kRegion, registers.address[0] + 0x38U, d1, kWordMask);
            set_logic_flags(registers, d1, 0x8000U, 0xffffU);
            registers.address[0] += 0x80U;
            next = static_cast<std::uint16_t>(d1 + 6U);
            set_add_word(registers, d1, 6U, next);
            d1 = next;
            registers.data[1] = (registers.data[1] & 0xffff0000U) | d1;
            d0 = static_cast<std::uint16_t>(d0 - 1U);
            registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
            if (d0 == 0xffffU) break;
        }
        registers.address[1] += 0x80U;
        registers.address[0] += 0x400U;
        d2 = static_cast<std::uint16_t>(d2 - 1U);
        registers.data[2] = (registers.data[2] & 0xffff0000U) | d2;
        if (d2 == 0xffffU) break;
    }

    auto child = call_child(context, 266U, 0x0000a708U, 0x0001362eU, 0x0000a70eU);
    if (child.status != TranslationStatus::complete || child.control != 1U) return child;
    child = call_child(context, 277U, 0x0000a70eU, 0x00013e3eU, 0x0000a714U);
    if (child.status != TranslationStatus::complete || child.control != 1U) return child;
    child = call_child(context, 323U, 0x0000a714U, 0x0001b922U, 0x0000a71aU);
    if (child.status != TranslationStatus::complete || child.control != 1U) return child;

    const auto gate = read_byte(host, 0x00000820U);
    set_btst(registers, (gate & 0x08U) != 0U);
    if ((gate & 0x08U) != 0U) {
        registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x0052U;
        set_logic_flags(registers, 0x0052U, 0x8000U, 0xffffU);
        registers.data[1] = 0U;
        set_logic_flags(registers, 0U, 0x80000000U, 0xffffffffU);
        d1 = host.read_memory_word(kRegion, 0x00000c02U, kWordMask);
        registers.data[1] = d1;
        set_logic_flags(registers, d1, 0x8000U, 0xffffU);
        if (d1 != 0U) {
            const auto dividend = registers.data[1];
            const auto quotient = dividend / 10U;
            const auto remainder = dividend % 10U;
            registers.data[1] = (remainder << 16U) | static_cast<std::uint16_t>(quotient);
            set_logic_flags(registers, static_cast<std::uint16_t>(quotient), 0x8000U, 0xffffU);
            registers.data[1] = (registers.data[1] << 16U) | (registers.data[1] >> 16U);
            set_logic_flags(registers, registers.data[1], 0x80000000U, 0xffffffffU);
            registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x0056U;
            set_logic_flags(registers, 0x0056U, 0x8000U, 0xffffU);
            const auto remainder_word = static_cast<std::uint16_t>(registers.data[1]);
            set_logic_flags(registers, remainder_word, 0x8000U, 0xffffU);
            if (remainder_word != 0U) {
                registers.data[0] = 0x00000058U;
                set_logic_flags(registers, 0x00000058U, 0x80000000U, 0xffffffffU);
            }
        }
        child = call_child(context, 308U, 0x0000a73eU, 0x00016ff8U, 0x0000a744U);
        if (child.status != TranslationStatus::complete || child.control != 1U) return child;
        d0 = host.read_memory_word(kRegion, 0x00000c00U, kWordMask);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
        set_logic_flags(registers, d0, 0x8000U, 0xffffU);
        next = static_cast<std::uint16_t>(d0 + d0);
        set_add_word(registers, d0, d0, next);
        d0 = next;
        registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
        registers.address[0] = 0x0000aba8U;
        d0 = host.read_memory_word(kRegion, static_cast<std::uint32_t>(
            static_cast<std::int64_t>(registers.address[0]) + static_cast<std::int16_t>(d0)),
            kWordMask);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | d0;
        set_logic_flags(registers, d0, 0x8000U, 0xffffU);
        child = call_child(context, 310U, 0x0000a752U, 0x0001703aU, 0x0000a758U);
        if (child.status != TranslationStatus::complete || child.control != 1U) return child;
    }

    const auto final_gate = read_byte(host, 0x00000820U);
    set_btst(registers, (final_gate & 0x08U) != 0U);
    if ((final_gate & 0x08U) != 0U) {
        registers.data[0] = 0x00000014U;
        set_logic_flags(registers, 0x14U, 0x80000000U, 0xffffffffU);
        const auto c03 = read_byte(host, 0x00000c03U);
        set_btst(registers, (c03 & 0x01U) != 0U);
        if ((c03 & 0x01U) != 0U) {
            registers.data[0] = 0x00000015U;
            set_logic_flags(registers, 0x15U, 0x80000000U, 0xffffffffU);
        }
        const auto value = static_cast<std::uint8_t>(registers.data[0]);
        host.write_hardware(2U, 1U, 0x72U, 0x0000a76cU, 0x00d00034U,
            static_cast<std::uint16_t>(value * 0x0101U), 0x00ffU);
        set_logic_flags(registers, value, 0x80U, 0xffU);
    }

    const auto return_address = pop_return(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated

#include "cpu_b_record_flag1_gate_variant_detail.h"

#include <optional>

namespace gain_ground::translated {
namespace cpu_b_record_flag1_gate_variant_detail {

constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kStatusMask = 0x001fU;
constexpr std::uint16_t kNegativeBit = 0x0008U;
constexpr std::uint16_t kZeroBit = 0x0004U;
constexpr std::uint16_t kOverflowBit = 0x0002U;
constexpr std::uint16_t kCarryBit = 0x0001U;

[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & 0x00ffffffU, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t data)
{
    host.write_memory_word(kRegion, address & 0x00ffffffU, data, kWordMask);
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const bool shared_alias = (address & 0xffff0000U) == 0xffff0000U;
    const auto offset = shared_alias
        ? (0x00030000U | ((address & 0x0000ffffU) & ~1U))
        : ((address & ~1U) & 0x00ffffffU);
    const auto word = host.read_memory_word(shared_alias ? kSharedRegion : kRegion, offset, mask);
    return static_cast<std::uint8_t>(odd ? (word & 0x00ffU) : (word >> 8U));
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    host.write_memory_word(kRegion, address & ~1U,
        static_cast<std::uint16_t>(value) << static_cast<std::uint16_t>(odd ? 0U : 8U),
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U));
}

void bset_memory(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t address, unsigned bit)
{
    const auto value = read_byte(host, address);
    write_byte(host, address, static_cast<std::uint8_t>(value | (1U << bit)));
    registers.status = static_cast<std::uint16_t>((registers.status & ~kZeroBit)
        | (((value & (1U << bit)) == 0U) ? kZeroBit : 0U));
}

void bclr_memory(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t address, unsigned bit)
{
    const auto value = read_byte(host, address);
    write_byte(host, address, static_cast<std::uint8_t>(value & ~(1U << bit)));
    registers.status = static_cast<std::uint16_t>((registers.status & ~kZeroBit)
        | (((value & (1U << bit)) == 0U) ? kZeroBit : 0U));
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = read_word(host, address);
    const auto low = read_word(host, address + 2U);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_word(host, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U));
    write_word(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value));
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    registers.program_counter = target;
    return target;
}

void set_logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & kExtendBit;
    if ((value & 0x8000U) != 0U) flags |= kNegativeBit;
    if (value == 0U) flags |= kZeroBit;
    registers.status = static_cast<std::uint16_t>((registers.status & ~kStatusMask) | flags);
}

void set_logic_long(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & kExtendBit;
    if ((value & 0x80000000U) != 0U) flags |= kNegativeBit;
    if (value == 0U) flags |= kZeroBit;
    registers.status = static_cast<std::uint16_t>((registers.status & ~kStatusMask) | flags);
}

void set_swap_flags(CpuRegisters &registers, std::uint32_t value)
{
    set_logic_long(registers, value);
}

void commit_cc(CpuRegisters &registers, bool extend, const ConditionCodes &codes)
{
    std::uint16_t flags = extend ? kExtendBit : 0U;
    if (codes.negative) flags |= kNegativeBit;
    if (codes.zero) flags |= kZeroBit;
    if (codes.overflow) flags |= kOverflowBit;
    if (codes.carry) flags |= kCarryBit;
    registers.status = static_cast<std::uint16_t>((registers.status & ~kStatusMask) | flags);
}

void add_word(CpuRegisters &registers, std::uint16_t addend, std::uint16_t &destination,
              bool &extend)
{
    const std::uint32_t wide = static_cast<std::uint32_t>(destination) + addend;
    const auto result = static_cast<std::uint16_t>(wide);
    const bool carry = wide > 0xffffU;
    const bool overflow = ((~(destination ^ addend)) & (destination ^ result) & 0x8000U) != 0U;
    destination = result;
    extend = carry;
    ConditionCodes codes;
    codes.carry = carry;
    codes.overflow = overflow;
    codes.zero = result == 0U;
    codes.negative = (result & 0x8000U) != 0U;
    commit_cc(registers, extend, codes);
}

void sub_word_immediate(CpuRegisters &registers, std::uint16_t subtrahend,
                        std::uint16_t &destination, bool &extend)
{
    const auto result = static_cast<std::uint16_t>(destination - subtrahend);
    const bool carry = destination < subtrahend;
    const bool overflow = ((destination ^ subtrahend) & (destination ^ result) & 0x8000U) != 0U;
    destination = result;
    ConditionCodes codes;
    codes.carry = carry;
    codes.overflow = overflow;
    codes.zero = result == 0U;
    codes.negative = (result & 0x8000U) != 0U;
    commit_cc(registers, extend, codes);
}

void and_word(CpuRegisters &registers, std::uint16_t mask, std::uint16_t &destination)
{
    const auto result = static_cast<std::uint16_t>(destination & mask);
    destination = result;
    ConditionCodes codes;
    codes.carry = false;
    codes.overflow = false;
    codes.zero = result == 0U;
    codes.negative = (result & 0x8000U) != 0U;
    const bool extend = (registers.status & kExtendBit) != 0U;
    commit_cc(registers, extend, codes);
}

void lsl_word(CpuRegisters &registers, unsigned count, std::uint16_t &destination, bool &extend)
{
    std::uint16_t result = destination;
    bool last = false;
    for (unsigned index = 0; index < count && index < 16U; ++index) {
        last = (result & 0x8000U) != 0U;
        result = static_cast<std::uint16_t>(result << 1U);
    }
    destination = result;
    extend = last;
    ConditionCodes codes;
    codes.carry = false;
    codes.overflow = false;
    codes.zero = result == 0U;
    codes.negative = (result & 0x8000U) != 0U;
    commit_cc(registers, extend, codes);
}

[[nodiscard]] bool compare_signed_gt(std::uint16_t left, std::uint16_t right,
                                     ConditionCodes &codes)
{
    const auto difference = static_cast<std::uint16_t>(left - right);
    const bool carry = left < right;
    const bool overflow = ((left ^ right) & (left ^ difference) & 0x8000U) != 0U;
    codes.carry = carry;
    codes.overflow = overflow;
    codes.zero = difference == 0U;
    codes.negative = (difference & 0x8000U) != 0U;
    const std::int16_t signed_left = static_cast<std::int16_t>(left);
    const std::int16_t signed_right = static_cast<std::int16_t>(right);
    return signed_left > signed_right;
}

[[nodiscard]] bool test_bit_clear(std::uint8_t value, unsigned bit)
{
    return (value & (1U << bit)) == 0U;
}

[[nodiscard]] std::optional<FunctionResult> scan_record_corners(
    FunctionContext &context, std::uint32_t base, std::uint16_t &x,
    std::uint16_t &y, bool &extend, std::uint16_t &d3)
{
    auto &registers = context.registers;
    auto &host = *context.host;
    registers.data[3] = 0U;
    sub_word_immediate(registers, 0x000aU, x, extend);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | x;
    sub_word_immediate(registers, 0x0009U, y, extend);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | y;

    struct Corner {
        std::uint32_t callsite;
        std::uint32_t continuation;
        std::uint32_t addend;
    };
    constexpr Corner corners[4] = {
        {0x0001deb4U, 0x0001debaU, 1U},
        {0x0001dec8U, 0x0001deceU, 2U},
        {0x0001dedcU, 0x0001dee2U, 4U},
        {0x0001def0U, 0x0001def6U, 8U},
    };

    for (const Corner &corner : corners) {
        switch (corner.addend) {
            case 1U: break;
            case 2U:
                add_word(registers, 0x0012U, y, extend);
                registers.data[1] = (registers.data[1] & 0xffff0000U) | y;
                break;
            case 4U:
                add_word(registers, 0x0014U, x, extend);
                registers.data[0] = (registers.data[0] & 0xffff0000U) | x;
                break;
            default:
                sub_word_immediate(registers, 0x0012U, y, extend);
                registers.data[1] = (registers.data[1] & 0xffff0000U) | y;
                break;
        }
        push_return(host, registers, corner.continuation);
        registers.program_counter = 0x00015edeU;
        const auto child = host.call_function(287U, 1U, 0x72U, 2U,
            corner.callsite, 0x00015edeU, context);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
        const auto marker = read_byte(host, registers.address[0]);
        ConditionCodes codes;
        codes.carry = false;
        codes.overflow = false;
        codes.zero = (marker & 0x09U) == 0U;
        codes.negative = (marker & 0x80U) != 0U;
        const bool extend_now = (registers.status & kExtendBit) != 0U;
        commit_cc(registers, extend_now, codes);
        registers.data[4] = (registers.data[4] & 0xffff0000U)
            | (static_cast<std::uint16_t>(marker) & 0x0009U);
        if ((marker & 0x09U) != 0U) {
            d3 = static_cast<std::uint16_t>(d3 + corner.addend);
            registers.data[3] = (registers.data[3] & 0xffff0000U) | d3;
        }
    }
    (void)base;
    return std::nullopt;
}

} // namespace cpu_b_record_flag1_gate_variant_detail

} // namespace gain_ground::translated

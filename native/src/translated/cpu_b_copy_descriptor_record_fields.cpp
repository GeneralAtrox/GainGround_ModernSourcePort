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

[[nodiscard]] ByteLocation byte_location(std::uint32_t address)
{
    return {address & ~1U,
        static_cast<std::uint16_t>((address & 1U) == 0U ? 0xff00U : 0x00ffU),
        (address & 1U) == 0U ? 8U : 0U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = byte_location(address);
    return static_cast<std::uint8_t>(
        host.read_memory_word(kRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = byte_location(address);
    host.write_memory_word(kRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void set_move_flags(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t sign_mask, std::uint32_t value_mask)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign_mask) != 0U) flags |= 0x0008U;
    if ((value & value_mask) == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult cpu_b_copy_descriptor_record_fields(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    std::uint32_t source = context.registers.address[0];
    const auto destination = context.registers.address[6];

    const auto copy_word = [&](std::uint32_t destination_offset, bool increment) {
        const auto value = host.read_memory_word(kRegion, source, kWordMask);
        host.write_memory_word(
            kRegion, destination + destination_offset, value, kWordMask);
        if (increment)
            source += 2U;
        set_move_flags(context.registers, value, 0x8000U, 0xffffU);
    };
    const auto copy_byte = [&](std::uint32_t destination_offset) {
        const auto value = read_byte(host, source);
        write_byte(host, destination + destination_offset, value);
        source += 1U;
        set_move_flags(context.registers, value, 0x80U, 0xffU);
    };

    copy_word(0x06U, true);
    copy_word(0x08U, true);
    copy_byte(0x01U);
    copy_byte(0x0bU);
    copy_word(0x0cU, true);
    copy_word(0x0eU, false);
    copy_word(0x12U, true);
    copy_word(0x16U, true);
    copy_word(0x1aU, true);

    context.registers.address[0] = source;
    const auto return_address = read_long(host, context.registers.address[7]);
    context.registers.address[7] += 4U;
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated

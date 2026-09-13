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

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void set_move_word_flags(CpuRegisters &registers, std::uint16_t value,
    std::uint16_t extend)
{
    std::uint16_t flags = extend;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult cpu_b_materialize_record_index(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    const auto base = context.registers.address[5];
    const auto current = host.read_memory_word(kRegion, base + 0x52U, kWordMask);
    set_move_word_flags(context.registers, current, context.registers.status & 0x0010U);

    if ((current & 0x8000U) == 0U) {
        const auto record = host.read_memory_word(kRegion, base + 0x58U, kWordMask);
        const auto shifted = static_cast<std::uint16_t>(record << 2U);
        const auto addend = read_byte(host, base + 0x3dU);
        const auto left_byte = static_cast<std::uint8_t>(shifted);
        const std::uint16_t byte_sum = static_cast<std::uint16_t>(
            static_cast<std::uint16_t>(left_byte) + addend);
        const auto result = static_cast<std::uint16_t>(
            (shifted & 0xff00U) | static_cast<std::uint8_t>(byte_sum));

        std::uint16_t extend{};
        if (byte_sum > 0xffU)
            extend = 0x0010U;
        context.registers.data[0] =
            (context.registers.data[0] & 0xffff0000U) | result;
        host.write_memory_word(kRegion, base + 0x52U, result, kWordMask);
        set_move_word_flags(context.registers, result, extend);
    }

    const auto return_address = read_long(host, context.registers.address[7]);
    context.registers.address[7] += 4U;
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated

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

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto byte = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, byte.address, byte.mask) >> byte.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
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

[[nodiscard]] FunctionResult call_child(FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite,
    std::uint32_t target, std::uint32_t continuation)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    registers.address[7] -= 4U;
    host.write_memory_word(kRegion, registers.address[7],
        static_cast<std::uint16_t>(continuation >> 16U), kWordMask);
    host.write_memory_word(kRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(continuation), kWordMask);
    registers.program_counter = target;
    return host.call_function(function_id, 1U, 0x72U, 2U,
        callsite, target, context);
}

[[nodiscard]] bool complete(const FunctionResult &result) noexcept
{
    return result.status == TranslationStatus::complete && result.control == 1U;
}
} // namespace

FunctionResult cpu_b_run_secondary_record_update_pipeline(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr
        || context.registers.program_counter != 0x000213d2U)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto record = registers.address[5];
    const auto reload = read_byte(host, record + 0x3fU);
    logic_byte(registers, reload);
    if (reload != 0U) {
        const auto value = host.read_memory_word(
            kRegion, (record + 0x1aU) & 0x00ffffffU, kWordMask);
        logic_word(registers, value);
        if ((value & 0x8000U) != 0U) {
            write_byte(host, record + 0x3fU, 0U);
            logic_byte(registers, 0U);
        } else {
            registers.address[6] = 0x00003400U;
            const auto counter = read_byte(host, registers.address[6] + 0x24U);
            const auto incremented = static_cast<std::uint8_t>(counter + 1U);
            write_byte(host, registers.address[6] + 0x24U, incremented);
            add_byte(registers, counter, 1U, incremented);
        }
    }

    write_byte(host, record + 0x0bU, 9U);
    logic_byte(registers, 9U);
    FunctionResult result = call_child(context, 328U,
        0x000213f4U, 0x0001bf1cU, 0x000213f8U);
    if (!complete(result)) return result;
    result = call_child(context, 352U,
        0x000213f8U, 0x0001dbc4U, 0x000213fcU);
    if (!complete(result)) return result;
    result = call_child(context, 353U,
        0x000213fcU, 0x0001dbf0U, 0x00021400U);
    if (!complete(result)) return result;
    result = call_child(context, 355U,
        0x00021400U, 0x0001de30U, 0x00021404U);
    if (!complete(result)) return result;
    result = call_child(context, 356U,
        0x00021404U, 0x0001e0eeU, 0x00021408U);
    if (!complete(result)) return result;
    result = call_child(context, 357U,
        0x00021408U, 0x0001e124U, 0x0002140cU);
    if (!complete(result)) return result;
    result = call_child(context, 359U,
        0x0002140cU, 0x0001e180U, 0x00021410U);
    if (!complete(result)) return result;

    const auto high = host.read_memory_word(
        kRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(
        kRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated

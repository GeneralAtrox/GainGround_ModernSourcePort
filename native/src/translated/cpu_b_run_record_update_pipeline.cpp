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

[[nodiscard]] std::uint8_t read_byte(
    ExecutionHost &host, std::uint32_t address)
{
    const auto byte = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, byte.address, byte.mask) >> byte.shift);
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

[[nodiscard]] bool test_bit(CpuRegisters &registers,
    std::uint8_t value, unsigned bit) noexcept
{
    const bool set = (value & (1U << bit)) != 0U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x0004U) | (set ? 0U : 0x0004U));
    return set;
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
    const auto high = host.read_memory_word(
        kRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(
        kRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
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

FunctionResult cpu_b_run_record_update_pipeline(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr
        || context.registers.program_counter != 0x000210b6U)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto record = registers.address[5];

    const bool alternate = test_bit(registers,
        read_byte(host, record + 0x41U), 3U);
    const auto reload = read_byte(host, record + 0x3fU);
    logic_byte(registers, reload);
    if (reload != 0U) {
        registers.address[6] = 0x00003400U;
        const auto address = registers.address[6] + 0x22U;
        const auto before = read_byte(host, address);
        const auto after = static_cast<std::uint8_t>(before + 1U);
        write_byte(host, address, after);
        add_byte(registers, before, 1U, after);
    }

    FunctionResult result{};
    if (alternate) {
        result = call_child(context, 334U,
            0x000210ccU, 0x0001cef6U, 0x000210d0U);
        if (!complete(result)) return result;
        result = call_child(context, 352U,
            0x000210d0U, 0x0001dbc4U, 0x000210d4U);
        if (!complete(result)) return result;
        result = call_child(context, 353U,
            0x000210d4U, 0x0001dbf0U, 0x000210d8U);
        if (!complete(result)) return result;
        result = call_child(context, 357U,
            0x000210d8U, 0x0001e124U, 0x000210dcU);
        if (!complete(result)) return result;
        result = call_child(context, 360U,
            0x000210dcU, 0x0001e304U, 0x000210e0U);
        if (!complete(result)) return result;
        result = call_child(context, 365U,
            0x000210e0U, 0x0001ece4U, 0x000210e4U);
        if (!complete(result)) return result;
    } else {
        result = call_child(context, 329U,
            0x000210f4U, 0x0001c108U, 0x000210f8U);
        if (!complete(result)) return result;
        result = call_child(context, 368U,
            0x000210f8U, 0x0001efdcU, 0x000210fcU);
        if (!complete(result)) return result;
        result = call_child(context, 352U,
            0x000210fcU, 0x0001dbc4U, 0x00021100U);
        if (!complete(result)) return result;
        result = call_child(context, 353U,
            0x00021100U, 0x0001dbf0U, 0x00021104U);
        if (!complete(result)) return result;
        result = call_child(context, 355U,
            0x00021104U, 0x0001de30U, 0x00021108U);
        if (!complete(result)) return result;
        result = call_child(context, 356U,
            0x00021108U, 0x0001e0eeU, 0x0002110cU);
        if (!complete(result)) return result;
        result = call_child(context, 357U,
            0x0002110cU, 0x0001e124U, 0x00021110U);
        if (!complete(result)) return result;
        result = call_child(context, 360U,
            0x00021110U, 0x0001e304U, 0x00021114U);
        if (!complete(result)) return result;
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated

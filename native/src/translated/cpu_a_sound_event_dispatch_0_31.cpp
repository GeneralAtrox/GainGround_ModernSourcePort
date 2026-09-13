#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {

namespace {

constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kFullWordMask = 0xffffU;
constexpr std::uint32_t kSharedAddressMask = 0x0003ffffU;

[[maybe_unused]] void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(
        kSharedRegion, address & kSharedAddressMask, kFullWordMask);
}

[[maybe_unused]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(
        kSharedRegion, address & kSharedAddressMask, kFullWordMask);
}

[[maybe_unused]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

[[maybe_unused]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto offset = address & kSharedAddressMask;
    const auto mask = static_cast<std::uint16_t>(
        (offset & 1U) == 0U ? 0xff00U : 0x00ffU);
    const auto shift = (offset & 1U) == 0U ? 8U : 0U;
    return static_cast<std::uint8_t>(host.read_memory_word(
        kSharedRegion, offset & ~1U, mask) >> shift);
}

[[maybe_unused]] void write_word(
    ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(
        kSharedRegion, address & kSharedAddressMask, value, kFullWordMask);
}

[[maybe_unused]] void write_long(
    ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
}

[[maybe_unused]] void write_byte(
    ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto offset = address & kSharedAddressMask;
    const auto even = (offset & 1U) == 0U;
    host.write_memory_word(kSharedRegion, offset & ~1U,
        static_cast<std::uint16_t>(even ? static_cast<unsigned>(value) << 8U : value),
        static_cast<std::uint16_t>(even ? 0xff00U : 0x00ffU));
}

[[maybe_unused]] void set_logic_flags(
    CpuRegisters &registers, std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[maybe_unused]] void set_add_byte_flags(
    CpuRegisters &registers, std::uint8_t left, std::uint8_t right,
    std::uint8_t result)
{
    const auto sum = static_cast<std::uint16_t>(
        static_cast<std::uint16_t>(left) + right);
    std::uint16_t flags{};
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if (sum > 0xffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[maybe_unused]] void set_add_word_flags(
    CpuRegisters &registers, std::uint16_t left, std::uint16_t right,
    std::uint16_t result)
{
    const auto sum = static_cast<std::uint32_t>(left) + right;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (sum > 0xffffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[maybe_unused]] void set_sub_byte_flags(
    CpuRegisters &registers, std::uint8_t left, std::uint8_t right,
    std::uint8_t result)
{
    std::uint16_t flags{};
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if (right > left) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[maybe_unused]] void set_compare_byte_flags(
    CpuRegisters &registers, std::uint8_t left, std::uint8_t right)
{
    const auto result = static_cast<std::uint8_t>(left - right);
    std::uint16_t flags = registers.status & 0x0010U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if (right > left) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[maybe_unused]] void left_shift_word_eight(CpuRegisters &registers)
{
    const auto prior = static_cast<std::uint16_t>(registers.data[0]);
    const auto result = static_cast<std::uint16_t>(prior << 8U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if ((prior & 0x0100U) != 0U) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[maybe_unused]] void move_stream_byte_to_data(
    ExecutionHost &host, CpuRegisters &registers, std::size_t data_register)
{
    const auto value = read_byte(host, registers.address[4]++);
    registers.data[data_register] =
        (registers.data[data_register] & 0xffffff00U) | value;
    set_logic_flags(registers, value, 0x80U);
}

[[maybe_unused]] FunctionResult return_from_subroutine(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto stack = registers.address[7];
    const auto target = (static_cast<std::uint32_t>(read_word(host, stack)) << 16U)
        | read_word(host, stack + 2U);
    registers.address[7] += 4U;
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

[[maybe_unused]] void push_return(
    ExecutionHost &host, CpuRegisters &registers, std::uint32_t address)
{
    registers.address[7] -= 4U;
    write_long(host, registers.address[7], address);
}

[[maybe_unused]] FunctionResult call_ym2151_writer(
    FunctionContext &context, std::uint32_t callsite, std::uint32_t return_address)
{
    auto &host = *context.host;
    push_return(host, context.registers, return_address);
    prefetch(host, 0x00084126U);
    prefetch(host, 0x00084128U);
    context.registers.program_counter = 0x00084126U;
    return host.call_function(88U, 0U, 0xffU, 2U,
        callsite, 0x00084126U, context);
}

[[maybe_unused]] FunctionResult call_native(
    FunctionContext &context, std::uint32_t function_id,
    std::uint32_t callsite, std::uint32_t target,
    std::uint32_t return_address)
{
    auto &host = *context.host;
    push_return(host, context.registers, return_address);
    prefetch(host, target);
    prefetch(host, target + 2U);
    context.registers.program_counter = target;
    return host.call_function(function_id, 0U, 0xffU, 2U,
        callsite, target, context);
}

[[maybe_unused]] void move_stream_byte_to_channel(
    ExecutionHost &host, CpuRegisters &registers, std::uint32_t offset)
{
    const auto value = read_byte(host, registers.address[4]++);
    write_byte(host, registers.address[3] + offset, value);
    set_logic_flags(registers, value, 0x80U);
}

[[maybe_unused]] void modify_channel_bit(
    ExecutionHost &host, CpuRegisters &registers, std::uint32_t offset,
    std::uint8_t bit, bool set)
{
    const auto address = registers.address[3] + offset;
    const auto prior = read_byte(host, address);
    const auto mask = static_cast<std::uint8_t>(1U << bit);
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
        | ((prior & mask) == 0U ? 0x0004U : 0U));
    const auto result = static_cast<std::uint8_t>(set ? prior | mask : prior & ~mask);
    write_byte(host, address, result);
}

} // namespace

FunctionResult cpu_a_sound_event_dispatch_0_31(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    auto value = static_cast<std::uint16_t>(registers.data[0]) & 0x001fU;
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    set_logic_flags(registers, value, 0x8000U);

    prefetch(host, 0x00083648U);
    auto doubled = static_cast<std::uint16_t>(value + value);
    set_add_word_flags(registers, value, value, doubled);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | doubled;

    prefetch(host, 0x0008364aU);
    value = doubled;
    doubled = static_cast<std::uint16_t>(value + value);
    set_add_word_flags(registers, value, value, doubled);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | doubled;

    prefetch(host, 0x0008364cU);
    prefetch(host, 0x0008364eU);
    const auto selector = static_cast<std::uint8_t>(doubled >> 2U);
    const auto slot = 0x00083650U + doubled;
    prefetch(host, slot);
    prefetch(host, slot + 2U);

    switch (selector) {
    case 0: {
        prefetch(host, 0x000836d0U);
        prefetch(host, 0x000836d2U);
        const auto stream = read_byte(host, registers.address[4]++);
        set_logic_flags(registers, stream, 0x80U);
        prefetch(host, 0x000836d4U);
        return return_from_subroutine(host, registers);
    }
    case 2:
    case 5:
    case 15:
    case 16:
    case 18:
    case 28:
    case 29:
    case 30:
    case 31:
        prefetch(host, 0x000836d4U);
        prefetch(host, 0x000836d6U);
        return return_from_subroutine(host, registers);
    case 1:
        prefetch(host, 0x000837a4U);
        prefetch(host, 0x000837a6U);
        value = read_byte(host, registers.address[4]++);
        prefetch(host, 0x000837a8U);
        write_byte(host, registers.address[3] + 3U,
            static_cast<std::uint8_t>(value));
        set_logic_flags(registers, value, 0x80U);
        prefetch(host, 0x000837aaU);
        return return_from_subroutine(host, registers);
    case 3:
        prefetch(host, 0x000837aaU);
        prefetch(host, 0x000837acU);
        value = read_byte(host, registers.address[4]++);
        prefetch(host, 0x000837aeU);
        write_byte(host, registers.address[3] + 13U,
            static_cast<std::uint8_t>(value));
        set_logic_flags(registers, value, 0x80U);
        registers.address[4] -= registers.address[5];
        prefetch(host, 0x000837b0U);
        prefetch(host, 0x000837b2U);
        prefetch(host, 0x000837b4U);
        write_long(host, registers.address[3] + 4U, registers.address[4]);
        set_logic_flags(registers, registers.address[4], 0x80000000U);
        registers.address[4] += registers.address[5];
        prefetch(host, 0x000837b6U);
        prefetch(host, 0x000837b8U);
        prefetch(host, 0x000837baU);
        prefetch(host, 0x000837bcU);
        write_byte(host, registers.address[3] + 15U, 0U);
        set_logic_flags(registers, 0U, 0x80U);
        prefetch(host, 0x000837beU);
        prefetch(host, 0x000837c0U);
        prefetch(host, 0x000837c2U);
        write_byte(host, registers.address[3] + 14U, 0U);
        set_logic_flags(registers, 0U, 0x80U);
        prefetch(host, 0x000837c4U);
        return return_from_subroutine(host, registers);
    case 4: {
        prefetch(host, 0x000837c4U);
        prefetch(host, 0x000837c6U);
        const auto raw_secondary = read_byte(host, registers.address[4]++);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | raw_secondary;
        set_logic_flags(registers, raw_secondary, 0x80U);
        prefetch(host, 0x000837c8U);
        auto secondary = static_cast<std::uint16_t>(registers.data[0]) & 0x000fU;
        registers.data[0] = (registers.data[0] & 0xffff0000U) | secondary;
        set_logic_flags(registers, secondary, 0x8000U);
        prefetch(host, 0x000837caU);
        auto shifted = static_cast<std::uint16_t>(secondary + secondary);
        set_add_word_flags(registers, secondary, secondary, shifted);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | shifted;
        prefetch(host, 0x000837ccU);
        secondary = shifted;
        shifted = static_cast<std::uint16_t>(secondary + secondary);
        set_add_word_flags(registers, secondary, secondary, shifted);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | shifted;
        prefetch(host, 0x000837ceU);
        prefetch(host, 0x000837d0U);
        const auto table_slot = 0x000837d2U + shifted;
        prefetch(host, table_slot);
        prefetch(host, table_slot + 2U);

        const auto subselector = static_cast<std::uint8_t>(shifted >> 2U);
        std::uint32_t target{};
        if (subselector == 0U || subselector == 12U) target = 0x00083802U;
        else if (subselector == 1U) target = 0x0008380cU;
        else if (subselector == 2U) target = 0x00083816U;
        else if (subselector == 3U) target = 0x00083820U;
        else if (subselector == 4U) target = 0x0008382aU;
        else if (subselector == 5U) target = 0x00083834U;
        else if (subselector == 6U) target = 0x0008383eU;
        else if (subselector == 7U) target = 0x00083848U;
        else if (subselector == 8U) target = 0x00083852U;
        else if (subselector == 9U) target = 0x0008385cU;
        else if (subselector == 10U) target = 0x00083866U;
        else if (subselector == 11U) target = 0x0008386cU;
        else return FunctionResult::unimplemented();
        prefetch(host, target);
        prefetch(host, target + 2U);

        const auto control_address = registers.address[6] + 0x32U;
        if (subselector == 0U || subselector == 12U) {
            prefetch(host, 0x00083806U);
            prefetch(host, 0x00083808U);
            write_word(host, control_address, 0U);
            set_logic_flags(registers, 0U, 0x8000U);
            prefetch(host, 0x0008380aU);
        } else if (subselector >= 1U && subselector <= 5U) {
            prefetch(host, target + 4U);
            prefetch(host, target + 6U);
            const auto prior = read_word(host, control_address);
            prefetch(host, target + 8U);
            const auto result = static_cast<std::uint16_t>(prior & 0x07ffU);
            write_word(host, control_address, result);
            set_logic_flags(registers, result, 0x8000U);
        } else if (subselector >= 6U && subselector <= 9U) {
            prefetch(host, target + 4U);
            prefetch(host, target + 6U);
            const auto prior = read_word(host, control_address);
            prefetch(host, target + 8U);
            const auto result = static_cast<std::uint16_t>(prior & 0xf800U);
            write_word(host, control_address, result);
            set_logic_flags(registers, result, 0x8000U);
        } else if (subselector == 10U) {
            prefetch(host, 0x0008386aU);
            const auto prior = read_word(host, control_address);
            const auto result = static_cast<std::uint16_t>(prior & 0xfffeU);
            write_word(host, control_address, result);
            set_logic_flags(registers, result, 0x8000U);
        }

        prefetch(host, 0x0008386cU);
        prefetch(host, 0x0008386eU);
        prefetch(host, 0x00083870U);
        prefetch(host, 0x00083872U);
        const auto channel_address = registers.address[3];
        const auto channel_flags = read_byte(host, channel_address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((channel_flags & 0x80U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x00083874U);
        write_byte(host, channel_address,
            static_cast<std::uint8_t>(channel_flags & 0x7fU));
        prefetch(host, 0x00083876U);
        prefetch(host, 0x00083878U);
        write_byte(host, registers.address[3] + 15U, 0U);
        set_logic_flags(registers, 0U, 0x80U);
        prefetch(host, 0x0008387aU);

        auto child = call_native(context, 96U,
            0x00083878U, 0x000842c0U, 0x0008387cU);
        if (child.status != TranslationStatus::complete) return child;
        child = call_native(context, 90U,
            0x0008387cU, 0x00084154U, 0x00083880U);
        if (child.status != TranslationStatus::complete) return child;
        prefetch(host, 0x00083884U);
        prefetch(host, 0x00083886U);
        const auto mode = read_byte(host, registers.address[3] + 2U);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((mode & 0x08U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x00083888U);
        if ((mode & 0x08U) == 0U) {
            prefetch(host, 0x000834aeU);
            prefetch(host, 0x000834b0U);
            return return_from_subroutine(host, registers);
        }
        prefetch(host, 0x0008388aU);
        prefetch(host, 0x0008388cU);
        child = call_native(context, 97U,
            0x0008388aU, 0x000842e4U, 0x0008388eU);
        if (child.status != TranslationStatus::complete) return child;
        registers.data[0] = (registers.data[0] & 0xffff0000U) | 0xfce0U;
        set_logic_flags(registers, 0xfce0U, 0x8000U);
        prefetch(host, 0x00083892U);
        prefetch(host, 0x00083894U);
        prefetch(host, 0x00083896U);
        prefetch(host, 0x00083898U);
        const auto displacement = static_cast<std::int16_t>(registers.data[0]);
        const auto peer_address = registers.address[3]
            + static_cast<std::uint32_t>(static_cast<std::int32_t>(displacement));
        const auto peer_flags = read_byte(host, peer_address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((peer_flags & 0x80U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x0008389aU);
        if ((peer_flags & 0x80U) == 0U) {
            prefetch(host, 0x000834aeU);
            prefetch(host, 0x000834b0U);
            return return_from_subroutine(host, registers);
        }
        prefetch(host, 0x0008389cU);
        prefetch(host, 0x0008389eU);
        prefetch(host, 0x000838a0U);
        prefetch(host, 0x000838a2U);
        const auto peer_prior = read_byte(host, peer_address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((peer_prior & 0x04U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x000838a4U);
        write_byte(host, peer_address,
            static_cast<std::uint8_t>(peer_prior & 0xfbU));
        prefetch(host, 0x000838a6U);
        set_compare_byte_flags(registers,
            static_cast<std::uint8_t>(registers.data[7]), 6U);
        prefetch(host, 0x000838a8U);
        if (static_cast<std::uint8_t>(registers.data[7]) < 6U) {
            prefetch(host, 0x000838b4U);
            prefetch(host, 0x000838b6U);
            registers.address[3] -= 0x320U;
            prefetch(host, 0x000838b8U);
            prefetch(host, 0x000838baU);
            child = call_native(context, 92U,
                0x000838b8U, 0x00084164U, 0x000838bcU);
            if (child.status != TranslationStatus::complete) return child;
            registers.address[3] += 0x320U;
            prefetch(host, 0x000838c0U);
            prefetch(host, 0x000838c2U);
            return return_from_subroutine(host, registers);
        }
        registers.data[0] = (registers.data[0] & 0xffff0000U)
            | static_cast<std::uint16_t>(registers.data[0] + 0x00a0U);
        prefetch(host, 0x000838acU);
        const auto adjusted = registers.address[3]
            + static_cast<std::uint32_t>(static_cast<std::int32_t>(
                static_cast<std::int16_t>(registers.data[0])));
        const auto adjusted_prior = read_byte(host, adjusted);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((adjusted_prior & 0x04U) == 0U ? 0x0004U : 0U));
        write_byte(host, adjusted,
            static_cast<std::uint8_t>(adjusted_prior & 0xfbU));
        prefetch(host, 0x000838b2U);
        return return_from_subroutine(host, registers);
    }
    case 6:
        prefetch(host, 0x00083744U);
        prefetch(host, 0x00083746U);
        value = read_byte(host, registers.address[4]++);
        prefetch(host, 0x00083748U);
        write_byte(host, registers.address[3] + 10U,
            static_cast<std::uint8_t>(value));
        set_logic_flags(registers, value, 0x80U);
        prefetch(host, 0x0008374aU);
        return return_from_subroutine(host, registers);
    case 7:
        prefetch(host, 0x0008374aU);
        prefetch(host, 0x0008374cU);
        value = read_byte(host, registers.address[4]++);
        prefetch(host, 0x0008374eU);
        write_byte(host, registers.address[3] + 9U,
            static_cast<std::uint8_t>(value));
        set_logic_flags(registers, value, 0x80U);
        prefetch(host, 0x00083750U);
        return return_from_subroutine(host, registers);
    case 8: {
        prefetch(host, 0x000838c2U);
        registers.data[0] = 0U;
        set_logic_flags(registers, 0U, 0x80000000U);
        prefetch(host, 0x000838c4U);
        registers.data[1] = registers.data[0];
        set_logic_flags(registers, registers.data[1], 0x80000000U);
        prefetch(host, 0x000838c6U);
        prefetch(host, 0x000838c8U);
        move_stream_byte_to_data(host, registers, 0U);
        left_shift_word_eight(registers);
        prefetch(host, 0x000838caU);
        prefetch(host, 0x000838ccU);
        move_stream_byte_to_data(host, registers, 0U);
        prefetch(host, 0x000838ceU);
        prefetch(host, 0x000838d0U);
        const auto prior = read_byte(host, registers.address[3] + 12U);
        const auto result = static_cast<std::uint8_t>(prior - 4U);
        prefetch(host, 0x000838d2U);
        write_byte(host, registers.address[3] + 12U, result);
        set_sub_byte_flags(registers, prior, 4U, result);
        prefetch(host, 0x000838d4U);
        const auto index = read_byte(host, registers.address[3] + 12U);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | index;
        set_logic_flags(registers, index, 0x80U);
        prefetch(host, 0x000838d6U);
        registers.data[1] &= 0xffff00ffU;
        set_logic_flags(registers, registers.data[1] & 0xffffU, 0x8000U);
        prefetch(host, 0x000838d8U);
        prefetch(host, 0x000838daU);
        prefetch(host, 0x000838dcU);
        write_long(host, registers.address[3] + index, registers.address[4]);
        set_logic_flags(registers, registers.address[4], 0x80000000U);
        registers.address[4] += static_cast<std::uint32_t>(
            static_cast<std::int32_t>(static_cast<std::int16_t>(registers.data[0])));
        prefetch(host, 0x000838deU);
        prefetch(host, 0x000838e0U);
        return return_from_subroutine(host, registers);
    }
    case 9: {
        prefetch(host, 0x000838e0U);
        prefetch(host, 0x000838e2U);
        prefetch(host, 0x000838e4U);
        const auto index = read_byte(host, registers.address[3] + 12U);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | index;
        set_logic_flags(registers, index, 0x80U);
        prefetch(host, 0x000838e6U);
        set_logic_flags(registers, index, 0x80U);
        prefetch(host, 0x000838e8U);
        prefetch(host, 0x000838eaU);
        prefetch(host, 0x000838ecU);
        registers.address[4] = read_long(host, registers.address[3] + index);
        prefetch(host, 0x000838eeU);
        prefetch(host, 0x000838f0U);
        const auto prior = read_byte(host, registers.address[3] + 12U);
        const auto result = static_cast<std::uint8_t>(prior + 4U);
        prefetch(host, 0x000838f2U);
        write_byte(host, registers.address[3] + 12U, result);
        set_add_byte_flags(registers, prior, 4U, result);
        return return_from_subroutine(host, registers);
    }
    case 10:
        prefetch(host, 0x000838f2U);
        prefetch(host, 0x000838f4U);
        move_stream_byte_to_data(host, registers, 0U);
        left_shift_word_eight(registers);
        prefetch(host, 0x000838f6U);
        prefetch(host, 0x000838f8U);
        move_stream_byte_to_data(host, registers, 0U);
        registers.address[4] += static_cast<std::uint32_t>(
            static_cast<std::int32_t>(static_cast<std::int16_t>(registers.data[0])));
        prefetch(host, 0x000838faU);
        prefetch(host, 0x000838fcU);
        return return_from_subroutine(host, registers);
    case 11: {
        prefetch(host, 0x000838fcU);
        prefetch(host, 0x000838feU);
        move_stream_byte_to_data(host, registers, 0U);
        prefetch(host, 0x00083900U);
        prefetch(host, 0x00083902U);
        const auto prior = read_byte(host, registers.address[3] + 8U);
        const auto addend = static_cast<std::uint8_t>(registers.data[0]);
        const auto result = static_cast<std::uint8_t>(prior + addend);
        prefetch(host, 0x00083904U);
        write_byte(host, registers.address[3] + 8U, result);
        set_add_byte_flags(registers, prior, addend, result);
        return return_from_subroutine(host, registers);
    }
    case 12: {
        prefetch(host, 0x00083904U);
        registers.data[0] = 0U;
        set_logic_flags(registers, 0U, 0x80000000U);
        prefetch(host, 0x00083906U);
        prefetch(host, 0x00083908U);
        move_stream_byte_to_data(host, registers, 0U);
        prefetch(host, 0x0008390aU);
        prefetch(host, 0x0008390cU);
        const auto channel_address = registers.address[3] + 64U
            + static_cast<std::uint8_t>(registers.data[0]);
        const auto channel_value = read_byte(host, channel_address);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | channel_value;
        set_logic_flags(registers, channel_value, 0x80U);
        prefetch(host, 0x0008390eU);
        move_stream_byte_to_data(host, registers, 2U);
        const auto incremented = static_cast<std::uint8_t>(channel_value + 1U);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | incremented;
        set_add_byte_flags(registers, channel_value, 1U, incremented);
        prefetch(host, 0x00083910U);
        set_compare_byte_flags(registers,
            static_cast<std::uint8_t>(registers.data[2]), incremented);
        prefetch(host, 0x00083912U);
        prefetch(host, 0x00083914U);
        if (static_cast<std::uint8_t>(registers.data[2]) == incremented) {
            prefetch(host, 0x0008391cU);
            prefetch(host, 0x0008391eU);
            prefetch(host, 0x00083920U);
            prefetch(host, 0x00083922U);
            write_byte(host, channel_address, 0U);
            set_logic_flags(registers, 0U, 0x80U);
            registers.address[4] += 2U;
            prefetch(host, 0x00083924U);
            prefetch(host, 0x00083926U);
            return return_from_subroutine(host, registers);
        }

        prefetch(host, 0x00083916U);
        prefetch(host, 0x00083918U);
        prefetch(host, 0x0008391aU);
        write_byte(host, channel_address, incremented);
        set_logic_flags(registers, incremented, 0x80U);
        prefetch(host, 0x0008391cU);
        prefetch(host, 0x000838f2U);
        prefetch(host, 0x000838f4U);
        move_stream_byte_to_data(host, registers, 0U);
        left_shift_word_eight(registers);
        prefetch(host, 0x000838f6U);
        prefetch(host, 0x000838f8U);
        move_stream_byte_to_data(host, registers, 0U);
        registers.address[4] += static_cast<std::uint32_t>(
            static_cast<std::int32_t>(static_cast<std::int16_t>(registers.data[0])));
        prefetch(host, 0x000838faU);
        prefetch(host, 0x000838fcU);
        return return_from_subroutine(host, registers);
    }
    case 13: {
        prefetch(host, 0x0008392eU);
        prefetch(host, 0x00083930U);
        prefetch(host, 0x00083932U);
        prefetch(host, 0x00083934U);
        const auto address = registers.address[3] + 1U;
        const auto prior = read_byte(host, address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((prior & 0x10U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x00083936U);
        write_byte(host, address, static_cast<std::uint8_t>(prior | 0x10U));
        return return_from_subroutine(host, registers);
    }
    case 14: {
        prefetch(host, 0x00083936U);
        prefetch(host, 0x00083938U);
        prefetch(host, 0x0008393aU);
        prefetch(host, 0x0008393cU);
        const auto first_address = registers.address[3];
        const auto first_prior = read_byte(host, first_address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((first_prior & 0x10U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x0008393eU);
        write_byte(host, first_address,
            static_cast<std::uint8_t>(first_prior & 0xefU));
        prefetch(host, 0x00083940U);
        prefetch(host, 0x00083942U);
        const auto second_address = registers.address[3] + 1U;
        const auto second_prior = read_byte(host, second_address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((second_prior & 0x10U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x00083944U);
        write_byte(host, second_address,
            static_cast<std::uint8_t>(second_prior & 0xefU));
        return return_from_subroutine(host, registers);
    }
    case 17: {
        prefetch(host, 0x0008395eU);
        registers.data[0] = 0U;
        set_logic_flags(registers, 0U, 0x80000000U);
        prefetch(host, 0x00083960U);
        prefetch(host, 0x00083962U);
        const auto event = read_byte(host, registers.address[4]++);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | event;
        set_logic_flags(registers, event, 0x80U);
        if (static_cast<std::uint8_t>(registers.data[0]) == 0U) {
            prefetch(host, 0x00083964U);
            prefetch(host, 0x000834aeU);
            prefetch(host, 0x000834b0U);
            return return_from_subroutine(host, registers);
        }
        prefetch(host, 0x00083964U);
        prefetch(host, 0x00083966U);
        prefetch(host, 0x00083968U);
        prefetch(host, 0x0008396aU);
        write_byte(host, registers.address[3] + 0x16U,
            static_cast<std::uint8_t>(registers.data[0]));
        set_logic_flags(registers,
            static_cast<std::uint8_t>(registers.data[0]), 0x80U);
        prefetch(host, 0x0008396cU);
        prefetch(host, 0x0008396eU);
        prefetch(host, 0x00083970U);
        const auto channel_flags = read_byte(host, registers.address[3]);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((channel_flags & 0x04U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x00083972U);
        if ((channel_flags & 0x04U) != 0U) {
            prefetch(host, 0x000834aeU);
            prefetch(host, 0x000834b0U);
            return return_from_subroutine(host, registers);
        }

        prefetch(host, 0x00083974U);
        prefetch(host, 0x00083976U);
        auto child = call_native(context, 92U,
            0x00083974U, 0x00084164U, 0x00083978U);
        if (child.status != TranslationStatus::complete) return child;
        prefetch(host, 0x0008397cU);
        prefetch(host, 0x0008397eU);
        const auto mode_flags = read_byte(host, registers.address[3] + 2U);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((mode_flags & 0x08U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x00083980U);
        if ((mode_flags & 0x08U) != 0U) {
            prefetch(host, 0x000834aeU);
            prefetch(host, 0x000834b0U);
            return return_from_subroutine(host, registers);
        }
        prefetch(host, 0x00083982U);
        prefetch(host, 0x00083984U);
        set_compare_byte_flags(registers,
            static_cast<std::uint8_t>(registers.data[7]), 4U);
        prefetch(host, 0x00083986U);
        prefetch(host, 0x00083988U);
        if (static_cast<std::uint8_t>(registers.data[7]) >= 4U) {
            prefetch(host, 0x000834aeU);
            prefetch(host, 0x000834b0U);
            return return_from_subroutine(host, registers);
        }

        prefetch(host, 0x0008398aU);
        registers.data[0] = 0x10U;
        set_logic_flags(registers, 0x10U, 0x80000000U);
        prefetch(host, 0x0008398cU);
        registers.data[1] = 0U;
        set_logic_flags(registers, 0U, 0x80000000U);
        prefetch(host, 0x0008398eU);
        const auto channel = static_cast<std::uint8_t>(registers.data[7]);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | channel;
        set_logic_flags(registers, channel, 0x80U);
        for (;;) {
            prefetch(host, 0x00083990U);
            prefetch(host, 0x00083992U);
            const auto prior = static_cast<std::uint8_t>(registers.data[0]);
            const auto result = static_cast<std::uint8_t>(prior - 4U);
            registers.data[0] = (registers.data[0] & 0xffffff00U) | result;
            set_sub_byte_flags(registers, prior, 4U, result);
            prefetch(host, 0x00083994U);
            prefetch(host, 0x00083996U);
            const auto counter = static_cast<std::uint16_t>(registers.data[1]);
            registers.data[1] = (registers.data[1] & 0xffff0000U)
                | static_cast<std::uint16_t>(counter - 1U);
            if (counter == 0U) {
                prefetch(host, 0x00083990U);
                break;
            }
        }
        prefetch(host, 0x00083998U);
        prefetch(host, 0x0008399aU);
        registers.address[1] = registers.address[6]
            + 0x30U + static_cast<std::uint32_t>(
                static_cast<std::int32_t>(
                    static_cast<std::int16_t>(registers.data[0])));
        prefetch(host, 0x0008399cU);
        prefetch(host, 0x0008399eU);
        prefetch(host, 0x000839a0U);
        const auto modulation = read_byte(host, registers.address[1] + 0x13U);
        registers.data[2] = (registers.data[2] & 0xffffff00U) | modulation;
        set_logic_flags(registers, modulation, 0x80U);
        prefetch(host, 0x000839a2U);
        child = call_native(context, 94U,
            0x000839a0U, 0x0008421aU, 0x000839a4U);
        if (child.status != TranslationStatus::complete) return child;
        return return_from_subroutine(host, registers);
    }
    case 19: {
        prefetch(host, 0x000836d6U);
        prefetch(host, 0x000836d8U);
        auto stream = read_byte(host, registers.address[4]++);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | stream;
        set_logic_flags(registers, stream, 0x80U);
        const auto prior = static_cast<std::uint8_t>(registers.data[0]);
        const auto addend = static_cast<std::uint8_t>(registers.data[7]);
        const auto result = static_cast<std::uint8_t>(prior + addend);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | result;
        set_add_byte_flags(registers, prior, addend, result);
        prefetch(host, 0x000836daU);
        prefetch(host, 0x000836dcU);
        stream = read_byte(host, registers.address[4]++);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | stream;
        set_logic_flags(registers, stream, 0x80U);
        prefetch(host, 0x000836deU);
        prefetch(host, 0x00084126U);
        prefetch(host, 0x00084128U);
        registers.program_counter = 0x00084126U;
        return host.call_function(88U, 0U, 0xffU, 1U,
            0x000836dcU, 0x00084126U, context);
    }
    case 20: {
        prefetch(host, 0x00083926U);
        prefetch(host, 0x00083928U);
        prefetch(host, 0x0008392aU);
        prefetch(host, 0x0008392cU);
        const auto address = registers.address[3];
        const auto prior = read_byte(host, address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((prior & 0x10U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x0008392eU);
        write_byte(host, address, static_cast<std::uint8_t>(prior | 0x10U));
        return return_from_subroutine(host, registers);
    }
    case 21: {
        prefetch(host, 0x0008376eU);
        prefetch(host, 0x00083770U);
        prefetch(host, 0x00083772U);
        prefetch(host, 0x00083774U);
        const auto bit_address = registers.address[3] + 1U;
        const auto bit_prior = read_byte(host, bit_address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((bit_prior & 0x08U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x00083776U);
        write_byte(host, bit_address, static_cast<std::uint8_t>(bit_prior | 0x08U));
        prefetch(host, 0x00083778U);
        prefetch(host, 0x0008377aU);
        const auto flags = read_byte(host, registers.address[3]);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((flags & 0x04U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x0008377cU);
        if ((flags & 0x04U) != 0U) {
            prefetch(host, 0x000834aeU);
            prefetch(host, 0x000834b0U);
            return return_from_subroutine(host, registers);
        }
        prefetch(host, 0x0008377eU);
        prefetch(host, 0x00083780U);
        prefetch(host, 0x00083782U);
        prefetch(host, 0x00083784U);
        const auto active_address = registers.address[3];
        const auto active_prior = read_byte(host, active_address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((active_prior & 0x04U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x00083786U);
        write_byte(host, active_address,
            static_cast<std::uint8_t>(active_prior | 0x04U));
        const auto channel = static_cast<std::uint8_t>(registers.data[7]);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | channel;
        set_logic_flags(registers, channel, 0x80U);
        registers.data[0] = 8U;
        set_logic_flags(registers, 8U, 0x80000000U);
        prefetch(host, 0x00083788U);
        prefetch(host, 0x0008378aU);
        prefetch(host, 0x00084126U);
        prefetch(host, 0x00084128U);
        registers.program_counter = 0x00084126U;
        return host.call_function(88U, 0U, 0xffU, 1U,
            0x00083788U, 0x00084126U, context);
    }
    case 22: {
        prefetch(host, 0x0008378cU);
        prefetch(host, 0x0008378eU);
        prefetch(host, 0x00083790U);
        prefetch(host, 0x00083792U);
        const auto flag_address = registers.address[3] + 1U;
        const auto flag_prior = read_byte(host, flag_address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((flag_prior & 0x08U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x00083794U);
        write_byte(host, flag_address,
            static_cast<std::uint8_t>(flag_prior & 0xf7U));
        prefetch(host, 0x00083796U);
        prefetch(host, 0x00083798U);
        const auto peer_flags = read_byte(host, registers.address[3] + 0x320U);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((peer_flags & 0x80U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x0008379aU);
        if ((peer_flags & 0x80U) != 0U) {
            prefetch(host, 0x000834aeU);
            prefetch(host, 0x000834b0U);
            return return_from_subroutine(host, registers);
        }
        prefetch(host, 0x0008379cU);
        prefetch(host, 0x0008379eU);
        prefetch(host, 0x000837a0U);
        prefetch(host, 0x000837a2U);
        const auto active_address = registers.address[3];
        const auto active_prior = read_byte(host, active_address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((active_prior & 0x04U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x000837a4U);
        write_byte(host, active_address,
            static_cast<std::uint8_t>(active_prior & 0xfbU));
        return return_from_subroutine(host, registers);
    }
    case 23: {
        prefetch(host, 0x000836e0U);
        prefetch(host, 0x000836e2U);
        move_stream_byte_to_data(host, registers, 1U);
        prefetch(host, 0x000836e4U);
        prefetch(host, 0x000836e6U);
        write_byte(host, registers.address[6] + 0xf2U,
            static_cast<std::uint8_t>(registers.data[1]));
        set_logic_flags(registers, static_cast<std::uint8_t>(registers.data[1]), 0x80U);
        registers.data[0] = 0x18U;
        set_logic_flags(registers, 0x18U, 0x80000000U);
        prefetch(host, 0x000836e8U);
        prefetch(host, 0x000836eaU);
        auto child = call_ym2151_writer(context, 0x000836e8U, 0x000836ecU);
        if (child.status != TranslationStatus::complete) return child;

        move_stream_byte_to_data(host, registers, 1U);
        prefetch(host, 0x000836f0U);
        prefetch(host, 0x000836f2U);
        write_byte(host, registers.address[6] + 0xf4U,
            static_cast<std::uint8_t>(registers.data[1]));
        set_logic_flags(registers, static_cast<std::uint8_t>(registers.data[1]), 0x80U);
        registers.data[0] = 0x19U;
        set_logic_flags(registers, 0x19U, 0x80000000U);
        prefetch(host, 0x000836f4U);
        prefetch(host, 0x000836f6U);
        child = call_ym2151_writer(context, 0x000836f4U, 0x000836f8U);
        if (child.status != TranslationStatus::complete) return child;

        move_stream_byte_to_data(host, registers, 1U);
        prefetch(host, 0x000836fcU);
        prefetch(host, 0x000836feU);
        write_byte(host, registers.address[6] + 0xf5U,
            static_cast<std::uint8_t>(registers.data[1]));
        set_logic_flags(registers, static_cast<std::uint8_t>(registers.data[1]), 0x80U);
        prefetch(host, 0x00083700U);
        child = call_ym2151_writer(context, 0x000836feU, 0x00083702U);
        if (child.status != TranslationStatus::complete) return child;

        move_stream_byte_to_data(host, registers, 1U);
        prefetch(host, 0x00083706U);
        prefetch(host, 0x00083708U);
        write_byte(host, registers.address[6] + 0xf6U,
            static_cast<std::uint8_t>(registers.data[1]));
        set_logic_flags(registers, static_cast<std::uint8_t>(registers.data[1]), 0x80U);
        registers.data[0] = 0x1bU;
        set_logic_flags(registers, 0x1bU, 0x80000000U);
        prefetch(host, 0x0008370aU);
        prefetch(host, 0x0008370cU);
        child = call_ym2151_writer(context, 0x0008370aU, 0x0008370eU);
        if (child.status != TranslationStatus::complete) return child;

        move_stream_byte_to_data(host, registers, 1U);
        prefetch(host, 0x00083712U);
        prefetch(host, 0x00083714U);
        write_byte(host, registers.address[6] + 0xf8U,
            static_cast<std::uint8_t>(registers.data[1]));
        set_logic_flags(registers, static_cast<std::uint8_t>(registers.data[1]), 0x80U);
        registers.data[0] = 0x38U;
        set_logic_flags(registers, 0x38U, 0x80000000U);
        prefetch(host, 0x00083716U);
        auto prior_d0 = static_cast<std::uint8_t>(registers.data[0]);
        auto result_d0 = static_cast<std::uint8_t>(prior_d0
            + static_cast<std::uint8_t>(registers.data[7]));
        registers.data[0] = (registers.data[0] & 0xffffff00U) | result_d0;
        set_add_byte_flags(registers, prior_d0,
            static_cast<std::uint8_t>(registers.data[7]), result_d0);
        prefetch(host, 0x00083718U);
        prefetch(host, 0x0008371aU);
        child = call_ym2151_writer(context, 0x00083718U, 0x0008371cU);
        if (child.status != TranslationStatus::complete) return child;

        registers.data[2] = 0U;
        set_logic_flags(registers, 0U, 0x80000000U);
        registers.data[3] = 3U;
        set_logic_flags(registers, 3U, 0x80000000U);
        prefetch(host, 0x00083720U);
        prefetch(host, 0x00083722U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x00a0U;
        set_logic_flags(registers, 0x00a0U, 0x8000U);
        prefetch(host, 0x00083724U);
        prior_d0 = static_cast<std::uint8_t>(registers.data[0]);
        result_d0 = static_cast<std::uint8_t>(prior_d0
            + static_cast<std::uint8_t>(registers.data[7]));
        registers.data[0] = (registers.data[0] & 0xffffff00U) | result_d0;
        set_add_byte_flags(registers, prior_d0,
            static_cast<std::uint8_t>(registers.data[7]), result_d0);

        for (;;) {
            prefetch(host, 0x00083726U);
            prefetch(host, 0x00083728U);
            const auto stream = read_byte(host, registers.address[4]++);
            set_logic_flags(registers, stream, 0x80U);
            prefetch(host, 0x0008372aU);
            if ((stream & 0x80U) != 0U) {
                prefetch(host, 0x0008372cU);
                prefetch(host, 0x0008372eU);
                const auto table_address = registers.address[3] + 0x24U
                    + static_cast<std::uint16_t>(registers.data[2]);
                auto table_value = read_byte(host, table_address);
                registers.data[1] = (registers.data[1] & 0xffffff00U) | table_value;
                set_logic_flags(registers, table_value, 0x80U);
                prefetch(host, 0x00083730U);
                const auto bit_was_clear = (table_value & 0x80U) == 0U;
                table_value = static_cast<std::uint8_t>(table_value | 0x80U);
                registers.data[1] = (registers.data[1] & 0xffffff00U) | table_value;
                registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
                    | (bit_was_clear ? 0x0004U : 0U));
                prefetch(host, 0x00083732U);
                prefetch(host, 0x00083734U);
                prefetch(host, 0x00083736U);
                write_byte(host, table_address, table_value);
                set_logic_flags(registers, table_value, 0x80U);
                prefetch(host, 0x00083738U);
                child = call_ym2151_writer(context, 0x00083736U, 0x0008373aU);
                if (child.status != TranslationStatus::complete) return child;
            } else {
                prefetch(host, 0x0008373aU);
                prefetch(host, 0x0008373cU);
            }
            auto d2 = static_cast<std::uint8_t>(registers.data[2]);
            const auto next_d2 = static_cast<std::uint8_t>(d2 + 1U);
            registers.data[2] = (registers.data[2] & 0xffffff00U) | next_d2;
            set_add_byte_flags(registers, d2, 1U, next_d2);
            prior_d0 = static_cast<std::uint8_t>(registers.data[0]);
            result_d0 = static_cast<std::uint8_t>(prior_d0 + 8U);
            registers.data[0] = (registers.data[0] & 0xffffff00U) | result_d0;
            set_add_byte_flags(registers, prior_d0, 8U, result_d0);
            prefetch(host, 0x0008373eU);
            prefetch(host, 0x00083740U);
            const auto counter = static_cast<std::uint16_t>(registers.data[3]);
            registers.data[3] = (registers.data[3] & 0xffff0000U)
                | static_cast<std::uint16_t>(counter - 1U);
            if (counter == 0U) {
                prefetch(host, 0x00083726U);
                break;
            }
        }
        prefetch(host, 0x00083742U);
        prefetch(host, 0x00083744U);
        return return_from_subroutine(host, registers);
    }
    case 24: {
        prefetch(host, 0x0008375eU);
        prefetch(host, 0x00083760U);
        prefetch(host, 0x00083762U);
        prefetch(host, 0x00083764U);
        const auto address = registers.address[3] + 1U;
        const auto prior = read_byte(host, address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((prior & 0x20U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x00083766U);
        write_byte(host, address, static_cast<std::uint8_t>(prior | 0x20U));
        return return_from_subroutine(host, registers);
    }
    case 25: {
        prefetch(host, 0x00083750U);
        prefetch(host, 0x00083752U);
        auto stream = read_byte(host, registers.address[4]++);
        prefetch(host, 0x00083754U);
        write_byte(host, registers.address[3] + 51U, stream);
        set_logic_flags(registers, stream, 0x80U);
        prefetch(host, 0x00083756U);
        stream = read_byte(host, registers.address[4]++);
        prefetch(host, 0x00083758U);
        write_byte(host, registers.address[3] + 48U, stream);
        set_logic_flags(registers, stream, 0x80U);
        prefetch(host, 0x0008375aU);
        stream = read_byte(host, registers.address[4]++);
        prefetch(host, 0x0008375cU);
        write_byte(host, registers.address[3] + 50U, stream);
        set_logic_flags(registers, stream, 0x80U);
        prefetch(host, 0x0008375eU);
        return return_from_subroutine(host, registers);
    }
    case 26: {
        prefetch(host, 0x00083766U);
        prefetch(host, 0x00083768U);
        prefetch(host, 0x0008376aU);
        prefetch(host, 0x0008376cU);
        const auto address = registers.address[3] + 1U;
        const auto prior = read_byte(host, address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((prior & 0x20U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x0008376eU);
        write_byte(host, address, static_cast<std::uint8_t>(prior & 0xdfU));
        return return_from_subroutine(host, registers);
    }
    case 27: {
        prefetch(host, 0x00083944U);
        prefetch(host, 0x00083946U);
        prefetch(host, 0x00083948U);
        for (int index = 7; index >= 0; --index) {
            registers.address[7] -= 4U;
            write_word(host, registers.address[7] + 2U,
                static_cast<std::uint16_t>(registers.data[index]));
            write_word(host, registers.address[7],
                static_cast<std::uint16_t>(registers.data[index] >> 16U));
        }
        prefetch(host, 0x0008394aU);
        registers.data[0] = 0U;
        set_logic_flags(registers, 0U, 0x80000000U);
        prefetch(host, 0x0008394cU);
        registers.data[1] = registers.data[0];
        set_logic_flags(registers, registers.data[1], 0x80000000U);
        prefetch(host, 0x0008394eU);
        auto stream = read_byte(host, registers.address[4]++);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | stream;
        set_logic_flags(registers, stream, 0x80U);
        prefetch(host, 0x00083950U);
        stream = read_byte(host, registers.address[4]++);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | stream;
        set_logic_flags(registers, stream, 0x80U);
        prefetch(host, 0x00083952U);
        left_shift_word_eight(registers);
        prefetch(host, 0x00083954U);
        const auto high = static_cast<std::uint16_t>(registers.data[0]);
        const auto low = static_cast<std::uint16_t>(registers.data[1]);
        const auto combined = static_cast<std::uint16_t>(high + low);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | combined;
        set_add_word_flags(registers, high, low, combined);
        prefetch(host, 0x00083956U);
        stream = read_byte(host, registers.address[4]++);
        registers.data[6] = (registers.data[6] & 0xffffff00U) | stream;
        set_logic_flags(registers, stream, 0x80U);
        prefetch(host, 0x00083958U);

        const auto trap_status = registers.status;
        registers.address[7] -= 2U;
        write_word(host, registers.address[7], 0x3958U);
        registers.address[7] -= 4U;
        write_word(host, registers.address[7], trap_status);
        write_word(host, registers.address[7] + 2U, 0x0008U);
        prefetch(host, 0x00000094U);
        prefetch(host, 0x00000096U);
        prefetch(host, 0x00080078U);
        prefetch(host, 0x0008007aU);
        registers.program_counter = 0x00080078U;
        const auto trap = host.call_function(515U, 0U, 0xffU, 4U,
            0x00083956U, 0x00080078U, context);
        if (trap.status != TranslationStatus::complete || trap.control != 2U)
            return trap;

        prefetch(host, 0x0008395aU);
        prefetch(host, 0x0008395cU);
        for (unsigned index = 0; index != 8U; ++index) {
            registers.data[index] = read_long(host, registers.address[7]);
            registers.address[7] += 4U;
        }
        prefetch(host, 0x0008395eU);
        return return_from_subroutine(host, registers);
    }
    default:
        return FunctionResult::unimplemented();
    }
}

} // namespace gain_ground::translated

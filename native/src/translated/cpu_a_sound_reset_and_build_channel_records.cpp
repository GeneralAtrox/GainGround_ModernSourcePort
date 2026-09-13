#include "gain_ground/contract_types.h"
#include "gain_ground/sound_caller_timing.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

[[nodiscard]] ByteLocation locate_byte(std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    return {address & 0x0003fffeU,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(SoundCallerTiming &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(SoundCallerTiming &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

[[nodiscard]] std::uint16_t read_word(SoundCallerTiming &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & kAddressMask, kWordMask);
}

void write_word(SoundCallerTiming &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address & kAddressMask, value, kWordMask);
}

[[nodiscard]] std::uint32_t read_long(SoundCallerTiming &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

void write_long(SoundCallerTiming &host, std::uint32_t address, std::uint32_t value)
{
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
}

void prefetch(SoundCallerTiming &host, std::uint32_t address)
{
    (void)read_word(host, address);
}

void set_logic(CpuRegisters &registers, std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

template <typename Wide>
void set_add(CpuRegisters &registers, Wide left, Wide right, Wide result,
    Wide sign, std::uint64_t limit)
{
    std::uint16_t flags{};
    if ((result & sign) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & sign) != 0U) flags |= 0x0002U;
    if (static_cast<std::uint64_t>(left) + right > limit) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

template <typename Wide>
void set_sub(CpuRegisters &registers, Wide left, Wide right, Wide result,
    Wide sign, bool affect_extend)
{
    std::uint16_t flags = affect_extend ? 0U : registers.status & 0x0010U;
    if ((result & sign) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & sign) != 0U) flags |= 0x0002U;
    if (right > left) flags |= static_cast<std::uint16_t>(
        affect_extend ? 0x0011U : 0x0001U);
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_bit_test(CpuRegisters &registers, bool prior_set)
{
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x0004U) | (prior_set ? 0U : 0x0004U));
}

void push_return(SoundCallerTiming &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_long(host, registers.address[7], value);
}

[[nodiscard]] FunctionResult call_native(SoundCallerTiming &host, FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite,
    std::uint32_t target, std::uint32_t return_address)
{
    // BSR.w: two internal clocks, then the original two stack writes and
    // two target prefetches. Inactive sections retain their existing timing.
    host.clocks(2U);
    push_return(host, context.registers, return_address);
    prefetch(host, target);
    prefetch(host, target + 2U);
    context.registers.program_counter = target;
    return host.call_function(function_id, 0U, 0xffU, 2U,
        callsite, target, context);
}

[[nodiscard]] FunctionResult finish(SoundCallerTiming &host, CpuRegisters &registers)
{
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_a_sound_reset_and_build_channel_records(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    SoundCallerTiming host(context);
    auto &registers = context.registers;

    const bool continuation_entry = registers.program_counter == 0x00083d80U;
    if (!continuation_entry) {
    host.begin(0x00083d24U);
    registers.data[7] = (registers.data[7] & 0xffff0000U) | 7U;
    set_logic(registers, 7U, 0x8000U);
    prefetch(host, 0x00083d28U);
    for (;;) {
        prefetch(host, 0x00083d2aU);
        auto child = call_native(host, context, 96U,
            0x00083d28U, 0x000842c0U, 0x00083d2cU);
        if (child.status != TranslationStatus::complete) return child;
        host.begin(0x00083d2cU);
        host.clocks(2U); // DBRA internal prefix before the target probe.
        const auto count = static_cast<std::uint16_t>(registers.data[7]);
        registers.data[7] = (registers.data[7] & 0xffff0000U)
            | static_cast<std::uint16_t>(count - 1U);
        prefetch(host, 0x00083d28U);
        if (count == 0U) break;
    }

    prefetch(host, 0x00083d30U);
    prefetch(host, 0x00083d32U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x0011U;
    set_logic(registers, 0x0011U, 0x8000U);
    prefetch(host, 0x00083d34U);
    registers.data[1] = 0U;
    set_logic(registers, 0U, 0x80000000U);
    prefetch(host, 0x00083d36U);
    registers.data[4] = 0U;
    set_logic(registers, 0U, 0x80000000U);
    prefetch(host, 0x00083d38U);
    prefetch(host, 0x00083d3aU);
    registers.address[0] = registers.address[6] + 0x180U;
    prefetch(host, 0x00083d3cU);
    for (;;) {
        prefetch(host, 0x00083d3eU);
        host.clocks(2U); // MOVE.W indexed destination.
        prefetch(host, 0x00083d40U);
        const auto displacement = static_cast<std::int16_t>(registers.data[4]);
        write_word(host, registers.address[0] + displacement,
            static_cast<std::uint16_t>(registers.data[1]));
        set_logic(registers, static_cast<std::uint16_t>(registers.data[1]), 0x8000U);
        prefetch(host, 0x00083d42U);
        const auto prior = static_cast<std::uint16_t>(registers.data[4]);
        const auto advanced = static_cast<std::uint16_t>(prior + 0x50U);
        registers.data[4] = (registers.data[4] & 0xffff0000U) | advanced;
        set_add(registers, prior, static_cast<std::uint16_t>(0x50U),
            advanced, static_cast<std::uint16_t>(0x8000U), 0xffffU);
        prefetch(host, 0x00083d44U);
        prefetch(host, 0x00083d46U);
        host.clocks(2U); // DBRA.
        const auto count = static_cast<std::uint16_t>(registers.data[0]);
        registers.data[0] = (registers.data[0] & 0xffff0000U)
            | static_cast<std::uint16_t>(count - 1U);
        prefetch(host, 0x00083d3cU);
        if (count == 0U) break;
    }

    prefetch(host, 0x00083d48U);
    prefetch(host, 0x00083d4aU);
    registers.address[0] = registers.address[6];
    prefetch(host, 0x00083d4cU);
    registers.data[1] = 0U;
    set_logic(registers, 0U, 0x80000000U);
    prefetch(host, 0x00083d4eU);
    registers.data[0] = 0U;
    set_logic(registers, 0U, 0x80000000U);
    prefetch(host, 0x00083d50U);
    prefetch(host, 0x00083d52U);
    prefetch(host, 0x00083d54U);
    const auto timer = read_word(host, registers.address[0] + 0x20U);
    const auto decremented_timer = static_cast<std::uint16_t>(timer - 1U);
    prefetch(host, 0x00083d56U);
    write_word(host, registers.address[0] + 0x20U, decremented_timer);
    set_sub(registers, timer, static_cast<std::uint16_t>(1U),
        decremented_timer, static_cast<std::uint16_t>(0x8000U), true);
    prefetch(host, 0x00083d58U);
    const auto cursor = read_word(host, registers.address[0] + 0x24U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | cursor;
    set_logic(registers, cursor, 0x8000U);
    const auto masked_cursor = static_cast<std::uint16_t>(cursor & 0x001eU);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | masked_cursor;
    set_logic(registers, masked_cursor, 0x8000U);
    prefetch(host, 0x00083d5aU);
    prefetch(host, 0x00083d5cU);
    prefetch(host, 0x00083d5eU);
    host.clocks(2U); // MOVE.W indexed source.
    prefetch(host, 0x00083d60U);
    const auto record_offset = read_word(host,
        registers.address[0] + static_cast<std::int16_t>(masked_cursor));
    registers.data[0] = (registers.data[0] & 0xffff0000U) | record_offset;
    set_logic(registers, record_offset, 0x8000U);
    prefetch(host, 0x00083d62U);
    prefetch(host, 0x00083d64U);
    const auto current_cursor = read_word(host, registers.address[0] + 0x24U);
    const auto advanced_cursor = static_cast<std::uint16_t>(current_cursor + 2U);
    prefetch(host, 0x00083d66U);
    write_word(host, registers.address[0] + 0x24U, advanced_cursor);
    set_add(registers, current_cursor, static_cast<std::uint16_t>(2U),
        advanced_cursor, static_cast<std::uint16_t>(0x8000U), 0xffffU);
    auto child = call_native(host, context, 84U,
        0x00083d64U, 0x00083de8U, 0x00083d68U);
    if (child.status != TranslationStatus::complete) return child;

    host.begin(0x00083d68U);
    host.clocks(2U); // MOVE.W indexed source.
    prefetch(host, 0x00083d6cU);
    const auto table_offset = read_word(host,
        registers.address[5] + static_cast<std::int32_t>(registers.data[1]));
    registers.data[1] = (registers.data[1] & 0xffff0000U) | table_offset;
    set_logic(registers, table_offset, 0x8000U);
    prefetch(host, 0x00083d6eU);
    registers.address[0] = registers.address[5]
        + static_cast<std::int32_t>(registers.data[1]);
    host.clocks(2U); // LEA indexed: internal clocks before each prefetch.
    prefetch(host, 0x00083d70U);
    host.clocks(2U);
    prefetch(host, 0x00083d72U);
    host.clocks(2U); // MOVE.W indexed source.
    prefetch(host, 0x00083d74U);
    const auto record_pointer = read_word(host,
        registers.address[0]
        + static_cast<std::int16_t>(registers.data[0]));
    registers.data[0] = (registers.data[0] & 0xffff0000U) | record_pointer;
    set_logic(registers, record_pointer, 0x8000U);
    prefetch(host, 0x00083d76U);
    if (record_pointer == 0U) {
        host.clocks(2U); // BEQ.w taken.
        prefetch(host, 0x00083de6U);
        prefetch(host, 0x00083de8U);
        return finish(host, registers);
    }

    host.clocks(4U); // BEQ.w not taken.
    prefetch(host, 0x00083d78U);
    prefetch(host, 0x00083d7aU);
    prefetch(host, 0x00083d7cU);
    registers.data[0] = record_pointer;
    set_logic(registers, registers.data[0], 0x80000000U);
    prefetch(host, 0x00083d7eU);
    registers.address[0] += registers.data[0];
    prefetch(host, 0x00083d80U);
    host.clocks(4U); // ANDI.L immediate register ALU tail.
    prefetch(host, 0x00083d82U);
    host.clocks(4U); // ADDA.L register ALU tail.
    }

    // Time the actual setup accesses, not a delay fitted to YM busy.
    if (continuation_entry) host.begin(0x00083d80U);
    auto repeat_count = read_word(host, registers.address[0]);
    registers.address[0] += 2U;
    registers.data[4] = (registers.data[4] & 0xffff0000U) | repeat_count;
    set_logic(registers, repeat_count, 0x8000U);

    for (;;) {
        // Later iterations finish DBRA's target refill here; on the first
        // iteration this completes MOVE.W (A0)+,D4.
        prefetch(host, 0x00083d84U);
        registers.address[3] = registers.address[6];
        prefetch(host, 0x00083d86U);
        prefetch(host, 0x00083d88U);
        const auto selector_byte = read_byte(host, registers.address[0] + 2U);
        registers.data[5] = (registers.data[5] & 0xffffff00U) | selector_byte;
        set_logic(registers, selector_byte, 0x80U);
        prefetch(host, 0x00083d8aU);
        auto selector = static_cast<std::uint16_t>(registers.data[5] & 0x001fU);
        registers.data[5] = (registers.data[5] & 0xffff0000U) | selector;
        set_logic(registers, selector, 0x8000U);
        prefetch(host, 0x00083d8cU);
        const auto doubled_selector = static_cast<std::uint16_t>(selector + selector);
        registers.data[5] = (registers.data[5] & 0xffff0000U) | doubled_selector;
        set_add(registers, selector, selector, doubled_selector,
            static_cast<std::uint16_t>(0x8000U), 0xffffU);
        prefetch(host, 0x00083d8eU);
        prefetch(host, 0x00083d90U);
        registers.address[2] = 0x000839a6U;
        prefetch(host, 0x00083d92U);
        prefetch(host, 0x00083d94U);
        host.clocks(2U); // ADDA.W indexed effective-address calculation.
        prefetch(host, 0x00083d96U);
        const auto displacement = static_cast<std::int16_t>(read_word(host,
            registers.address[2] + static_cast<std::int16_t>(doubled_selector)));
        registers.address[3] += displacement;
        prefetch(host, 0x00083d98U);
        host.clocks(4U); // ADDA.W long address-register ALU tail.
        const bool bit4 = (doubled_selector & 0x0010U) != 0U;
        set_bit_test(registers, bit4);
        prefetch(host, 0x00083d9aU);
        prefetch(host, 0x00083d9cU);
        host.clocks(2U); // BTST #4,D5 internal tail.
        bool prefetched_copy_instruction = false;
        if (bit4) {
            host.clocks(4U); // BEQ not taken, before its single fetch.
            prefetch(host, 0x00083d9eU);
            prefetch(host, 0x00083da0U);
            prefetch(host, 0x00083da2U);
            const auto bit2_address = registers.address[3] - 0x320U;
            const auto prior = read_byte(host, bit2_address);
            set_bit_test(registers, (prior & 0x04U) != 0U);
            prefetch(host, 0x00083da4U);
            write_byte(host, bit2_address, static_cast<std::uint8_t>(prior | 0x04U));
            const auto d5_byte = static_cast<std::uint8_t>(registers.data[5]);
            set_sub(registers, d5_byte, static_cast<std::uint8_t>(0x1cU),
                static_cast<std::uint8_t>(d5_byte - 0x1cU),
                static_cast<std::uint8_t>(0x80U), false);
            prefetch(host, 0x00083da6U);
            prefetch(host, 0x00083da8U);
            if ((registers.status & 0x0001U) == 0U) {
                host.clocks(4U); // BCS not taken.
                prefetch(host, 0x00083daaU);
                prefetch(host, 0x00083dacU);
                prefetch(host, 0x00083daeU);
                const auto bit7_address = registers.address[3] - 0x280U;
                const auto prior_bit7 = read_byte(host, bit7_address);
                set_bit_test(registers, (prior_bit7 & 0x80U) != 0U);
                prefetch(host, 0x00083db0U);
                write_byte(host, bit7_address,
                    static_cast<std::uint8_t>(prior_bit7 & 0x7fU));
                prefetched_copy_instruction = true;
            } else {
                host.clocks(2U); // BCS taken, before the two target fetches.
            }
        } else {
            host.clocks(2U); // BEQ taken, before the two target fetches.
        }

        if (!prefetched_copy_instruction) {
            prefetch(host, 0x00083daeU);
            prefetch(host, 0x00083db0U);
        }
        const auto first_word = read_word(host, registers.address[0]);
        registers.address[0] += 2U;
        write_word(host, registers.address[3], first_word);
        registers.address[3] += 2U;
        set_logic(registers, first_word, 0x8000U);
        registers.data[7] = 0U;
        set_logic(registers, 0U, 0x80000000U);
        prefetch(host, 0x00083db2U);
        prefetch(host, 0x00083db4U);
        const auto channel = read_byte(host, registers.address[0]++);
        registers.data[7] = (registers.data[7] & 0xffffff00U) | channel;
        set_logic(registers, channel, 0x80U);
        prefetch(host, 0x00083db6U);
        write_byte(host, registers.address[3]++, channel);
        set_logic(registers, channel, 0x80U);
        prefetch(host, 0x00083db8U);
        const auto masked_channel = static_cast<std::uint16_t>(
            registers.data[7] & 0x0007U);
        registers.data[7] = (registers.data[7] & 0xffff0000U) | masked_channel;
        set_logic(registers, masked_channel, 0x8000U);
        prefetch(host, 0x00083dbaU);
        prefetch(host, 0x00083dbcU);
        auto child = call_native(host, context, 86U,
            0x00083dbaU, 0x00083ee4U, 0x00083dbeU);
        if (child.status != TranslationStatus::complete) return child;

        host.begin(0x00083dbeU);
        const auto copied_byte = read_byte(host, registers.address[0]++);
        write_byte(host, registers.address[3]++, copied_byte);
        set_logic(registers, copied_byte, 0x80U);
        registers.data[5] = registers.address[0];
        set_logic(registers, registers.data[5], 0x80000000U);
        prefetch(host, 0x00083dc2U);
        prefetch(host, 0x00083dc4U);
        const auto relative = read_long(host, registers.address[0]);
        registers.address[0] += 4U;
        const auto before_add = registers.data[5];
        registers.data[5] += relative;
        set_add(registers, before_add, relative, registers.data[5],
            0x80000000U, 0xffffffffULL);
        const auto before_sub = registers.data[5];
        registers.data[5] -= registers.address[5];
        set_sub(registers, before_sub, registers.address[5], registers.data[5],
            0x80000000U, true);
        prefetch(host, 0x00083dc6U);
        host.clocks(2U); // ADD.L (A0)+,D5 ALU tail.
        prefetch(host, 0x00083dc8U);
        host.clocks(4U); // SUB.L A5,D5 ALU tail.
        write_long(host, registers.address[3], registers.data[5]);
        registers.address[3] += 4U;
        set_logic(registers, registers.data[5], 0x80000000U);
        prefetch(host, 0x00083dcaU);
        const auto copied_long = read_long(host, registers.address[0]);
        registers.address[0] += 4U;
        write_long(host, registers.address[3], copied_long);
        registers.address[3] += 4U;
        set_logic(registers, copied_long, 0x80000000U);
        prefetch(host, 0x00083dccU);
        prefetch(host, 0x00083dceU);
        write_word(host, registers.address[3], 0x5001U);
        registers.address[3] += 2U;
        set_logic(registers, 0x5001U, 0x8000U);
        prefetch(host, 0x00083dd0U);
        prefetch(host, 0x00083dd2U);
        const auto count_byte = read_byte(host, registers.address[0] - 9U);
        registers.data[5] = (registers.data[5] & 0xffffff00U) | count_byte;
        set_logic(registers, count_byte, 0x80U);
        const auto decremented = static_cast<std::uint8_t>(count_byte - 1U);
        registers.data[5] = (registers.data[5] & 0xffffff00U) | decremented;
        set_sub(registers, count_byte, static_cast<std::uint8_t>(1U), decremented,
            static_cast<std::uint8_t>(0x80U), true);
        prefetch(host, 0x00083dd4U);
        prefetch(host, 0x00083dd6U);
        write_byte(host, registers.address[3]++, decremented);
        set_logic(registers, decremented, 0x80U);
        registers.data[5] = 0U;
        set_logic(registers, 0U, 0x80000000U);
        prefetch(host, 0x00083dd8U);
        prefetch(host, 0x00083ddaU);
        write_byte(host, registers.address[3]++, 0U);
        set_logic(registers, 0U, 0x80U);
        registers.data[6] = 0x0fU;
        set_logic(registers, registers.data[6], 0x80000000U);
        prefetch(host, 0x00083ddcU);
        for (;;) {
            prefetch(host, 0x00083ddeU);
            write_long(host, registers.address[3], registers.data[5]);
            registers.address[3] += 4U;
            set_logic(registers, registers.data[5], 0x80000000U);
            prefetch(host, 0x00083de0U);
            host.clocks(2U); // Inner DBRA, including its expired target probe.
            const auto zero_count = static_cast<std::uint16_t>(registers.data[6]);
            registers.data[6] = (registers.data[6] & 0xffff0000U)
                | static_cast<std::uint16_t>(zero_count - 1U);
            prefetch(host, 0x00083ddcU);
            if (zero_count == 0U) break;
        }

        prefetch(host, 0x00083de2U);
        prefetch(host, 0x00083de4U);
        host.clocks(2U); // Outer DBRA.
        const auto outer_count = static_cast<std::uint16_t>(registers.data[4]);
        registers.data[4] = (registers.data[4] & 0xffff0000U)
            | static_cast<std::uint16_t>(outer_count - 1U);
        prefetch(host, 0x00083d82U);
        if (outer_count == 0U) break;
        repeat_count = static_cast<std::uint16_t>(outer_count - 1U);
        (void)repeat_count;
    }

    prefetch(host, 0x00083de6U);
    prefetch(host, 0x00083de8U);
    return finish(host, registers);
}

} // namespace gain_ground::translated

#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgram = 1U;
constexpr std::uint16_t kShared = 3U;
constexpr std::uint16_t kWord = 0xffffU;
constexpr std::uint16_t kByte = 0x00ffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void pf(ExecutionHost &host, std::uint32_t pc)
{
    (void)host.read_memory_word(kProgram, pc, kWord);
}

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t pc, std::uint32_t address)
{
    return static_cast<std::uint8_t>(
        host.read_hardware(1U, 0U, 0xffU, pc, address & ~1U, kByte));
}

void write_byte(ExecutionHost &host, std::uint32_t pc,
                std::uint32_t address, std::uint8_t value)
{
    host.write_hardware(2U, 0U, 0xffU, pc, address & ~1U,
        static_cast<std::uint16_t>(value) * 0x0101U, kByte);
}

void logic_byte(CpuRegisters &r, std::uint8_t value)
{
    const auto flags = static_cast<std::uint16_t>(
        (value == 0U ? 0x0004U : 0U) | ((value & 0x80U) != 0U ? 0x0008U : 0U));
    r.status = static_cast<std::uint16_t>((r.status & ~0x000fU) | flags);
}

void logic_word(CpuRegisters &r, std::uint16_t value)
{
    const auto flags = static_cast<std::uint16_t>(
        (value == 0U ? 0x0004U : 0U) | ((value & 0x8000U) != 0U ? 0x0008U : 0U));
    r.status = static_cast<std::uint16_t>((r.status & ~0x000fU) | flags);
}

void bit_test(CpuRegisters &r, bool set)
{
    r.status = static_cast<std::uint16_t>((r.status & ~0x0004U)
        | (set ? 0U : 0x0004U));
}

void lsr_byte(CpuRegisters &r, std::uint32_t &data)
{
    const auto source = static_cast<std::uint8_t>(data);
    const auto value = static_cast<std::uint8_t>(source >> 1U);
    const bool carry = (source & 1U) != 0U;
    data = (data & 0xffffff00U) | value;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU)
        | (carry ? 0x0011U : 0U) | (value == 0U ? 0x0004U : 0U));
}

void lsl_byte_two(CpuRegisters &r, std::uint32_t &data)
{
    const auto source = static_cast<std::uint8_t>(data);
    const auto value = static_cast<std::uint8_t>(source << 2U);
    const bool carry = (source & 0x40U) != 0U;
    data = (data & 0xffffff00U) | value;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU)
        | (carry ? 0x0011U : 0U) | (value == 0U ? 0x0004U : 0U)
        | ((value & 0x80U) != 0U ? 0x0008U : 0U));
}

void compare_byte(CpuRegisters &r, std::uint8_t destination, std::uint8_t source)
{
    const auto result = static_cast<std::uint8_t>(destination - source);
    const bool overflow = ((destination ^ source) & (destination ^ result) & 0x80U) != 0U;
    const auto flags = static_cast<std::uint16_t>(
        (result == 0U ? 0x0004U : 0U) | ((result & 0x80U) != 0U ? 0x0008U : 0U)
        | (overflow ? 0x0002U : 0U) | (source > destination ? 0x0001U : 0U));
    r.status = static_cast<std::uint16_t>((r.status & ~0x000fU) | flags);
}

void add_byte(CpuRegisters &r, std::uint32_t &data, std::uint8_t source)
{
    const auto destination = static_cast<std::uint8_t>(data);
    const auto sum = static_cast<std::uint16_t>(
        static_cast<std::uint16_t>(destination) + static_cast<std::uint16_t>(source));
    const auto result = static_cast<std::uint8_t>(sum);
    const bool carry = sum > 0xffU;
    const bool overflow = ((~(destination ^ source) & (destination ^ result)) & 0x80U) != 0U;
    data = (data & 0xffffff00U) | result;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU)
        | (carry ? 0x0011U : 0U) | (result == 0U ? 0x0004U : 0U)
        | ((result & 0x80U) != 0U ? 0x0008U : 0U) | (overflow ? 0x0002U : 0U));
}

void push_long(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    const auto offset = r.address[7] & kSharedMask;
    host.write_memory_word(kShared, offset, static_cast<std::uint16_t>(value >> 16U), kWord);
    host.write_memory_word(kShared, offset + 2U, static_cast<std::uint16_t>(value), kWord);
}

FunctionResult call_force_interrupt(FunctionContext &context,
                                    std::uint32_t callsite,
                                    std::uint32_t return_pc)
{
    auto &host = *context.host;
    auto &r = context.registers;
    push_long(host, r, return_pc);
    pf(host, 0x000017e4U);
    pf(host, 0x000017e6U);
    r.program_counter = 0x000017e4U;
    return host.call_function(21U, 0U, 0xffU, 2U, callsite,
        0x000017e4U, context);
}

void wait_status_bit0_clear(ExecutionHost &host, CpuRegisters &r,
                            std::uint32_t test_pc, std::uint32_t fdc)
{
    for (;;) {
        pf(host, test_pc + 4U);
        pf(host, test_pc + 6U);
        const auto status = read_byte(host, test_pc, fdc);
        bit_test(r, (status & 1U) != 0U);
        if ((status & 1U) == 0U)
            return;
        pf(host, test_pc + 8U);
        pf(host, test_pc);
        pf(host, test_pc + 2U);
    }
}
} // namespace

FunctionResult cpu_a_fdc_seek_track(FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x00001806U)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto fdc = r.address[5];

    r.data[0] = (r.data[0] & 0xffffff00U) | static_cast<std::uint8_t>(r.data[7]);
    logic_byte(r, static_cast<std::uint8_t>(r.data[0]));
    lsr_byte(r, r.data[0]);
    pf(host, 0x0000180aU);
    pf(host, 0x0000180cU);
    pf(host, 0x0000180eU);
    const auto current_track = read_byte(host, 0x0000180aU, fdc + 2U);
    compare_byte(r, static_cast<std::uint8_t>(r.data[0]), current_track);
    pf(host, 0x00001810U);
    const bool forced_interrupt = (r.status & 1U) != 0U;
    if (forced_interrupt) {
        pf(host, 0x00001812U);
        const auto child = call_force_interrupt(context, 0x00001810U, 0x00001814U);
        if (child.status != TranslationStatus::complete)
            return child;
    }

    if (!forced_interrupt)
        pf(host, 0x00001814U);
    r.data[0] = 1U;
    logic_byte(r, 1U);
    if (!forced_interrupt)
        pf(host, 0x00001816U);
    pf(host, 0x00001818U);

    for (;;) {
        r.data[1] = (r.data[1] & 0xffff0000U) | static_cast<std::uint16_t>(r.data[7]);
        logic_word(r, static_cast<std::uint16_t>(r.data[1]));
        lsr_byte(r, r.data[1]);
        pf(host, 0x0000181aU);
        pf(host, 0x0000181cU);
        wait_status_bit0_clear(host, r, 0x0000181aU, fdc);

        pf(host, 0x00001822U);
        pf(host, 0x00001824U);
        pf(host, 0x00001826U);
        write_byte(host, 0x00001822U, fdc + 6U,
            static_cast<std::uint8_t>(r.data[1]));
        logic_byte(r, static_cast<std::uint8_t>(r.data[1]));

        r.data[1] = (r.data[1] & 0xffff0000U) | static_cast<std::uint16_t>(r.data[7]);
        logic_word(r, static_cast<std::uint16_t>(r.data[1]));
        pf(host, 0x00001828U);
        pf(host, 0x0000182aU);
        r.data[1] = (r.data[1] & 0xffffff00U)
            | (static_cast<std::uint8_t>(r.data[1]) & 1U);
        logic_byte(r, static_cast<std::uint8_t>(r.data[1]));
        pf(host, 0x0000182cU);
        lsl_byte_two(r, r.data[1]);
        pf(host, 0x0000182eU);
        pf(host, 0x00001830U);
        add_byte(r, r.data[1], 10U);
        pf(host, 0x00001832U);
        pf(host, 0x00001834U);
        pf(host, 0x00001836U);
        write_byte(host, 0x00001832U, fdc + 10U,
            static_cast<std::uint8_t>(r.data[1]));
        logic_byte(r, static_cast<std::uint8_t>(r.data[1]));

        pf(host, 0x00001838U);
        wait_status_bit0_clear(host, r, 0x00001836U, fdc);
        pf(host, 0x0000183eU);
        pf(host, 0x00001840U);
        pf(host, 0x00001842U);
        pf(host, 0x00001844U);
        write_byte(host, 0x0000183eU, fdc, 0x14U);
        logic_byte(r, 0x14U);

        pf(host, 0x00001846U);
        for (;;) {
            pf(host, 0x00001848U);
            pf(host, 0x0000184aU);
            const auto irq = read_byte(host, 0x00001844U, fdc + 8U);
            bit_test(r, (irq & 2U) != 0U);
            if ((irq & 2U) != 0U)
                break;
            pf(host, 0x0000184cU);
            pf(host, 0x00001844U);
            pf(host, 0x00001846U);
        }

        pf(host, 0x0000184cU);
        pf(host, 0x0000184eU);
        pf(host, 0x00001850U);
        pf(host, 0x00001852U);
        const auto status = read_byte(host, 0x0000184cU, fdc);
        bit_test(r, (status & 0x10U) != 0U);
        pf(host, 0x00001854U);
        if ((status & 0x10U) == 0U) {
            pf(host, 0x00001860U);
            pf(host, 0x00001862U);
            const auto stack = r.address[7] & kSharedMask;
            const auto target = (static_cast<std::uint32_t>(
                host.read_memory_word(kShared, stack, kWord)) << 16U)
                | host.read_memory_word(kShared, stack + 2U, kWord);
            r.address[7] += 4U;
            pf(host, target);
            pf(host, target + 2U);
            r.program_counter = target;
            return FunctionResult::complete(1U, target);
        }

        pf(host, 0x00001856U);
        const auto child = call_force_interrupt(context, 0x00001854U, 0x00001858U);
        if (child.status != TranslationStatus::complete)
            return child;

        const auto counter = static_cast<std::uint16_t>(r.data[0] - 1U);
        r.data[0] = (r.data[0] & 0xffff0000U) | counter;
        if (counter != 0xffffU) {
            pf(host, 0x00001816U);
            pf(host, 0x00001818U);
            continue;
        }

        // DBRA performs its target fetch even when the exhausted counter falls through.
        pf(host, 0x00001816U);
        pf(host, 0x0000185cU);
        pf(host, 0x0000185eU);
        pf(host, 0x0000197cU);
        pf(host, 0x0000197eU);
        r.program_counter = 0x0000197cU;
        return host.call_function(516U, 0U, 0xffU, 1U, 0x0000185cU,
            0x0000197cU, context);
    }
}

} // namespace gain_ground::translated

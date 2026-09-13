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

std::uint8_t read_hardware_byte(ExecutionHost &host, std::uint32_t pc,
                                std::uint32_t address)
{
    return static_cast<std::uint8_t>(host.read_hardware(
        1U, 0U, 0xffU, pc, address & ~1U, kByte));
}

void write_hardware_byte(ExecutionHost &host, std::uint32_t pc,
                         std::uint32_t address, std::uint8_t value)
{
    host.write_hardware(2U, 0U, 0xffU, pc, address & ~1U,
        static_cast<std::uint16_t>(value) * 0x0101U, kByte);
}

void logic_byte(CpuRegisters &r, std::uint8_t value)
{
    r.status = static_cast<std::uint16_t>((r.status & ~0x000fU)
        | (value == 0U ? 0x0004U : 0U)
        | ((value & 0x80U) != 0U ? 0x0008U : 0U));
}

void logic_word(CpuRegisters &r, std::uint16_t value)
{
    r.status = static_cast<std::uint16_t>((r.status & ~0x000fU)
        | (value == 0U ? 0x0004U : 0U)
        | ((value & 0x8000U) != 0U ? 0x0008U : 0U));
}

void logic_long(CpuRegisters &r, std::uint32_t value)
{
    r.status = static_cast<std::uint16_t>((r.status & ~0x000fU)
        | (value == 0U ? 0x0004U : 0U)
        | ((value & 0x80000000U) != 0U ? 0x0008U : 0U));
}

void bit_test(CpuRegisters &r, bool set)
{
    r.status = static_cast<std::uint16_t>((r.status & ~0x0004U)
        | (set ? 0U : 0x0004U));
}

void push_long(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    const auto offset = r.address[7] & kSharedMask;
    host.write_memory_word(kShared, offset,
        static_cast<std::uint16_t>(value >> 16U), kWord);
    host.write_memory_word(kShared, offset + 2U,
        static_cast<std::uint16_t>(value), kWord);
}

FunctionResult call_child(FunctionContext &context, std::uint32_t function_id,
                          std::uint32_t callsite, std::uint32_t target,
                          std::uint32_t return_pc)
{
    auto &host = *context.host;
    auto &r = context.registers;
    push_long(host, r, return_pc);
    pf(host, target);
    pf(host, target + 2U);
    r.program_counter = target;
    return host.call_function(function_id, 0U, 0xffU, 2U,
        callsite, target, context);
}

void wait_bit0_clear(ExecutionHost &host, CpuRegisters &r,
                     std::uint32_t test_pc, std::uint32_t fdc)
{
    for (;;) {
        pf(host, test_pc + 4U);
        pf(host, test_pc + 6U);
        const auto value = read_hardware_byte(host, test_pc, fdc);
        bit_test(r, (value & 1U) != 0U);
        if ((value & 1U) == 0U)
            return;
        pf(host, test_pc + 8U);
        pf(host, test_pc);
        pf(host, test_pc + 2U);
    }
}

std::uint8_t read_shared_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto word = host.read_memory_word(kShared,
        (address & kSharedMask) & ~1U,
        (address & 1U) != 0U ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>((address & 1U) != 0U ? word : word >> 8U);
}

void write_shared_byte(ExecutionHost &host, std::uint32_t address,
                       std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    host.write_memory_word(kShared, (address & kSharedMask) & ~1U,
        static_cast<std::uint16_t>(value) << (odd ? 0U : 8U),
        odd ? 0x00ffU : 0xff00U);
}

FunctionResult return_from_subroutine(FunctionContext &context)
{
    auto &host = *context.host;
    auto &r = context.registers;
    const auto sp = r.address[7] & kSharedMask;
    const auto target = (static_cast<std::uint32_t>(
        host.read_memory_word(kShared, sp, kWord)) << 16U)
        | host.read_memory_word(kShared, sp + 2U, kWord);
    r.address[7] += 4U;
    pf(host, target);
    pf(host, target + 2U);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_a_fdc_transfer_sector_payload(FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x00001710U)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto fdc = r.address[5];

    pf(host, 0x00001714U);
    pf(host, 0x00001716U);
    const auto present = read_hardware_byte(host, 0x00001710U, fdc + 8U);
    bit_test(r, (present & 0x80U) != 0U);
    if ((present & 0x80U) == 0U) {
        pf(host, 0x00001718U);
        pf(host, 0x00001954U);
        pf(host, 0x00001956U);
        r.program_counter = 0x00001954U;
        return FunctionResult::complete(3U, 0x00001954U);
    }

    pf(host, 0x00001718U);
    pf(host, 0x0000171aU);
    pf(host, 0x0000171cU);
    r.address[0] = r.address[2];
    r.data[5] = (r.data[5] & 0xffff0000U) | 4U;
    logic_word(r, 4U);
    pf(host, 0x0000171eU);
    pf(host, 0x00001720U);

    for (;;) {
        r.data[2] = r.data[1];
        logic_long(r, r.data[2]);
        pf(host, 0x00001722U);
        pf(host, 0x00001724U);
        auto child = call_child(context, 22U, 0x00001722U,
            0x00001806U, 0x00001726U);
        if (child.status != TranslationStatus::complete)
            return child;
        r.data[1] = r.data[2];
        logic_long(r, r.data[1]);

        pf(host, 0x0000172aU);
        wait_bit0_clear(host, r, 0x00001728U, fdc);
        pf(host, 0x00001730U);
        pf(host, 0x00001732U);
        pf(host, 0x00001734U);
        pf(host, 0x00001736U);
        write_hardware_byte(host, 0x00001730U, fdc + 4U, 1U);
        logic_byte(r, 1U);

        pf(host, 0x00001738U);
        wait_bit0_clear(host, r, 0x00001736U, fdc);
        pf(host, 0x0000173eU);
        pf(host, 0x00001740U);
        pf(host, 0x00001742U);
        pf(host, 0x00001744U);
        write_hardware_byte(host, 0x0000173eU, fdc + 6U, 7U);
        logic_byte(r, 7U);

        pf(host, 0x00001746U);
        r.data[0] = (r.data[0] & 0xffffff00U) | 0xb6U;
        logic_byte(r, 0xb6U);
        pf(host, 0x00001748U);
        pf(host, 0x0000174aU);
        bit_test(r, (r.data[7] & 1U) != 0U);
        pf(host, 0x0000174cU);
        if ((r.data[7] & 1U) != 0U) {
            pf(host, 0x0000174eU);
            pf(host, 0x00001750U);
            bit_test(r, (r.data[0] & 8U) != 0U);
            r.data[0] |= 8U;
        } else {
            pf(host, 0x0000174eU);
        }

        pf(host, 0x00001752U);
        pf(host, 0x00001754U);
        wait_bit0_clear(host, r, 0x00001752U, fdc);
        pf(host, 0x0000175aU);
        pf(host, 0x0000175cU);
        pf(host, 0x0000175eU);
        write_hardware_byte(host, 0x0000175aU, fdc,
            static_cast<std::uint8_t>(r.data[0]));
        logic_byte(r, static_cast<std::uint8_t>(r.data[0]));
        pf(host, 0x00001760U);
        r.data[2] = (r.data[2] & 0xffff0000U) | 0x2cffU;
        logic_word(r, 0x2cffU);

        for (;;) {
            pf(host, 0x00001762U);
            pf(host, 0x00001764U);
            const auto value = read_shared_byte(host, r.address[0]);
            ++r.address[0];
            r.data[0] = (r.data[0] & 0xffffff00U) | value;
            logic_byte(r, value);
            pf(host, 0x00001766U);
            child = call_child(context, 25U, 0x00001764U,
                0x0000193cU, 0x00001768U);
            if (child.status != TranslationStatus::complete)
                return child;

            pf(host, 0x0000176cU);
            pf(host, 0x0000176eU);
            const auto irq = read_hardware_byte(host, 0x00001768U, fdc + 8U);
            bit_test(r, (irq & 2U) != 0U);
            pf(host, 0x00001770U);
            if ((r.status & 4U) != 0U) {
                const auto counter = static_cast<std::uint16_t>(r.data[2] - 1U);
                r.data[2] = (r.data[2] & 0xffff0000U) | counter;
                if (counter != 0xffffU)
                    continue;
            }
            break;
        }

        pf(host, 0x00001772U);
        pf(host, 0x00001774U);
        write_shared_byte(host, r.address[0], static_cast<std::uint8_t>(r.data[7]));
        logic_byte(r, static_cast<std::uint8_t>(r.data[7]));
        pf(host, 0x00001776U);
        wait_bit0_clear(host, r, 0x00001774U, fdc);
        pf(host, 0x0000177cU);
        pf(host, 0x0000177eU);
        pf(host, 0x00001780U);
        const auto status = read_hardware_byte(host, 0x0000177cU, fdc);
        r.data[0] = (r.data[0] & 0xffffff00U) | status;
        logic_byte(r, status);
        pf(host, 0x00001782U);
        const auto masked = static_cast<std::uint8_t>(status & 0xfcU);
        r.data[0] = (r.data[0] & 0xffffff00U) | masked;
        logic_byte(r, masked);
        pf(host, 0x00001784U);
        r.status = host.apply_controlled_status(
            0U, 0xffU, 0x00001784U, fdc & ~1U, r.status);
        pf(host, 0x00001786U);
        if ((r.status & 4U) != 0U) {
            pf(host, 0x00001794U);
            pf(host, 0x00001796U);
            return return_from_subroutine(context);
        }

        pf(host, 0x00001788U);
        pf(host, 0x0000178aU);
        r.address[0] = r.address[2];
        child = call_child(context, 21U, 0x00001788U,
            0x000017e4U, 0x0000178cU);
        if (child.status != TranslationStatus::complete)
            return child;
        const auto retry = static_cast<std::uint16_t>(r.data[5] - 1U);
        r.data[5] = (r.data[5] & 0xffff0000U) | retry;
        pf(host, 0x00001720U);
        if (retry != 0xffffU)
            continue;

        pf(host, 0x00001790U);
        pf(host, 0x00001792U);
        r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | 1U);
        pf(host, 0x00001794U);
        pf(host, 0x00001794U);
        pf(host, 0x00001796U);
        return return_from_subroutine(context);
    }
}

} // namespace gain_ground::translated

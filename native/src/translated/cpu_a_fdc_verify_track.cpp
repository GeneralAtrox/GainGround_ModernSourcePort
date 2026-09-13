#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgram = 1U;
constexpr std::uint16_t kShared = 3U;
constexpr std::uint16_t kWord = 0xffffU;
constexpr std::uint16_t kByte = 0x00ffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void prefetch(ExecutionHost &host, std::uint32_t pc)
{
    (void)host.read_memory_word(kProgram, pc, kWord);
}

void logic_byte(CpuRegisters &r, std::uint8_t value)
{
    r.status = static_cast<std::uint16_t>((r.status & ~0x000fU)
        | (value == 0U ? 0x0004U : 0U)
        | ((value & 0x80U) != 0U ? 0x0008U : 0U));
}

void test_bit(CpuRegisters &r, bool set)
{
    r.status = static_cast<std::uint16_t>((r.status & ~0x0004U)
        | (set ? 0U : 0x0004U));
}

std::uint8_t hardware(ExecutionHost &host, std::uint32_t pc, std::uint32_t address)
{
    return static_cast<std::uint8_t>(host.read_hardware(1U, 0U, 0xffU, pc,
        address & ~1U, kByte));
}

void write_hardware(ExecutionHost &host, CpuRegisters &r, std::uint32_t pc,
                    std::uint32_t address, std::uint8_t value)
{
    host.write_hardware(2U, 0U, 0xffU, pc, address & ~1U,
        static_cast<std::uint16_t>(value) * 0x0101U, kByte);
    logic_byte(r, value);
}

void busy_wait(ExecutionHost &host, CpuRegisters &r, std::uint32_t test_pc,
               std::uint32_t branch_pc, std::uint32_t fdc)
{
    for (;;) {
        prefetch(host, test_pc + 4U);
        prefetch(host, branch_pc);
        const auto value = hardware(host, test_pc, fdc);
        test_bit(r, (value & 1U) != 0U);
        if ((value & 1U) == 0U)
            break;
        prefetch(host, test_pc);
        prefetch(host, test_pc + 2U);
    }
}

FunctionResult return_from_subroutine(FunctionContext &context)
{
    auto &host = *context.host;
    auto &r = context.registers;
    const auto sp = r.address[7] & kSharedMask;
    const auto target = (static_cast<std::uint32_t>(host.read_memory_word(kShared, sp, kWord)) << 16U)
        | host.read_memory_word(kShared, sp + 2U, kWord);
    r.address[7] += 4U;
    prefetch(host, target);
    prefetch(host, target + 2U);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_a_fdc_verify_track(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto fdc = r.address[5];

    prefetch(host, 0x000016a4U);
    prefetch(host, 0x000016a6U);
    const auto present = hardware(host, 0x000016a0U, fdc + 8U);
    test_bit(r, (present & 0x80U) != 0U);
    if ((present & 0x80U) == 0U) {
        prefetch(host, 0x000016a8U);
        prefetch(host, 0x00001954U);
        prefetch(host, 0x00001956U);
        r.program_counter = 0x00001954U;
        return FunctionResult::complete(3U, 0x00001954U);
    }

    prefetch(host, 0x000016a8U);
    prefetch(host, 0x000016aaU);
    r.address[0] = r.address[2];
    prefetch(host, 0x000016acU);
    prefetch(host, 0x000016aeU);
    busy_wait(host, r, 0x000016acU, 0x000016b2U, fdc);
    prefetch(host, 0x000016b4U);
    prefetch(host, 0x000016b6U);
    prefetch(host, 0x000016b8U);
    prefetch(host, 0x000016baU);
    write_hardware(host, r, 0x000016b4U, fdc + 4U, 1U);
    prefetch(host, 0x000016bcU);
    busy_wait(host, r, 0x000016baU, 0x000016c0U, fdc);
    prefetch(host, 0x000016c2U);
    prefetch(host, 0x000016c4U);
    prefetch(host, 0x000016c6U);
    prefetch(host, 0x000016c8U);
    write_hardware(host, r, 0x000016c2U, fdc + 6U, 7U);
    prefetch(host, 0x000016caU);
    r.data[0] = (r.data[0] & 0xffffff00U) | 0x96U;
    logic_byte(r, 0x96U);
    prefetch(host, 0x000016ccU);
    prefetch(host, 0x000016ceU);
    test_bit(r, (r.data[7] & 1U) != 0U);
    prefetch(host, 0x000016d0U);
    if ((r.data[7] & 1U) != 0U) {
        prefetch(host, 0x000016d2U);
        prefetch(host, 0x000016d4U);
        test_bit(r, (r.data[0] & 8U) != 0U);
        r.data[0] |= 8U;
    } else {
        prefetch(host, 0x000016d2U);
    }
    prefetch(host, 0x000016d6U);
    prefetch(host, 0x000016d8U);
    busy_wait(host, r, 0x000016d6U, 0x000016dcU, fdc);
    prefetch(host, 0x000016deU);
    prefetch(host, 0x000016e0U);
    prefetch(host, 0x000016e2U);
    write_hardware(host, r, 0x000016deU, fdc, static_cast<std::uint8_t>(r.data[0]));
    prefetch(host, 0x000016e4U);
    r.data[2] = (r.data[2] & 0xffff0000U) | 0x2cffU;
    r.status = static_cast<std::uint16_t>(r.status & ~0x000fU);

    for (;;) {
        prefetch(host, 0x000016e6U);
        prefetch(host, 0x000016e8U);
        r.address[7] -= 4U;
        const auto stack = r.address[7] & kSharedMask;
        host.write_memory_word(kShared, stack, 0U, kWord);
        host.write_memory_word(kShared, stack + 2U, 0x16eaU, kWord);
        prefetch(host, 0x00001924U);
        prefetch(host, 0x00001926U);
        const auto child = host.call_function(24U, 0U, 0xffU, 2U,
            0x000016e6U, 0x00001924U, context);
        if (child.status != TranslationStatus::complete)
            return child;

        const auto word = host.read_memory_word(kShared,
            (r.address[0] & kSharedMask) & ~1U,
            (r.address[0] & 1U) != 0U ? 0x00ffU : 0xff00U);
        const auto input = static_cast<std::uint8_t>((r.address[0] & 1U) != 0U
            ? word : word >> 8U);
        ++r.address[0];
        const auto value = static_cast<std::uint8_t>(r.data[0]);
        const auto difference = static_cast<std::uint8_t>(value - input);
        const auto overflow = ((value ^ input) & (value ^ difference) & 0x80U) != 0U;
        r.status = static_cast<std::uint16_t>((r.status & ~0x000fU)
            | (difference == 0U ? 4U : 0U) | ((difference & 0x80U) != 0U ? 8U : 0U)
            | (overflow ? 2U : 0U) | (input > value ? 1U : 0U));
        if (difference != 0U) {
            prefetch(host, 0x0000170aU);
            prefetch(host, 0x0000170cU);
            r.status = static_cast<std::uint16_t>((r.status & 0xffe0U) | 1U);
            prefetch(host, 0x0000170eU);
            return return_from_subroutine(context);
        }

        prefetch(host, 0x000016eeU);
        prefetch(host, 0x000016f0U);
        prefetch(host, 0x000016f2U);
        prefetch(host, 0x000016f4U);
        test_bit(r, (hardware(host, 0x000016eeU, fdc + 8U) & 2U) != 0U);
        prefetch(host, 0x000016f6U);
        if ((r.status & 4U) != 0U) {
            const auto counter = static_cast<std::uint16_t>(r.data[2] - 1U);
            r.data[2] = (r.data[2] & 0xffff0000U) | counter;
            if (counter != 0xffffU)
                continue;
        }
        break;
    }

    prefetch(host, 0x000016f8U);
    prefetch(host, 0x000016faU);
    busy_wait(host, r, 0x000016f8U, 0x000016feU, fdc);
    prefetch(host, 0x00001700U);
    prefetch(host, 0x00001702U);
    prefetch(host, 0x00001704U);
    const auto status = hardware(host, 0x00001700U, fdc);
    r.data[0] = (r.data[0] & 0xffffff00U) | status;
    logic_byte(r, status);
    prefetch(host, 0x00001706U);
    const auto masked = static_cast<std::uint8_t>(status & 0xfcU);
    r.data[0] = (r.data[0] & 0xffffff00U) | masked;
    logic_byte(r, masked);
    prefetch(host, 0x00001708U);
    prefetch(host, 0x0000170aU);
    if (masked != 0U) {
        prefetch(host, 0x0000170cU);
        r.status = static_cast<std::uint16_t>((r.status & 0xffe0U) | 1U);
    }
    prefetch(host, 0x0000170eU);
    prefetch(host, 0x00001710U);
    return return_from_subroutine(context);
}
} // namespace gain_ground::translated

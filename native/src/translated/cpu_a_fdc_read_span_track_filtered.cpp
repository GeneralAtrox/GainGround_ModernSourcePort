#include "gain_ground/contract_types.h"
#include "cpu_a_fdc_read_span_detail.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgram = 1U;
constexpr std::uint16_t kShared = 3U;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void pf(ExecutionHost &host, std::uint32_t pc)
{
    (void)host.read_memory_word(kProgram, pc, 0xffffU);
}

void logic(CpuRegisters &r, std::uint32_t value, std::uint32_t sign)
{
    r.status = static_cast<std::uint16_t>((r.status & ~0x000fU)
        | (value == 0U ? 0x0004U : 0U)
        | ((value & sign) != 0U ? 0x0008U : 0U));
}

void bit_test(CpuRegisters &r, bool set)
{
    r.status = static_cast<std::uint16_t>((r.status & ~0x0004U)
        | (set ? 0U : 0x0004U));
}

void subtract_flags(CpuRegisters &r, std::uint32_t destination,
                    std::uint32_t source, std::uint32_t result,
                    std::uint32_t sign, std::uint32_t mask)
{
    destination &= mask;
    source &= mask;
    result &= mask;
    std::uint16_t flags = r.status & 0x0010U;
    if ((result & sign) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((destination ^ source) & (destination ^ result) & sign) != 0U)
        flags |= 0x0002U;
    if (source > destination) flags |= 0x0001U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

std::uint8_t read_hardware_byte(ExecutionHost &host, std::uint32_t pc,
                                std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto value = host.read_hardware(
        1U, 0U, 0xffU, pc, address & ~1U, mask);
    return static_cast<std::uint8_t>(odd ? value : value >> 8U);
}

void write_hardware_byte(ExecutionHost &host, std::uint32_t pc,
                         std::uint32_t address, std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    host.write_hardware(2U, 0U, 0xffU, pc, address & ~1U,
        static_cast<std::uint16_t>(value) * 0x0101U,
        odd ? 0x00ffU : 0xff00U);
}

std::uint8_t read_shared_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto word = host.read_memory_word(kShared,
        (address & kSharedMask) & ~1U, odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

std::uint32_t read_shared_long(ExecutionHost &host, std::uint32_t address)
{
    const auto offset = address & kSharedMask;
    return (static_cast<std::uint32_t>(host.read_memory_word(
                kShared, offset, 0xffffU)) << 16U)
        | host.read_memory_word(
            kShared, (offset + 2U) & kSharedMask, 0xffffU);
}

void push_long(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    const auto offset = r.address[7] & kSharedMask;
    host.write_memory_word(kShared, offset,
        static_cast<std::uint16_t>(value >> 16U), 0xffffU);
    host.write_memory_word(kShared, (offset + 2U) & kSharedMask,
        static_cast<std::uint16_t>(value), 0xffffU);
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

void wait_bit0_clear(FunctionContext &context, std::uint32_t test_pc,
                     std::uint32_t branch_pc, std::uint32_t address)
{
    auto &host = *context.host;
    auto &r = context.registers;
    bool target_prefetched = false;
    for (;;) {
        if (!target_prefetched)
            pf(host, test_pc + 2U);
        pf(host, test_pc + 4U);
        pf(host, test_pc + 6U);
        const auto value = read_hardware_byte(host, test_pc, address);
        bit_test(r, (value & 1U) != 0U);
        r.status = host.apply_controlled_status(
            0U, 0xffU, branch_pc, address & ~1U, r.status);
        if ((r.status & 0x0004U) != 0U)
            return;
        pf(host, branch_pc + 2U);
        pf(host, test_pc);
        pf(host, test_pc + 2U);
        target_prefetched = true;
    }
}

FunctionResult continue_at(FunctionContext &context, std::uint32_t pc)
{
    context.registers.program_counter = pc;
    return cpu_a_fdc_read_span_continuation(context, pc);
}
} // namespace

FunctionResult cpu_a_fdc_read_span_track_filtered(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr
        || context.registers.program_counter != 0x00000ee4U) {
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};
    }

    auto &host = *context.host;
    auto &r = context.registers;

    pf(host, 0x00000ee8U);
    pf(host, 0x00000eeaU);
    const auto original = read_hardware_byte(host, 0x00000ee4U, 0x00b00005U);
    r.data[2] = (r.data[2] & 0xffffff00U) | original;
    logic(r, original, 0x80U);
    pf(host, 0x00000eecU);
    const auto inverted = static_cast<std::uint8_t>(~original);
    r.data[2] = (r.data[2] & 0xffffff00U) | inverted;
    logic(r, inverted, 0x80U);
    pf(host, 0x00000eeeU);
    pf(host, 0x00000ef0U);
    pf(host, 0x00000ef2U);
    write_hardware_byte(host, 0x00000eecU, 0x00b00005U, inverted);
    logic(r, inverted, 0x80U);

    pf(host, 0x00000ef4U);
    r.address[7] -= 4U;
    logic(r, read_shared_long(host, r.address[7]), 0x80000000U);
    pf(host, 0x00000ef6U);
    const auto stack_probe = read_shared_long(host, r.address[7]);
    r.address[7] += 4U;
    logic(r, stack_probe, 0x80000000U);

    pf(host, 0x00000ef8U);
    pf(host, 0x00000efaU);
    pf(host, 0x00000efcU);
    const auto current = read_hardware_byte(host, 0x00000ef6U, 0x00b00005U);
    subtract_flags(r, inverted, current,
        static_cast<std::uint8_t>(inverted - current), 0x80U, 0xffU);
    r.status = host.apply_controlled_status(
        0U, 0xffU, 0x00000efcU, 0x00b00004U, r.status);
    pf(host, 0x00000efeU);
    if ((r.status & 0x0004U) == 0U) {
        pf(host, 0x00000eb2U);
        return continue_at(context, 0x00000eb2U);
    }

    pf(host, 0x00000f00U);
    pf(host, 0x00000f02U);
    pf(host, 0x00000f04U);
    r.address[5] = 0x00b00001U;
    pf(host, 0x00000f06U);
    r.data[2] = r.data[0];
    logic(r, r.data[2], 0x80000000U);
    pf(host, 0x00000f08U);
    r.data[4] = r.data[1];
    logic(r, r.data[4], 0x80000000U);

    pf(host, 0x00000f0aU);
    wait_bit0_clear(context, 0x00000f0aU, 0x00000f10U, r.address[5]);
    pf(host, 0x00000f12U);
    pf(host, 0x00000f14U);
    pf(host, 0x00000f16U);
    pf(host, 0x00000f18U);
    write_hardware_byte(host, 0x00000f12U, r.address[5], 0xd0U);
    logic(r, 0xd0U, 0x80U);
    wait_bit0_clear(context, 0x00000f18U, 0x00000f1eU, r.address[5]);
    pf(host, 0x00000f20U);
    pf(host, 0x00000f22U);
    pf(host, 0x00000f24U);
    pf(host, 0x00000f26U);
    write_hardware_byte(host, 0x00000f20U, r.address[5] + 6U, 0xc0U);
    logic(r, 0xc0U, 0x80U);
    pf(host, 0x00000f28U);
    pf(host, 0x00000f2aU);
    pf(host, 0x00000f2cU);
    write_hardware_byte(host, 0x00000f26U, r.address[5], 0xfeU);
    logic(r, 0xfeU, 0x80U);
    wait_bit0_clear(context, 0x00000f2cU, 0x00000f32U, r.address[5]);
    pf(host, 0x00000f34U);
    pf(host, 0x00000f36U);
    pf(host, 0x00000f38U);
    pf(host, 0x00000f3aU);
    write_hardware_byte(host, 0x00000f34U, r.address[5] + 6U, 0x8aU);
    logic(r, 0x8aU, 0x80U);
    pf(host, 0x00000f3cU);
    pf(host, 0x00000f3eU);
    pf(host, 0x00000f40U);
    write_hardware_byte(host, 0x00000f3aU, r.address[5], 0xfdU);
    logic(r, 0xfdU, 0x80U);
    r.data[1] = r.data[4];
    logic(r, r.data[1], 0x80000000U);
    pf(host, 0x00000f42U);
    r.data[0] = r.data[2];
    logic(r, r.data[0], 0x80000000U);

    pf(host, 0x00000f44U);
    pf(host, 0x00000f46U);
    const auto child = call_child(context, 18U, 0x00000f44U,
        0x00001796U, 0x00000f48U);
    if (child.status != TranslationStatus::complete)
        return child;
    r.address[2] = r.address[0];

    pf(host, 0x00000f4cU);
    pf(host, 0x00000f4eU);
    const auto requested_track = read_shared_byte(host, r.address[0] + 0x2d00U);
    subtract_flags(r, static_cast<std::uint8_t>(r.data[7]), requested_track,
        static_cast<std::uint8_t>(r.data[7] - requested_track), 0x80U, 0xffU);
    pf(host, 0x00000f50U);
    if ((r.status & 0x0004U) != 0U) {
        pf(host, 0x00000e8cU);
        return continue_at(context, 0x00000e8cU);
    }
    pf(host, 0x00000f52U);
    pf(host, 0x00000f54U);
    pf(host, 0x00000e84U);
    return continue_at(context, 0x00000e84U);
}

} // namespace gain_ground::translated

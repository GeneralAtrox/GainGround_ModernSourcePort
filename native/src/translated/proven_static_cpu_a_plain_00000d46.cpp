#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgram = 1U;
constexpr std::uint16_t kShared = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kByteMask = 0x00ffU;

void pf(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgram, address, kWordMask);
}

std::uint8_t rb(ExecutionHost &host, std::uint32_t address)
{
    const auto word = host.read_memory_word(kProgram, address & ~1U,
        (address & 1U) == 0U ? 0xff00U : kByteMask);
    return static_cast<std::uint8_t>((address & 1U) == 0U ? word >> 8U : word);
}

std::uint16_t rw(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kProgram, address, kWordMask);
}

std::uint16_t rs(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kShared, address & 0x0003ffffU, kWordMask);
}

void ws(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kShared, address & 0x0003ffffU, value, kWordMask);
}

std::uint8_t rh(ExecutionHost &host, std::uint32_t pc, std::uint32_t address)
{
    return static_cast<std::uint8_t>(host.read_hardware(
        1U, 0U, 0xffU, pc, address & ~1U, kByteMask));
}

void wh(ExecutionHost &host, std::uint32_t pc,
    std::uint32_t address, std::uint8_t value)
{
    host.write_hardware(2U, 0U, 0xffU, pc, address & ~1U,
        static_cast<std::uint16_t>(value) * 0x0101U, kByteMask);
}

void logic8(CpuRegisters &r, std::uint8_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void logic16(CpuRegisters &r, std::uint16_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void logic32(CpuRegisters &r, std::uint32_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void bit_test(CpuRegisters &r, bool set)
{
    r.status = static_cast<std::uint16_t>(
        (r.status & ~0x0004U) | (set ? 0U : 0x0004U));
}

void push_long(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    ws(host, r.address[7], static_cast<std::uint16_t>(value >> 16U));
    ws(host, r.address[7] + 2U, static_cast<std::uint16_t>(value));
}

FunctionResult call(FunctionContext &context, std::uint32_t function_id,
    std::uint32_t site, std::uint32_t target, std::uint32_t return_address)
{
    auto &host = *context.host;
    auto &r = context.registers;
    push_long(host, r, return_address);
    pf(host, target);
    pf(host, target + 2U);
    r.program_counter = target;
    return host.call_function(function_id, 0U, 0xffU, 2U,
        site, target, context);
}

FunctionResult branch_error(FunctionContext &context, std::uint32_t site)
{
    auto &host = *context.host;
    pf(host, 0x0000196cU);
    pf(host, 0x0000196eU);
    context.registers.program_counter = 0x0000196cU;
    return host.call_function(613U, 0U, 0xffU, 1U,
        site, 0x0000196cU, context);
}

FunctionResult rts(FunctionContext &context)
{
    auto &host = *context.host;
    auto &r = context.registers;
    const auto stack = r.address[7] & 0x0003ffffU;
    const auto target = (static_cast<std::uint32_t>(rs(host, stack)) << 16U)
        | rs(host, stack + 2U);
    r.address[7] += 4U;
    pf(host, target);
    pf(host, target + 2U);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

bool completed(FunctionResult const &result)
{
    return result.status == TranslationStatus::complete;
}
} // namespace

FunctionResult proven_static_cpu_a_plain_00000d46(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;

    const auto retry_count = rw(host, r.address[0]);
    r.address[0] += 2U;
    r.address[7] -= 2U;
    pf(host, 0x00000d4aU);
    ws(host, r.address[7], retry_count);
    logic16(r, retry_count);

    r.data[6] = 1U;
    logic32(r, 1U);

    bool initial_status_test = true;
    for (;;) {
        if (!initial_status_test)
            pf(host, 0x00000d4aU);
        initial_status_test = false;
        pf(host, 0x00000d4cU);
        pf(host, 0x00000d4eU);
        pf(host, 0x00000d50U);
        const auto status = rh(host, 0x00000d4aU, r.address[5]);
        bit_test(r, (status & 1U) != 0U);
        pf(host, 0x00000d52U);
        r.status = host.apply_controlled_status(0U, 0xffU,
            0x00000d50U, r.address[5] & ~1U, r.status);
        if ((r.status & 0x0004U) != 0U) break;
    }

    pf(host, 0x00000d54U);
    pf(host, 0x00000d56U);
    pf(host, 0x00000d58U);
    wh(host, 0x00000d52U, r.address[5], 0xf4U);
    logic8(r, 0xf4U);

    r.data[4] = 0U;
    logic32(r, 0U);
    pf(host, 0x00000d5aU);
    pf(host, 0x00000d5cU);
    auto child = call(context, 437U, 0x00000d5aU,
        0x00000df6U, 0x00000d5eU);
    if (!completed(child)) return child;

read_outer_count:
    r.data[2] = 0U;
    logic32(r, 0U);
    pf(host, 0x00000d62U);
    const auto outer_count = rb(host, r.address[0]++);
    r.data[2] = outer_count;
    logic8(r, outer_count);
    r.address[1] = r.address[0];

outer_loop:
    pf(host, 0x00000d64U);
    r.address[0] = r.address[1];
    pf(host, 0x00000d66U);
    r.data[4] = 2U;
    logic32(r, 2U);
    pf(host, 0x00000d68U);
    pf(host, 0x00000d6aU);
    child = call(context, 437U, 0x00000d68U,
        0x00000df6U, 0x00000d6cU);
    if (!completed(child)) return child;

    r.data[0] = (r.data[0] & 0xffff0000U)
        | static_cast<std::uint16_t>(r.data[7]);
    logic16(r, static_cast<std::uint16_t>(r.data[0]));
    const auto prior_byte = static_cast<std::uint8_t>(r.data[0]);
    const auto shifted = static_cast<std::uint8_t>(prior_byte >> 1U);
    r.data[0] = (r.data[0] & 0xffffff00U) | shifted;
    {
        std::uint16_t flags{};
        if (shifted == 0U) flags |= 0x0004U;
        if ((prior_byte & 1U) != 0U) flags |= 0x0011U;
        r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
    }
    pf(host, 0x00000d70U);
    pf(host, 0x00000d72U);
    child = call(context, 25U, 0x00000d70U,
        0x0000193cU, 0x00000d74U);
    if (!completed(child)) return child;

    auto error = [&](std::uint32_t test_pc, std::uint32_t branch_pc,
                     std::uint32_t fallthrough) -> FunctionResult {
        pf(host, test_pc + 4U);
        pf(host, branch_pc);
        const auto observed = rh(host, test_pc, r.address[5] + 8U);
        bit_test(r, (observed & 2U) != 0U);
        pf(host, branch_pc + 2U);
        r.status = host.apply_controlled_status(0U, 0xffU, branch_pc,
            (r.address[5] + 8U) & ~1U, r.status);
        if ((r.status & 0x0004U) == 0U)
            return branch_error(context, branch_pc);
        pf(host, fallthrough);
        return FunctionResult::complete(0U, fallthrough);
    };

    auto checked = error(0x00000d74U, 0x00000d7aU, 0x00000d7eU);
    if (checked.control != 0U) return checked;

    r.data[0] = (r.data[0] & 0xffff0000U)
        | static_cast<std::uint16_t>(r.data[7]);
    logic16(r, static_cast<std::uint16_t>(r.data[0]));
    pf(host, 0x00000d80U); pf(host, 0x00000d82U);
    const auto and_value = static_cast<std::uint8_t>(r.data[0] & 1U);
    r.data[0] = (r.data[0] & 0xffffff00U) | and_value;
    logic8(r, and_value);
    pf(host, 0x00000d84U); pf(host, 0x00000d86U);
    child = call(context, 25U, 0x00000d84U,
        0x0000193cU, 0x00000d88U);
    if (!completed(child)) return child;
    checked = error(0x00000d88U, 0x00000d8eU, 0x00000d92U);
    if (checked.control != 0U) return checked;

    const auto d6_byte = static_cast<std::uint8_t>(r.data[6]);
    r.data[0] = (r.data[0] & 0xffffff00U) | d6_byte;
    logic8(r, d6_byte);
    pf(host, 0x00000d94U);
    {
        const auto before = static_cast<std::uint8_t>(r.data[6]);
        const auto result = static_cast<std::uint8_t>(before + 1U);
        r.data[6] = (r.data[6] & 0xffffff00U) | result;
        std::uint16_t flags{};
        if ((result & 0x80U) != 0U) flags |= 0x0008U;
        if (result == 0U) flags |= 0x0004U;
        if (before == 0x7fU) flags |= 0x0002U;
        if (before == 0xffU) flags |= 0x0011U;
        r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
    }
    pf(host, 0x00000d96U); pf(host, 0x00000d98U);
    child = call(context, 25U, 0x00000d96U,
        0x0000193cU, 0x00000d9aU);
    if (!completed(child)) return child;
    checked = error(0x00000d9aU, 0x00000da0U, 0x00000da4U);
    if (checked.control != 0U) return checked;

    pf(host, 0x00000da6U);
    const auto stream_byte = rb(host, r.address[0]++);
    r.data[0] = (r.data[0] & 0xffffff00U) | stream_byte;
    logic8(r, stream_byte);
    pf(host, 0x00000da8U);
    child = call(context, 25U, 0x00000da6U,
        0x0000193cU, 0x00000daaU);
    if (!completed(child)) return child;
    checked = error(0x00000daaU, 0x00000db0U, 0x00000db4U);
    if (checked.control != 0U) return checked;

    r.data[4] = 4U;
    logic32(r, 4U);
    pf(host, 0x00000db6U); pf(host, 0x00000db8U);
    child = call(context, 437U, 0x00000db6U,
        0x00000df6U, 0x00000dbaU);
    if (!completed(child)) return child;

    r.data[1] = (r.data[1] & 0xffff0000U) | rw(host, r.address[0]);
    r.address[0] += 2U;
    logic16(r, static_cast<std::uint16_t>(r.data[1]));

    pf(host, 0x00000dbeU);
    const auto payload = rb(host, r.address[0]++);
    r.data[0] = (r.data[0] & 0xffffff00U) | payload;
    logic8(r, payload);
    pf(host, 0x00000dc0U);
    child = call(context, 25U, 0x00000dbeU,
        0x0000193cU, 0x00000dc2U);
    if (!completed(child)) return child;

after_payload_write:
    pf(host, 0x00000dc6U);
    pf(host, 0x00000dc8U);
    const auto transfer_status = rh(host, 0x00000dc2U, r.address[5] + 8U);
    bit_test(r, (transfer_status & 2U) != 0U);
    pf(host, 0x00000dcaU);
    if ((r.status & 0x0004U) != 0U) {
        const auto next = static_cast<std::uint16_t>(r.data[1] - 1U);
        r.data[1] = (r.data[1] & 0xffff0000U) | next;
        if (next != 0xffffU) {
            pf(host, 0x00000dbeU);
            pf(host, 0x00000dc0U);
            r.address[7] -= 4U;
            ws(host, r.address[7], 0x0000U);
            r.status = host.apply_controlled_status(0U, 0xffU,
                0x00000dccU, (r.address[5] + 8U) & ~1U, r.status);
            ws(host, r.address[7] + 2U, 0x00000dc2U);
            pf(host, 0x0000193cU);
            pf(host, 0x0000193eU);
            r.program_counter = 0x0000193cU;
            child = host.call_function(25U, 0U, 0xffU, 2U,
                0x00000dbeU, 0x0000193cU, context);
            if (!completed(child)) return child;
            goto after_payload_write;
        }
    }
    pf(host, 0x00000dbeU);
    pf(host, 0x00000dccU); pf(host, 0x00000dceU);
    r.status = host.apply_controlled_status(0U, 0xffU,
        0x00000dccU, (r.address[5] + 8U) & ~1U, r.status);
    if ((r.status & 0x0004U) == 0U)
        return branch_error(context, 0x00000dccU);

    pf(host, 0x00000dd0U);
    r.data[4] = 1U;
    logic32(r, 1U);
    pf(host, 0x00000dd2U); pf(host, 0x00000dd4U);
    child = call(context, 437U, 0x00000dd2U,
        0x00000df6U, 0x00000dd6U);
    if (!completed(child)) return child;

    {
        const auto next = static_cast<std::uint16_t>(r.data[2] - 1U);
        r.data[2] = (r.data[2] & 0xffff0000U) | next;
        if (next != 0xffffU) goto outer_loop;
        pf(host, 0x00000d64U);
    }

    pf(host, 0x00000ddaU);
    ++r.address[0];
    pf(host, 0x00000ddcU);
    {
        pf(host, 0x00000ddeU);
        const auto stack_value = rs(host, r.address[7]);
        pf(host, 0x00000de0U);
        const auto next = static_cast<std::uint16_t>(stack_value - 1U);
        ws(host, r.address[7], next);
        std::uint16_t flags{};
        if ((next & 0x8000U) != 0U) flags |= 0x0008U;
        if (next == 0U) flags |= 0x0004U;
        if (stack_value == 0x8000U) flags |= 0x0002U;
        if (stack_value == 0U) flags |= 0x0011U;
        r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
    }
    if ((r.status & 0x0004U) == 0U) {
        pf(host, 0x00000d5eU);
        pf(host, 0x00000d60U);
        goto read_outer_count;
    }

    pf(host, 0x00000de2U);
    r.address[7] += 2U;
    pf(host, 0x00000de4U); pf(host, 0x00000de6U);
    r.data[0] = (r.data[0] & 0xffffff00U) | 0x4eU;
    logic8(r, 0x4eU);

final_wait:
    pf(host, 0x00000de8U); pf(host, 0x00000deaU);
    child = call(context, 25U, 0x00000de8U,
        0x0000193cU, 0x00000decU);
    if (!completed(child)) return child;
    pf(host, 0x00000df0U);
    pf(host, 0x00000df2U);
    const auto final_status = rh(host, 0x00000decU, r.address[5] + 8U);
    bit_test(r, (final_status & 2U) != 0U);
    r.status = host.apply_controlled_status(0U, 0xffU,
        0x00000df2U, (r.address[5] + 8U) & ~1U, r.status);
    pf(host, 0x00000df4U);
    if ((r.status & 0x0004U) != 0U) goto final_wait;
    pf(host, 0x00000df6U);
    return rts(context);
}

} // namespace gain_ground::translated

#include "gain_ground/contract_types.h"

#include <array>
#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kMask = 0xffffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kSharedRegion, address & kSharedMask, kMask);
}

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kSharedRegion, address & kSharedMask, kMask);
}

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = read_word(host, address);
    const auto low = read_word(host, address + 2U);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long_predecrement(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    host.write_memory_word(kSharedRegion, (r.address[7] + 2U) & kSharedMask,
        static_cast<std::uint16_t>(value), kMask);
    host.write_memory_word(kSharedRegion, r.address[7] & kSharedMask,
        static_cast<std::uint16_t>(value >> 16U), kMask);
}

void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    host.write_memory_word(kSharedRegion, r.address[7] & kSharedMask,
        static_cast<std::uint16_t>(value >> 16U), kMask);
    host.write_memory_word(kSharedRegion, (r.address[7] + 2U) & kSharedMask,
        static_cast<std::uint16_t>(value), kMask);
}

void save_all(ExecutionHost &host, CpuRegisters &r)
{
    const std::array<std::uint32_t, 15> values{
        r.address[6], r.address[5], r.address[4], r.address[3], r.address[2],
        r.address[1], r.address[0], r.data[7], r.data[6], r.data[5],
        r.data[4], r.data[3], r.data[2], r.data[1], r.data[0]};
    for (const auto value : values) write_long_predecrement(host, r, value);
}

void restore_all(ExecutionHost &host, CpuRegisters &r)
{
    for (unsigned i = 0; i != 8U; ++i) {
        r.data[i] = read_long(host, r.address[7]);
        r.address[7] += 4U;
    }
    for (unsigned i = 0; i != 7U; ++i) {
        r.address[i] = read_long(host, r.address[7]);
        r.address[7] += 4U;
    }
}

FunctionResult rte(ExecutionHost &host, CpuRegisters &r, std::uint32_t next_word)
{
    (void)read_word(host, r.address[7]);
    prefetch(host, next_word);
    const auto status = read_word(host, r.address[7]);
    r.address[7] += 2U;
    const auto target = read_long(host, r.address[7]);
    r.address[7] += 4U;
    prefetch(host, target);
    prefetch(host, target + 2U);
    r.status = status;
    r.program_counter = target;
    return FunctionResult::complete(2U, target);
}
} // namespace

FunctionResult cpu_a_irq4_rowscroll_service(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;

    prefetch(host, 0x00080fcaU);
    const auto gate = host.read_memory_word(kSharedRegion, 0x00038000U, 0x00ffU);
    r.status = static_cast<std::uint16_t>(
        (r.status & ~0x0004U) | ((gate & 0x00ffU) == 0U ? 0x0004U : 0U));

    if ((gate & 0x00ffU) != 0U) {
        prefetch(host, 0x00080fccU);
        prefetch(host, 0x00080f86U);
        prefetch(host, 0x00080f88U);
        prefetch(host, 0x00080f8aU);
        write_long_predecrement(host, r, r.data[0]);
        r.data[0] = 0x0000007fU;
        r.status = static_cast<std::uint16_t>(r.status & ~0x000fU);
        prefetch(host, 0x00080f8cU);
        for (;;) {
            prefetch(host, 0x00080f8eU);
            const auto counter = static_cast<std::uint16_t>(r.data[0] - 1U);
            r.data[0] = (r.data[0] & 0xffff0000U) | counter;
            prefetch(host, 0x00080f8cU);
            if (counter == 0xffffU) break;
        }
        prefetch(host, 0x00080f90U);
        prefetch(host, 0x00080f92U);
        prefetch(host, 0x00080f94U);
        r.data[0] = read_long(host, r.address[7]);
        r.address[7] += 4U;
        return rte(host, r, 0x00080f96U);
    }

    prefetch(host, 0x00080fccU);
    prefetch(host, 0x00080fceU);
    prefetch(host, 0x00080fd0U);
    save_all(host, r);
    prefetch(host, 0x00080fd2U);
    prefetch(host, 0x0008102aU);
    push_return(host, r, 0x00080fd4U);
    prefetch(host, 0x0008102cU);
    r.program_counter = 0x0008102aU;
    const auto child = host.call_function(75U, 0U, 0xffU, 2U,
        0x00080fd0U, 0x0008102aU, context);
    if (child.status != TranslationStatus::complete || r.program_counter != 0x00080fd4U)
        return child;

    prefetch(host, 0x00080fd8U);
    restore_all(host, r);
    return rte(host, r, 0x00080fdaU);
}

} // namespace gain_ground::translated

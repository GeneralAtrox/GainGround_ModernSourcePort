#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address, kMask);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address, value, kMask);
}

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

void set_logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (static_cast<std::uint32_t>(left) + right > 0xffffU)
        flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void move_word_to_memory(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t address, std::uint16_t value)
{
    write_word(host, address, value);
    set_logic_word(registers, value);
}

void clear_word(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t address)
{
    (void)read_word(host, address);
    write_word(host, address, 0U);
    set_logic_word(registers, 0U);
}
} // namespace

FunctionResult cpu_b_load_entry_pointer_18_alt(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    r.address[0] = read_long(host, r.address[3] + 0x18U);

    auto d0 = read_word(host, r.address[0]);
    r.address[0] += 2U;
    r.data[0] = (r.data[0] & 0xffff0000U) | d0;
    set_logic_word(r, d0);
    move_word_to_memory(host, r, r.address[6] + 0x16U, d0);
    clear_word(host, r, r.address[6] + 0x18U);
    move_word_to_memory(host, r, r.address[6] + 0x2eU, d0);
    move_word_to_memory(host, r, r.address[6] + 0x30U, d0);

    r.address[0] = static_cast<std::uint32_t>(r.address[0] + 0x18U);
    d0 = read_word(host, r.address[5] + 0x12U);
    r.data[0] = (r.data[0] & 0xffff0000U) | d0;
    set_logic_word(r, d0);
    move_word_to_memory(host, r, r.address[6] + 0x2aU, d0);
    move_word_to_memory(host, r, r.address[6] + 0x2cU, d0);
    auto adjustment = read_word(host, r.address[0]);
    r.address[0] += 2U;
    auto sum = static_cast<std::uint16_t>(d0 + adjustment);
    r.data[0] = (r.data[0] & 0xffff0000U) | sum;
    set_add_word(r, d0, adjustment, sum);
    move_word_to_memory(host, r, r.address[6] + 0x12U, sum);
    clear_word(host, r, r.address[6] + 0x14U);

    d0 = read_word(host, r.address[5] + 0x1aU);
    r.data[0] = (r.data[0] & 0xffff0000U) | d0;
    set_logic_word(r, d0);
    move_word_to_memory(host, r, r.address[6] + 0x32U, d0);
    move_word_to_memory(host, r, r.address[6] + 0x34U, d0);
    adjustment = read_word(host, r.address[0]);
    r.address[0] += 2U;
    sum = static_cast<std::uint16_t>(d0 + adjustment);
    r.data[0] = (r.data[0] & 0xffff0000U) | sum;
    set_add_word(r, d0, adjustment, sum);
    move_word_to_memory(host, r, r.address[6] + 0x1aU, sum);
    clear_word(host, r, r.address[6] + 0x1cU);

    const auto target = read_long(host, r.address[7]);
    r.address[7] += 4U;
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated

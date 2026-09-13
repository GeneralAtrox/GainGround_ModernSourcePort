#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;
constexpr std::uint16_t kX = 0x0010U;
constexpr std::uint16_t kN = 0x0008U;
constexpr std::uint16_t kZ = 0x0004U;
constexpr std::uint16_t kV = 0x0002U;
constexpr std::uint16_t kC = 0x0001U;

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{ return host.read_memory_word(kRegion, address & 0x00ffffffU, kMask); }

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{ host.write_memory_word(kRegion, address & 0x00ffffffU, value, kMask); }

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
}

void set_logic(CpuRegisters &r, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & kX;
    if ((value & sign) != 0U) flags |= kN;
    if ((value & mask) == 0U) flags |= kZ;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_add(CpuRegisters &r, std::uint32_t left, std::uint32_t right,
    std::uint32_t result, std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags{};
    if ((result & sign) != 0U) flags |= kN;
    if ((result & mask) == 0U) flags |= kZ;
    if (((~(left ^ right)) & (left ^ result) & sign) != 0U) flags |= kV;
    if ((left & mask) + (right & mask) > mask) flags |= kX | kC;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_sub(CpuRegisters &r, std::uint32_t left, std::uint32_t right,
    std::uint32_t result, std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags{};
    if ((result & sign) != 0U) flags |= kN;
    if ((result & mask) == 0U) flags |= kZ;
    if (((left ^ right) & (left ^ result) & sign) != 0U) flags |= kV;
    if ((right & mask) > (left & mask)) flags |= kX | kC;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

std::uint16_t add_word(CpuRegisters &r, std::uint16_t left, std::uint16_t right)
{
    const auto result = static_cast<std::uint16_t>(left + right);
    set_add(r, left, right, result, 0x8000U, 0xffffU);
    return result;
}

std::uint16_t subtract_word(CpuRegisters &r,
    std::uint16_t left, std::uint16_t right)
{
    const auto result = static_cast<std::uint16_t>(left - right);
    set_sub(r, left, right, result, 0x8000U, 0xffffU);
    return result;
}

template <unsigned Count>
std::uint16_t shift_left_word(CpuRegisters &r, std::uint16_t value)
{
    bool carry{};
    bool overflow{};
    auto result = value;
    for (unsigned step = 0; step < Count; ++step) {
        carry = (result & 0x8000U) != 0U;
        const auto shifted = static_cast<std::uint16_t>(result << 1U);
        overflow = overflow || ((result ^ shifted) & 0x8000U) != 0U;
        result = shifted;
    }
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= kN;
    if (result == 0U) flags |= kZ;
    if (carry) flags |= kX | kC;
    if (overflow) flags |= kV;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
    return result;
}

template <unsigned Count>
std::uint32_t shift_left_long(CpuRegisters &r, std::uint32_t value)
{
    bool carry{};
    bool overflow{};
    auto result = value;
    for (unsigned step = 0; step < Count; ++step) {
        carry = (result & 0x80000000U) != 0U;
        const auto shifted = result << 1U;
        overflow = overflow || ((result ^ shifted) & 0x80000000U) != 0U;
        result = shifted;
    }
    std::uint16_t flags{};
    if ((result & 0x80000000U) != 0U) flags |= kN;
    if (result == 0U) flags |= kZ;
    if (carry) flags |= kX | kC;
    if (overflow) flags |= kV;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
    return result;
}

std::uint16_t shift_right_word_six(CpuRegisters &r, std::uint16_t value)
{
    const bool carry = (value & (1U << 5U)) != 0U;
    const auto result = static_cast<std::uint16_t>(value >> 6U);
    std::uint16_t flags{};
    if (result == 0U) flags |= kZ;
    if (carry) flags |= kX | kC;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
    return result;
}

void set_data_word(CpuRegisters &r, unsigned index, std::uint16_t value)
{ r.data[index] = (r.data[index] & 0xffff0000U) | value; }

void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t target)
{
    r.address[7] -= 4U;
    write_long(host, r.address[7], target);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto target = read_long(host, r.address[7]);
    r.address[7] += 4U;
    return target;
}

FunctionResult call_child(FunctionContext &context, std::uint32_t id,
    std::uint32_t callsite, std::uint32_t target, std::uint32_t continuation)
{
    auto &r = context.registers;
    push_return(*context.host, r, continuation);
    r.program_counter = target;
    return context.host->call_function(
        id, 1U, 0x72U, 2U, callsite, target, context);
}
} // namespace

FunctionResult cpu_b_initialize_record_from_table(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto a5 = r.address[5];
    const auto a6 = r.address[6];
    const auto entry = r.program_counter;
    if (entry != 0x0001fb84U && entry != 0x0001fb88U
        && entry != 0x0001fb8cU && entry != 0x0001fba2U)
        return {TranslationStatus::contract_violation, 0U, entry};

    if (entry == 0x0001fb88U) goto resume_1fb88;
    if (entry == 0x0001fb8cU) goto resume_1fb8c;
    if (entry == 0x0001fba2U) goto resume_1fba2;

    {
        const auto child = call_child(context, 378U,
            0x0001fb84U, 0x0001fc88U, 0x0001fb88U);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
    }

resume_1fb88:
    {
        const auto child = call_child(context, 379U,
            0x0001fb88U, 0x0001fd24U, 0x0001fb8cU);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
    }

resume_1fb8c:
    r.data[0] &= 0x0000ffffU;
    set_logic(r, r.data[0], 0x80000000U, 0xffffffffU);
    set_data_word(r, 7U, static_cast<std::uint16_t>(r.data[0]));
    set_logic(r, static_cast<std::uint16_t>(r.data[7]), 0x8000U, 0xffffU);
    r.data[3] = 0U;
    set_logic(r, 0U, 0x80000000U, 0xffffffffU);
    set_data_word(r, 3U, read_word(host, a5 + 0x16U));
    set_logic(r, static_cast<std::uint16_t>(r.data[3]), 0x8000U, 0xffffU);
    set_data_word(r, 3U, add_word(r,
        static_cast<std::uint16_t>(r.data[3]), 8U));
    set_data_word(r, 3U, shift_left_word<3U>(r,
        static_cast<std::uint16_t>(r.data[3])));
    {
        const auto child = call_child(context, 377U,
            0x0001fb9eU, 0x0001fc4aU, 0x0001fba2U);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
    }

resume_1fba2:
    set_data_word(r, 7U, subtract_word(r,
        static_cast<std::uint16_t>(r.data[7]),
        static_cast<std::uint16_t>(r.data[2])));
    r.data[7] = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[7])));
    set_logic(r, r.data[7], 0x80000000U, 0xffffffffU);
    r.data[7] = shift_left_long<8U>(r, r.data[7]);
    r.data[7] = shift_left_long<4U>(r, r.data[7]);
    r.data[7] &= 0xffffe000U;
    set_logic(r, r.data[7], 0x80000000U, 0xffffffffU);
    write_long(host, a6 + 0x22U, r.data[7]);
    set_logic(r, r.data[7], 0x80000000U, 0xffffffffU);
    write_long(host, a6 + 0x50U, 0x00002000U);
    set_logic(r, 0x00002000U, 0x80000000U, 0xffffffffU);

    auto d0 = read_word(host, a6 + 0x5cU);
    set_data_word(r, 0U, d0); set_logic(r, d0, 0x8000U, 0xffffU);
    d0 = add_word(r, d0, 0x0080U); set_data_word(r, 0U, d0);
    d0 = static_cast<std::uint16_t>(d0 & 0x0700U);
    set_data_word(r, 0U, d0); set_logic(r, d0, 0x8000U, 0xffffU);
    d0 = shift_right_word_six(r, d0); set_data_word(r, 0U, d0);
    r.address[0] = read_long(host, a6 + 0x48U);
    auto value = read_word(host, r.address[0] + d0);
    write_word(host, a6 + 0x06U, value); set_logic(r, value, 0x8000U, 0xffffU);
    value = read_word(host, r.address[0] + d0 + 2U);
    write_word(host, a6, value); set_logic(r, value, 0x8000U, 0xffffU);

    r.address[0] = read_long(host, a5 + 0x66U);
    auto d2 = read_word(host, r.address[0] + 0x18U);
    set_data_word(r, 2U, d2); set_logic(r, d2, 0x8000U, 0xffffU);
    d2 = add_word(r, d2, d2); set_data_word(r, 2U, d2);
    auto d1 = d2; set_data_word(r, 1U, d1); set_logic(r, d1, 0x8000U, 0xffffU);
    d1 = shift_left_word<4U>(r, d1); set_data_word(r, 1U, d1);
    d1 = add_word(r, d1, d2); set_data_word(r, 1U, d1);
    r.address[0] = static_cast<std::uint32_t>(0x0002a6e0U
        + static_cast<std::int16_t>(d1)) & 0x00ffffffU;
    d1 = read_word(host, a5 + 0x16U); set_data_word(r, 1U, d1);
    set_logic(r, d1, 0x8000U, 0xffffU);
    d1 = add_word(r, d1, read_word(host, r.address[0]));
    r.address[0] += 2U; set_data_word(r, 1U, d1);
    for (const auto offset : {0x16U, 0x2eU, 0x30U}) {
        write_word(host, a6 + offset, d1); set_logic(r, d1, 0x8000U, 0xffffU);
    }
    write_word(host, a6 + 0x18U, 0U); set_logic(r, 0U, 0x8000U, 0xffffU);
    (void)read_long(host, a6 + 0x2eU);
    // 68000 CLR.L commits the low word before the high word.
    write_word(host, a6 + 0x30U, 0U);
    write_word(host, a6 + 0x2eU, 0U);
    set_logic(r, 0U, 0x80000000U, 0xffffffffU);

    d0 = read_word(host, a5 + 0x5cU); set_data_word(r, 0U, d0);
    set_logic(r, d0, 0x8000U, 0xffffU);
    d0 = add_word(r, d0, 0x0080U); set_data_word(r, 0U, d0);
    d0 = static_cast<std::uint16_t>(d0 & 0x0700U);
    set_data_word(r, 0U, d0); set_logic(r, d0, 0x8000U, 0xffffU);
    d0 = shift_right_word_six(r, d0); set_data_word(r, 0U, d0);
    r.address[0] = static_cast<std::uint32_t>(r.address[0]
        + static_cast<std::int16_t>(d0)) & 0x00ffffffU;
    d0 = read_word(host, a5 + 0x12U); set_data_word(r, 0U, d0);
    set_logic(r, d0, 0x8000U, 0xffffU);
    d0 = add_word(r, d0, read_word(host, r.address[0])); r.address[0] += 2U;
    set_data_word(r, 0U, d0);
    for (const auto offset : {0x2aU, 0x2cU, 0x12U}) {
        write_word(host, a6 + offset, d0); set_logic(r, d0, 0x8000U, 0xffffU);
    }
    (void)read_word(host, a6 + 0x14U);
    write_word(host, a6 + 0x14U, 0U); set_logic(r, 0U, 0x8000U, 0xffffU);
    d0 = read_word(host, a5 + 0x1aU); set_data_word(r, 0U, d0);
    set_logic(r, d0, 0x8000U, 0xffffU);
    d0 = add_word(r, d0, read_word(host, r.address[0])); r.address[0] += 2U;
    set_data_word(r, 0U, d0);
    for (const auto offset : {0x32U, 0x34U, 0x1aU}) {
        write_word(host, a6 + offset, d0); set_logic(r, d0, 0x8000U, 0xffffU);
    }
    (void)read_word(host, a6 + 0x1cU);
    write_word(host, a6 + 0x1cU, 0U); set_logic(r, 0U, 0x8000U, 0xffffU);

    r.program_counter = pop_return(host, r);
    return FunctionResult::complete(1U, r.program_counter);
}

} // namespace gain_ground::translated

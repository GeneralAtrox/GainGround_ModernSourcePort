constexpr std::uint16_t kProgram = 1U;
constexpr std::uint16_t kShared = 3U;
constexpr std::uint16_t kWord = 0xffffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void pf(ExecutionHost &host, std::uint32_t pc)
{
    (void)host.read_memory_word(kProgram, pc, kWord);
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
                    std::uint32_t sign, std::uint32_t mask, bool update_x)
{
    destination &= mask;
    source &= mask;
    result &= mask;
    std::uint16_t flags{};
    if ((result & sign) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((destination ^ source) & (destination ^ result) & sign) != 0U)
        flags |= 0x0002U;
    if (source > destination) flags |= 0x0001U;
    if (update_x && source > destination) flags |= 0x0010U;
    const auto preserved = update_x ? 0U : (r.status & 0x0010U);
    r.status = static_cast<std::uint16_t>(
        (r.status & ~0x001fU) | preserved | flags);
}

void add_flags(CpuRegisters &r, std::uint32_t left, std::uint32_t right,
               std::uint32_t result, std::uint32_t sign,
               std::uint64_t limit)
{
    std::uint16_t flags{};
    if ((result & sign) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & sign) != 0U) flags |= 0x0002U;
    if (static_cast<std::uint64_t>(left) + right > limit) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

std::uint8_t read_hardware_byte(ExecutionHost &host, std::uint32_t pc,
                                std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto value = host.read_hardware(1U, 0U, 0xffU, pc,
        address & ~1U, mask);
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

void write_shared_byte(ExecutionHost &host, std::uint32_t address,
                       std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    host.write_memory_word(kShared, (address & kSharedMask) & ~1U,
        static_cast<std::uint16_t>(value) << (odd ? 0U : 8U),
        odd ? 0x00ffU : 0xff00U);
}

std::uint32_t read_shared_long(ExecutionHost &host, std::uint32_t address)
{
    const auto offset = address & kSharedMask;
    return (static_cast<std::uint32_t>(host.read_memory_word(
                kShared, offset, kWord)) << 16U)
        | host.read_memory_word(kShared, (offset + 2U) & kSharedMask, kWord);
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

FunctionResult return_from_subroutine(FunctionContext &context)
{
    auto &host = *context.host;
    auto &r = context.registers;
    const auto target = read_shared_long(host, r.address[7]);
    r.address[7] += 4U;
    pf(host, target);
    pf(host, target + 2U);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
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

std::uint8_t read_copy_byte(ExecutionHost &host, std::uint32_t pc,
                            std::uint32_t address)
{
    if ((address & 0x00f00000U) == 0x00b00000U)
        return read_hardware_byte(host, pc, address);
    return read_shared_byte(host, address);
}

void write_copy_byte(ExecutionHost &host, std::uint32_t pc,
                     std::uint32_t address, std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    const auto write_region_byte = [&](std::uint16_t region,
                                       std::uint32_t offset) {
        host.write_memory_word(region, offset & ~1U,
            static_cast<std::uint16_t>(value) << (odd ? 0U : 8U),
            odd ? 0x00ffU : 0xff00U);
    };
    if (address >= 0x00200000U && address < 0x00208000U) {
        write_region_byte(5U, address - 0x00200000U);
        return;
    }
    if (address >= 0x00208000U && address < 0x0020c000U) {
        write_region_byte(6U, address - 0x00208000U);
        return;
    }
    if (address >= 0x0020c000U && address < 0x00210000U) {
        write_region_byte(7U, address - 0x0020c000U);
        return;
    }
    if (address >= 0x00280000U && address < 0x002a0000U) {
        write_region_byte(8U, address - 0x00280000U);
        return;
    }
    if (address >= 0x00400000U && address < 0x00404000U) {
        write_region_byte(9U, address - 0x00400000U);
        return;
    }
    if (address >= 0x00404000U && address < 0x00404020U) {
        write_region_byte(10U, address - 0x00404000U);
        return;
    }
    if (address >= 0x00800000U && address < 0x00a00000U) {
        host.write_hardware(2U, 0U, 0xffU, pc, address & ~1U,
            static_cast<std::uint16_t>(value) * 0x0101U,
            odd ? 0x00ffU : 0xff00U);
        const auto canonical = (address & ~1U) & ~0x001ffe00U;
        if (odd && canonical == 0x0080000eU)
            host.write_hardware(4U, 0U, 0xffU, pc, 0x0080000eU,
                value, 0x00ffU);
        if (odd && canonical == 0x00800100U)
            host.write_hardware(3U, 0U, 0xffU, pc, 0x00800100U,
                value, 0x00ffU);
        if (odd && canonical == 0x00800102U)
            host.write_hardware(3U, 0U, 0xffU, pc, 0x00800101U,
                value, 0x00ffU);
        return;
    }
    const auto sprite_address = address & ~0x00180000U;
    if (sprite_address >= 0x00600000U && sprite_address < 0x00640000U) {
        host.write_memory_word(11U, (address & 0x0003ffffU) & ~1U,
            static_cast<std::uint16_t>(value) << (odd ? 0U : 8U),
            odd ? 0x00ffU : 0xff00U);
        return;
    }
    if (address >= 0x00400000U && address < 0x00800000U) {
        host.write_hardware(2U, 0U, 0xffU, pc, address & ~1U,
            static_cast<std::uint16_t>(value) * 0x0101U,
            odd ? 0x00ffU : 0xff00U);
        return;
    }
    if (address >= 0x00f00000U && address < 0x00f80000U) {
        host.write_memory_word(2U, (address & kSharedMask) & ~1U,
            static_cast<std::uint16_t>(value) << (odd ? 0U : 8U),
            odd ? 0x00ffU : 0xff00U);
        return;
    }
    write_shared_byte(host, address, value);
}

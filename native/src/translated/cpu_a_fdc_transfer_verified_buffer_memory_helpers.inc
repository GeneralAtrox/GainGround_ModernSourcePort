constexpr std::uint16_t kProgram = 1U;
constexpr std::uint16_t kMain = 2U;
constexpr std::uint16_t kShared = 3U;
constexpr std::uint16_t kWord = 0xffffU;
constexpr std::uint16_t kByte = 0x00ffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void pf(ExecutionHost &host, std::uint32_t pc)
{
    (void)host.read_memory_word(kProgram, pc, kWord);
}

std::uint16_t rs(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kShared, address & kSharedMask, kWord);
}

void ws(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kShared, address & kSharedMask, value, kWord);
}

std::uint8_t rsb(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto word = host.read_memory_word(kShared,
        (address & kSharedMask) & ~1U, odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

void wsb(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    if ((address & 0x00ff0000U) == 0x00b80000U) {
        host.write_hardware(2U, 0U, 0xffU, 0x00001130U, address & ~1U,
            static_cast<std::uint16_t>(value) * 0x0101U,
            odd ? 0x00ffU : 0xff00U);
        return;
    }
    host.write_memory_word(kMain, (address & kSharedMask) & ~1U,
        static_cast<std::uint16_t>(value) << (odd ? 0U : 8U),
        odd ? 0x00ffU : 0xff00U);
}

std::uint8_t rhb(ExecutionHost &host, std::uint32_t pc,
                 std::uint32_t address)
{
    return static_cast<std::uint8_t>(host.read_hardware(
        1U, 0U, 0xffU, pc, address & ~1U, kByte));
}

void whb(ExecutionHost &host, std::uint32_t pc, std::uint32_t address,
         std::uint8_t value)
{
    host.write_hardware(2U, 0U, 0xffU, pc, address & ~1U,
        static_cast<std::uint16_t>(value) * 0x0101U, kByte);
}

std::uint8_t rhb_addressed(ExecutionHost &host, std::uint32_t pc,
                           std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto word = host.read_hardware(1U, 0U, 0xffU, pc, address & ~1U,
        odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

std::uint32_t rhl(ExecutionHost &host, std::uint32_t pc,
                  std::uint32_t address)
{
    const auto high = host.read_hardware(
        1U, 0U, 0xffU, pc, address, kWord);
    const auto low = host.read_hardware(
        1U, 0U, 0xffU, pc, address + 2U, kWord);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void whl(ExecutionHost &host, std::uint32_t pc, std::uint32_t address,
         std::uint32_t value)
{
    host.write_hardware(2U, 0U, 0xffU, pc, address,
        static_cast<std::uint16_t>(value >> 16U), kWord);
    host.write_hardware(2U, 0U, 0xffU, pc, address + 2U,
        static_cast<std::uint16_t>(value), kWord);
}

void logic_byte(CpuRegisters &r, std::uint8_t value)
{
    r.status = static_cast<std::uint16_t>((r.status & ~0x000fU)
        | (value == 0U ? 0x0004U : 0U)
        | ((value & 0x80U) != 0U ? 0x0008U : 0U));
}

void logic_long(CpuRegisters &r, std::uint32_t value)
{
    r.status = static_cast<std::uint16_t>((r.status & ~0x000fU)
        | (value == 0U ? 0x0004U : 0U)
        | ((value & 0x80000000U) != 0U ? 0x0008U : 0U));
}

void compare_byte(CpuRegisters &r, std::uint8_t destination,
                  std::uint8_t source)
{
    const auto result = static_cast<std::uint8_t>(destination - source);
    const bool carry = source > destination;
    const bool overflow = ((destination ^ source) & (destination ^ result)
        & 0x80U) != 0U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU)
        | (result == 0U ? 0x0004U : 0U)
        | ((result & 0x80U) != 0U ? 0x0008U : 0U)
        | (overflow ? 0x0002U : 0U)
        | (carry ? 0x0011U : 0U));
}

void push_long(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    ws(host, r.address[7], static_cast<std::uint16_t>(value >> 16U));
    ws(host, r.address[7] + 2U, static_cast<std::uint16_t>(value));
}

FunctionResult call(FunctionContext &context, std::uint32_t id,
                    std::uint32_t site, std::uint32_t target,
                    std::uint32_t return_pc)
{
    auto &host = *context.host;
    auto &r = context.registers;
    push_long(host, r, return_pc);
    pf(host, target);
    pf(host, target + 2U);
    r.program_counter = target;
    return host.call_function(id, 0U, 0xffU, 2U, site, target, context);
}

bool complete(FunctionResult const &result)
{
    return result.status == TranslationStatus::complete;
}

FunctionResult terminal(FunctionContext &context, std::uint32_t target)
{
    pf(*context.host, target);
    pf(*context.host, target + 2U);
    context.registers.program_counter = target;
    return FunctionResult::complete(3U, target);
}

FunctionResult terminal_call(FunctionContext &context, std::uint32_t id,
                             std::uint32_t site, std::uint32_t target)
{
    auto &host = *context.host;
    pf(host, target);
    pf(host, target + 2U);
    context.registers.program_counter = target;
    return host.call_function(id, 0U, 0xffU, 1U, site, target, context);
}

FunctionResult do_return(FunctionContext &context)
{
    auto &host = *context.host;
    auto &r = context.registers;
    const auto stack = r.address[7] & kSharedMask;
    const auto target = (static_cast<std::uint32_t>(rs(host, stack)) << 16U)
        | rs(host, stack + 2U);
    r.address[7] += 4U;
    pf(host, target);
    pf(host, target + 2U);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

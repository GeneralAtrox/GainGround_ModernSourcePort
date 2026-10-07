constexpr std::uint16_t kRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

[[nodiscard]] ByteLocation locate_byte(std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    return {address & 0x0003fffeU,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(SoundCallerTiming &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(SoundCallerTiming &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

[[nodiscard]] std::uint16_t read_word(SoundCallerTiming &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & kAddressMask, kWordMask);
}

void write_word(SoundCallerTiming &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address & kAddressMask, value, kWordMask);
}

[[nodiscard]] std::uint32_t read_long(SoundCallerTiming &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

void write_long(SoundCallerTiming &host, std::uint32_t address, std::uint32_t value)
{
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
}

void prefetch(SoundCallerTiming &host, std::uint32_t address)
{
    (void)read_word(host, address);
}

void set_logic(CpuRegisters &registers, std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

template <typename Wide>
void set_add(CpuRegisters &registers, Wide left, Wide right, Wide result,
    Wide sign, std::uint64_t limit)
{
    std::uint16_t flags{};
    if ((result & sign) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & sign) != 0U) flags |= 0x0002U;
    if (static_cast<std::uint64_t>(left) + right > limit) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

template <typename Wide>
void set_sub(CpuRegisters &registers, Wide left, Wide right, Wide result,
    Wide sign, bool affect_extend)
{
    std::uint16_t flags = affect_extend ? 0U : registers.status & 0x0010U;
    if ((result & sign) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & sign) != 0U) flags |= 0x0002U;
    if (right > left) flags |= static_cast<std::uint16_t>(
        affect_extend ? 0x0011U : 0x0001U);
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_bit_test(CpuRegisters &registers, bool prior_set)
{
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x0004U) | (prior_set ? 0U : 0x0004U));
}

void push_return(SoundCallerTiming &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_long(host, registers.address[7], value);
}

[[nodiscard]] FunctionResult call_native(SoundCallerTiming &host, FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite,
    std::uint32_t target, std::uint32_t return_address)
{
    // BSR.w: two internal clocks, then the original two stack writes and
    // two target prefetches. Inactive sections retain their existing timing.
    host.clocks(2U);
    push_return(host, context.registers, return_address);
    prefetch(host, target);
    prefetch(host, target + 2U);
    context.registers.program_counter = target;
    return host.call_function(function_id, 0U, 0xffU, 2U,
        callsite, target, context);
}

[[nodiscard]] FunctionResult finish(SoundCallerTiming &host, CpuRegisters &registers)
{
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

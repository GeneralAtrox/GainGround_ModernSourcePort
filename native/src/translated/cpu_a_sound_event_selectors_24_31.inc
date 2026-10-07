FunctionResult handle_cpu_a_sound_event_selectors_24_31(
    ExecutionHost &host, CpuRegisters &registers, FunctionContext &context,
    std::uint8_t selector, std::uint16_t value)
{
    (void)value;
    switch (selector) {
    case 24: {
        prefetch(host, 0x0008375eU);
        prefetch(host, 0x00083760U);
        prefetch(host, 0x00083762U);
        prefetch(host, 0x00083764U);
        const auto address = registers.address[3] + 1U;
        const auto prior = read_byte(host, address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((prior & 0x20U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x00083766U);
        write_byte(host, address, static_cast<std::uint8_t>(prior | 0x20U));
        return return_from_subroutine(host, registers);
    }
    case 25: {
        prefetch(host, 0x00083750U);
        prefetch(host, 0x00083752U);
        auto stream = read_byte(host, registers.address[4]++);
        prefetch(host, 0x00083754U);
        write_byte(host, registers.address[3] + 51U, stream);
        set_logic_flags(registers, stream, 0x80U);
        prefetch(host, 0x00083756U);
        stream = read_byte(host, registers.address[4]++);
        prefetch(host, 0x00083758U);
        write_byte(host, registers.address[3] + 48U, stream);
        set_logic_flags(registers, stream, 0x80U);
        prefetch(host, 0x0008375aU);
        stream = read_byte(host, registers.address[4]++);
        prefetch(host, 0x0008375cU);
        write_byte(host, registers.address[3] + 50U, stream);
        set_logic_flags(registers, stream, 0x80U);
        prefetch(host, 0x0008375eU);
        return return_from_subroutine(host, registers);
    }
    case 26: {
        prefetch(host, 0x00083766U);
        prefetch(host, 0x00083768U);
        prefetch(host, 0x0008376aU);
        prefetch(host, 0x0008376cU);
        const auto address = registers.address[3] + 1U;
        const auto prior = read_byte(host, address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((prior & 0x20U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x0008376eU);
        write_byte(host, address, static_cast<std::uint8_t>(prior & 0xdfU));
        return return_from_subroutine(host, registers);
    }
    case 27: {
        prefetch(host, 0x00083944U);
        prefetch(host, 0x00083946U);
        prefetch(host, 0x00083948U);
        for (int index = 7; index >= 0; --index) {
            registers.address[7] -= 4U;
            write_word(host, registers.address[7] + 2U,
                static_cast<std::uint16_t>(registers.data[index]));
            write_word(host, registers.address[7],
                static_cast<std::uint16_t>(registers.data[index] >> 16U));
        }
        prefetch(host, 0x0008394aU);
        registers.data[0] = 0U;
        set_logic_flags(registers, 0U, 0x80000000U);
        prefetch(host, 0x0008394cU);
        registers.data[1] = registers.data[0];
        set_logic_flags(registers, registers.data[1], 0x80000000U);
        prefetch(host, 0x0008394eU);
        auto stream = read_byte(host, registers.address[4]++);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | stream;
        set_logic_flags(registers, stream, 0x80U);
        prefetch(host, 0x00083950U);
        stream = read_byte(host, registers.address[4]++);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | stream;
        set_logic_flags(registers, stream, 0x80U);
        prefetch(host, 0x00083952U);
        left_shift_word_eight(registers);
        prefetch(host, 0x00083954U);
        const auto high = static_cast<std::uint16_t>(registers.data[0]);
        const auto low = static_cast<std::uint16_t>(registers.data[1]);
        const auto combined = static_cast<std::uint16_t>(high + low);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | combined;
        set_add_word_flags(registers, high, low, combined);
        prefetch(host, 0x00083956U);
        stream = read_byte(host, registers.address[4]++);
        registers.data[6] = (registers.data[6] & 0xffffff00U) | stream;
        set_logic_flags(registers, stream, 0x80U);
        prefetch(host, 0x00083958U);

        const auto trap_status = registers.status;
        registers.address[7] -= 2U;
        write_word(host, registers.address[7], 0x3958U);
        registers.address[7] -= 4U;
        write_word(host, registers.address[7], trap_status);
        write_word(host, registers.address[7] + 2U, 0x0008U);
        prefetch(host, 0x00000094U);
        prefetch(host, 0x00000096U);
        prefetch(host, 0x00080078U);
        prefetch(host, 0x0008007aU);
        registers.program_counter = 0x00080078U;
        const auto trap = host.call_function(515U, 0U, 0xffU, 4U,
            0x00083956U, 0x00080078U, context);
        if (trap.status != TranslationStatus::complete || trap.control != 2U)
            return trap;

        prefetch(host, 0x0008395aU);
        prefetch(host, 0x0008395cU);
        for (unsigned index = 0; index != 8U; ++index) {
            registers.data[index] = read_long(host, registers.address[7]);
            registers.address[7] += 4U;
        }
        prefetch(host, 0x0008395eU);
        return return_from_subroutine(host, registers);
    }
    default:
        return FunctionResult::unimplemented();
    }
}

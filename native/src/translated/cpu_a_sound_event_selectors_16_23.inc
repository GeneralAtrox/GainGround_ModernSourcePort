FunctionResult handle_cpu_a_sound_event_selectors_16_23(
    ExecutionHost &host, CpuRegisters &registers, FunctionContext &context,
    std::uint8_t selector, std::uint16_t value)
{
    (void)value;
    switch (selector) {
    case 17: {
        prefetch(host, 0x0008395eU);
        registers.data[0] = 0U;
        set_logic_flags(registers, 0U, 0x80000000U);
        prefetch(host, 0x00083960U);
        prefetch(host, 0x00083962U);
        const auto event = read_byte(host, registers.address[4]++);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | event;
        set_logic_flags(registers, event, 0x80U);
        if (static_cast<std::uint8_t>(registers.data[0]) == 0U) {
            prefetch(host, 0x00083964U);
            prefetch(host, 0x000834aeU);
            prefetch(host, 0x000834b0U);
            return return_from_subroutine(host, registers);
        }
        prefetch(host, 0x00083964U);
        prefetch(host, 0x00083966U);
        prefetch(host, 0x00083968U);
        prefetch(host, 0x0008396aU);
        write_byte(host, registers.address[3] + 0x16U,
            static_cast<std::uint8_t>(registers.data[0]));
        set_logic_flags(registers,
            static_cast<std::uint8_t>(registers.data[0]), 0x80U);
        prefetch(host, 0x0008396cU);
        prefetch(host, 0x0008396eU);
        prefetch(host, 0x00083970U);
        const auto channel_flags = read_byte(host, registers.address[3]);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((channel_flags & 0x04U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x00083972U);
        if ((channel_flags & 0x04U) != 0U) {
            prefetch(host, 0x000834aeU);
            prefetch(host, 0x000834b0U);
            return return_from_subroutine(host, registers);
        }

        prefetch(host, 0x00083974U);
        prefetch(host, 0x00083976U);
        auto child = call_native(context, 92U,
            0x00083974U, 0x00084164U, 0x00083978U);
        if (child.status != TranslationStatus::complete) return child;
        prefetch(host, 0x0008397cU);
        prefetch(host, 0x0008397eU);
        const auto mode_flags = read_byte(host, registers.address[3] + 2U);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((mode_flags & 0x08U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x00083980U);
        if ((mode_flags & 0x08U) != 0U) {
            prefetch(host, 0x000834aeU);
            prefetch(host, 0x000834b0U);
            return return_from_subroutine(host, registers);
        }
        prefetch(host, 0x00083982U);
        prefetch(host, 0x00083984U);
        set_compare_byte_flags(registers,
            static_cast<std::uint8_t>(registers.data[7]), 4U);
        prefetch(host, 0x00083986U);
        prefetch(host, 0x00083988U);
        if (static_cast<std::uint8_t>(registers.data[7]) >= 4U) {
            prefetch(host, 0x000834aeU);
            prefetch(host, 0x000834b0U);
            return return_from_subroutine(host, registers);
        }

        prefetch(host, 0x0008398aU);
        registers.data[0] = 0x10U;
        set_logic_flags(registers, 0x10U, 0x80000000U);
        prefetch(host, 0x0008398cU);
        registers.data[1] = 0U;
        set_logic_flags(registers, 0U, 0x80000000U);
        prefetch(host, 0x0008398eU);
        const auto channel = static_cast<std::uint8_t>(registers.data[7]);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | channel;
        set_logic_flags(registers, channel, 0x80U);
        for (;;) {
            prefetch(host, 0x00083990U);
            prefetch(host, 0x00083992U);
            const auto prior = static_cast<std::uint8_t>(registers.data[0]);
            const auto result = static_cast<std::uint8_t>(prior - 4U);
            registers.data[0] = (registers.data[0] & 0xffffff00U) | result;
            set_sub_byte_flags(registers, prior, 4U, result);
            prefetch(host, 0x00083994U);
            prefetch(host, 0x00083996U);
            const auto counter = static_cast<std::uint16_t>(registers.data[1]);
            registers.data[1] = (registers.data[1] & 0xffff0000U)
                | static_cast<std::uint16_t>(counter - 1U);
            if (counter == 0U) {
                prefetch(host, 0x00083990U);
                break;
            }
        }
        prefetch(host, 0x00083998U);
        prefetch(host, 0x0008399aU);
        registers.address[1] = registers.address[6]
            + 0x30U + static_cast<std::uint32_t>(
                static_cast<std::int32_t>(
                    static_cast<std::int16_t>(registers.data[0])));
        prefetch(host, 0x0008399cU);
        prefetch(host, 0x0008399eU);
        prefetch(host, 0x000839a0U);
        const auto modulation = read_byte(host, registers.address[1] + 0x13U);
        registers.data[2] = (registers.data[2] & 0xffffff00U) | modulation;
        set_logic_flags(registers, modulation, 0x80U);
        prefetch(host, 0x000839a2U);
        child = call_native(context, 94U,
            0x000839a0U, 0x0008421aU, 0x000839a4U);
        if (child.status != TranslationStatus::complete) return child;
        return return_from_subroutine(host, registers);
    }
    case 19: {
        prefetch(host, 0x000836d6U);
        prefetch(host, 0x000836d8U);
        auto stream = read_byte(host, registers.address[4]++);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | stream;
        set_logic_flags(registers, stream, 0x80U);
        const auto prior = static_cast<std::uint8_t>(registers.data[0]);
        const auto addend = static_cast<std::uint8_t>(registers.data[7]);
        const auto result = static_cast<std::uint8_t>(prior + addend);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | result;
        set_add_byte_flags(registers, prior, addend, result);
        prefetch(host, 0x000836daU);
        prefetch(host, 0x000836dcU);
        stream = read_byte(host, registers.address[4]++);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | stream;
        set_logic_flags(registers, stream, 0x80U);
        prefetch(host, 0x000836deU);
        prefetch(host, 0x00084126U);
        prefetch(host, 0x00084128U);
        registers.program_counter = 0x00084126U;
        return host.call_function(88U, 0U, 0xffU, 1U,
            0x000836dcU, 0x00084126U, context);
    }
    case 20: {
        prefetch(host, 0x00083926U);
        prefetch(host, 0x00083928U);
        prefetch(host, 0x0008392aU);
        prefetch(host, 0x0008392cU);
        const auto address = registers.address[3];
        const auto prior = read_byte(host, address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((prior & 0x10U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x0008392eU);
        write_byte(host, address, static_cast<std::uint8_t>(prior | 0x10U));
        return return_from_subroutine(host, registers);
    }
    case 21: {
        prefetch(host, 0x0008376eU);
        prefetch(host, 0x00083770U);
        prefetch(host, 0x00083772U);
        prefetch(host, 0x00083774U);
        const auto bit_address = registers.address[3] + 1U;
        const auto bit_prior = read_byte(host, bit_address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((bit_prior & 0x08U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x00083776U);
        write_byte(host, bit_address, static_cast<std::uint8_t>(bit_prior | 0x08U));
        prefetch(host, 0x00083778U);
        prefetch(host, 0x0008377aU);
        const auto flags = read_byte(host, registers.address[3]);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((flags & 0x04U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x0008377cU);
        if ((flags & 0x04U) != 0U) {
            prefetch(host, 0x000834aeU);
            prefetch(host, 0x000834b0U);
            return return_from_subroutine(host, registers);
        }
        prefetch(host, 0x0008377eU);
        prefetch(host, 0x00083780U);
        prefetch(host, 0x00083782U);
        prefetch(host, 0x00083784U);
        const auto active_address = registers.address[3];
        const auto active_prior = read_byte(host, active_address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((active_prior & 0x04U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x00083786U);
        write_byte(host, active_address,
            static_cast<std::uint8_t>(active_prior | 0x04U));
        const auto channel = static_cast<std::uint8_t>(registers.data[7]);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | channel;
        set_logic_flags(registers, channel, 0x80U);
        registers.data[0] = 8U;
        set_logic_flags(registers, 8U, 0x80000000U);
        prefetch(host, 0x00083788U);
        prefetch(host, 0x0008378aU);
        prefetch(host, 0x00084126U);
        prefetch(host, 0x00084128U);
        registers.program_counter = 0x00084126U;
        return host.call_function(88U, 0U, 0xffU, 1U,
            0x00083788U, 0x00084126U, context);
    }
    case 22: {
        prefetch(host, 0x0008378cU);
        prefetch(host, 0x0008378eU);
        prefetch(host, 0x00083790U);
        prefetch(host, 0x00083792U);
        const auto flag_address = registers.address[3] + 1U;
        const auto flag_prior = read_byte(host, flag_address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((flag_prior & 0x08U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x00083794U);
        write_byte(host, flag_address,
            static_cast<std::uint8_t>(flag_prior & 0xf7U));
        prefetch(host, 0x00083796U);
        prefetch(host, 0x00083798U);
        const auto peer_flags = read_byte(host, registers.address[3] + 0x320U);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((peer_flags & 0x80U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x0008379aU);
        if ((peer_flags & 0x80U) != 0U) {
            prefetch(host, 0x000834aeU);
            prefetch(host, 0x000834b0U);
            return return_from_subroutine(host, registers);
        }
        prefetch(host, 0x0008379cU);
        prefetch(host, 0x0008379eU);
        prefetch(host, 0x000837a0U);
        prefetch(host, 0x000837a2U);
        const auto active_address = registers.address[3];
        const auto active_prior = read_byte(host, active_address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((active_prior & 0x04U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x000837a4U);
        write_byte(host, active_address,
            static_cast<std::uint8_t>(active_prior & 0xfbU));
        return return_from_subroutine(host, registers);
    }
    case 23: {
        prefetch(host, 0x000836e0U);
        prefetch(host, 0x000836e2U);
        move_stream_byte_to_data(host, registers, 1U);
        prefetch(host, 0x000836e4U);
        prefetch(host, 0x000836e6U);
        write_byte(host, registers.address[6] + 0xf2U,
            static_cast<std::uint8_t>(registers.data[1]));
        set_logic_flags(registers, static_cast<std::uint8_t>(registers.data[1]), 0x80U);
        registers.data[0] = 0x18U;
        set_logic_flags(registers, 0x18U, 0x80000000U);
        prefetch(host, 0x000836e8U);
        prefetch(host, 0x000836eaU);
        auto child = call_ym2151_writer(context, 0x000836e8U, 0x000836ecU);
        if (child.status != TranslationStatus::complete) return child;

        move_stream_byte_to_data(host, registers, 1U);
        prefetch(host, 0x000836f0U);
        prefetch(host, 0x000836f2U);
        write_byte(host, registers.address[6] + 0xf4U,
            static_cast<std::uint8_t>(registers.data[1]));
        set_logic_flags(registers, static_cast<std::uint8_t>(registers.data[1]), 0x80U);
        registers.data[0] = 0x19U;
        set_logic_flags(registers, 0x19U, 0x80000000U);
        prefetch(host, 0x000836f4U);
        prefetch(host, 0x000836f6U);
        child = call_ym2151_writer(context, 0x000836f4U, 0x000836f8U);
        if (child.status != TranslationStatus::complete) return child;

        move_stream_byte_to_data(host, registers, 1U);
        prefetch(host, 0x000836fcU);
        prefetch(host, 0x000836feU);
        write_byte(host, registers.address[6] + 0xf5U,
            static_cast<std::uint8_t>(registers.data[1]));
        set_logic_flags(registers, static_cast<std::uint8_t>(registers.data[1]), 0x80U);
        prefetch(host, 0x00083700U);
        child = call_ym2151_writer(context, 0x000836feU, 0x00083702U);
        if (child.status != TranslationStatus::complete) return child;

        move_stream_byte_to_data(host, registers, 1U);
        prefetch(host, 0x00083706U);
        prefetch(host, 0x00083708U);
        write_byte(host, registers.address[6] + 0xf6U,
            static_cast<std::uint8_t>(registers.data[1]));
        set_logic_flags(registers, static_cast<std::uint8_t>(registers.data[1]), 0x80U);
        registers.data[0] = 0x1bU;
        set_logic_flags(registers, 0x1bU, 0x80000000U);
        prefetch(host, 0x0008370aU);
        prefetch(host, 0x0008370cU);
        child = call_ym2151_writer(context, 0x0008370aU, 0x0008370eU);
        if (child.status != TranslationStatus::complete) return child;

        move_stream_byte_to_data(host, registers, 1U);
        prefetch(host, 0x00083712U);
        prefetch(host, 0x00083714U);
        write_byte(host, registers.address[6] + 0xf8U,
            static_cast<std::uint8_t>(registers.data[1]));
        set_logic_flags(registers, static_cast<std::uint8_t>(registers.data[1]), 0x80U);
        registers.data[0] = 0x38U;
        set_logic_flags(registers, 0x38U, 0x80000000U);
        prefetch(host, 0x00083716U);
        auto prior_d0 = static_cast<std::uint8_t>(registers.data[0]);
        auto result_d0 = static_cast<std::uint8_t>(prior_d0
            + static_cast<std::uint8_t>(registers.data[7]));
        registers.data[0] = (registers.data[0] & 0xffffff00U) | result_d0;
        set_add_byte_flags(registers, prior_d0,
            static_cast<std::uint8_t>(registers.data[7]), result_d0);
        prefetch(host, 0x00083718U);
        prefetch(host, 0x0008371aU);
        child = call_ym2151_writer(context, 0x00083718U, 0x0008371cU);
        if (child.status != TranslationStatus::complete) return child;

        registers.data[2] = 0U;
        set_logic_flags(registers, 0U, 0x80000000U);
        registers.data[3] = 3U;
        set_logic_flags(registers, 3U, 0x80000000U);
        prefetch(host, 0x00083720U);
        prefetch(host, 0x00083722U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x00a0U;
        set_logic_flags(registers, 0x00a0U, 0x8000U);
        prefetch(host, 0x00083724U);
        prior_d0 = static_cast<std::uint8_t>(registers.data[0]);
        result_d0 = static_cast<std::uint8_t>(prior_d0
            + static_cast<std::uint8_t>(registers.data[7]));
        registers.data[0] = (registers.data[0] & 0xffffff00U) | result_d0;
        set_add_byte_flags(registers, prior_d0,
            static_cast<std::uint8_t>(registers.data[7]), result_d0);

        for (;;) {
            prefetch(host, 0x00083726U);
            prefetch(host, 0x00083728U);
            const auto stream = read_byte(host, registers.address[4]++);
            set_logic_flags(registers, stream, 0x80U);
            prefetch(host, 0x0008372aU);
            if ((stream & 0x80U) != 0U) {
                prefetch(host, 0x0008372cU);
                prefetch(host, 0x0008372eU);
                const auto table_address = registers.address[3] + 0x24U
                    + static_cast<std::uint16_t>(registers.data[2]);
                auto table_value = read_byte(host, table_address);
                registers.data[1] = (registers.data[1] & 0xffffff00U) | table_value;
                set_logic_flags(registers, table_value, 0x80U);
                prefetch(host, 0x00083730U);
                const auto bit_was_clear = (table_value & 0x80U) == 0U;
                table_value = static_cast<std::uint8_t>(table_value | 0x80U);
                registers.data[1] = (registers.data[1] & 0xffffff00U) | table_value;
                registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
                    | (bit_was_clear ? 0x0004U : 0U));
                prefetch(host, 0x00083732U);
                prefetch(host, 0x00083734U);
                prefetch(host, 0x00083736U);
                write_byte(host, table_address, table_value);
                set_logic_flags(registers, table_value, 0x80U);
                prefetch(host, 0x00083738U);
                child = call_ym2151_writer(context, 0x00083736U, 0x0008373aU);
                if (child.status != TranslationStatus::complete) return child;
            } else {
                prefetch(host, 0x0008373aU);
                prefetch(host, 0x0008373cU);
            }
            auto d2 = static_cast<std::uint8_t>(registers.data[2]);
            const auto next_d2 = static_cast<std::uint8_t>(d2 + 1U);
            registers.data[2] = (registers.data[2] & 0xffffff00U) | next_d2;
            set_add_byte_flags(registers, d2, 1U, next_d2);
            prior_d0 = static_cast<std::uint8_t>(registers.data[0]);
            result_d0 = static_cast<std::uint8_t>(prior_d0 + 8U);
            registers.data[0] = (registers.data[0] & 0xffffff00U) | result_d0;
            set_add_byte_flags(registers, prior_d0, 8U, result_d0);
            prefetch(host, 0x0008373eU);
            prefetch(host, 0x00083740U);
            const auto counter = static_cast<std::uint16_t>(registers.data[3]);
            registers.data[3] = (registers.data[3] & 0xffff0000U)
                | static_cast<std::uint16_t>(counter - 1U);
            if (counter == 0U) {
                prefetch(host, 0x00083726U);
                break;
            }
        }
        prefetch(host, 0x00083742U);
        prefetch(host, 0x00083744U);
        return return_from_subroutine(host, registers);
    }
    default:
        return FunctionResult::unimplemented();
    }
}

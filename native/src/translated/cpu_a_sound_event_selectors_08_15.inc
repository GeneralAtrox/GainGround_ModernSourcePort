FunctionResult handle_cpu_a_sound_event_selectors_08_15(
    ExecutionHost &host, CpuRegisters &registers, FunctionContext &context,
    std::uint8_t selector, std::uint16_t value)
{
    (void)context;
    (void)value;
    switch (selector) {
    case 8: {
        prefetch(host, 0x000838c2U);
        registers.data[0] = 0U;
        set_logic_flags(registers, 0U, 0x80000000U);
        prefetch(host, 0x000838c4U);
        registers.data[1] = registers.data[0];
        set_logic_flags(registers, registers.data[1], 0x80000000U);
        prefetch(host, 0x000838c6U);
        prefetch(host, 0x000838c8U);
        move_stream_byte_to_data(host, registers, 0U);
        left_shift_word_eight(registers);
        prefetch(host, 0x000838caU);
        prefetch(host, 0x000838ccU);
        move_stream_byte_to_data(host, registers, 0U);
        prefetch(host, 0x000838ceU);
        prefetch(host, 0x000838d0U);
        const auto prior = read_byte(host, registers.address[3] + 12U);
        const auto result = static_cast<std::uint8_t>(prior - 4U);
        prefetch(host, 0x000838d2U);
        write_byte(host, registers.address[3] + 12U, result);
        set_sub_byte_flags(registers, prior, 4U, result);
        prefetch(host, 0x000838d4U);
        const auto index = read_byte(host, registers.address[3] + 12U);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | index;
        set_logic_flags(registers, index, 0x80U);
        prefetch(host, 0x000838d6U);
        registers.data[1] &= 0xffff00ffU;
        set_logic_flags(registers, registers.data[1] & 0xffffU, 0x8000U);
        prefetch(host, 0x000838d8U);
        prefetch(host, 0x000838daU);
        prefetch(host, 0x000838dcU);
        write_long(host, registers.address[3] + index, registers.address[4]);
        set_logic_flags(registers, registers.address[4], 0x80000000U);
        registers.address[4] += static_cast<std::uint32_t>(
            static_cast<std::int32_t>(static_cast<std::int16_t>(registers.data[0])));
        prefetch(host, 0x000838deU);
        prefetch(host, 0x000838e0U);
        return return_from_subroutine(host, registers);
    }
    case 9: {
        prefetch(host, 0x000838e0U);
        prefetch(host, 0x000838e2U);
        prefetch(host, 0x000838e4U);
        const auto index = read_byte(host, registers.address[3] + 12U);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | index;
        set_logic_flags(registers, index, 0x80U);
        prefetch(host, 0x000838e6U);
        set_logic_flags(registers, index, 0x80U);
        prefetch(host, 0x000838e8U);
        prefetch(host, 0x000838eaU);
        prefetch(host, 0x000838ecU);
        registers.address[4] = read_long(host, registers.address[3] + index);
        prefetch(host, 0x000838eeU);
        prefetch(host, 0x000838f0U);
        const auto prior = read_byte(host, registers.address[3] + 12U);
        const auto result = static_cast<std::uint8_t>(prior + 4U);
        prefetch(host, 0x000838f2U);
        write_byte(host, registers.address[3] + 12U, result);
        set_add_byte_flags(registers, prior, 4U, result);
        return return_from_subroutine(host, registers);
    }
    case 10:
        prefetch(host, 0x000838f2U);
        prefetch(host, 0x000838f4U);
        move_stream_byte_to_data(host, registers, 0U);
        left_shift_word_eight(registers);
        prefetch(host, 0x000838f6U);
        prefetch(host, 0x000838f8U);
        move_stream_byte_to_data(host, registers, 0U);
        registers.address[4] += static_cast<std::uint32_t>(
            static_cast<std::int32_t>(static_cast<std::int16_t>(registers.data[0])));
        prefetch(host, 0x000838faU);
        prefetch(host, 0x000838fcU);
        return return_from_subroutine(host, registers);
    case 11: {
        prefetch(host, 0x000838fcU);
        prefetch(host, 0x000838feU);
        move_stream_byte_to_data(host, registers, 0U);
        prefetch(host, 0x00083900U);
        prefetch(host, 0x00083902U);
        const auto prior = read_byte(host, registers.address[3] + 8U);
        const auto addend = static_cast<std::uint8_t>(registers.data[0]);
        const auto result = static_cast<std::uint8_t>(prior + addend);
        prefetch(host, 0x00083904U);
        write_byte(host, registers.address[3] + 8U, result);
        set_add_byte_flags(registers, prior, addend, result);
        return return_from_subroutine(host, registers);
    }
    case 12: {
        prefetch(host, 0x00083904U);
        registers.data[0] = 0U;
        set_logic_flags(registers, 0U, 0x80000000U);
        prefetch(host, 0x00083906U);
        prefetch(host, 0x00083908U);
        move_stream_byte_to_data(host, registers, 0U);
        prefetch(host, 0x0008390aU);
        prefetch(host, 0x0008390cU);
        const auto channel_address = registers.address[3] + 64U
            + static_cast<std::uint8_t>(registers.data[0]);
        const auto channel_value = read_byte(host, channel_address);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | channel_value;
        set_logic_flags(registers, channel_value, 0x80U);
        prefetch(host, 0x0008390eU);
        move_stream_byte_to_data(host, registers, 2U);
        const auto incremented = static_cast<std::uint8_t>(channel_value + 1U);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | incremented;
        set_add_byte_flags(registers, channel_value, 1U, incremented);
        prefetch(host, 0x00083910U);
        set_compare_byte_flags(registers,
            static_cast<std::uint8_t>(registers.data[2]), incremented);
        prefetch(host, 0x00083912U);
        prefetch(host, 0x00083914U);
        if (static_cast<std::uint8_t>(registers.data[2]) == incremented) {
            prefetch(host, 0x0008391cU);
            prefetch(host, 0x0008391eU);
            prefetch(host, 0x00083920U);
            prefetch(host, 0x00083922U);
            write_byte(host, channel_address, 0U);
            set_logic_flags(registers, 0U, 0x80U);
            registers.address[4] += 2U;
            prefetch(host, 0x00083924U);
            prefetch(host, 0x00083926U);
            return return_from_subroutine(host, registers);
        }

        prefetch(host, 0x00083916U);
        prefetch(host, 0x00083918U);
        prefetch(host, 0x0008391aU);
        write_byte(host, channel_address, incremented);
        set_logic_flags(registers, incremented, 0x80U);
        prefetch(host, 0x0008391cU);
        prefetch(host, 0x000838f2U);
        prefetch(host, 0x000838f4U);
        move_stream_byte_to_data(host, registers, 0U);
        left_shift_word_eight(registers);
        prefetch(host, 0x000838f6U);
        prefetch(host, 0x000838f8U);
        move_stream_byte_to_data(host, registers, 0U);
        registers.address[4] += static_cast<std::uint32_t>(
            static_cast<std::int32_t>(static_cast<std::int16_t>(registers.data[0])));
        prefetch(host, 0x000838faU);
        prefetch(host, 0x000838fcU);
        return return_from_subroutine(host, registers);
    }
    case 13: {
        prefetch(host, 0x0008392eU);
        prefetch(host, 0x00083930U);
        prefetch(host, 0x00083932U);
        prefetch(host, 0x00083934U);
        const auto address = registers.address[3] + 1U;
        const auto prior = read_byte(host, address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((prior & 0x10U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x00083936U);
        write_byte(host, address, static_cast<std::uint8_t>(prior | 0x10U));
        return return_from_subroutine(host, registers);
    }
    case 14: {
        prefetch(host, 0x00083936U);
        prefetch(host, 0x00083938U);
        prefetch(host, 0x0008393aU);
        prefetch(host, 0x0008393cU);
        const auto first_address = registers.address[3];
        const auto first_prior = read_byte(host, first_address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((first_prior & 0x10U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x0008393eU);
        write_byte(host, first_address,
            static_cast<std::uint8_t>(first_prior & 0xefU));
        prefetch(host, 0x00083940U);
        prefetch(host, 0x00083942U);
        const auto second_address = registers.address[3] + 1U;
        const auto second_prior = read_byte(host, second_address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((second_prior & 0x10U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x00083944U);
        write_byte(host, second_address,
            static_cast<std::uint8_t>(second_prior & 0xefU));
        return return_from_subroutine(host, registers);
    }
    default:
        return FunctionResult::unimplemented();
    }
}

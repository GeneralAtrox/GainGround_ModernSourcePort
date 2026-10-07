FunctionResult handle_cpu_a_sound_event_selectors_00_07(
    ExecutionHost &host, CpuRegisters &registers, FunctionContext &context,
    std::uint8_t selector, std::uint16_t value)
{
    switch (selector) {
    case 0: {
        prefetch(host, 0x000836d0U);
        prefetch(host, 0x000836d2U);
        const auto stream = read_byte(host, registers.address[4]++);
        set_logic_flags(registers, stream, 0x80U);
        prefetch(host, 0x000836d4U);
        return return_from_subroutine(host, registers);
    }
    case 2:
    case 5:
    case 15:
    case 16:
    case 18:
    case 28:
    case 29:
    case 30:
    case 31:
        prefetch(host, 0x000836d4U);
        prefetch(host, 0x000836d6U);
        return return_from_subroutine(host, registers);
    case 1:
        prefetch(host, 0x000837a4U);
        prefetch(host, 0x000837a6U);
        value = read_byte(host, registers.address[4]++);
        prefetch(host, 0x000837a8U);
        write_byte(host, registers.address[3] + 3U,
            static_cast<std::uint8_t>(value));
        set_logic_flags(registers, value, 0x80U);
        prefetch(host, 0x000837aaU);
        return return_from_subroutine(host, registers);
    case 3:
        prefetch(host, 0x000837aaU);
        prefetch(host, 0x000837acU);
        value = read_byte(host, registers.address[4]++);
        prefetch(host, 0x000837aeU);
        write_byte(host, registers.address[3] + 13U,
            static_cast<std::uint8_t>(value));
        set_logic_flags(registers, value, 0x80U);
        registers.address[4] -= registers.address[5];
        prefetch(host, 0x000837b0U);
        prefetch(host, 0x000837b2U);
        prefetch(host, 0x000837b4U);
        write_long(host, registers.address[3] + 4U, registers.address[4]);
        set_logic_flags(registers, registers.address[4], 0x80000000U);
        registers.address[4] += registers.address[5];
        prefetch(host, 0x000837b6U);
        prefetch(host, 0x000837b8U);
        prefetch(host, 0x000837baU);
        prefetch(host, 0x000837bcU);
        write_byte(host, registers.address[3] + 15U, 0U);
        set_logic_flags(registers, 0U, 0x80U);
        prefetch(host, 0x000837beU);
        prefetch(host, 0x000837c0U);
        prefetch(host, 0x000837c2U);
        write_byte(host, registers.address[3] + 14U, 0U);
        set_logic_flags(registers, 0U, 0x80U);
        prefetch(host, 0x000837c4U);
        return return_from_subroutine(host, registers);
    case 4: {
        prefetch(host, 0x000837c4U);
        prefetch(host, 0x000837c6U);
        const auto raw_secondary = read_byte(host, registers.address[4]++);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | raw_secondary;
        set_logic_flags(registers, raw_secondary, 0x80U);
        prefetch(host, 0x000837c8U);
        auto secondary = static_cast<std::uint16_t>(registers.data[0]) & 0x000fU;
        registers.data[0] = (registers.data[0] & 0xffff0000U) | secondary;
        set_logic_flags(registers, secondary, 0x8000U);
        prefetch(host, 0x000837caU);
        auto shifted = static_cast<std::uint16_t>(secondary + secondary);
        set_add_word_flags(registers, secondary, secondary, shifted);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | shifted;
        prefetch(host, 0x000837ccU);
        secondary = shifted;
        shifted = static_cast<std::uint16_t>(secondary + secondary);
        set_add_word_flags(registers, secondary, secondary, shifted);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | shifted;
        prefetch(host, 0x000837ceU);
        prefetch(host, 0x000837d0U);
        const auto table_slot = 0x000837d2U + shifted;
        prefetch(host, table_slot);
        prefetch(host, table_slot + 2U);

        const auto subselector = static_cast<std::uint8_t>(shifted >> 2U);
        std::uint32_t target{};
        if (subselector == 0U || subselector == 12U) target = 0x00083802U;
        else if (subselector == 1U) target = 0x0008380cU;
        else if (subselector == 2U) target = 0x00083816U;
        else if (subselector == 3U) target = 0x00083820U;
        else if (subselector == 4U) target = 0x0008382aU;
        else if (subselector == 5U) target = 0x00083834U;
        else if (subselector == 6U) target = 0x0008383eU;
        else if (subselector == 7U) target = 0x00083848U;
        else if (subselector == 8U) target = 0x00083852U;
        else if (subselector == 9U) target = 0x0008385cU;
        else if (subselector == 10U) target = 0x00083866U;
        else if (subselector == 11U) target = 0x0008386cU;
        else return FunctionResult::unimplemented();
        prefetch(host, target);
        prefetch(host, target + 2U);

        const auto control_address = registers.address[6] + 0x32U;
        if (subselector == 0U || subselector == 12U) {
            prefetch(host, 0x00083806U);
            prefetch(host, 0x00083808U);
            write_word(host, control_address, 0U);
            set_logic_flags(registers, 0U, 0x8000U);
            prefetch(host, 0x0008380aU);
        } else if (subselector >= 1U && subselector <= 5U) {
            prefetch(host, target + 4U);
            prefetch(host, target + 6U);
            const auto prior = read_word(host, control_address);
            prefetch(host, target + 8U);
            const auto result = static_cast<std::uint16_t>(prior & 0x07ffU);
            write_word(host, control_address, result);
            set_logic_flags(registers, result, 0x8000U);
        } else if (subselector >= 6U && subselector <= 9U) {
            prefetch(host, target + 4U);
            prefetch(host, target + 6U);
            const auto prior = read_word(host, control_address);
            prefetch(host, target + 8U);
            const auto result = static_cast<std::uint16_t>(prior & 0xf800U);
            write_word(host, control_address, result);
            set_logic_flags(registers, result, 0x8000U);
        } else if (subselector == 10U) {
            prefetch(host, 0x0008386aU);
            const auto prior = read_word(host, control_address);
            const auto result = static_cast<std::uint16_t>(prior & 0xfffeU);
            write_word(host, control_address, result);
            set_logic_flags(registers, result, 0x8000U);
        }

        prefetch(host, 0x0008386cU);
        prefetch(host, 0x0008386eU);
        prefetch(host, 0x00083870U);
        prefetch(host, 0x00083872U);
        const auto channel_address = registers.address[3];
        const auto channel_flags = read_byte(host, channel_address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((channel_flags & 0x80U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x00083874U);
        write_byte(host, channel_address,
            static_cast<std::uint8_t>(channel_flags & 0x7fU));
        prefetch(host, 0x00083876U);
        prefetch(host, 0x00083878U);
        write_byte(host, registers.address[3] + 15U, 0U);
        set_logic_flags(registers, 0U, 0x80U);
        prefetch(host, 0x0008387aU);

        auto child = call_native(context, 96U,
            0x00083878U, 0x000842c0U, 0x0008387cU);
        if (child.status != TranslationStatus::complete) return child;
        child = call_native(context, 90U,
            0x0008387cU, 0x00084154U, 0x00083880U);
        if (child.status != TranslationStatus::complete) return child;
        prefetch(host, 0x00083884U);
        prefetch(host, 0x00083886U);
        const auto mode = read_byte(host, registers.address[3] + 2U);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((mode & 0x08U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x00083888U);
        if ((mode & 0x08U) == 0U) {
            prefetch(host, 0x000834aeU);
            prefetch(host, 0x000834b0U);
            return return_from_subroutine(host, registers);
        }
        prefetch(host, 0x0008388aU);
        prefetch(host, 0x0008388cU);
        child = call_native(context, 97U,
            0x0008388aU, 0x000842e4U, 0x0008388eU);
        if (child.status != TranslationStatus::complete) return child;
        registers.data[0] = (registers.data[0] & 0xffff0000U) | 0xfce0U;
        set_logic_flags(registers, 0xfce0U, 0x8000U);
        prefetch(host, 0x00083892U);
        prefetch(host, 0x00083894U);
        prefetch(host, 0x00083896U);
        prefetch(host, 0x00083898U);
        const auto displacement = static_cast<std::int16_t>(registers.data[0]);
        const auto peer_address = registers.address[3]
            + static_cast<std::uint32_t>(static_cast<std::int32_t>(displacement));
        const auto peer_flags = read_byte(host, peer_address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((peer_flags & 0x80U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x0008389aU);
        if ((peer_flags & 0x80U) == 0U) {
            prefetch(host, 0x000834aeU);
            prefetch(host, 0x000834b0U);
            return return_from_subroutine(host, registers);
        }
        prefetch(host, 0x0008389cU);
        prefetch(host, 0x0008389eU);
        prefetch(host, 0x000838a0U);
        prefetch(host, 0x000838a2U);
        const auto peer_prior = read_byte(host, peer_address);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((peer_prior & 0x04U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x000838a4U);
        write_byte(host, peer_address,
            static_cast<std::uint8_t>(peer_prior & 0xfbU));
        prefetch(host, 0x000838a6U);
        set_compare_byte_flags(registers,
            static_cast<std::uint8_t>(registers.data[7]), 6U);
        prefetch(host, 0x000838a8U);
        if (static_cast<std::uint8_t>(registers.data[7]) < 6U) {
            prefetch(host, 0x000838b4U);
            prefetch(host, 0x000838b6U);
            registers.address[3] -= 0x320U;
            prefetch(host, 0x000838b8U);
            prefetch(host, 0x000838baU);
            child = call_native(context, 92U,
                0x000838b8U, 0x00084164U, 0x000838bcU);
            if (child.status != TranslationStatus::complete) return child;
            registers.address[3] += 0x320U;
            prefetch(host, 0x000838c0U);
            prefetch(host, 0x000838c2U);
            return return_from_subroutine(host, registers);
        }
        registers.data[0] = (registers.data[0] & 0xffff0000U)
            | static_cast<std::uint16_t>(registers.data[0] + 0x00a0U);
        prefetch(host, 0x000838acU);
        const auto adjusted = registers.address[3]
            + static_cast<std::uint32_t>(static_cast<std::int32_t>(
                static_cast<std::int16_t>(registers.data[0])));
        const auto adjusted_prior = read_byte(host, adjusted);
        registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
            | ((adjusted_prior & 0x04U) == 0U ? 0x0004U : 0U));
        write_byte(host, adjusted,
            static_cast<std::uint8_t>(adjusted_prior & 0xfbU));
        prefetch(host, 0x000838b2U);
        return return_from_subroutine(host, registers);
    }
    case 6:
        prefetch(host, 0x00083744U);
        prefetch(host, 0x00083746U);
        value = read_byte(host, registers.address[4]++);
        prefetch(host, 0x00083748U);
        write_byte(host, registers.address[3] + 10U,
            static_cast<std::uint8_t>(value));
        set_logic_flags(registers, value, 0x80U);
        prefetch(host, 0x0008374aU);
        return return_from_subroutine(host, registers);
    case 7:
        prefetch(host, 0x0008374aU);
        prefetch(host, 0x0008374cU);
        value = read_byte(host, registers.address[4]++);
        prefetch(host, 0x0008374eU);
        write_byte(host, registers.address[3] + 9U,
            static_cast<std::uint8_t>(value));
        set_logic_flags(registers, value, 0x80U);
        prefetch(host, 0x00083750U);
        return return_from_subroutine(host, registers);
    default:
        return FunctionResult::unimplemented();
    }
}

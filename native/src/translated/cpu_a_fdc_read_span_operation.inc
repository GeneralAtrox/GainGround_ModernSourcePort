FunctionResult cpu_a_fdc_read_span(FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x00000e1eU)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;

    pf(host, 0x00000e22U);
    pf(host, 0x00000e24U);
    const auto original = read_hardware_byte(host, 0x00000e1eU, 0x00b00005U);
    r.data[2] = (r.data[2] & 0xffffff00U) | original;
    logic(r, original, 0x80U);

    pf(host, 0x00000e26U);
    const auto inverted = static_cast<std::uint8_t>(~original);
    r.data[2] = (r.data[2] & 0xffffff00U) | inverted;
    logic(r, inverted, 0x80U);

    pf(host, 0x00000e28U);
    pf(host, 0x00000e2aU);
    pf(host, 0x00000e2cU);
    write_hardware_byte(host, 0x00000e26U, 0x00b00005U, inverted);
    logic(r, inverted, 0x80U);

    pf(host, 0x00000e2eU);
    r.address[7] -= 4U;
    const auto first_stack = read_shared_long(host, r.address[7]);
    logic(r, first_stack, 0x80000000U);

    pf(host, 0x00000e30U);
    const auto second_stack = read_shared_long(host, r.address[7]);
    r.address[7] += 4U;
    logic(r, second_stack, 0x80000000U);

    pf(host, 0x00000e32U);
    pf(host, 0x00000e34U);
    pf(host, 0x00000e36U);
    const auto current = read_hardware_byte(host, 0x00000e30U, 0x00b00005U);
    const auto compare_result = static_cast<std::uint8_t>(inverted - current);
    subtract_flags(r, inverted, current, compare_result, 0x80U, 0xffU, false);
    r.status = host.apply_controlled_status(
        0U, 0xffU, 0x00000e36U, 0x00b00004U, r.status);
    pf(host, 0x00000e38U);

    if ((r.status & 0x0004U) == 0U) {
        pf(host, 0x00000eb2U);
        pf(host, 0x00000eb4U);

        auto child = call_child(context, 443U, 0x00000eb2U,
            0x000017a4U, 0x00000eb6U);
        if (child.status != TranslationStatus::complete)
            return child;

        bool child_return_prefetched = true;
        for (;;) {
        if (!child_return_prefetched)
            pf(host, 0x00000eb8U);
        child_return_prefetched = false;
        pf(host, 0x00000ebaU);
        pf(host, 0x00000ebcU);
        host.write_hardware(2U, 0U, 0xffU, 0x00000eb6U,
            0x00bc0000U, static_cast<std::uint16_t>(r.data[7]), 0xffffU);
        logic(r, static_cast<std::uint16_t>(r.data[7]), 0x8000U);

        pf(host, 0x00000ebeU);
        pf(host, 0x00000ec0U);
        pf(host, 0x00000ec2U);
        r.address[0] = 0x00b00000U;

        pf(host, 0x00000ec4U);
        r.address[0] += r.data[6];

        pf(host, 0x00000ec6U);
        pf(host, 0x00000ec8U);
        pf(host, 0x00000ecaU);
        r.data[0] = 0x00040000U;
        logic(r, r.data[0], 0x80000000U);

        pf(host, 0x00000eccU);
        const auto before_sub = r.data[0];
        r.data[0] -= r.data[6];
        subtract_flags(r, before_sub, r.data[6], r.data[0],
            0x80000000U, 0xffffffffU, true);

        pf(host, 0x00000eceU);
        r.data[6] = 0U;
        logic(r, 0U, 0x80000000U);

        pf(host, 0x00000ed0U);
        const auto count_before = r.data[1];
        r.data[1] -= r.data[0];
        subtract_flags(r, count_before, r.data[0], r.data[1],
            0x80000000U, 0xffffffffU, true);

        pf(host, 0x00000ed2U);
        if ((r.status & 1U) != 0U) {
            pf(host, 0x00000ed4U);
            const auto add_left = r.data[0];
            r.data[0] += r.data[1];
            add_flags(r, add_left, r.data[1], r.data[0],
                0x80000000U, 0xffffffffULL);
            pf(host, 0x00000ed6U);
            r.data[1] = 0U;
            logic(r, 0U, 0x80000000U);
        } else {
            pf(host, 0x00000ed6U);
        }

        for (;;) {
            pf(host, 0x00000ed8U);
            const auto value = read_copy_byte(host, 0x00000ed6U, r.address[0]);
            ++r.address[0];
            write_copy_byte(host, 0x00000ed6U, r.address[1], value);
            ++r.address[1];
            logic(r, value, 0x80U);

            pf(host, 0x00000edaU);
            const auto decrement_before = r.data[0];
            --r.data[0];
            subtract_flags(r, decrement_before, 1U, r.data[0],
                0x80000000U, 0xffffffffU, true);

            pf(host, 0x00000edcU);
            if ((r.status & 1U) == 0U) {
                pf(host, 0x00000ed6U);
                continue;
            }
            pf(host, 0x00000edeU);
            break;
        }

        const auto d7_before = static_cast<std::uint16_t>(r.data[7]);
        const auto d7_after = static_cast<std::uint16_t>(d7_before + 1U);
        r.data[7] = (r.data[7] & 0xffff0000U) | d7_after;
        add_flags(r, d7_before, 1U, d7_after, 0x8000U, 0xffffU);

        pf(host, 0x00000ee0U);
        logic(r, r.data[1], 0x80000000U);
        pf(host, 0x00000ee2U);
        if ((r.status & 4U) == 0U) {
            pf(host, 0x00000eb6U);
            continue;
        }
        pf(host, 0x00000ee4U);
        return return_from_subroutine(context);
        }
    }

    pf(host, 0x00000e3aU);
    pf(host, 0x00000e3cU);
    pf(host, 0x00000e3eU);
    r.address[5] = 0x00b00001U;

    pf(host, 0x00000e40U);
    r.data[2] = r.data[0];
    logic(r, r.data[2], 0x80000000U);
    pf(host, 0x00000e42U);
    r.data[4] = r.data[1];
    logic(r, r.data[4], 0x80000000U);

    pf(host, 0x00000e44U);
    wait_bit0_clear(context, 0x00000e44U, 0x00000e4aU, r.address[5]);
    pf(host, 0x00000e4cU);
    pf(host, 0x00000e4eU);
    pf(host, 0x00000e50U);
    pf(host, 0x00000e52U);
    write_hardware_byte(host, 0x00000e4cU, r.address[5], 0xd0U);
    logic(r, 0xd0U, 0x80U);

    wait_bit0_clear(context, 0x00000e52U, 0x00000e58U, r.address[5]);
    pf(host, 0x00000e5aU);
    pf(host, 0x00000e5cU);
    pf(host, 0x00000e5eU);
    pf(host, 0x00000e60U);
    write_hardware_byte(host, 0x00000e5aU, r.address[5] + 6U, 0xc0U);
    logic(r, 0xc0U, 0x80U);

    pf(host, 0x00000e62U);
    pf(host, 0x00000e64U);
    pf(host, 0x00000e66U);
    write_hardware_byte(host, 0x00000e60U, r.address[5], 0xfeU);
    logic(r, 0xfeU, 0x80U);

    wait_bit0_clear(context, 0x00000e66U, 0x00000e6cU, r.address[5]);
    pf(host, 0x00000e6eU);
    pf(host, 0x00000e70U);
    pf(host, 0x00000e72U);
    pf(host, 0x00000e74U);
    write_hardware_byte(host, 0x00000e6eU, r.address[5] + 6U, 0x8aU);
    logic(r, 0x8aU, 0x80U);

    pf(host, 0x00000e76U);
    pf(host, 0x00000e78U);
    pf(host, 0x00000e7aU);
    write_hardware_byte(host, 0x00000e74U, r.address[5], 0xfdU);
    logic(r, 0xfdU, 0x80U);

    r.data[1] = r.data[4];
    logic(r, r.data[1], 0x80000000U);
    pf(host, 0x00000e7cU);
    r.data[0] = r.data[2];
    logic(r, r.data[0], 0x80000000U);

    pf(host, 0x00000e7eU);
    pf(host, 0x00000e80U);
    auto child = call_child(context, 18U, 0x00000e7eU,
        0x00001796U, 0x00000e82U);
    if (child.status != TranslationStatus::complete)
        return child;

    r.address[2] = r.address[0];

    for (;;) {
        pf(host, 0x00000e86U);
        child = call_child(context, 15U, 0x00000e84U,
            0x0000161aU, 0x00000e88U);
        if (child.status != TranslationStatus::complete)
            return child;
        if ((r.status & 1U) != 0U) {
            pf(host, 0x00000e8cU);
            pf(host, 0x00001984U);
            pf(host, 0x00001986U);
            r.program_counter = 0x00001984U;
            return host.call_function(517U, 0U, 0xffU, 1U,
                0x00000e88U, 0x00001984U, context);
        }

        pf(host, 0x00000e8cU);
        r.address[0] = static_cast<std::uint32_t>(r.address[2]
            + static_cast<std::int16_t>(r.data[6]));

        pf(host, 0x00000e8eU);
        pf(host, 0x00000e90U);
        pf(host, 0x00000e92U);
        r.data[0] = 0x00002d00U;
        logic(r, r.data[0], 0x80000000U);

        pf(host, 0x00000e94U);
        const auto word_before = static_cast<std::uint16_t>(r.data[0]);
        const auto word_source = static_cast<std::uint16_t>(r.data[6]);
        const auto word_result = static_cast<std::uint16_t>(word_before - word_source);
        r.data[0] = (r.data[0] & 0xffff0000U) | word_result;
        subtract_flags(r, word_before, word_source, word_result,
            0x8000U, 0xffffU, true);

        pf(host, 0x00000e96U);
        r.data[6] = 0U;
        logic(r, 0U, 0x80000000U);

        pf(host, 0x00000e98U);
        const auto count_before = r.data[1];
        r.data[1] -= r.data[0];
        subtract_flags(r, count_before, r.data[0], r.data[1],
            0x80000000U, 0xffffffffU, true);

        pf(host, 0x00000e9aU);
        if ((r.status & 1U) != 0U) {
            pf(host, 0x00000e9cU);
            const auto add_left = r.data[0];
            r.data[0] += r.data[1];
            add_flags(r, add_left, r.data[1], r.data[0],
                0x80000000U, 0xffffffffULL);
            pf(host, 0x00000e9eU);
            pf(host, 0x00000ea0U);
            r.data[1] = 0U;
            logic(r, 0U, 0x80000000U);
        } else {
            pf(host, 0x00000e9cU);
            pf(host, 0x00000e9eU);
        }

        pf(host, 0x00000ea2U);
        const auto subq_before = static_cast<std::uint16_t>(r.data[0]);
        const auto subq_after = static_cast<std::uint16_t>(subq_before - 1U);
        r.data[0] = (r.data[0] & 0xffff0000U) | subq_after;
        subtract_flags(r, subq_before, 1U, subq_after,
            0x8000U, 0xffffU, true);

        pf(host, 0x00000ea4U);
        for (;;) {
            pf(host, 0x00000ea6U);
            const auto value = read_shared_byte(host, r.address[0]);
            ++r.address[0];
            write_copy_byte(host, 0x00000ea4U, r.address[1], value);
            ++r.address[1];
            logic(r, value, 0x80U);

            pf(host, 0x00000ea8U);
            auto counter = static_cast<std::uint16_t>(r.data[0] - 1U);
            r.data[0] = (r.data[0] & 0xffff0000U) | counter;
            if (counter != 0xffffU) {
                pf(host, 0x00000ea4U);
                continue;
            }
            pf(host, 0x00000ea4U);
            pf(host, 0x00000eaaU);
            break;
        }

        pf(host, 0x00000eacU);
        const auto d7_before = static_cast<std::uint16_t>(r.data[7]);
        const auto d7_after = static_cast<std::uint16_t>(d7_before + 1U);
        r.data[7] = (r.data[7] & 0xffff0000U) | d7_after;
        add_flags(r, d7_before, 1U, d7_after, 0x8000U, 0xffffU);

        pf(host, 0x00000eaeU);
        logic(r, r.data[1], 0x80000000U);
        pf(host, 0x00000eb0U);
        if ((r.status & 4U) == 0U) {
            pf(host, 0x00000e84U);
            continue;
        }
        pf(host, 0x00000eb2U);
        return return_from_subroutine(context);
    }
}


FunctionResult cpu_a_fdc_transfer_verified_buffer(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;

    pf(host, 0x0000101cU);
    pf(host, 0x0000101eU);
    auto value = rhb(host, 0x00001018U, 0x00b00005U);
    r.data[0] = (r.data[0] & 0xffffff00U) | value;
    logic_byte(r, value);
    value = static_cast<std::uint8_t>(~value);
    r.data[0] = (r.data[0] & 0xffffff00U) | value;
    logic_byte(r, value);
    pf(host, 0x00001020U);
    pf(host, 0x00001022U);
    pf(host, 0x00001024U);
    pf(host, 0x00001026U);
    whb(host, 0x00001020U, 0x00b00005U, value);

    pf(host, 0x00001028U);
    r.address[7] -= 4U;
    auto scratch = (static_cast<std::uint32_t>(rs(host, r.address[7])) << 16U)
        | rs(host, r.address[7] + 2U);
    logic_long(r, scratch);
    pf(host, 0x0000102aU);
    scratch = (static_cast<std::uint32_t>(rs(host, r.address[7])) << 16U)
        | rs(host, r.address[7] + 2U);
    r.address[7] += 4U;
    logic_long(r, scratch);
    pf(host, 0x0000102cU);
    pf(host, 0x0000102eU);
    pf(host, 0x00001030U);
    const auto compare = rhb(host, 0x0000102aU, 0x00b00005U);
    compare_byte(r, static_cast<std::uint8_t>(r.data[0]), compare);
    r.status = host.apply_controlled_status(
        0U, 0xffU, 0x00001030U, 0x00b00004U, r.status);
    pf(host, 0x00001032U);
    const bool alternate = (r.status & 0x0004U) == 0U;

    if (!alternate) {
        pf(host, 0x00001034U);
        pf(host, 0x00001036U);
        pf(host, 0x00001038U);
        pf(host, 0x0000103aU);
        r.address[5] = 0x00b00001U;
        pf(host, 0x0000103cU);
        pf(host, 0x0000103eU);
        r.address[7] -= 2U;
        ws(host, r.address[7], static_cast<std::uint16_t>(r.data[1]));
        pf(host, 0x00001040U);
        r.address[7] -= 4U;
        ws(host, r.address[7] + 2U, static_cast<std::uint16_t>(r.address[1]));
        ws(host, r.address[7], static_cast<std::uint16_t>(r.address[1] >> 16U));

        for (;;) {
            pf(host, 0x00001042U);
            pf(host, 0x00001044U);
            const auto status = rhb(host, 0x0000103eU, r.address[5]);
            logic_byte(r, static_cast<std::uint8_t>(status & 1U));
            r.status = host.apply_controlled_status(
                0U, 0xffU, 0x00001044U, 0x00b00000U, r.status);
            if ((r.status & 0x0004U) != 0U) break;
            pf(host, 0x00001046U);
            pf(host, 0x0000103eU);
            pf(host, 0x00001040U);
        }
        pf(host, 0x00001046U);
        pf(host, 0x00001048U);
        pf(host, 0x0000104aU);
        pf(host, 0x0000104cU);
        whb(host, 0x00001046U, r.address[5], 0xd0U);
        for (;;) {
            pf(host, 0x0000104eU);
            pf(host, 0x00001050U);
            pf(host, 0x00001052U);
            const auto status = rhb(host, 0x0000104cU, r.address[5]);
            logic_byte(r, static_cast<std::uint8_t>(status & 1U));
            r.status = host.apply_controlled_status(
                0U, 0xffU, 0x00001052U, 0x00b00000U, r.status);
            if ((r.status & 0x0004U) != 0U) break;
            pf(host, 0x00001054U);
            pf(host, 0x0000104cU);
        }
        pf(host, 0x00001054U);
        pf(host, 0x00001056U);
        pf(host, 0x00001058U);
        pf(host, 0x0000105aU);
        whb(host, 0x00001054U, r.address[5] + 6U, 0xc0U);
        pf(host, 0x0000105cU);
        pf(host, 0x0000105eU);
        pf(host, 0x00001060U);
        whb(host, 0x0000105aU, r.address[5], 0xfeU);
        for (;;) {
            pf(host, 0x00001062U);
            pf(host, 0x00001064U);
            pf(host, 0x00001066U);
            const auto status = rhb(host, 0x00001060U, r.address[5]);
            logic_byte(r, static_cast<std::uint8_t>(status & 1U));
            r.status = host.apply_controlled_status(
                0U, 0xffU, 0x00001066U, 0x00b00000U, r.status);
            if ((r.status & 0x0004U) != 0U) break;
            pf(host, 0x00001068U);
            pf(host, 0x00001060U);
        }
        pf(host, 0x00001068U);
        pf(host, 0x0000106aU);
        pf(host, 0x0000106cU);
        pf(host, 0x0000106eU);
        whb(host, 0x00001068U, r.address[5] + 6U, 0x8aU);
        pf(host, 0x00001070U);
        pf(host, 0x00001072U);
        pf(host, 0x00001074U);
        whb(host, 0x0000106eU, r.address[5], 0xfdU);
        pf(host, 0x00001076U);
        r.data[7] = 0U;
        pf(host, 0x00001078U);
        r.address[2] = r.address[0];
        pf(host, 0x0000107aU);

        auto child = call(context, 15U, 0x00001078U,
            0x0000161aU, 0x0000107cU);
        if (!complete(child)) return child;
        if ((r.status & 1U) != 0U) {
            r.data[7] = 0U;
            pf(host, 0x00001080U);
            pf(host, 0x00001082U);
            child = call(context, 439U, 0x00001080U,
                0x00001258U, 0x00001084U);
            if (!complete(child)) return child;
        } else {
            pf(host, 0x00001086U);
            pf(host, 0x00001088U);
            child = call(context, 14U, 0x00001086U,
                0x00001284U, 0x0000108aU);
            if (!complete(child)) return child;
            r.status = host.apply_controlled_status(
                0U, 0xffU, 0x0000108aU, 0x00b00000U, r.status);
            if ((r.status & 1U) == 0U) goto verify_first;
        }

        pf(host, 0x0000108eU);
        r.data[7] = 1U;
        pf(host, 0x00001090U);
        pf(host, 0x00001092U);
        child = call(context, 15U, 0x00001090U,
            0x0000161aU, 0x00001094U);
        if (!complete(child)) return child;
        if ((r.status & 1U) != 0U) {
            r.data[7] = 1U;
            pf(host, 0x00001098U);
            pf(host, 0x0000109aU);
            child = call(context, 439U, 0x00001098U,
                0x00001258U, 0x0000109cU);
            if (!complete(child)) return child;
            goto restore_error;
        }
        pf(host, 0x000010a0U);
        pf(host, 0x000010a2U);
        child = call(context, 14U, 0x000010a0U,
            0x00001284U, 0x000010a4U);
        if (!complete(child)) return child;
        r.status = host.apply_controlled_status(
            0U, 0xffU, 0x000010a4U, 0x00b00000U, r.status);
        if ((r.status & 1U) != 0U) goto restore_error;

        pf(host, 0x000010a8U);
        r.data[7] = 0U;
        pf(host, 0x000010aaU);
        pf(host, 0x000010acU);
        child = call(context, 17U, 0x000010aaU,
            0x00001710U, 0x000010aeU);
        if (!complete(child)) return child;
        r.status = host.apply_controlled_status(
            0U, 0xffU, 0x000010aeU, 0x00b00000U, r.status);
        if ((r.status & 1U) != 0U)
            return terminal_call(context, 518U, 0x000010aeU, 0x0000198cU);
        pf(host, 0x000010b2U);
        pf(host, 0x000010b4U);
        child = call(context, 16U, 0x000010b2U,
            0x000016a0U, 0x000010b6U);
        if (!complete(child)) return child;
        r.status = host.apply_controlled_status(
            0U, 0xffU, 0x000010b6U, 0x00b00000U, r.status);
        if ((r.status & 1U) != 0U)
            return terminal_call(context, 518U, 0x000010b6U, 0x0000198cU);
        pf(host, 0x000010baU);
        pf(host, 0x000010bcU);
        goto restore_and_return;

verify_first:
        pf(host, 0x000010bcU);
        r.data[7] = 1U;
        pf(host, 0x000010beU);
        pf(host, 0x000010c0U);
        child = call(context, 22U, 0x000010beU,
            0x00001806U, 0x000010c2U);
        if (!complete(child)) return child;
        child = call(context, 16U, 0x000010c2U,
            0x000016a0U, 0x000010c6U);
        if (!complete(child)) return child;
        r.status = host.apply_controlled_status(
            0U, 0xffU, 0x000010c6U, 0x00b00000U, r.status);
        if ((r.status & 1U) != 0U) {
            pf(host, 0x000010caU);
            child = call(context, 438U, 0x000010c8U,
                0x00001236U, 0x000010ccU);
            if (!complete(child)) return child;
            goto restore_after_prefetch;
        }

restore_and_return:
        pf(host, 0x000010ccU);
        pf(host, 0x000010ceU);
restore_after_prefetch:
        r.address[0] = r.address[2];
        pf(host, 0x000010d0U);
        {
            const auto stack = r.address[7] & kSharedMask;
            r.address[1] = (static_cast<std::uint32_t>(rs(host, stack)) << 16U)
                | rs(host, stack + 2U);
            r.address[7] += 4U;
            pf(host, 0x000010d2U);
            r.data[1] = (r.data[1] & 0xffff0000U) | rs(host, r.address[7]);
            r.address[7] += 2U;
        }
        pf(host, 0x000010d4U);
        {
            auto count = static_cast<std::uint16_t>(r.data[1] - 1U);
            r.data[1] = (r.data[1] & 0xffff0000U) | count;
            do {
                pf(host, 0x000010d6U);
                const auto byte = rsb(host, r.address[0]++);
                wsb(host, r.address[1]++, byte);
                count = static_cast<std::uint16_t>(count - 1U);
                r.data[1] = (r.data[1] & 0xffff0000U) | count;
                pf(host, 0x000010d8U);
                pf(host, 0x000010d4U);
                if (count == 0xffffU) pf(host, 0x000010daU);
            } while (count != 0xffffU);
        }
        pf(host, 0x000010dcU);
        return do_return(context);

restore_error:
        pf(host, 0x000010dcU);
        pf(host, 0x000010deU);
        pf(host, 0x000010e0U);
        pf(host, 0x000010e2U);
        r.address[7] += 6U;
        r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | 1U);
        pf(host, 0x000010e2U);
        pf(host, 0x000010e4U);
        return do_return(context);
    }

    pf(host, 0x000010e4U);
    pf(host, 0x000010e6U);
    r.data[6] = (r.data[6] & 0xffff0000U)
        | static_cast<std::uint16_t>(r.data[1]);
    r.data[7] = 0U;
    pf(host, 0x000010e8U);
    pf(host, 0x000010eaU);
    auto child = call(context, 440U, 0x000010e8U,
        0x000012a8U, 0x000010ecU);
    if (!complete(child)) return child;
    r.status = host.apply_controlled_status(
        0U, 0xffU, 0x000010ecU, 0x00b81ffeU, r.status);
    if ((r.status & 1U) == 0U) goto alternate_second;
    pf(host, 0x000010f0U);
    pf(host, 0x000010f2U);
    r.data[7] = (r.data[7] & 0xffff0000U) | 0x2000U;
    pf(host, 0x000010f4U);
    pf(host, 0x000010f6U);
    child = call(context, 440U, 0x000010f4U,
        0x000012a8U, 0x000010f8U);
    if (!complete(child)) return child;
    r.status = host.apply_controlled_status(
        0U, 0xffU, 0x000010f8U, 0x00b83ffeU, r.status);
    if ((r.status & 1U) != 0U) goto alternate_error;

    pf(host, 0x000010fcU);
    pf(host, 0x000010feU);
    r.address[0] = 0x00b82000U;
    pf(host, 0x00001100U);
    pf(host, 0x00001102U);
    pf(host, 0x00001104U);
    r.address[2] = 0x00b80000U;
    pf(host, 0x00001106U);
    pf(host, 0x00001108U);
    r.data[0] = (r.data[0] & 0xffff0000U) | 0x07ffU;
    pf(host, 0x0000110aU);
    pf(host, 0x0000110cU);
    pf(host, 0x0000110eU);
    for (;;) {
        const auto value_long = rhl(host, 0x0000110cU, r.address[0]);
        r.address[0] += 4U;
        whl(host, 0x0000110cU, r.address[2], value_long);
        r.address[2] += 4U;
        auto count = static_cast<std::uint16_t>(r.data[0] - 1U);
        r.data[0] = (r.data[0] & 0xffff0000U) | count;
        pf(host, 0x00001110U);
        pf(host, 0x0000110cU);
        if (count == 0xffffU) {
            pf(host, 0x00001112U);
            break;
        }
        pf(host, 0x0000110eU);
    }
    pf(host, 0x00001114U);
    child = call(context, 442U, 0x00001112U,
        0x000012ceU, 0x00001116U);
    if (!complete(child)) return child;
    r.status = host.apply_controlled_status(
        0U, 0xffU, 0x00001116U, 0x00b83ffeU, r.status);
    if ((r.status & 1U) != 0U) return terminal(context, 0x00001de2U);
    pf(host, 0x0000111aU);
    pf(host, 0x0000111cU);
    goto alternate_copy;

alternate_second:
    pf(host, 0x0000111cU);
    pf(host, 0x0000111eU);
    child = call(context, 442U, 0x0000111cU,
        0x000012ceU, 0x00001120U);
    if (!complete(child)) return child;
    if ((r.status & 1U) == 0U) goto alternate_copy;
    pf(host, 0x00001124U);
    child = call(context, 441U, 0x00001122U,
        0x000012b6U, 0x00001126U);
    if (!complete(child)) return child;
    child = call(context, 442U, 0x00001126U,
        0x000012ceU, 0x0000112aU);
    if (!complete(child)) return child;
    r.status = host.apply_controlled_status(
        0U, 0xffU, 0x0000112aU, 0x00b83ffeU, r.status);
    if ((r.status & 1U) != 0U) return terminal(context, 0x00001de2U);

alternate_copy:
    pf(host, 0x0000112eU);
    pf(host, 0x00001130U);
    pf(host, 0x00001132U);
    {
        auto count = static_cast<std::uint16_t>(r.data[6] - 1U);
        r.data[6] = (r.data[6] & 0xffff0000U) | count;
        do {
            const auto byte = rhb_addressed(
                host, 0x00001130U, r.address[0]++);
            wsb(host, r.address[1]++, byte);
            pf(host, 0x00001134U);
            count = static_cast<std::uint16_t>(count - 1U);
            r.data[6] = (r.data[6] & 0xffff0000U) | count;
            pf(host, 0x00001130U);
            if (count != 0xffffU) {
                pf(host, 0x00001132U);
            } else {
                pf(host, 0x00001136U);
                pf(host, 0x00001138U);
            }
        } while (count != 0xffffU);
    }
    return do_return(context);

alternate_error:
    pf(host, 0x00001138U);
    pf(host, 0x0000113aU);
    pf(host, 0x0000113cU);
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | 1U);
    pf(host, 0x0000113cU);
    pf(host, 0x0000113eU);
    return do_return(context);
}


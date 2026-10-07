CpuAProcessAssetLoadRequestStep handle_cpu_a_process_asset_load_request_pc_080520_end(
    FunctionContext &c, CpuRegisters &r, unverified::Machine &m,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind)
{
    (void)transfer_kind;
    switch (pc) {
        case 0x80520U: { // 4e75 rts 
            next = 0x80522U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x80520U, r.program_counter)) return {true, (*event)};
            return {true, (result)};
            break;
        }
    default: return {};
    }
    return {true, std::nullopt};
}

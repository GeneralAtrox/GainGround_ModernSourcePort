// Implemented but unverified. Original bytes: 66f6
// Rule: bne_rel8 fff266f8da0353198863462b6cf5bdf1e56c8d9095f69e816b00a14d0a9f6193
inline bool sound_bne(gain_ground::TimingInstruction &timing, SoundInstructionOps &native)
{
    if (!timing.good()) return false;
    // timing-node b0n0 i
    // 308 bbci1
    // timing-node b0n1 lines
    // Reference emitter has no executable statement for this node.
    // timing-node b0n2 =t
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n3 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n4 istep
    if (!timing.clocks(2U)) return false;
    // timing-node b0n5 cgoto
    { const auto choice = ((native.registers.status & 4U) == 0U);
      if (choice) goto timing_b2n0;
      goto timing_b1n0; }
    // timing-node b1n0 label
timing_b1n0:;
    // timing-node b1n1 i
    // 045 bbci2
    // timing-node b1n2 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b1n3 i
    {
    if (!timing.good()) return false;
    }
    // timing-node b1n4 lines
    // Reference emitter has no executable statement for this node.
    // timing-node b1n5 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b1n6 i
    {
    if (!timing.good()) return false;
    }
    // timing-node b1n7 istep
    if (!timing.clocks(2U)) return false;
    // timing-node b1n8 set_trace
    {
    if (!timing.good()) return false;
    }
    // timing-node b1n9 goto
    goto timing_b3n0;
    // timing-node b2n0 label
timing_b2n0:;
    // timing-node b2n1 i
    // 0c5 bbci3
    // timing-node b2n2 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b2n3 bus
    // Reference emitter has no executable statement for this node.
    // timing-node b2n4 lines
    // Reference emitter has no executable statement for this node.
    // timing-node b2n5 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b2n6 bus_end
    {
      const auto bus_address = static_cast<std::uint32_t>(native.branch_pc());
      const auto bus_mask = static_cast<std::uint16_t>(0xffffU);
      const auto bus_result = timing.read("b2n6", "program", 16U,
          bus_address, bus_mask, [&]() { return native.program(timing, bus_address); });
      if (!timing.good()) return false;
      native.prefetched = bus_result;
      if (!timing.good()) return false;
    }
    // timing-node b2n7 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b2n8 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b2n9 goto
    goto timing_b3n0;
    // timing-node b3n0 label
timing_b3n0:;
    // timing-node b3n1 i
    // 363 b
    // timing-node b3n2 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b3n3 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b3n4 bus
    // Reference emitter has no executable statement for this node.
    // timing-node b3n5 lines
    // Reference emitter has no executable statement for this node.
    // timing-node b3n6 drop_critical
    // Reference emitter has no executable statement for this node.
    // timing-node b3n7 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b3n8 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b3n9 i
    // 34c mmrw3
    // timing-node b3n10 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b3n11 i
    {
    native.sample_interrupt();
    if (!timing.good()) return false;
    }
    // timing-node b3n12 lines
    // Reference emitter has no executable statement for this node.
    // timing-node b3n13 bus_end
    {
      const auto bus_address = static_cast<std::uint32_t>(native.branch_pc() + 2U);
      const auto bus_mask = static_cast<std::uint16_t>(0xffffU);
      const auto bus_result = timing.read("b3n13", "program", 16U,
          bus_address, bus_mask, [&]() { return native.program(timing, bus_address); });
      if (!timing.good()) return false;
      native.prefetched = bus_result;
      if (!timing.good()) return false;
    }
    // timing-node b3n14 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b3n15 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b3n16 i
    {
    if (!timing.good()) return false;
    }
    // timing-node b3n17 next_trace
    {
    native.registers.program_counter = native.next_pc();
    return timing.complete(native.next_pc(), native.state, "next_trace");
    }
    return timing.unsupported("reference graph fell through");
}

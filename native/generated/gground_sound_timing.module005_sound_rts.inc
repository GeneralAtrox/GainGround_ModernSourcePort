// Implemented but unverified. Original bytes: 4e75
// Rule: rts 537ce6d13cb42f7423373a6bcfa5010cfb7f363dd6663bdfa73899aa6b7dbb58
inline bool sound_rts(gain_ground::TimingInstruction &timing, SoundInstructionOps &native)
{
    if (!timing.good()) return false;
    // timing-node b0n0 i
    // 126 rts1
    // timing-node b0n1 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n2 bus
    // Reference emitter has no executable statement for this node.
    // timing-node b0n3 lines
    // Reference emitter has no executable statement for this node.
    // timing-node b0n4 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n5 i
    // alu r=0 c=1 m=.....  i=....... 1 a=31:m_dbin d=-1
    // timing-node b0n6 alu
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n7 bus_end
    {
      const auto bus_address = static_cast<std::uint32_t>(native.stack);
      const auto bus_mask = static_cast<std::uint16_t>(0xffffU);
      const auto bus_result = timing.read("b0n7", "data", 16U,
          bus_address, bus_mask, [&]() { return native.data_word(bus_address); });
      if (!timing.good()) return false;
      native.return_high = bus_result;
      if (!timing.good()) return false;
    }
    // timing-node b0n8 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n9 i
    // 116 rts2
    // timing-node b0n10 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n11 bus
    // Reference emitter has no executable statement for this node.
    // timing-node b0n12 lines
    // Reference emitter has no executable statement for this node.
    // timing-node b0n13 =16h
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n14 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n15 bus_end
    {
      const auto bus_address = static_cast<std::uint32_t>(native.stack + 2U);
      const auto bus_mask = static_cast<std::uint16_t>(0xffffU);
      const auto bus_result = timing.read("b0n15", "data", 16U,
          bus_address, bus_mask, [&]() { return native.data_word(bus_address); });
      if (!timing.good()) return false;
      native.return_low = bus_result;
      if (!timing.good()) return false;
      native.registers.address[7] = native.stack + 4U;
    }
    // timing-node b0n16 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n17 i
    // 303 rts3
    // timing-node b0n18 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n19 bus
    // Reference emitter has no executable statement for this node.
    // timing-node b0n20 lines
    // Reference emitter has no executable statement for this node.
    // timing-node b0n21 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n22 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n23 bus_end
    {
      const auto bus_address = static_cast<std::uint32_t>(native.return_pc());
      const auto bus_mask = static_cast<std::uint16_t>(0xffffU);
      const auto bus_result = timing.read("b0n23", "program", 16U,
          bus_address, bus_mask, [&]() { return native.program(timing, bus_address); });
      if (!timing.good()) return false;
      native.prefetched = bus_result;
      if (!timing.good()) return false;
    }
    // timing-node b0n24 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n25 i
    // 363 b
    // timing-node b0n26 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n27 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n28 bus
    // Reference emitter has no executable statement for this node.
    // timing-node b0n29 lines
    // Reference emitter has no executable statement for this node.
    // timing-node b0n30 drop_critical
    // Reference emitter has no executable statement for this node.
    // timing-node b0n31 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n32 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n33 i
    // 34c mmrw3
    // timing-node b0n34 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n35 i
    {
    native.sample_interrupt();
    if (!timing.good()) return false;
    }
    // timing-node b0n36 lines
    // Reference emitter has no executable statement for this node.
    // timing-node b0n37 bus_end
    {
      const auto bus_address = static_cast<std::uint32_t>(native.return_pc() + 2U);
      const auto bus_mask = static_cast<std::uint16_t>(0xffffU);
      const auto bus_result = timing.read("b0n37", "program", 16U,
          bus_address, bus_mask, [&]() { return native.program(timing, bus_address); });
      if (!timing.good()) return false;
      native.prefetched = bus_result;
      if (!timing.good()) return false;
    }
    // timing-node b0n38 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n39 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n40 i
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n41 next_trace
    {
    native.registers.program_counter = native.next_pc();
    return timing.complete(native.next_pc(), native.state, "next_trace");
    }
    return timing.unsupported("reference graph fell through");
}

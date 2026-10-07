// Implemented but unverified. Original bytes: 0839000700800103
// Rule: btst_imm8_adr32 b85b357580f3d21b80f77855336e2282f590d5e57cce43a397e6e9c6cc91ce38
inline bool sound_btst(gain_ground::TimingInstruction &timing, SoundInstructionOps &native)
{
    if (!timing.good()) return false;
    // timing-node b0n0 i
    // 2b9 o#w1
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
    // timing-node b0n5 =16l
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n6 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n7 i
    // alu r=13 c=0 m=.....  i=b...... 9 a=31:m_dbin d=none
    // timing-node b0n8 bus_end
    {
      const auto bus_address = static_cast<std::uint32_t>(native.pc + 4U);
      const auto bus_mask = static_cast<std::uint16_t>(0xffffU);
      const auto bus_result = timing.read("b0n8", "program", 16U,
          bus_address, bus_mask, [&]() { return native.program(timing, bus_address); });
      if (!timing.good()) return false;
      native.prefetched = bus_result;
      if (!timing.good()) return false;
    }
    // timing-node b0n9 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n10 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n11 i
    // 1e2 abll1
    // timing-node b0n12 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n13 bus
    // Reference emitter has no executable statement for this node.
    // timing-node b0n14 lines
    // Reference emitter has no executable statement for this node.
    // timing-node b0n15 =16h
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n16 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n17 i
    // alu r=13 c=0 m=.....  i=b...... 9 a=31:m_dbin d=none
    // timing-node b0n18 bus_end
    {
      const auto bus_address = static_cast<std::uint32_t>(native.pc + 6U);
      const auto bus_mask = static_cast<std::uint16_t>(0xffffU);
      const auto bus_result = timing.read("b0n18", "program", 16U,
          bus_address, bus_mask, [&]() { return native.program(timing, bus_address); });
      if (!timing.good()) return false;
      native.prefetched = bus_result;
      if (!timing.good()) return false;
    }
    // timing-node b0n19 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n20 i
    // 009 abll2
    // timing-node b0n21 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n22 bus
    // Reference emitter has no executable statement for this node.
    // timing-node b0n23 lines
    // Reference emitter has no executable statement for this node.
    // timing-node b0n24 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n25 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n26 =16l
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n27 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n28 i
    // alu r=13 c=0 m=.....  i=b...... 9 a=9:m_dt d=31:m_dbin
    // timing-node b0n29 alu
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n30 bus_end
    {
      const auto bus_address = static_cast<std::uint32_t>(native.pc + 8U);
      const auto bus_mask = static_cast<std::uint16_t>(0xffffU);
      const auto bus_result = timing.read("b0n30", "program", 16U,
          bus_address, bus_mask, [&]() { return native.program(timing, bus_address); });
      if (!timing.good()) return false;
      native.prefetched = bus_result;
      if (!timing.good()) return false;
    }
    // timing-node b0n31 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n32 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n33 i
    // 004 abll3
    // timing-node b0n34 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n35 bus
    // Reference emitter has no executable statement for this node.
    // timing-node b0n36 lines
    // Reference emitter has no executable statement for this node.
    // timing-node b0n37 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n38 i
    // alu r=13 c=1 m=.....  i=b...... 1 a=31:m_dbin d=-1
    // timing-node b0n39 alu
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n40 bus_end
    {
      const auto bus_address = static_cast<std::uint32_t>(0x800103U);
      const auto bus_mask = static_cast<std::uint16_t>(0xffU);
      const auto bus_result = timing.read("b0n40", "data", 8U,
          bus_address, bus_mask, [&]() { return native.status_read(bus_address, bus_mask); });
      if (!timing.good()) return false;
      native.status_value = bus_result;
      if (!timing.good()) return false;
    }
    // timing-node b0n41 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n42 i
    // 215 btsm1
    // timing-node b0n43 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n44 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n45 bus
    // Reference emitter has no executable statement for this node.
    // timing-node b0n46 lines
    // Reference emitter has no executable statement for this node.
    // timing-node b0n47 drop_critical
    // Reference emitter has no executable statement for this node.
    // timing-node b0n48 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n49 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n50 i
    // alu r=13 c=1 m=..z..  i=b....i. 1 a=31:m_dbin d=36:m_dcro8
    // timing-node b0n51 alu
    {
    native.test_flags();
    if (!timing.good()) return false;
    }
    // timing-node b0n52 i
    // 34c mmrw3
    // timing-node b0n53 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n54 i
    {
    native.sample_interrupt();
    if (!timing.good()) return false;
    }
    // timing-node b0n55 lines
    // Reference emitter has no executable statement for this node.
    // timing-node b0n56 i
    // alu r=13 c=0 m=.....  i=b...... 9 a=none d=none
    // timing-node b0n57 bus_end
    {
      const auto bus_address = static_cast<std::uint32_t>(native.pc + 10U);
      const auto bus_mask = static_cast<std::uint16_t>(0xffffU);
      const auto bus_result = timing.read("b0n57", "program", 16U,
          bus_address, bus_mask, [&]() { return native.program(timing, bus_address); });
      if (!timing.good()) return false;
      native.prefetched = bus_result;
      if (!timing.good()) return false;
    }
    // timing-node b0n58 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n59 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n60 i
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n61 next_trace
    {
    native.registers.program_counter = native.next_pc();
    return timing.complete(native.next_pc(), native.state, "next_trace");
    }
    return timing.unsupported("reference graph fell through");
}

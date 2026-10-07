// Implemented but unverified. Original bytes: 13c000800101
// Rule: move_b_ds_adr32 d3232d58bcfa816e6dac42e31631dfc2f81777e706b3c797fa1324819db57986
inline bool sound_move(gain_ground::TimingInstruction &timing, SoundInstructionOps &native)
{
    if (!timing.good()) return false;
    // timing-node b0n0 i
    // 1ea ralw1
    // timing-node b0n1 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n2 bus
    // Reference emitter has no executable statement for this node.
    // timing-node b0n3 lines
    // Reference emitter has no executable statement for this node.
    // timing-node b0n4 =16h
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n5 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n6 bus_end
    {
      const auto bus_address = static_cast<std::uint32_t>(native.pc + 4U);
      const auto bus_mask = static_cast<std::uint16_t>(0xffffU);
      const auto bus_result = timing.read("b0n6", "program", 16U,
          bus_address, bus_mask, [&]() { return native.program(timing, bus_address); });
      if (!timing.good()) return false;
      native.prefetched = bus_result;
      if (!timing.good()) return false;
    }
    // timing-node b0n7 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n8 i
    // 2bb ralw2
    // timing-node b0n9 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n10 bus
    // Reference emitter has no executable statement for this node.
    // timing-node b0n11 lines
    // Reference emitter has no executable statement for this node.
    // timing-node b0n12 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n13 =16l
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n14 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n15 i
    // alu r=2 c=1 m=.nzvc  i=b....i. 1 a=4:m_da[ry] d=-1
    // timing-node b0n16 alu
    {
    native.move_flags();
    if (!timing.good()) return false;
    }
    // timing-node b0n17 bus_end
    {
      const auto bus_address = static_cast<std::uint32_t>(native.pc + 6U);
      const auto bus_mask = static_cast<std::uint16_t>(0xffffU);
      const auto bus_result = timing.read("b0n17", "program", 16U,
          bus_address, bus_mask, [&]() { return native.program(timing, bus_address); });
      if (!timing.good()) return false;
      native.prefetched = bus_result;
      if (!timing.good()) return false;
    }
    // timing-node b0n18 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n19 i
    // 389 maww2
    // timing-node b0n20 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n21 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n22 i
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n23 bus
    // Reference emitter has no executable statement for this node.
    // timing-node b0n24 lines
    // Reference emitter has no executable statement for this node.
    // timing-node b0n25 =8xl
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n26 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n27 bus_end
    {
      const auto bus_address = static_cast<std::uint32_t>(native.port);
      const auto bus_mask = static_cast<std::uint16_t>(0xffU);
      const auto bus_value = static_cast<std::uint16_t>(native.write_value());
      timing.write("b0n27", "data", 8U, bus_address,
          bus_mask, bus_value, [&]() { native.port_write(bus_address, bus_mask, bus_value); });
      if (!timing.good()) return false;
    }
    // timing-node b0n28 i
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n29 set_trace
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n30 i
    // 363 b
    // timing-node b0n31 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n32 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n33 bus
    // Reference emitter has no executable statement for this node.
    // timing-node b0n34 lines
    // Reference emitter has no executable statement for this node.
    // timing-node b0n35 drop_critical
    // Reference emitter has no executable statement for this node.
    // timing-node b0n36 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n37 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n38 i
    // 34c mmrw3
    // timing-node b0n39 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n40 i
    {
    native.sample_interrupt();
    if (!timing.good()) return false;
    }
    // timing-node b0n41 lines
    // Reference emitter has no executable statement for this node.
    // timing-node b0n42 bus_end
    {
      const auto bus_address = static_cast<std::uint32_t>(native.pc + 8U);
      const auto bus_mask = static_cast<std::uint16_t>(0xffffU);
      const auto bus_result = timing.read("b0n42", "program", 16U,
          bus_address, bus_mask, [&]() { return native.program(timing, bus_address); });
      if (!timing.good()) return false;
      native.prefetched = bus_result;
      if (!timing.good()) return false;
    }
    // timing-node b0n43 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n44 =
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n45 i
    {
    if (!timing.good()) return false;
    }
    // timing-node b0n46 next_trace
    {
    native.registers.program_counter = native.next_pc();
    return timing.complete(native.next_pc(), native.state, "next_trace");
    }
    return timing.unsupported("reference graph fell through");
}

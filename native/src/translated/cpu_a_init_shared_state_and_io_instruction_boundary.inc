bool begin_cpu_a_init_shared_state_and_io_instruction(
    CpuBIrqTiming &timing, std::uint32_t next)
{
    switch (next) {
    case 0x8011aU:
    case 0x80122U:
    case 0x8012aU:
    case 0x80130U:
    case 0x80136U:
    case 0x80140U:
    case 0x80142U:
    case 0x80146U:
    case 0x8014cU:
    case 0x80150U:
    case 0x80152U:
    case 0x80156U:
    case 0x8015cU:
    case 0x80162U:
    case 0x80166U:
    case 0x8016aU:
    case 0x8016cU:
    case 0x8016eU:
    case 0x80172U:
    case 0x80178U:
    case 0x8017cU:
    case 0x8017eU:
    case 0x80182U:
    case 0x80188U:
    case 0x8018cU:
    case 0x8018eU:
    case 0x80194U:
    case 0x8019aU:
    case 0x8019eU:
    case 0x801a0U:
    case 0x801a6U:
    case 0x801aaU:
    case 0x801acU:
    case 0x801b2U:
    case 0x801b8U:
    case 0x801baU:
    case 0x801c0U:
        timing.begin(next);
        return true;
    default:
        return false;
    }
}

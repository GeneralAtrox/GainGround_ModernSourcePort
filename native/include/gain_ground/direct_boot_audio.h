#pragma once
#include "gain_ground/contract_types.h"

namespace gain_ground {
class System24Devices;
// Fast-loader I/O setup, including a neutral DAC latch before output enable.
void prepare_direct_boot_devices(System24Devices &devices);
// Retain the BIOS sound initialization skipped by direct asset loading.
// Runs the original 0x4f0/0x4f4 calls and real native children; caller owns
// scheduling. Use a scratch context, then the original cleared game-entry CPU.
FunctionResult run_direct_boot_audio(FunctionContext &context) noexcept;
}

#include "gain_ground/system24_devices.h"
#include "gain_ground/direct_boot_audio.h"
#include <iostream>
#include <stdexcept>

using namespace gain_ground;
void require(bool value, const char *message) { if (!value) throw std::runtime_error(message); }
int main() { try {
    System24Devices device;
    device.write(0x800100, 0x20, 255);
    device.write(0x800102, 0xc0, 255);
    require((*device.read(0x800102,255) & 128) == 0, "YM accepted a write while held in reset");
    prepare_direct_boot_devices(device);
    require(device.read(0x80001c,255) == 4 && device.read(0x80001e,255) == 0x88,
            "Direct loader omitted original BIOS I/O state");
    device.write(0x800100, 0x20, 255);
    device.write(0x800102, 0xc0, 255);
    require((*device.read(0x800102,255) & 128) != 0, "Released YM did not accept its write");

    System24Devices reset_edge;
    prepare_direct_boot_devices(reset_edge);
    auto ym = [&](unsigned reg, unsigned value) {
        reset_edge.write(0x800100, reg, 255);
        reset_edge.write(0x800102, value, 255);
    };
    ym(0x10, 0xff); ym(0x11, 3); ym(0x14, 5);
    reset_edge.advance(1000000);
    require((*reset_edge.read(0x800102,255) & 1) != 0, "YM timer did not establish reset prestate");
    reset_edge.write(0x80001c, 0, 255);
    require((*reset_edge.read(0x800102,255) & 1) != 0, "YM reset incorrectly occurred on assertion");
    reset_edge.write(0x80001c, 4, 255);
    require((*reset_edge.read(0x800102,255) & 3) == 0, "YM release failed to reset timer status");

    // screen_device::vpos starts at visible.bottom+1 (384). The driver's
    // scanline timer first asserts sprite IRQ at line 0, 40 scanlines later.
    device.write(0xa00004, 24, 65535);
    device.advance(0); require(device.irq_level(0)==0,"Unexpected reset-time video IRQ");
    device.advance(1639999); require(device.irq_level(0)==0,"Sprite IRQ asserted early");
    device.advance(1640000); require(device.irq_level(0)==5,"First sprite IRQ phase differs from MAME");
    device.advance(1681000); require(device.irq_level(0)==0,"Sprite IRQ lasted more than one scanline");
    device.advance(17383999); require(device.irq_level(0)==0,"VBLANK IRQ asserted early");
    device.advance(17384000); require(device.irq_level(0)==4,"First VBLANK IRQ phase differs from MAME");
    device.advance(17425000); require(device.irq_level(0)==0,"VBLANK IRQ lasted more than one scanline");
    device.advance(19024000); require(device.irq_level(0)==5,"Second sprite IRQ phase differs from MAME");
    std::cout << "PASS: reset gating, BIOS I/O state and first video interrupt boundaries\n";
    return 0;
} catch(const std::exception &e) { std::cerr << e.what() << '\n'; return 1; } }

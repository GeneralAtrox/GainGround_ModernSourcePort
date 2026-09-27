#include "gain_ground/runtime_host.h"
#include "gain_ground/system24_video.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace gain_ground;
namespace {
void check(bool ok, const char *message) { if (!ok) throw std::runtime_error(message); }
void word(RuntimeHost &host, unsigned region, unsigned offset, unsigned value) {
    const std::array<std::uint8_t,2> bytes{static_cast<std::uint8_t>(value >> 8), static_cast<std::uint8_t>(value)};
    check(host.load_region(static_cast<std::uint16_t>(region), offset, bytes), "load failed");
}
std::uint32_t pixel(const System24Video &video, unsigned x, unsigned y) {
    return video.pixels()[(495U - x) * 384U + y];
}
void scene(RuntimeHost &host, unsigned layer) {
    // ROM-free scene: an opaque red foreground tile overlaps a blue sprite.
    for (unsigned id : {2U,5U,6U,7U,8U,9U,10U,11U}) {
        std::vector<std::uint8_t> zero(host.region_bytes(static_cast<std::uint16_t>(id)).size());
        check(host.load_region(static_cast<std::uint16_t>(id), 0, zero), "clear failed");
    }
    word(host, 5, layer * 8192, 0x8001);
    for (unsigned i = 0; i < 32; i += 2) word(host, 8, 32 + i, 0x1111);
    for (unsigned i = 0; i < 4; ++i) {
        word(host, 6, 0x2008 + i * 2, i == layer ? 0 : 0x8000);
    }
    if (layer == 1) for (unsigned i = 0; i < 384 * 8; i += 2) word(host, 7, i, 0xffff);
    word(host, 9, 2, 0x100f); // red
    word(host, 9, 0x2004, 0x4f00); // blue
    word(host, 11, 0, 1); word(host, 11, 2, 0x3f);
    word(host, 11, 4, 0x1000); word(host, 11, 6, 0x100);
    word(host, 11, 10, 8); word(host, 11, 16, 0xffff);
    word(host, 11, 0x1002, 0x0200); // sprite pen 2 -> palette 0x1002
    for (unsigned i = 0; i < 32; i += 2) word(host, 11, 0x20000 + i, 0x2222);
    for (unsigned i = 8; i < 12; ++i) word(host, 10, i * 2, 2);
    word(host, 10, 2, 5); word(host, 10, 6, 3);
}
}
int main() { try {
    RuntimeHost host;
    System24Video video;
    for (const auto layer : {0U, 1U}) {
        scene(host, layer);
        video.render(host);
        check(pixel(video, 2, 2) == 0xff0000, "disabled effect must retain opaque foreground");
        word(host, 10, 2, 2); word(host, 10, 6, 2);
        video.render(host);
        check(pixel(video, 2, 2) == 0x0000ff, "disabled effect must retain sprite priority");
        word(host, 2, 0xd0e, layer == 0 ? 0x8000 : 0x8100);
        video.render(host);
        check(pixel(video, 2, 2) == 0x0000ff, "foreground effect must keep the character fully opaque");
        const auto low = video.pixels();
        if (layer == 0) word(host, 10, 2, 5);
        word(host, 10, 6, 3);
        const std::vector<std::uint8_t> before(host.region_bytes(10).begin(), host.region_bytes(10).end());
        video.render(host);
        check(video.pixels() == low, "opposite IRQ phase must produce identical pixels");
        check(std::equal(before.begin(), before.end(), host.region_bytes(10).begin()), "render must not alter mixer RAM");
        word(host, 11, 10, 24); // Move sprite immediately, with no historical image to blend.
        video.render(host);
        check(pixel(video, 2, 2) == 0xff0000, "moving sprite must leave no trail");
        check(pixel(video, 18, 2) == 0x0000ff, "unoccluded sprite must stay fully opaque");
    }
    scene(host, 0);
    word(host, 2, 0xd0e, 0x8100);
    video.render(host);
    check(pixel(video, 2, 2) == 0xff0000, "mode 0x81 must leave the first foreground priority alone");
    word(host, 2, 0xd0e, 0x8000);
    word(host, 5, 2 * 8192, 0x8001);
    word(host, 6, 0x200c, 0);
    word(host, 10, 10, 7);
    video.render(host);
    check(pixel(video, 2, 2) == 0xff0000, "nonalternating foreground must still occlude the character");
    word(host, 10, 26, 1);
    video.render(host);
    for (const auto rgb : video.pixels()) check(rgb == 0, "blanking must remain black");
    std::cout << "Foreground visibility: both modes, phase invariance, opaque characters, other occlusion, motion, RAM and blanking passed\n";
    return 0;
} catch (const std::exception &error) { std::cerr << error.what() << '\n'; return 1; } }

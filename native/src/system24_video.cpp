// System 24 rasterization derived from segaic24.cpp/segas24.cpp.
// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
#include "gain_ground/system24_video.h"
#include "gain_ground/runtime_host.h"
#include <algorithm>
#include <array>
#include <numeric>
#include <span>

namespace gain_ground {
namespace {
template<class T> struct Bitmap {
    std::vector<T> data = std::vector<T>(496 * 384);
    T &pix(int y, int x) { return data[std::size_t(y) * 496 + x]; }
};
using bitmap_ind16 = Bitmap<std::uint16_t>;
using bitmap_ind8 = Bitmap<std::uint8_t>;
struct rectangle { int min_x = 0, max_x = 495, min_y = 0, max_y = 383; };
#include "gground_sprite_rasterizer.inc"

std::uint16_t word(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    return static_cast<std::uint16_t>((bytes[offset] << 8U) | bytes[offset + 1U]);
}
std::vector<std::uint16_t> words(std::span<const std::uint8_t> bytes)
{
    std::vector<std::uint16_t> result(bytes.size() / 2U);
    for (std::size_t i = 0; i < result.size(); ++i) result[i] = word(bytes, i * 2U);
    return result;
}
std::uint32_t color(std::uint16_t value, bool alternate)
{
    int r = ((value & 15) << 4) | ((value & 0x1000) ? 8 : 0);
    int g = (value & 0xf0) | ((value & 0x2000) ? 8 : 0);
    int b = ((value >> 4) & 0xf0) | ((value & 0x4000) ? 8 : 0);
    r |= r >> 5; g |= g >> 5; b |= b >> 5;
    if (alternate) {
        const auto shade = [value](int c) { return int((value & 0x8000) ? 255 - 0.6 * (255 - c) : 0.6 * c); };
        r = shade(r); g = shade(g); b = shade(b);
    }
    return std::uint32_t((r << 16) | (g << 8) | b);
}
} // namespace

System24Video::System24Video() : pixels_(width * height) {}

void System24Video::render(const RuntimeHost &host)
{
    auto tile = words(host.region_bytes(5));
    const auto scroll = words(host.region_bytes(6));
    const auto masks = words(host.region_bytes(7));
    tile.insert(tile.end(), scroll.begin(), scroll.end());
    tile.insert(tile.end(), masks.begin(), masks.end());
    const auto chars = host.region_bytes(8);
    const auto palette = host.region_bytes(9);
    const auto mixer = words(host.region_bytes(10));
    if (mixer[13] & 1U) { std::fill(pixels_.begin(), pixels_.end(), 0U); return; }
    auto sprites = words(host.region_bytes(11));
    const auto compose = [&](const auto &priorities) {
        bitmap_ind16 bitmap;
        bitmap_ind8 priority;
        std::array<int, 12> order{};
        std::iota(order.begin(), order.end(), 0);
        std::sort(order.begin(), order.end(), [&](int a, int b) {
            constexpr int tie[] = {0,1,2,3,4,5,6,7,-4,-3,-2,-1};
            return (priorities[a] & 7U) == (priorities[b] & 7U) ? tie[b] < tie[a] : (priorities[a] & 7U) < (priorities[b] & 7U);
        });
        const auto draw_tile = [&](int submitted, int level, bool opaque) {
            const int layer = submitted >> 1;
            const int h = tile[0x5000 + layer], v = tile[0x5004 + layer];
            const int mode = (tile[0x5004 + (layer & 2)] >> 13) & 3;
            if ((v & 0x8000) || (mode && (layer & 1))) return;
            for (int y = 0; y < 384; ++y) {
                const int hs = (h & 0x8000) ? tile[0x4000 + 0x200 * layer + y] : h;
                const int sy = (y + v) & 511;
                for (int x = 0; x < 496; ++x) {
                    int actual = layer;
                    if (mode == 1) actual ^= (((-v) & 512) ? 0 : 1) ^ (y >= ((-v) & 511));
                    else if (mode) actual ^= ((hs & 512) ? 0 : 1) ^ (x >= (hs & 511));
                    else {
                        auto mask = tile[(submitted & 4 ? 0x6800 : 0x6000) + y * 4 + x / 128];
                        if (layer & 1) mask = static_cast<std::uint16_t>(~mask);
                        if (mask & (0x8000U >> ((x & 127) >> 3))) continue;
                    }
                    const int sx = (x - hs) & 511;
                    const auto entry = tile[actual * 0x1000 + (sy >> 3) * 64 + (sx >> 3)];
                    const auto packed = chars[(entry & 0xfffU) * 32U + (sy & 7) * 4U + ((sx & 7) >> 1)];
                    const auto pen = (packed >> ((sx & 1) ? 0U : 4U)) & 15U;
                    if (mode && ((entry >> 15) != (submitted & 1))) continue;
                    if (!opaque && (!pen || ((entry >> 15) != (submitted & 1)))) continue;
                    bitmap.pix(y,x) = static_cast<std::uint16_t>(((entry >> 7) & 255U) * 16U + pen);
                    priority.pix(y,x) |= static_cast<std::uint8_t>(1U << level);
                }
            }
        };
        for (int i = 11; i >= 0; --i) if (order[i] < 8 && !(order[i] & 1)) draw_tile(order[i], 0, true);
        std::array<int,4> sprite_priority{};
        int level = 0;
        for (const int i : order) {
            if (i < 8) draw_tile(i, level, false);
            else sprite_priority[i - 8] = level++;
        }
        draw_sprites(sprites, bitmap, {}, priority, sprite_priority.data());
        return bitmap;
    };
    const auto bitmap = compose(mixer);
    const auto ram = host.region_bytes(2);
    const auto effect = ram[0xd0eU];
    // IRQ5 alternates foreground priorities to simulate translucency. Compose
    // both orders from this SAME scene and blend them instead: no frame history,
    // motion trails, game-state writes or stage-specific archer exceptions.
    std::vector<std::uint16_t> other;
    if (effect & 0x80U) {
        auto alternate = mixer;
        alternate[3] ^= 1U;
        if (!(effect & 1U)) alternate[1] ^= 7U;
        other = compose(alternate).data;
    }
    std::array<std::uint32_t,16384> colors{};
    for (std::size_t i = 0; i < colors.size(); ++i) colors[i] = color(word(palette, (i & 8191) * 2U), (i & 8192) != 0);
    for (int y = 0; y < 384; ++y) for (int x = 0; x < 496; ++x) {
        const auto index = std::size_t(y) * 496 + x;
        auto rgb = colors[bitmap.data[index] & 16383U];
        if (!other.empty()) {
            const auto second = colors[other[index] & 16383U];
            // Per-channel 50% opacity, retaining unchanged pixels exactly.
            rgb = (rgb & second) + (((rgb ^ second) & 0xfefefeU) >> 1U);
        }
        pixels_[std::size_t(495 - x) * 384 + y] = rgb;
    }
}
} // namespace gain_ground

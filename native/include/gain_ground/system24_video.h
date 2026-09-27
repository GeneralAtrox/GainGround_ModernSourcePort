#pragma once
#include <cstdint>
#include <vector>

namespace gain_ground {
class RuntimeHost;
// Software presentation from live System 24 memory with steady translucency.
class System24Video {
public:
    static constexpr int width = 384, height = 496;
    System24Video();
    void render(const RuntimeHost &host);
    const std::vector<std::uint32_t> &pixels() const noexcept { return pixels_; }
private:
    std::vector<std::uint32_t> pixels_;
};
} // namespace gain_ground

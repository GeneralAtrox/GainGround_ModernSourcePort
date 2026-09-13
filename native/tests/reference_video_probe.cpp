// Verification only: render captured RAM with the production renderer.
// No CPU execution or expected pixels enter the renderer.
#include "gain_ground/runtime_host.h"
#include "gain_ground/system24_video.h"
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

int main(int argc, char **argv)
{
    try {
        if (argc != 3) throw std::runtime_error("usage: reference_video_probe <ram> <rgb>");
        std::ifstream input(std::filesystem::path(argv[1]), std::ios::binary);
        std::array<char,8> magic{};
        input.read(magic.data(), magic.size());
        if (magic != std::array<char,8>{'G','G','V','R','A','M','1','\0'})
            throw std::runtime_error("invalid captured RAM probe header");
        gain_ground::RuntimeHost host;
        // Fixed native video-region order; exact sizes come from the bus ABI.
        for (unsigned id = 5; id <= 11; ++id) {
            std::vector<std::uint8_t> bytes(host.region_bytes(id).size());
            input.read(reinterpret_cast<char *>(bytes.data()), bytes.size());
            if (!input || !host.load_region(id, 0, bytes))
                throw std::runtime_error("truncated captured video RAM");
        }
        if (input.peek() != std::char_traits<char>::eof())
            throw std::runtime_error("unexpected captured RAM suffix");
        gain_ground::System24Video video;
        video.render(host);
        std::ofstream output(std::filesystem::path(argv[2]), std::ios::binary);
        for (const auto pixel : video.pixels()) {
            const std::array<char,3> rgb{char(pixel >> 16), char(pixel >> 8), char(pixel)};
            output.write(rgb.data(), rgb.size());
        }
        if (!output) throw std::runtime_error("cannot write native RGB output");
        return 0;
    } catch (const std::exception &error) {
        std::cerr << error.what() << '\n';
        return 2;
    }
}

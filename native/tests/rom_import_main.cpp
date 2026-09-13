#include "gain_ground/rom_import.h"
#include <iostream>
int main(int argc, char **argv) {
    if (argc != 3) { std::cerr << "Usage: gain_ground_rom_import <ROM directory or ZIP> <cache directory>\n"; return 2; }
    std::string error;
    if (!gain_ground::import_rom_set(argv[1],argv[2],&gain_ground::matches_hash,error)) {
        std::cerr << error << '\n'; return 1;
    }
    if (!gain_ground::valid_rom_cache(argv[2],&gain_ground::matches_hash)) return 2;
    std::cout << "BIOS and all 45 extracted payloads validated\n";
}

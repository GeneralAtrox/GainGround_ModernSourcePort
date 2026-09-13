// Replay one stopped actor invocation from a saved runtime failure packet.
#include "gain_ground/runtime_host.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>
#include <vector>

int main(int argc, char **argv) {
    if (argc != 2 && argc != 3) return 2;
    const bool retry_removed_actor = argc == 3 && std::string_view(argv[2]) == "--retry-removed-actor";
    if (argc == 3 && !retry_removed_actor) return 2;
    const std::filesystem::path directory(argv[1]);
    gain_ground::RuntimeHost host;
    for (const auto region : {2U, 3U}) {
        std::ifstream input(directory / (region == 2U ? "cpu-b-ram.bin" : "cpu-a-shared-ram.bin"), std::ios::binary);
        const std::vector<std::uint8_t> bytes{std::istreambuf_iterator<char>(input), {}};
        if (bytes.size() != 0x40000U || !host.load_region(region, 0U, bytes)) return 2;
    }
    gain_ground::FunctionContext context{};
    std::ifstream status(directory / "status.txt");
    std::string line;
    bool cpu_b = false;
    unsigned registers = 0;
    const std::regex cpu_pattern("cpu=1 state=([0-9]+) pc=([0-9]+) sr=([0-9]+)");
    const std::regex reg_pattern("D([0-7])=([0-9]+) A[0-7]=([0-9]+)");
    while (std::getline(status, line)) {
        std::smatch match;
        if (std::regex_match(line, match, cpu_pattern)) {
            cpu_b = true; context.cpu = 1U;
            context.state = static_cast<std::uint8_t>(std::stoul(match[1]));
            context.registers.program_counter = std::stoul(match[2]);
            context.registers.status = static_cast<std::uint16_t>(std::stoul(match[3]));
        } else if (cpu_b && std::regex_match(line, match, reg_pattern)) {
            const auto index = std::stoul(match[1]);
            context.registers.data[index] = std::stoul(match[2]);
            context.registers.address[index] = std::stoul(match[3]);
            ++registers;
        }
    }
    if (!cpu_b || registers != 8U) return 2;
    const auto ram = host.region_bytes(2U);
    if (retry_removed_actor) {
        // Saved post-rejection state: F387 only cleared the actor's active bit
        // and unwound two stack slots. Restore F381's invocation entry; the
        // out-of-bounds position and scheduler return word remain unchanged.
        const auto actor = context.registers.address[5];
        if (actor + 0x40U >= ram.size() || context.registers.program_counter != 0x8594U ||
            context.registers.address[7] != 0x7ffeU || (ram[actor] & 0x80U)) return 2;
        context.registers.program_counter = (std::uint32_t(ram[actor+2]) << 24U) |
            (std::uint32_t(ram[actor+3]) << 16U) | (std::uint32_t(ram[actor+4]) << 8U) | ram[actor+5];
        if (context.registers.program_counter != 0x1fea2U) return 2;
        context.registers.address[7] -= 4U;
        const std::uint8_t active = ram[actor] | 0x80U;
        if (!host.load_region(2U, actor, std::span(&active, 1U))) return 2;
    }
    const auto sp = context.registers.address[7];
    if (sp > ram.size() - 4U) return 2;
    const auto expected_pc = (std::uint32_t(ram[sp]) << 24U) |
        (std::uint32_t(ram[sp+1]) << 16U) | (std::uint32_t(ram[sp+2]) << 8U) | ram[sp+3];
    host.select_cpu(1U);
    const auto result = host.run(context);
    const bool passed = !host.faulted() && result.status == gain_ground::TranslationStatus::complete &&
        result.control == 1U && result.exit_program_counter == expected_pc &&
        context.registers.program_counter == expected_pc && context.registers.address[7] == sp + 4U &&
        (!retry_removed_actor || (host.region_bytes(2U)[context.registers.address[5]] & 0x80U) == 0U);
    std::cout << "passed=" << passed << " pc=" << std::hex << context.registers.program_counter
        << " expected=" << expected_pc << " sp=" << context.registers.address[7]
        << " control=" << unsigned(result.control) << " fault=" << host.fault().message << '\n';
    return passed ? 0 : 1;
}

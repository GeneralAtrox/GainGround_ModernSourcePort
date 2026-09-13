#include "gain_ground/fixture_bundle.h"
#include "gain_ground/fixture_compare.h"

#include "gground_functions.h"

#include <algorithm>
#include <array>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace {

std::size_t parse_index(std::string_view value)
{
    std::size_t result{};
    const auto parsed = std::from_chars(value.data(), value.data() + value.size(), result);
    if (parsed.ec != std::errc{} || parsed.ptr != value.data() + value.size())
        throw std::runtime_error("invalid numeric argument: " + std::string(value));
    return result;
}

} // namespace

int main(int argc, char **argv)
{
    try {
        if (argc < 2 || argc > 5)
            throw std::runtime_error(
                "usage: gain_ground_fixture_compare <bundle.ggfz> "
                "[--record N | --inspect-record N | --function ID [--all] | "
                "--all-implemented | --all-records]");
        const gain_ground::FixtureBundle bundle{std::filesystem::path(argv[1])};

        if (argc == 3 && std::string_view(argv[2]) == "--all-records") {
            gain_ground::FixtureBatchReport batch;
            const auto count = static_cast<std::size_t>(bundle.header().fixture_count);
            std::vector<bool> selected(gain_ground::generated::kFunctions.size());
            std::vector<bool> diverged(gain_ground::generated::kFunctions.size());
            std::vector<bool> unimplemented(gain_ground::generated::kFunctions.size());
            for (std::size_t record_index = 0; record_index < count; ++record_index) {
                if (!bundle.records()[record_index].native_replay_eligible)
                    continue;
                const auto fixture = bundle.load(record_index);
                if (fixture.function_id >= selected.size())
                    throw std::runtime_error("fixture function ID is out of range");
                selected[fixture.function_id] = true;
                ++batch.selected_fixtures;
                const auto report = gain_ground::compare_fixture(fixture, record_index);
                if (report.status == gain_ground::ComparisonStatus::unimplemented) {
                    ++batch.unimplemented_fixtures;
                    unimplemented[fixture.function_id] = true;
                } else {
                    ++batch.executed_fixtures;
                    if (report.status == gain_ground::ComparisonStatus::passed)
                        ++batch.passed_fixtures;
                    else {
                        ++batch.diverged_fixtures;
                        diverged[fixture.function_id] = true;
                    }
                }
                if (report.status != gain_ground::ComparisonStatus::passed
                    && !batch.has_first_failure) {
                    batch.has_first_failure = true;
                    batch.first_failure = report;
                }
            }
            for (std::size_t id = 0; id < selected.size(); ++id) {
                if (!selected[id]) continue;
                ++batch.selected_functions;
                if (!gain_ground::generated::kFunctions[id].implemented
                    || unimplemented[id]) {
                    ++batch.unimplemented_functions;
                } else {
                    ++batch.implemented_functions;
                    if (diverged[id])
                        ++batch.diverged_functions;
                    else
                        ++batch.passed_functions;
                }
            }
            batch.status = batch.diverged_fixtures != 0U
                ? gain_ground::ComparisonStatus::diverged
                : (batch.unimplemented_fixtures != 0U
                    ? gain_ground::ComparisonStatus::unimplemented
                    : gain_ground::ComparisonStatus::passed);
            std::cout << gain_ground::batch_report_json(batch);
            return batch.status == gain_ground::ComparisonStatus::passed
                ? 0 : (batch.status == gain_ground::ComparisonStatus::unimplemented ? 2 : 1);
        }

        if (argc == 3 && std::string_view(argv[2]) == "--all-implemented") {
            std::vector<std::uint32_t> function_ids;
            for (const auto &function : gain_ground::generated::kFunctions)
                if (function.implemented)
                    function_ids.push_back(function.id);
            auto report = gain_ground::compare_function_fixtures(bundle, function_ids);
            gain_ground::enforce_exact_compatibility_histogram(report);
            std::cout << gain_ground::batch_report_json(report);
            if (report.status == gain_ground::ComparisonStatus::passed)
                return 0;
            if (report.status == gain_ground::ComparisonStatus::unimplemented)
                return 2;
            return 1;
        }

        if (argc == 5) {
            if (std::string_view(argv[2]) != "--function" || std::string_view(argv[4]) != "--all")
                throw std::runtime_error("five-argument mode must be --function ID --all");
            const auto value = parse_index(argv[3]);
            if (value >= gain_ground::generated::kFunctions.size())
                throw std::runtime_error("function ID is out of range");
            const std::array<std::uint32_t, 1> function_ids{static_cast<std::uint32_t>(value)};
            const auto report = gain_ground::compare_function_fixtures(bundle, function_ids);
            std::cout << gain_ground::batch_report_json(report);
            if (report.status == gain_ground::ComparisonStatus::passed)
                return 0;
            if (report.status == gain_ground::ComparisonStatus::unimplemented)
                return 2;
            return 1;
        }

        if (argc == 3)
            throw std::runtime_error("unknown option: " + std::string(argv[2]));

        std::size_t record_index{};
        if (argc == 4) {
            const std::string_view option(argv[2]);
            const auto value = parse_index(argv[3]);
            if (option == "--inspect-record") {
                const auto fixture = bundle.load(value);
                std::cout << "record=" << value << " function=" << fixture.function_id
                          << " control=" << static_cast<unsigned>(fixture.control)
                          << " entry_kind=" << static_cast<unsigned>(fixture.entry_kind)
                          << " entry_pc=0x" << std::hex << fixture.entry_pc
                          << " exit_pc=0x" << fixture.exit_pc
                          << " callsite=0x" << fixture.callsite
                          << " expected_return=0x" << fixture.expected_return
                          << " entry_sr=0x" << fixture.entry_registers.status
                          << " exit_sr=0x" << fixture.exit_registers.status
                          << std::dec
                          << " instructions=" << fixture.instruction_count
                          << " edges=" << fixture.edges.size()
                          << " memory=" << fixture.memory_prestates.size()
                          << " deltas=" << fixture.memory_deltas.size()
                          << " calls=" << fixture.calls.size()
                          << " hardware=" << fixture.hardware_effects.size()
                          << '\n';
                for (std::size_t index = 0; index != fixture.entry_registers.data.size(); ++index)
                    std::cout << "register D" << index
                              << " entry=0x" << std::hex << fixture.entry_registers.data[index]
                              << " exit=0x" << fixture.exit_registers.data[index] << std::dec << '\n';
                for (std::size_t index = 0; index != fixture.entry_registers.address.size(); ++index)
                    std::cout << "register A" << index
                              << " entry=0x" << std::hex << fixture.entry_registers.address[index]
                              << " exit=0x" << fixture.exit_registers.address[index] << std::dec << '\n';
                for (const auto &item : fixture.edges)
                    std::cout << "edge from_state=" << static_cast<unsigned>(item.from_state)
                              << " to_state=" << static_cast<unsigned>(item.to_state)
                              << " kind=" << static_cast<unsigned>(item.kind)
                              << " opcode=0x" << std::hex << item.opcode
                              << " source=0x" << item.source_pc
                              << " target=0x" << item.target_pc << std::dec << '\n';
                for (const auto &item : fixture.memory_prestates)
                    std::cout << "memory sequence=" << item.first_sequence
                              << " region=" << item.region
                              << " offset=0x" << std::hex << item.offset
                              << " initial=0x" << item.initial
                              << " read_mask=0x" << item.read_mask
                              << std::dec << " accesses=" << item.access_count << '\n';
                for (const auto &item : fixture.memory_deltas)
                    std::cout << "delta first_sequence=" << item.first_sequence
                              << " last_sequence=" << item.last_sequence
                              << " region=" << item.region
                              << " offset=0x" << std::hex << item.offset
                              << " initial=0x" << item.initial
                              << " final=0x" << item.final_value
                              << " write_mask=0x" << item.write_mask
                              << std::dec << " writes=" << item.write_count << '\n';
                for (const auto &item : fixture.calls)
                    std::cout << "call sequence=" << item.sequence
                              << " function=" << item.function_id
                              << " cpu=" << static_cast<unsigned>(item.target_cpu)
                              << " state=" << static_cast<unsigned>(item.target_state)
                              << " kind=" << static_cast<unsigned>(item.kind)
                              << " callsite=0x" << std::hex << item.callsite
                              << " target=0x" << item.target << std::dec << '\n';
                for (const auto &item : fixture.hardware_effects)
                    std::cout << "hardware sequence=" << item.sequence
                              << " kind=" << static_cast<unsigned>(item.kind)
                              << " address=0x" << std::hex << item.address
                              << " data=0x" << item.data
                              << " mask=0x" << item.memory_mask << std::dec << '\n';
                return 0;
            }
            if (option == "--record") {
                record_index = value;
            } else if (option == "--function") {
                if (value >= gain_ground::generated::kFunctions.size())
                    throw std::runtime_error("function ID is out of range");
                const auto &function = gain_ground::generated::kFunctions[value];
                const auto first = function.first_fixture;
                const auto last = first + function.fixture_count;
                const auto eligible = std::find_if(
                    bundle.records().begin() + static_cast<std::ptrdiff_t>(first),
                    bundle.records().begin() + static_cast<std::ptrdiff_t>(last),
                    [](const auto &record) { return record.native_replay_eligible; });
                if (eligible == bundle.records().begin() + static_cast<std::ptrdiff_t>(last))
                    throw std::runtime_error("function has no native-replay-eligible fixture");
                record_index = static_cast<std::size_t>(eligible - bundle.records().begin());
            } else {
                throw std::runtime_error("unknown option: " + std::string(option));
            }
        }
        const auto report = gain_ground::compare_fixture(bundle.load(record_index), record_index);
        std::cout << gain_ground::comparison_report_json(report);
        if (report.status == gain_ground::ComparisonStatus::passed)
            return 0;
        if (report.status == gain_ground::ComparisonStatus::unimplemented)
            return 2;
        return 1;
    } catch (const std::exception &error) {
        std::cerr << "ERROR: " << error.what() << '\n';
        return 1;
    }
}

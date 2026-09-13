#include "gain_ground/fixture_bundle.h"
#include "gain_ground/fixture_compare.h"

#include "gground_corpus_fixture_catalog.h"
#include "gground_assets.h"
#include "gground_checkpoints.h"
#include "gground_fixture_contract.h"
#include "gground_functions.h"
#include "gground_memory_map.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void require(bool condition, const std::string &message)
{
    if (!condition)
        throw std::runtime_error(message);
}

const gain_ground::FixtureRegionContract *find_region(std::uint16_t id)
{
    const auto &regions = gain_ground::generated::kFixtureRegions;
    const auto found = std::find_if(regions.begin(), regions.end(), [id](const auto &item) { return item.id == id; });
    return found == regions.end() ? nullptr : &*found;
}

void validate_fixture(const gain_ground::FixtureRecord &fixture, std::size_t record_index)
{
    const auto &function = gain_ground::generated::kFunctions.at(fixture.function_id);
    require(fixture.cpu == function.cpu, "fixture CPU differs at record " + std::to_string(record_index));
    require(fixture.state == function.state, "fixture state differs at record " + std::to_string(record_index));
    require(fixture.entry_pc == function.address, "fixture entry PC differs at record " + std::to_string(record_index));
    require(fixture.control >= 1 && (fixture.control <= 5 || fixture.control == 8),
            "fixture is not a complete test input");
    require(fixture.execution_pc >= function.body_min && fixture.execution_pc <= function.body_max,
        "fixture execution PC is outside function body at record " + std::to_string(record_index));
    require(fixture.entry_registers.program_counter == fixture.execution_pc,
        "entry-register PC differs at record " + std::to_string(record_index));

    for (const auto &item : fixture.memory_prestates) {
        const auto *region = find_region(item.region);
        require(region != nullptr, "fixture prestate references an unknown region");
        require(item.offset + 1U < region->bytes, "fixture prestate exceeds its region");
    }
    for (const auto &item : fixture.memory_deltas) {
        const auto *region = find_region(item.region);
        require(region != nullptr, "fixture delta references an unknown region");
        require(!region->immutable, "fixture writes an immutable region");
        require(item.offset + 1U < region->bytes, "fixture delta exceeds its region");
    }
}

} // namespace

int main(int argc, char **argv)
{
    try {
        require(argc == 2 || (argc == 3 && std::string(argv[2]) == "--all"),
            "usage: gain_ground_fixture_smoke <bundle.ggfz> [--all]");
        const bool all = argc == 3;
        const gain_ground::FixtureBundle bundle{std::filesystem::path(argv[1])};
        const auto &header = bundle.header();
        const auto &records = bundle.records();

        require(header.function_count == gain_ground::generated::kFixtureFunctionCount,
            "bundle function count differs from generated fixture catalog");
        require(header.fixture_count == gain_ground::generated::kFixtureRecordCount,
            "bundle fixture count differs from generated fixture catalog");
        require(header.function_count == gain_ground::generated::kFunctions.size(),
            "bundle function count differs from generated translation table");
        require(gain_ground::hexadecimal(header.identities[0].data(), header.identities[0].size()) ==
                gain_ground::generated::kFixtureCorpusIdentitySha256,
            "bundle corpus identity differs from generated fixture catalog");
        require(gain_ground::hexadecimal(header.base_mame_commit.data(), header.base_mame_commit.size()) ==
                gain_ground::generated::kMemoryMapMameCommit,
            "bundle base MAME commit differs from generated memory map");
        const auto fixture_contract_identity = gain_ground::hexadecimal(
            header.identities[3].data(), header.identities[3].size());
        // Control result 8 is a backward-compatible completion classification;
        // existing aggregate records contain only the unchanged controls 1-5.
        require(fixture_contract_identity == gain_ground::generated::kFixtureContractSha256 ||
                fixture_contract_identity ==
                    "2d221c324e3ff7665c396cb1af85eeafa2b0b167b446c7cf04ce28f7e5793529",
            "bundle fixture-contract identity differs from generated fixture contract");

        std::array<std::byte, 6> endian_probe{};
        gain_ground::generated::write_be_u16(endian_probe, 0, 0x1234U);
        gain_ground::generated::write_be_u32(endian_probe, 2, 0x89abcdefU);
        require(gain_ground::generated::read_be_u16(endian_probe, 0) == 0x1234U &&
                gain_ground::generated::read_be_u32(endian_probe, 2) == 0x89abcdefU,
            "generated big-endian accessors differ");
        require(gain_ground::generated::kLoadRecords.size() == 45 &&
                gain_ground::generated::kLoadTableRanges.size() == 8 &&
                gain_ground::generated::kAssetSets.size() == 42 &&
                gain_ground::generated::kRetainedArenaBytes == 14112,
            "generated asset inventory differs");
        require(gain_ground::generated::kFunctionCheckpointMasks.size() == gain_ground::generated::kFunctions.size() &&
                gain_ground::generated::kCheckpointKinds.size() == 7 &&
                (gain_ground::generated::kFunctionCheckpointMasks[44] & 0x00000002U) != 0,
            "generated checkpoint inventory differs");

        std::vector<std::uint32_t> counts(static_cast<std::size_t>(header.function_count));
        for (const auto &record : records)
            ++counts.at(record.function_id);

        std::size_t missing_functions{};
        std::size_t sampled{};
        std::size_t implemented_functions{};
        std::size_t fallback_stubs{};
        std::size_t first_unimplemented_fixture = records.size();
        std::size_t first_unimplemented_function = gain_ground::generated::kFunctions.size();
        for (std::size_t index = 0; index != gain_ground::generated::kFunctions.size(); ++index) {
            const auto &function = gain_ground::generated::kFunctions[index];
            const auto &catalog = gain_ground::generated::kFixtureFunctions[index];
            require(function.id == index && catalog.id == index, "generated function ID ordering differs");
            require(function.cpu == catalog.cpu && function.state == catalog.state &&
                    function.address == catalog.address && function.label == catalog.label,
                "generated function metadata differs at ID " + std::to_string(index));
            require(function.first_fixture == catalog.first_record &&
                    function.fixture_count == catalog.record_count && function.fixture_count == counts[index],
                "generated fixture range differs at ID " + std::to_string(index));

            if (function.implemented) {
                ++implemented_functions;
                require(!function.implementation_source.empty(),
                    "catalogued implementation has no source at ID " + std::to_string(index));
            } else {
                ++fallback_stubs;
                if (first_unimplemented_function == gain_ground::generated::kFunctions.size())
                    first_unimplemented_function = index;
                require(function.implementation_source.empty(),
                    "fallback stub unexpectedly names an implementation source at ID " + std::to_string(index));
                gain_ground::FunctionContext context;
                context.cpu = function.cpu;
                context.state = function.state;
                context.registers.program_counter = function.address;
                require(function.entry(context).status == gain_ground::TranslationStatus::unimplemented,
                    "generated fallback is not explicitly unimplemented at ID " + std::to_string(index));
                if (function.fixture_count != 0 && first_unimplemented_fixture == records.size())
                    first_unimplemented_fixture = function.first_fixture;
            }

            if (function.fixture_count == 0) {
                ++missing_functions;
                continue;
            }
            const auto fixture = bundle.load(function.first_fixture);
            validate_fixture(fixture, function.first_fixture);
            ++sampled;
        }
        require(missing_functions == gain_ground::generated::kFunctionCountWithoutCompleteFixtures,
            "missing-fixture function count differs");
        require(implemented_functions == gain_ground::generated::kImplementedFunctionCount,
            "catalogued implementation count differs");
        require(fallback_stubs + implemented_functions == gain_ground::generated::kFunctions.size(),
            "implementation/stub ownership does not cover every function");

        if (first_unimplemented_fixture != records.size()) {
            const auto comparison = gain_ground::compare_fixture(
                bundle.load(first_unimplemented_fixture), first_unimplemented_fixture);
            require(comparison.status == gain_ground::ComparisonStatus::unimplemented &&
                    comparison.first_divergence.subsystem == "translation" &&
                    comparison.first_divergence.field == "status",
                "fixture comparator did not identify the explicit unimplemented boundary");
        }

        if (first_unimplemented_function != gain_ground::generated::kFunctions.size()) {
            const std::array<std::uint32_t, 1> first_function{
                static_cast<std::uint32_t>(first_unimplemented_function)};
            const auto batch = gain_ground::compare_function_fixtures(bundle, first_function);
            require(batch.status == gain_ground::ComparisonStatus::unimplemented &&
                    batch.selected_functions == 1 && batch.implemented_functions == 0 &&
                    batch.unimplemented_functions == 1 && batch.executed_fixtures == 0 &&
                    batch.has_first_failure,
                "exhaustive function comparator did not enforce implementation ownership");
            require(gain_ground::batch_report_json(batch).find(
                        "\"schema\": \"gground-native-fixture-batch-comparison\"") != std::string::npos,
                "exhaustive function comparator did not emit its structured schema");
        } else {
            require(fallback_stubs == 0U &&
                    implemented_functions == gain_ground::generated::kFunctions.size(),
                "zero-fallback catalog does not have complete native ownership");
        }

        const auto &function618 = gain_ground::generated::kFunctions[618U];
        require(function618.fixture_count == 1U,
            "function 618 digest regression requires one fixture");
        const auto function618_fixture = bundle.load(function618.first_fixture);
        const auto reordered_function618 = gain_ground::compare_fixture(
            function618_fixture, function618.first_fixture + 17U);
        require(reordered_function618.status == gain_ground::ComparisonStatus::passed,
            "function 618 compatibility binding changed under aggregate reorder");

        auto wrong_digest = function618_fixture;
        wrong_digest.bundle.payload_digest[0] ^= 0x01U;
        const auto wrong_digest_report = gain_ground::compare_fixture(
            wrong_digest, function618.first_fixture + 17U);
        require(wrong_digest_report.status == gain_ground::ComparisonStatus::diverged
                && wrong_digest_report.first_divergence.subsystem == "memory"
                && wrong_digest_report.first_divergence.field == "2:0x00007fee.flags",
            "function 618 compatibility accepted a mismatched payload digest");

        auto wrong_call = function618_fixture;
        require(!wrong_call.calls.empty(),
            "function 618 digest regression requires captured calls");
        wrong_call.calls.back().target ^= 0x02U;
        const auto wrong_call_report = gain_ground::compare_fixture(
            wrong_call, function618.first_fixture + 17U);
        require(wrong_call_report.status == gain_ground::ComparisonStatus::diverged
                && wrong_call_report.first_divergence.subsystem == "call"
                && wrong_call_report.first_divergence.field == "target",
            "function 618 compatibility accepted a mismatched call boundary");

        const auto &function628 = gain_ground::generated::kFunctions[628U];
        require(function628.fixture_count == 4U,
            "function 628 continuation regression requires four fixtures");
        auto function628_fixture = bundle.load(function628.first_fixture);
        std::size_t function628_record = function628.first_fixture;
        for (std::uint32_t offset = 1U; offset != function628.fixture_count; ++offset) {
            auto candidate = bundle.load(function628.first_fixture + offset);
            if (candidate.calls.size() > function628_fixture.calls.size()) {
                function628_fixture = std::move(candidate);
                function628_record = function628.first_fixture + offset;
            }
        }
        const auto marker = std::find_if(function628_fixture.calls.begin(),
            function628_fixture.calls.end(), [](const auto &call) {
                return call.function_id == 118U && call.target_cpu == 1U
                    && call.target_state == 0x72U && call.kind == 1U
                    && call.callsite == 0x000085b4U && call.target == 0x000085b0U;
            });
        require(marker != function628_fixture.calls.end(),
            "function 628 continuation regression requires a captured wait marker");
        const auto marker_index = static_cast<std::size_t>(
            std::distance(function628_fixture.calls.begin(), marker));

        auto wrong_marker = function628_fixture;
        wrong_marker.calls[marker_index].target ^= 0x02U;
        require(gain_ground::compare_fixture(wrong_marker, function628_record).status
                == gain_ground::ComparisonStatus::diverged,
            "function 628 comparator accepted a wrong wait-loop marker");

        auto missing_marker = function628_fixture;
        missing_marker.calls.erase(missing_marker.calls.begin()
            + static_cast<std::ptrdiff_t>(marker_index));
        require(gain_ground::compare_fixture(missing_marker, function628_record).status
                == gain_ground::ComparisonStatus::diverged,
            "function 628 comparator accepted a missing wait-loop marker");

        auto reordered_marker = function628_fixture;
        require(marker_index + 1U < reordered_marker.calls.size(),
            "function 628 continuation regression requires a following captured call");
        std::swap(reordered_marker.calls[marker_index],
            reordered_marker.calls[marker_index + 1U]);
        require(gain_ground::compare_fixture(reordered_marker, function628_record).status
                == gain_ground::ComparisonStatus::diverged,
            "function 628 comparator accepted a reordered wait-loop marker");

        if (all) {
            for (std::size_t index = 0; index != records.size(); ++index) {
                validate_fixture(bundle.load(index), index);
                if ((index + 1) % 1000 == 0)
                    std::cout << "validated " << (index + 1) << '/' << records.size() << " fixtures\n";
            }
        }

        std::cout << "Gain Ground native fixture smoke passed\n"
                  << "  functions: " << header.function_count << "\n"
                  << "  complete fixture records indexed: " << header.fixture_count << "\n"
                  << "  functions sampled: " << sampled << "\n"
                  << "  catalogued behavior implementations: " << implemented_functions << "\n"
                  << "  generated fallback stubs: " << fallback_stubs << "\n";
        return 0;
    } catch (const std::exception &error) {
        std::cerr << "ERROR: " << error.what() << '\n';
        return 1;
    }
}

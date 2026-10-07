#include "gain_ground/fixture_compare.h"
#include "fixture_compare_internal.h"

#include "gground_functions.h"
#include "gground_checkpoints.h"
#include "gground_fixture_marker_compatibility.h"

#include <array>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>

namespace gain_ground {
using fixture_compare_detail::ComparingHost;
using fixture_compare_detail::hex;
using fixture_compare_detail::number;
namespace {
std::string json_escape(std::string_view value)
{
    std::ostringstream stream;
    for (const unsigned char character : value) {
        switch (character) {
        case '\\': stream << "\\\\"; break;
        case '"': stream << "\\\""; break;
        case '\b': stream << "\\b"; break;
        case '\f': stream << "\\f"; break;
        case '\n': stream << "\\n"; break;
        case '\r': stream << "\\r"; break;
        case '\t': stream << "\\t"; break;
        default:
            if (character < 0x20)
                stream << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<unsigned>(character);
            else
                stream << character;
        }
    }
    return stream.str();
}
}

void compare_registers(
    const CpuRegisters &expected,
    const CpuRegisters &actual,
    FixtureComparisonReport &report)
{
    auto set = [&report](std::string field, std::uint32_t expected_value, std::uint32_t actual_value, unsigned width) {
        if (report.first_divergence.field.empty() && expected_value != actual_value)
            report.first_divergence = {"registers", std::move(field), hex(expected_value, width), hex(actual_value, width), 0xffffffffU};
    };
    for (std::size_t index = 0; index != expected.data.size(); ++index)
        set("D" + number(index), expected.data[index], actual.data[index], 8);
    for (std::size_t index = 0; index != expected.address.size(); ++index)
        set("A" + number(index), expected.address[index], actual.address[index], 8);
    set("PC", expected.program_counter, actual.program_counter, 8);
    set("SR", expected.status, actual.status, 4);
}

FixtureComparisonReport compare_fixture(const FixtureRecord &fixture, std::size_t fixture_record)
{
    FixtureComparisonReport report;
    report.fixture_record = fixture_record;
    report.function_id = fixture.function_id;
    report.source_index = fixture.bundle.source_index;
    report.source_fixture_ordinal = fixture.bundle.source_fixture_ordinal;
    if (!fixture.bundle.native_replay_eligible) {
        report.first_divergence = {
            "fixture", "native_replay_eligible", "true", "false", 0xffffffffU};
        return report;
    }
    if (fixture.function_id >= generated::kFunctions.size()) {
        report.first_divergence = {"function", "id", "known function", number(fixture.function_id), 0xffffffffU};
        return report;
    }

    const auto &function = generated::kFunctions[fixture.function_id];
    report.function_label = std::string(function.label);
    report.checkpoint_mask = generated::kFunctionCheckpointMasks[fixture.function_id];
    for (const auto &kind : generated::kCheckpointKinds)
        if ((report.checkpoint_mask & kind.mask) != 0)
            report.checkpoint_kinds.emplace_back(kind.name);
    ComparingHost host(fixture, fixture_record);
    FunctionContext context;
    context.registers = fixture.entry_registers;
    context.cpu = fixture.cpu;
    context.state = fixture.state;
    context.host = &host;
    const auto result = function.entry(context);
    if (result.status == TranslationStatus::unimplemented) {
        report.status = ComparisonStatus::unimplemented;
        report.first_divergence = {"translation", "status", "complete", "unimplemented", 0xffffffffU};
        return report;
    }
    if (result.status != TranslationStatus::complete) {
        host.finish();
        if (host.diverged())
            report.first_divergence = host.divergence();
        else
            report.first_divergence = {"translation", "status", "complete", "contract_violation", 0xffffffffU};
        return report;
    }

    host.finish();
    report.compatibility_rules = host.compatibility_rules();
    if (host.diverged())
        report.first_divergence = host.divergence();
    if (report.first_divergence.field.empty() && result.control != fixture.control)
        report.first_divergence = {"control", "result", number(fixture.control), number(result.control), 0xffffffffU};
    if (report.first_divergence.field.empty() && result.exit_program_counter != fixture.exit_pc)
        report.first_divergence = {"control", "exit_pc", hex(fixture.exit_pc, 8), hex(result.exit_program_counter, 8), 0xffffffffU};
    compare_registers(fixture.exit_registers, context.registers, report);
    report.status = report.first_divergence.field.empty() ? ComparisonStatus::passed : ComparisonStatus::diverged;
    return report;
}

FixtureBatchReport compare_function_fixtures(
    const FixtureBundle &bundle,
    std::span<const std::uint32_t> function_ids)
{
    FixtureBatchReport batch;
    std::vector<bool> selected(generated::kFunctions.size());
    for (const auto function_id : function_ids) {
        if (function_id >= generated::kFunctions.size())
            throw std::runtime_error("function ID is out of range: " + number(function_id));
        if (selected[function_id])
            throw std::runtime_error("function ID is duplicated: " + number(function_id));
        selected[function_id] = true;

        const auto &function = generated::kFunctions[function_id];
        std::vector<std::size_t> fixture_records;
        for (std::size_t record_index = 0; record_index != bundle.records().size(); ++record_index) {
            if (bundle.records()[record_index].function_id == function_id
                && bundle.records()[record_index].native_replay_eligible)
                fixture_records.push_back(record_index);
        }
        ++batch.selected_functions;
        batch.selected_fixtures += fixture_records.size();
        if (!function.implemented) {
            ++batch.unimplemented_functions;
            batch.status = ComparisonStatus::unimplemented;
            batch.has_first_failure = true;
            batch.first_failure.status = ComparisonStatus::unimplemented;
            batch.first_failure.fixture_record = fixture_records.empty() ? 0U : fixture_records.front();
            batch.first_failure.function_id = function.id;
            batch.first_failure.function_label = std::string(function.label);
            batch.first_failure.first_divergence = {
                "translation", "implementation_catalog", "catalogued", "fallback_stub", 0xffffffffU};
            break;
        }

        ++batch.implemented_functions;
        if (fixture_records.empty()) {
            ++batch.diverged_functions;
            batch.status = ComparisonStatus::diverged;
            batch.has_first_failure = true;
            batch.first_failure.status = ComparisonStatus::diverged;
            batch.first_failure.fixture_record = 0U;
            batch.first_failure.function_id = function.id;
            batch.first_failure.function_label = std::string(function.label);
            batch.first_failure.first_divergence = {
                "fixture", "count", "at least one complete fixture", "0", 0xffffffffU};
            break;
        }
        bool function_passed = true;
        for (const auto record_index : fixture_records) {
            const auto fixture = bundle.load(record_index);
            ++batch.executed_fixtures;
            auto report = compare_fixture(fixture, record_index);
            batch.compatibility_rules.insert(
                batch.compatibility_rules.end(),
                report.compatibility_rules.begin(), report.compatibility_rules.end());
            for (const auto &rule : report.compatibility_rules) {
                batch.compatibility_rule_hits.push_back({
                    rule,
                    report.source_index,
                    report.source_fixture_ordinal,
                    report.function_id,
                });
            }
            if (report.status == ComparisonStatus::passed) {
                ++batch.passed_fixtures;
                continue;
            }

            function_passed = false;
            batch.has_first_failure = true;
            batch.first_failure = std::move(report);
            if (batch.first_failure.status == ComparisonStatus::unimplemented) {
                ++batch.unimplemented_functions;
                ++batch.unimplemented_fixtures;
                batch.status = ComparisonStatus::unimplemented;
            } else {
                ++batch.diverged_functions;
                ++batch.diverged_fixtures;
                batch.status = ComparisonStatus::diverged;
            }
            break;
        }
        if (function_passed)
            ++batch.passed_functions;
        else
            break;
    }
    return batch;
}

void enforce_exact_compatibility_histogram(FixtureBatchReport &report)
{
    if (report.status != ComparisonStatus::passed)
        return;

    std::map<std::string, std::size_t> actual;
    for (const auto &rule : report.compatibility_rules)
        ++actual[rule];

    for (const auto &expected : generated::kFixtureMarkerCompatibilityExpectedRuleHits) {
        const auto found = actual.find(std::string(expected.rule));
        const auto count = found == actual.end() ? 0U : found->second;
        if (count != expected.count) {
            report.status = ComparisonStatus::diverged;
            report.has_first_failure = true;
            report.first_failure.status = ComparisonStatus::diverged;
            report.first_failure.first_divergence = {
                "compatibility", "rule_histogram." + std::string(expected.rule),
                number(expected.count), number(count), 0xffffffffU};
            return;
        }
        actual.erase(std::string(expected.rule));
    }
    if (!actual.empty()) {
        report.status = ComparisonStatus::diverged;
        report.has_first_failure = true;
        report.first_failure.status = ComparisonStatus::diverged;
        report.first_failure.first_divergence = {
            "compatibility", "unexpected_rule." + actual.begin()->first,
            "0", number(actual.begin()->second), 0xffffffffU};
        return;
    }
    if (report.compatibility_rules.size()
            != generated::kFixtureMarkerCompatibilityExpectedHitCount) {
        report.status = ComparisonStatus::diverged;
        report.has_first_failure = true;
        report.first_failure.status = ComparisonStatus::diverged;
        report.first_failure.first_divergence = {
            "compatibility", "total_hit_count",
            number(generated::kFixtureMarkerCompatibilityExpectedHitCount),
            number(report.compatibility_rules.size()), 0xffffffffU};
    }
}

std::string comparison_status_name(ComparisonStatus status)
{
    switch (status) {
    case ComparisonStatus::passed: return "passed";
    case ComparisonStatus::unimplemented: return "unimplemented";
    case ComparisonStatus::diverged: return "diverged";
    }
    return "unknown";
}

std::string comparison_report_json(const FixtureComparisonReport &report)
{
    std::ostringstream stream;
    stream << "{\n"
           << "  \"schema\": \"gground-native-fixture-comparison\",\n"
           << "  \"schemaVersion\": 1,\n"
           << "  \"status\": \"" << comparison_status_name(report.status) << "\",\n"
           << "  \"fixtureRecord\": " << report.fixture_record << ",\n"
           << "  \"sourceIndex\": " << report.source_index << ",\n"
           << "  \"sourceFixtureOrdinal\": " << report.source_fixture_ordinal << ",\n"
           << "  \"functionId\": " << report.function_id << ",\n"
           << "  \"functionLabel\": \"" << json_escape(report.function_label) << "\",\n"
           << "  \"checkpointMask\": \"" << hex(report.checkpoint_mask, 8) << "\",\n"
           << "  \"checkpointKinds\": [";
    for (std::size_t index = 0; index != report.checkpoint_kinds.size(); ++index) {
        if (index != 0)
            stream << ", ";
        stream << "\"" << json_escape(report.checkpoint_kinds[index]) << "\"";
    }
    stream << "],\n"
           << "  \"compatibilityRuleHitCount\": " << report.compatibility_rules.size() << ",\n"
           << "  \"compatibilityRuleCounts\": {";
    std::map<std::string, std::size_t> compatibility_counts;
    for (const auto &rule : report.compatibility_rules)
        ++compatibility_counts[rule];
    std::size_t compatibility_index = 0;
    for (const auto &[rule, count] : compatibility_counts) {
        if (compatibility_index++ != 0)
            stream << ", ";
        stream << "\"" << json_escape(rule) << "\": " << count;
    }
    stream << "},\n"
           << "  \"firstDivergence\": {\n"
           << "    \"subsystem\": \"" << json_escape(report.first_divergence.subsystem) << "\",\n"
           << "    \"field\": \"" << json_escape(report.first_divergence.field) << "\",\n"
           << "    \"expected\": \"" << json_escape(report.first_divergence.expected) << "\",\n"
           << "    \"actual\": \"" << json_escape(report.first_divergence.actual) << "\",\n"
           << "    \"sequence\": ";
    if (report.first_divergence.sequence == 0xffffffffU)
        stream << "null\n";
    else
        stream << report.first_divergence.sequence << "\n";
    stream << "  }\n}\n";
    return stream.str();
}

std::string batch_report_json(const FixtureBatchReport &report)
{
    std::ostringstream stream;
    stream << "{\n"
           << "  \"schema\": \"gground-native-fixture-batch-comparison\",\n"
           << "  \"schemaVersion\": 1,\n"
           << "  \"status\": \"" << comparison_status_name(report.status) << "\",\n"
           << "  \"functions\": {\n"
           << "    \"selected\": " << report.selected_functions << ",\n"
           << "    \"implemented\": " << report.implemented_functions << ",\n"
           << "    \"passed\": " << report.passed_functions << ",\n"
           << "    \"unimplemented\": " << report.unimplemented_functions << ",\n"
           << "    \"diverged\": " << report.diverged_functions << "\n"
           << "  },\n"
           << "  \"fixtures\": {\n"
           << "    \"selected\": " << report.selected_fixtures << ",\n"
           << "    \"executed\": " << report.executed_fixtures << ",\n"
           << "    \"passed\": " << report.passed_fixtures << ",\n"
           << "    \"unimplemented\": " << report.unimplemented_fixtures << ",\n"
           << "    \"diverged\": " << report.diverged_fixtures << "\n"
           << "  },\n"
           << "  \"compatibilityRuleHitCount\": " << report.compatibility_rules.size() << ",\n"
           << "  \"compatibilityRuleCounts\": {";
    std::map<std::string, std::size_t> compatibility_counts;
    for (const auto &rule : report.compatibility_rules)
        ++compatibility_counts[rule];
    std::size_t compatibility_index = 0;
    for (const auto &[rule, count] : compatibility_counts) {
        if (compatibility_index++ != 0)
            stream << ", ";
        stream << "\"" << json_escape(rule) << "\": " << count;
    }
    stream << "},\n"
           << "  \"compatibilityRuleHits\": [";
    for (std::size_t index = 0; index != report.compatibility_rule_hits.size(); ++index) {
        if (index != 0)
            stream << ",";
        const auto &hit = report.compatibility_rule_hits[index];
        stream << "{\"rule\":\"" << json_escape(hit.rule)
               << "\",\"sourceIndex\":" << hit.source_index
               << ",\"sourceFixtureOrdinal\":" << hit.source_fixture_ordinal
               << ",\"rootFunctionId\":" << hit.root_function_id << "}";
    }
    stream << "],\n"
           << "  \"stoppedAtFirstFailure\": " << (report.has_first_failure ? "true" : "false") << ",\n"
           << "  \"firstFailure\": ";
    if (report.has_first_failure)
        stream << comparison_report_json(report.first_failure);
    else
        stream << "null\n";
    stream << "}\n";
    return stream.str();
}

} // namespace gain_ground

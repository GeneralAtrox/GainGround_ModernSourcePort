#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <span>
#include <vector>

#include "gain_ground/fixture_bundle.h"

namespace gain_ground {

enum class ComparisonStatus : std::uint8_t {
    passed,
    unimplemented,
    diverged,
};

struct FirstDivergence {
    std::string subsystem;
    std::string field;
    std::string expected;
    std::string actual;
    std::uint32_t sequence{0xffffffffU};
};

struct FixtureComparisonReport {
    ComparisonStatus status{ComparisonStatus::diverged};
    std::size_t fixture_record{};
    std::uint32_t function_id{};
    std::uint32_t source_index{};
    std::uint64_t source_fixture_ordinal{};
    std::string function_label;
    std::uint32_t checkpoint_mask{};
    std::vector<std::string> checkpoint_kinds;
    std::vector<std::string> compatibility_rules;
    FirstDivergence first_divergence;
};

struct CompatibilityRuleHit {
    std::string rule;
    std::uint32_t source_index{};
    std::uint64_t source_fixture_ordinal{};
    std::uint32_t root_function_id{};
};

struct FixtureBatchReport {
    ComparisonStatus status{ComparisonStatus::passed};
    std::size_t selected_functions{};
    std::size_t implemented_functions{};
    std::size_t passed_functions{};
    std::size_t unimplemented_functions{};
    std::size_t diverged_functions{};
    std::size_t selected_fixtures{};
    std::size_t executed_fixtures{};
    std::size_t passed_fixtures{};
    std::size_t unimplemented_fixtures{};
    std::size_t diverged_fixtures{};
    std::vector<std::string> compatibility_rules;
    std::vector<CompatibilityRuleHit> compatibility_rule_hits;
    bool has_first_failure{};
    FixtureComparisonReport first_failure;
};

[[nodiscard]] FixtureComparisonReport compare_fixture(
    const FixtureRecord &fixture,
    std::size_t fixture_record);
[[nodiscard]] std::string comparison_status_name(ComparisonStatus status);
[[nodiscard]] std::string comparison_report_json(const FixtureComparisonReport &report);
[[nodiscard]] FixtureBatchReport compare_function_fixtures(
    const FixtureBundle &bundle,
    std::span<const std::uint32_t> function_ids);
void enforce_exact_compatibility_histogram(FixtureBatchReport &report);
[[nodiscard]] std::string batch_report_json(const FixtureBatchReport &report);

} // namespace gain_ground

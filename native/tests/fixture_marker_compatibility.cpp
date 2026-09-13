#include "gain_ground/fixture_compare.h"

#include "gground_fixture_marker_compatibility.h"

#include <stdexcept>
#include <string>

namespace {

void require(bool condition, const char *message)
{
    if (!condition)
        throw std::runtime_error(message);
}

gain_ground::FixtureBatchReport exact_report()
{
    gain_ground::FixtureBatchReport report;
    for (const auto &expected :
            gain_ground::generated::kFixtureMarkerCompatibilityExpectedRuleHits)
        for (std::uint32_t index = 0; index != expected.count; ++index)
            report.compatibility_rules.emplace_back(expected.rule);
    return report;
}

} // namespace

int main()
{
    using gain_ground::ComparisonStatus;
    using gain_ground::enforce_exact_compatibility_histogram;
    using gain_ground::generated::fixture_marker_compatibility_allowed;

    auto exact = exact_report();
    enforce_exact_compatibility_histogram(exact);
    require(exact.status == ComparisonStatus::passed,
        "exact compatibility histogram was rejected");

    auto missing = exact_report();
    missing.compatibility_rules.pop_back();
    enforce_exact_compatibility_histogram(missing);
    require(missing.status == ComparisonStatus::diverged
            && missing.first_failure.first_divergence.subsystem == "compatibility",
        "missing compatibility hit was accepted");

    auto extra = exact_report();
    extra.compatibility_rules.emplace_back("not_a_reviewed_rule");
    enforce_exact_compatibility_histogram(extra);
    require(extra.status == ComparisonStatus::diverged
            && extra.first_failure.first_divergence.field
                == "unexpected_rule.not_a_reviewed_rule",
        "unexpected compatibility rule was accepted");

    auto mismatched = exact_report();
    mismatched.compatibility_rules.front() = "not_a_reviewed_rule";
    enforce_exact_compatibility_histogram(mismatched);
    require(mismatched.status == ComparisonStatus::diverged,
        "mismatched compatibility histogram was accepted");

    constexpr auto rule = "omitted_current_f404_f550_marker";
    require(fixture_marker_compatibility_allowed(rule, 32U, 404U, 0U),
        "reviewed compatibility provenance was rejected");
    require(!fixture_marker_compatibility_allowed("not_a_reviewed_rule", 32U, 404U, 0U),
        "unknown compatibility predicate was accepted");
    require(!fixture_marker_compatibility_allowed(rule, 31U, 404U, 0U),
        "wrong compatibility source was accepted");
    require(!fixture_marker_compatibility_allowed(rule, 32U, 403U, 0U),
        "wrong compatibility root was accepted");
    require(!fixture_marker_compatibility_allowed(rule, 32U, 404U, 1U),
        "wrong compatibility ordinal was accepted");
}

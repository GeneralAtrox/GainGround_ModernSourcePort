
bool fixture_marker_compatibility_allowed(std::string_view rule, std::uint32_t source_index,
    std::uint32_t root_function_id, std::uint64_t source_fixture_ordinal) noexcept
{
    std::uint16_t rule_id = 0U;
    for (std::size_t index = 1U; index < kFixtureMarkerCompatibilityRuleNames.size(); ++index)
        if (kFixtureMarkerCompatibilityRuleNames[index] == rule) { rule_id = static_cast<std::uint16_t>(index); break; }
    for (const auto &candidate : kFixtureMarkerCompatibilityRanges)
        if (candidate.rule_id == rule_id && candidate.source_index == source_index
            && candidate.root_function_id == root_function_id
            && candidate.source_fixture_ordinal_minimum <= source_fixture_ordinal
            && source_fixture_ordinal <= candidate.source_fixture_ordinal_maximum) return true;
    return false;
}


#include "gain_ground/fixture_bundle.h"
#include "gain_ground/fixture_compare.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <stdexcept>
#include <string_view>

using namespace gain_ground;
namespace {
void require(bool condition, const char *message)
{
    if (!condition) throw std::runtime_error(message);
}
void check(const FixtureBundle &bundle, std::string_view digest)
{
    const auto &index = bundle.records();
    const auto found = std::find_if(index.begin(), index.end(), [&](const auto &record) {
        return record.function_id == 116U && hexadecimal(record.payload_digest.data(), record.payload_digest.size()) == digest;
    });
    require(found != index.end(), "original input-partition record missing");
    const auto ordinal = static_cast<std::size_t>(found - index.begin());
    const auto original = bundle.load(ordinal);
    require(original.control == 5U, "enclosing regression must reach its captured interrupt boundary");
    const auto result = compare_fixture(original, ordinal);
    require(result.status == ComparisonStatus::passed && result.compatibility_rules.empty(),
        "enclosing input sample must retain the complete captured result");
    const auto marker = std::find_if(original.calls.begin(), original.calls.end(), [](const auto &call) {
        return call.function_id == 120U && call.target_cpu == 1U && call.target_state == 0x72U &&
            call.kind == 0U && call.callsite == 0x85ceU && call.target == 0x85d0U;
    });
    require(marker != original.calls.end(), "original input partition is missing");
    const auto position = static_cast<std::size_t>(marker - original.calls.begin());
    const auto sample = std::find_if(original.hardware_effects.begin(), original.hardware_effects.end(), [](const auto &event) {
        return event.kind == 1U && event.pc == 0x85d0U && event.address == 0x800008U;
    });
    require(sample != original.hardware_effects.end(), "original fourth input read is missing");
    const auto hardware = static_cast<std::size_t>(sample - original.hardware_effects.begin());
    const std::array<std::string_view, 12> fields{"sequence", "function_id", "target_cpu", "target_state", "kind", "callsite", "target",
        "sequence", "sequence", "sequence", "address", "2:0x0000080c.final"};
    for (unsigned mutation = 0; mutation < fields.size(); ++mutation) {
        auto fixture = original;
        auto &call = fixture.calls[position];
        switch (mutation) {
        case 0: ++call.sequence; break;
        case 1: ++call.function_id; break;
        case 2: call.target_cpu ^= 1U; break;
        case 3: call.target_state ^= 1U; break;
        case 4: call.kind = 2U; break;
        case 5: call.callsite += 2U; break;
        case 6: call.target += 2U; break;
        case 7: fixture.calls.erase(fixture.calls.begin() + position); break;
        case 8:
            require(position + 1U < fixture.calls.size(), "later captured child is missing");
            std::swap(fixture.calls[position], fixture.calls[position + 1U]); break;
        case 9: --fixture.hardware_effects[hardware].sequence; break;
        case 10: fixture.hardware_effects[hardware].address += 2U; break;
        case 11: fixture.hardware_effects[hardware].data ^= 1U; break;
        }
        const auto failed = compare_fixture(fixture, ordinal);
        require(failed.status == ComparisonStatus::diverged &&
            failed.first_divergence.subsystem == (mutation < 9U ? "call" : mutation < 11U ? "hardware" : "memory") &&
            failed.first_divergence.field == fields[mutation],
            "damaged partition or input read did not fail at its exact contract field");
    }
}
}
int main(int argc, char **argv)
{
    try {
        require(argc == 2, "expected original fixture bundle");
        const FixtureBundle bundle{argv[1]};
        for (const auto digest : {
            "8308576ac1a6976fe6923dfd57270b00f6d79caa65a2c4058dd90e9b1aa26ec3",
            "f09722bdf74c281ca607d49473ccb90f7c5df7998c09e46470a5a26bfbe2e80a",
            "c5228f0075edb4ff267ce7704cd7f8e90bc2c68a414114502e165bb04d97e9bb",
            "447852924fe7ff1f5cff983444366ecbff3a2d98b6c4fa381c9eda61c090b7ee"}) check(bundle, digest);
        const std::array<std::uint32_t, 2> functions{119U, 120U};
        const auto standalone = compare_function_fixtures(bundle, functions);
        require(standalone.status == ComparisonStatus::passed && standalone.executed_fixtures == 11U &&
            standalone.compatibility_rules.empty(), "standalone sampler boundaries must remain unchanged");
        std::cout << "Four enclosing records pass; 48 damaged contracts rejected; 11 standalone sampler boundaries pass\n";
    } catch (const std::exception &error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}

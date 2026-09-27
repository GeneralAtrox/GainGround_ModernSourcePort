#include "gain_ground/fixture_bundle.h"
#include "gain_ground/fixture_compare.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string_view>

using namespace gain_ground;

namespace {
void require(bool condition, const char *message)
{
    if (!condition) throw std::runtime_error(message);
}

void check(const FixtureBundle &bundle, std::uint32_t root, std::string_view digest)
{
    const auto &index = bundle.records();
    const auto found = std::find_if(index.begin(), index.end(), [&](const auto &record) {
        return record.function_id == root &&
            hexadecimal(record.payload_digest.data(), record.payload_digest.size()) == digest;
    });
    require(found != index.end(), "original enclosing-loop fixture is missing");
    const auto ordinal = static_cast<std::size_t>(found - index.begin());
    const auto original = bundle.load(ordinal);
    require(original.control == 5U && original.calls.size() > 2000U,
        "regression requires the long original loop ending at an interrupt");
    const auto passed = compare_fixture(original, ordinal);
    require(passed.status == ComparisonStatus::passed && passed.compatibility_rules.empty(),
        "enclosing loop did not retain the exact original interrupt boundary");
    const auto marker = std::find_if(original.calls.begin(), original.calls.end(), [](const auto &call) {
        return call.function_id == 118U && call.target_cpu == 1U && call.target_state == 0x72U &&
            call.kind == 1U && call.callsite == 0x85b4U && call.target == 0x85b0U;
    });
    require(marker != original.calls.end(), "original branch marker is missing");
    const auto position = static_cast<std::size_t>(marker - original.calls.begin());
    for (unsigned mutation = 0; mutation < 10U; ++mutation) {
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
            require(position + 1U < fixture.calls.size(), "following branch marker is missing");
            std::swap(fixture.calls[position], fixture.calls[position + 1U]);
            break;
        case 9: {
            FixtureHardwareEffect pending{};
            pending.sequence = call.sequence;
            fixture.hardware_effects.insert(fixture.hardware_effects.begin(), pending);
            break;
        }
        }
        const auto failed = compare_fixture(fixture, ordinal);
        require(failed.status == ComparisonStatus::diverged,
            "comparator admitted a damaged enclosing-loop contract");
        require(failed.first_divergence.subsystem == (mutation == 9U ? "hardware" : "call") &&
            failed.first_divergence.field == (mutation == 9U ? "continuation_marker_order" : "continuation_marker"),
            "damaged marker did not fail at its exact continuation boundary");
    }
}
} // namespace

int main(int argc, char **argv)
{
    try {
        require(argc == 2, "expected original fixture bundle");
        const FixtureBundle bundle{argv[1]};
        check(bundle, 115U, "71188e5786652b0f89b74b4321f434b12149db18ed98efe896598fe2c2171796");
        check(bundle, 116U, "7ae65aeeb78f6909841c192e0533353a0af3758568e5ac69bf85248a90f5fc19");
        std::cout << "Two original enclosing loops pass; twenty damaged marker/order contracts are rejected\n";
    } catch (const std::exception &error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}

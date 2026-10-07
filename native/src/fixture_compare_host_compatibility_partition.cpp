#include "fixture_compare_internal.h"
#include <cstdlib>
#include <iostream>
#include <utility>
namespace gain_ground::fixture_compare_detail {
std::optional<FunctionResult> ComparingHost::try_partition_compatibility(std::uint32_t function_id, std::uint8_t target_cpu, std::uint8_t target_state, std::uint8_t kind, std::uint32_t callsite, std::uint32_t target, FunctionContext &context)
    {
        const bool omitted_legacy_f329_partition_marker = fixture_.function_id == 329U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && ((function_id == 550U && callsite == 0x0001c10eU
                    && target == 0x0001d9daU)
                || (function_id == 551U && callsite == 0x0001c118U
                    && target == 0x0001d9eaU))
            && call_index_ >= fixture_.calls.size();
        if (omitted_legacy_f329_partition_marker) {
            record_compatibility_rule("omitted_legacy_f329_partition_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[function_id].entry(context);
        }

        const bool omitted_legacy_f327_partition_marker = fixture_.function_id == 327U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && ((function_id == 550U && callsite == 0x0001bdc6U
                    && target == 0x0001d9daU)
                || (function_id == 551U && callsite == 0x0001bdd0U
                    && target == 0x0001d9eaU))
            && call_index_ >= fixture_.calls.size();
        if (omitted_legacy_f327_partition_marker) {
            record_compatibility_rule("omitted_legacy_f327_partition_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[function_id].entry(context);
        }

        const bool omitted_legacy_f330_partition_marker = fixture_.function_id == 330U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && ((function_id == 550U && callsite == 0x0001c1e6U
                    && target == 0x0001d9daU)
                || (function_id == 551U && callsite == 0x0001c1f0U
                    && target == 0x0001d9eaU))
            && call_index_ >= fixture_.calls.size();
        if (omitted_legacy_f330_partition_marker) {
            record_compatibility_rule("omitted_legacy_f330_partition_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[function_id].entry(context);
        }

        const bool omitted_legacy_f331_partition_marker = fixture_.function_id == 331U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && ((function_id == 550U && callsite == 0x0001c384U
                    && target == 0x0001d9daU)
                || (function_id == 551U && callsite == 0x0001c38eU
                    && target == 0x0001d9eaU))
            && call_index_ >= fixture_.calls.size();
        if (omitted_legacy_f331_partition_marker) {
            record_compatibility_rule("omitted_legacy_f331_partition_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[function_id].entry(context);
        }

        const bool omitted_legacy_f332_partition_marker = fixture_.function_id == 332U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && ((function_id == 550U && callsite == 0x0001cc6cU
                    && target == 0x0001d9daU)
                || (function_id == 551U && callsite == 0x0001cc76U
                    && target == 0x0001d9eaU))
            && call_index_ >= fixture_.calls.size();
        if (omitted_legacy_f332_partition_marker) {
            record_compatibility_rule("omitted_legacy_f332_partition_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[function_id].entry(context);
        }

        const bool omitted_legacy_f336_partition_marker =
            (fixture_.function_id == 330U || fixture_.function_id == 336U)
            && target_cpu == 1U && target_state == 0x72U
            && ((function_id == 548U
                    && ((kind == 1U && callsite == 0x0001d32eU)
                        || (kind == 0U && callsite == 0x0001d3aeU))
                    && target == 0x0001d3b2U)
                || (function_id == 593U && kind == 1U
                    && callsite == 0x0001d3b6U && target == 0x0001d3baU))
            && call_index_ >= fixture_.calls.size();
        if (omitted_legacy_f336_partition_marker) {
            record_compatibility_rule("omitted_legacy_f336_partition_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[function_id].entry(context);
        }

        const bool omitted_legacy_f186_partition_chain =
            (fixture_.function_id == 186U || fixture_.function_id == 187U
                || fixture_.function_id == 296U)
            && call_index_ >= fixture_.calls.size()
            && target_cpu == 1U && target_state == 0x72U
            && ((function_id == 541U && kind == 0U
                    && callsite == 0x0001611aU && target == 0x0001611eU)
                || (function_id == 585U
                    && ((kind == 0U && callsite == 0x0001611eU)
                        || (kind == 1U && callsite == 0x0001614eU))
                    && target == 0x00016122U)
                || (function_id == 586U && kind == 1U
                    && (callsite == 0x0001612aU || callsite == 0x00016132U)
                    && target == 0x00016134U)
                || (function_id == 587U
                    && ((kind == 1U && callsite == 0x0001612eU)
                        || (kind == 0U && callsite == 0x00016134U))
                    && target == 0x00016138U)
                || (function_id == 588U && kind == 1U
                    && callsite == 0x00016132U && target == 0x00016140U)
                || (function_id == 589U
                    && ((kind == 1U && callsite == 0x0001613eU)
                        || (kind == 0U && callsite == 0x00016140U))
                    && target == 0x00016142U));
        if (omitted_legacy_f186_partition_chain) {
            record_compatibility_rule("omitted_legacy_f186_partition_chain");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[function_id].entry(context);
        }

        return std::nullopt;
    }
}

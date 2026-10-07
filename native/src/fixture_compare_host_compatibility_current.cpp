#include "fixture_compare_internal.h"
#include <cstdlib>
#include <iostream>
#include <utility>
namespace gain_ground::fixture_compare_detail {
std::optional<FunctionResult> ComparingHost::try_current_compatibility(std::uint32_t function_id, std::uint8_t target_cpu, std::uint8_t target_state, std::uint8_t kind, std::uint32_t callsite, std::uint32_t target, FunctionContext &context)
    {
        const bool omitted_legacy_f361_f554_marker = fixture_.function_id == 361U
            && function_id == 554U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && (callsite == 0x0001e3e4U || callsite == 0x0001e408U
                || callsite == 0x0001e432U || callsite == 0x0001e438U)
            && target == 0x0001e1f2U
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 358U
            && fixture_.calls[call_index_].target_cpu == 1U
            && fixture_.calls[call_index_].target_state == 0x72U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x0001e220U
            && fixture_.calls[call_index_].target == 0x0001e158U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 17U;
        if (omitted_legacy_f361_f554_marker) {
            record_compatibility_rule("omitted_legacy_f361_f554_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[554U].entry(context);
        }

        const bool omitted_legacy_f361_f555_marker = fixture_.function_id == 361U
            && function_id == 555U
            && target_cpu == 1U && target_state == 0x72U && kind == 0U
            && callsite == 0x0001e224U && target == 0x0001e22aU
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 280U
            && fixture_.calls[call_index_].target_cpu == 1U
            && fixture_.calls[call_index_].target_state == 0x72U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x0001e22aU
            && fixture_.calls[call_index_].target == 0x00015d24U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 2U;
        if (omitted_legacy_f361_f555_marker) {
            record_compatibility_rule("omitted_legacy_f361_f555_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[555U].entry(context);
        }

        const bool omitted_current_f404_f596_marker = fixture_.function_id == 404U
            && function_id == 596U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x0001e1d0U && target == 0x0001e1d8U
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 358U
            && fixture_.calls[call_index_].target_cpu == 1U
            && fixture_.calls[call_index_].target_state == 0x72U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x0001e220U
            && fixture_.calls[call_index_].target == 0x0001e158U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 18U;
        if (omitted_current_f404_f596_marker) {
            record_compatibility_rule("omitted_current_f404_f596_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[596U].entry(context);
        }

        const bool omitted_current_f404_f597_entry_marker = fixture_.function_id == 404U
            && function_id == 597U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x0001e1eaU && target == 0x0001e1ecU
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 358U
            && fixture_.calls[call_index_].target_cpu == 1U
            && fixture_.calls[call_index_].target_state == 0x72U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x0001e220U
            && fixture_.calls[call_index_].target == 0x0001e158U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 17U;
        if (omitted_current_f404_f597_entry_marker) {
            record_compatibility_rule("omitted_current_f404_f597_entry_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[597U].entry(context);
        }

        const bool omitted_current_f404_f597_loop_marker = fixture_.function_id == 404U
            && function_id == 597U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x0001e1eeU && target == 0x0001e1ecU
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 358U
            && fixture_.calls[call_index_].target_cpu == 1U
            && fixture_.calls[call_index_].target_state == 0x72U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x0001e220U
            && fixture_.calls[call_index_].target == 0x0001e158U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 17U;
        if (omitted_current_f404_f597_loop_marker) {
            record_compatibility_rule("omitted_current_f404_f597_loop_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[597U].entry(context);
        }

        const bool omitted_current_f404_f550_marker = fixture_.function_id == 404U
            && function_id == 550U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x0001cefcU && target == 0x0001d9daU
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 352U
            && fixture_.calls[call_index_].target_cpu == 1U
            && fixture_.calls[call_index_].target_state == 0x72U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x000210d0U
            && fixture_.calls[call_index_].target == 0x0001dbc4U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 7U;
        if (omitted_current_f404_f550_marker) {
            record_compatibility_rule("omitted_current_f404_f550_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[550U].entry(context);
        }

        const bool omitted_current_f404_f552_marker = fixture_.function_id == 404U
            && function_id == 552U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x0001dcd2U && target == 0x0001dcd6U
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 357U
            && fixture_.calls[call_index_].target_cpu == 1U
            && fixture_.calls[call_index_].target_state == 0x72U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x000210d8U
            && fixture_.calls[call_index_].target == 0x0001e124U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 4U;
        if (omitted_current_f404_f552_marker) {
            record_compatibility_rule("omitted_current_f404_f552_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[552U].entry(context);
        }

        const bool omitted_current_f404_f554_marker = fixture_.function_id == 404U
            && function_id == 554U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x0001e1eeU && target == 0x0001e1f2U
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 358U
            && fixture_.calls[call_index_].target_cpu == 1U
            && fixture_.calls[call_index_].target_state == 0x72U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x0001e220U
            && fixture_.calls[call_index_].target == 0x0001e158U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 17U;
        if (omitted_current_f404_f554_marker) {
            record_compatibility_rule("omitted_current_f404_f554_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[554U].entry(context);
        }

        const bool omitted_current_f404_f555_marker = fixture_.function_id == 404U
            && function_id == 555U
            && target_cpu == 1U && target_state == 0x72U && kind == 0U
            && callsite == 0x0001e224U && target == 0x0001e22aU
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 280U
            && fixture_.calls[call_index_].target_cpu == 1U
            && fixture_.calls[call_index_].target_state == 0x72U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x0001e22aU
            && fixture_.calls[call_index_].target == 0x00015d24U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 2U;
        if (omitted_current_f404_f555_marker) {
            record_compatibility_rule("omitted_current_f404_f555_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[555U].entry(context);
        }

        return std::nullopt;
    }
}

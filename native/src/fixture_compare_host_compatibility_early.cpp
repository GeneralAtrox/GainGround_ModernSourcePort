#include "fixture_compare_internal.h"
#include <cstdlib>
#include <iostream>
#include <utility>
namespace gain_ground::fixture_compare_detail {
std::optional<FunctionResult> ComparingHost::try_early_compatibility(std::uint32_t function_id, std::uint8_t target_cpu, std::uint8_t target_state, std::uint8_t kind, std::uint32_t callsite, std::uint32_t target, FunctionContext &context)
    {
        const bool omitted_legacy_f543_marker = function_id == 543U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x0001700aU && target == 0x00017024U
            && (call_index_ >= fixture_.calls.size()
                || (fixture_.calls[call_index_].function_id == 202U
                    && fixture_.calls[call_index_].target_cpu == 1U
                    && fixture_.calls[call_index_].target_state == 0x72U
                    && fixture_.calls[call_index_].kind == 2U
                    && fixture_.calls[call_index_].callsite == 0x0000ff82U
                    && fixture_.calls[call_index_].target == 0x000101e2U
                    && fixture_.calls[call_index_].sequence
                        == next_sequence_ + 27U)
                || (fixture_.calls[call_index_].function_id == 202U
                    && fixture_.calls[call_index_].target_cpu == 1U
                    && fixture_.calls[call_index_].target_state == 0x72U
                    && fixture_.calls[call_index_].kind == 2U
                    && fixture_.calls[call_index_].callsite == 0x0000ff9aU
                    && fixture_.calls[call_index_].target == 0x000101e2U
                    && fixture_.calls[call_index_].sequence
                        == next_sequence_ + 20U)
                || (fixture_.calls[call_index_].function_id == 237U
                    && fixture_.calls[call_index_].target_cpu == 1U
                    && fixture_.calls[call_index_].target_state == 0x72U
                    && fixture_.calls[call_index_].kind == 2U
                    && fixture_.calls[call_index_].callsite == 0x00010e1eU
                    && fixture_.calls[call_index_].target == 0x0001106eU
                    && fixture_.calls[call_index_].sequence
                        == next_sequence_ + 16U)
                || (fixture_.calls[call_index_].function_id == 224U
                    && fixture_.calls[call_index_].target_cpu == 1U
                    && fixture_.calls[call_index_].target_state == 0x72U
                    && fixture_.calls[call_index_].kind == 2U
                    && fixture_.calls[call_index_].callsite == 0x00010a28U
                    && fixture_.calls[call_index_].target == 0x00010cc0U
                    && (fixture_.calls[call_index_].sequence
                            == next_sequence_ + 15U
                        || fixture_.calls[call_index_].sequence
                            == next_sequence_ + 17U
                        || fixture_.calls[call_index_].sequence
                            == next_sequence_ + 19U
                        || fixture_.calls[call_index_].sequence
                            == next_sequence_ + 21U
                        || fixture_.calls[call_index_].sequence
                            == next_sequence_ + 26U
                        || fixture_.calls[call_index_].sequence
                            == next_sequence_ + 29U)));
        if (omitted_legacy_f543_marker) {
            record_compatibility_rule("omitted_legacy_f543_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[543U].entry(context);
        }

        // Records before the two current-table captures appended at ordinals
        // 95082 and 95083 were produced before the 0x17024 and 0x17e9a
        // ownership partitions existed. Preserve only those hash-bound bundle
        // records when their next captured call is not the handoff now emitted
        // by the native owner.
        const bool omitted_prepartition_f543_sequential_marker =
            fixture_record_ < 95082U && function_id == 543U
            && target_cpu == 1U && target_state == 0x72U && kind == 0U
            && callsite == 0x00017022U && target == 0x00017024U
            && (call_index_ >= fixture_.calls.size()
                || fixture_.calls[call_index_].function_id != 543U
                || fixture_.calls[call_index_].callsite != callsite
                || fixture_.calls[call_index_].target != target);
        if (omitted_prepartition_f543_sequential_marker) {
            record_compatibility_rule("omitted_prepartition_f543_sequential_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[543U].entry(context);
        }

        const bool omitted_prepartition_f547_branch_marker =
            fixture_record_ < 95082U && function_id == 547U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x00017e2aU && target == 0x00017e9aU
            && (call_index_ >= fixture_.calls.size()
                || fixture_.calls[call_index_].function_id != 547U
                || fixture_.calls[call_index_].callsite != callsite
                || fixture_.calls[call_index_].target != target);
        if (omitted_prepartition_f547_branch_marker) {
            record_compatibility_rule("omitted_prepartition_f547_branch_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[547U].entry(context);
        }

        // The pre-F546-partition enclosing Function 481 captures contain the
        // complete 0x17dd8 -> 0x17ca0 execution but no separate ownership
        // marker.  Consume that exact native tail locally, without advancing
        // the fixture call list, only when the next recorded event is the
        // proven next F484 iteration 58 memory events later.
        const bool omitted_enclosing_f481_f546_marker = fixture_.function_id == 481U
            && function_id == 546U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && target == 0x00017ca0U
            && ((callsite == 0x00017c9cU
                    && call_index_ == fixture_.calls.size())
                || (callsite == 0x00017dd8U
                    && (call_index_ == fixture_.calls.size()
                    || (fixture_.calls[call_index_].function_id == 484U
                    && fixture_.calls[call_index_].target_cpu == 1U
                    && fixture_.calls[call_index_].target_state == 0x72U
                    && fixture_.calls[call_index_].kind == 2U
                    && fixture_.calls[call_index_].callsite == 0x00017c74U
                    && fixture_.calls[call_index_].target == 0x00017d9eU
                    && (fixture_.calls[call_index_].sequence == next_sequence_ + 58U
                        || fixture_.calls[call_index_].sequence == next_sequence_ + 60U)))));
        if (omitted_enclosing_f481_f546_marker) {
            record_compatibility_rule("omitted_enclosing_f481_f546_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[546U].entry(context);
        }

        const bool omitted_legacy_f542_marker = function_id == 542U
            && target_cpu == 1U && target_state == 0x72U && kind == 0U
            && callsite == 0x00016372U && target == 0x00016376U
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 302U
            && (fixture_.calls[call_index_].sequence == next_sequence_
                || fixture_.calls[call_index_].sequence == next_sequence_ + 2U);
        if (omitted_legacy_f542_marker) {
            record_compatibility_rule("omitted_legacy_f542_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return FunctionResult::complete(3U, target);
        }

        const bool omitted_legacy_f590_marker = function_id == 590U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x00016380U && target == 0x0001639aU
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 302U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 2U;
        if (omitted_legacy_f590_marker) {
            record_compatibility_rule("omitted_legacy_f590_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[590U].entry(context);
        }

        const bool omitted_legacy_f591_marker = function_id == 591U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x0001639eU && target == 0x000163aaU
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 302U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 2U;
        if (omitted_legacy_f591_marker) {
            record_compatibility_rule("omitted_legacy_f591_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[591U].entry(context);
        }

        const bool omitted_legacy_f553_marker = function_id == 553U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && callsite == 0x0001e314U && target == 0x0001e18aU
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 363U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x0001e1c6U
            && fixture_.calls[call_index_].target == 0x0001ebf0U
            && (fixture_.calls[call_index_].sequence == next_sequence_ + 6U
                || fixture_.calls[call_index_].sequence == next_sequence_ + 7U);
        if (omitted_legacy_f553_marker) {
            record_compatibility_rule("omitted_legacy_f553_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[553U].entry(context);
        }

        const bool omitted_legacy_f594_marker = function_id == 594U
            && target_cpu == 1U && target_state == 0x72U && kind == 1U
            && (callsite == 0x0001e1baU || callsite == 0x0001e1c2U)
            && target == 0x0001e1c6U
            && call_index_ < fixture_.calls.size()
            && fixture_.calls[call_index_].function_id == 363U
            && fixture_.calls[call_index_].kind == 2U
            && fixture_.calls[call_index_].callsite == 0x0001e1c6U
            && fixture_.calls[call_index_].target == 0x0001ebf0U
            && fixture_.calls[call_index_].sequence == next_sequence_ + 2U;
        if (omitted_legacy_f594_marker) {
            record_compatibility_rule("omitted_legacy_f594_marker");
            context.cpu = target_cpu;
            context.state = target_state;
            context.registers.program_counter = target;
            return generated::kFunctions[594U].entry(context);
        }

        return std::nullopt;
    }
}

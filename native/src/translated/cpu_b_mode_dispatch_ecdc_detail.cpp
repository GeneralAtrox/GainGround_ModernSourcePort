#include "cpu_b_mode_dispatch_ecdc_detail.h"

namespace gain_ground::translated::cpu_b_mode_dispatch_ecdc_detail {

bool dispatch_record_setup(FunctionContext &c, CpuRegisters &r,
    unverified::Machine &m, CpuBIrqTiming &t, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind,
    bool &returned_from_child, std::optional<FunctionResult> &outcome)
{
    switch (pc) {
        case 0xed36U: { // 3b6800060036: move-memory-to-extended-memory
            next = 0xed3cU;
            const auto source = (r.address[0] + 0x6U);
            t.prefetch(pc + 4U);
            const auto value = t.word(source);
            const auto destination = (r.address[5] + 0x36U);
            m.logic(value, 16U);
            t.prefetch(pc + 6U);
            t.word(destination, value);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed3cU: { // 3b6800080038: move-memory-to-extended-memory
            next = 0xed42U;
            const auto source = (r.address[0] + 0x8U);
            t.prefetch(pc + 4U);
            const auto value = t.word(source);
            const auto destination = (r.address[5] + 0x38U);
            m.logic(value, 16U);
            t.prefetch(pc + 6U);
            t.word(destination, value);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed42U: { // 3b68000a003a: move-memory-to-extended-memory
            next = 0xed48U;
            const auto source = (r.address[0] + 0xaU);
            t.prefetch(pc + 4U);
            const auto value = t.word(source);
            const auto destination = (r.address[5] + 0x3aU);
            m.logic(value, 16U);
            t.prefetch(pc + 6U);
            t.word(destination, value);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed48U: { // 42ad003c: clear-memory
            next = 0xed4cU;
            const auto address = (r.address[5] + 0x3cU);
            t.prefetch(pc + 4U);
            (void)t.word(address);
            (void)t.word(address + 2U);
            m.logic(0U, 32U);
            t.prefetch(pc + 6U);
            t.word(address + 2U, 0U);
            t.word(address, 0U);
            break;
        }
        case 0xed4cU: { // 1b7c0000005a: move-immediate-8-to-displacement
            next = 0xed52U;
            const auto value = 0x0U;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[5] + 0x5aU);
            t.prefetch(pc + 6U);
            m.logic(value, 8U);
            t.byte(destination, static_cast<std::uint8_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed52U: { // 1b7c0000005b: move-immediate-8-to-displacement
            next = 0xed58U;
            const auto value = 0x0U;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[5] + 0x5bU);
            t.prefetch(pc + 6U);
            m.logic(value, 8U);
            t.byte(destination, static_cast<std::uint8_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed58U: { // 3b7c00000048: move-immediate-16-to-displacement
            next = 0xed5eU;
            const auto value = 0x0U;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[5] + 0x48U);
            t.prefetch(pc + 6U);
            m.logic(value, 16U);
            t.word(destination, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed5eU: { // 3b6d00700012: move-memory-to-extended-memory
            next = 0xed64U;
            const auto source = (r.address[5] + 0x70U);
            t.prefetch(pc + 4U);
            const auto value = t.word(source);
            const auto destination = (r.address[5] + 0x12U);
            m.logic(value, 16U);
            t.prefetch(pc + 6U);
            t.word(destination, value);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed64U: { // 3b7c00080016: move-immediate-16-to-displacement
            next = 0xed6aU;
            const auto value = 0x8U;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[5] + 0x16U);
            t.prefetch(pc + 6U);
            m.logic(value, 16U);
            t.word(destination, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed6aU: { // 3b7c0180001a: move-immediate-16-to-displacement
            next = 0xed70U;
            const auto value = 0x180U;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[5] + 0x1aU);
            t.prefetch(pc + 6U);
            m.logic(value, 16U);
            t.word(destination, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed70U: { // 3b7c00060058: move-immediate-16-to-displacement
            next = 0xed76U;
            const auto value = 0x6U;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[5] + 0x58U);
            t.prefetch(pc + 6U);
            m.logic(value, 16U);
            t.word(destination, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed76U: { // 3b7c00180052: move-immediate-16-to-displacement
            next = 0xed7cU;
            const auto value = 0x18U;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[5] + 0x52U);
            t.prefetch(pc + 6U);
            m.logic(value, 16U);
            t.word(destination, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed7cU: { // 426d005c: clear-memory
            next = 0xed80U;
            const auto address = (r.address[5] + 0x5cU);
            t.prefetch(pc + 4U);
            (void)t.word(address);
            m.logic(0U, 16U);
            t.prefetch(pc + 6U);
            t.word(address, 0U);
            break;
        }
        case 0xed80U: { // 61000f14: bsr
            next = 0xed84U;
            const auto target = 0xfc96U;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::bsr);
            if (result.status != TranslationStatus::complete || result.control != 1U) {
                outcome = result;
                return true;
            }
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0xed84U: { // 41f90020220a: lea-absolute-long
            next = 0xed8aU;
            const auto value = 0x20220aU;
            r.address[0] = (r.address[0] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[0] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed8aU: { // 7000: moveq
            next = 0xed8cU;
            r.data[0] = 0x0U;
            m.logic(0x0U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xed8cU: { // 102d004b: move-to-data-register
            next = 0xed90U;
            t.prefetch(pc + 4U);
            const auto source = (r.address[5] + 0x4bU);
            const auto value = t.byte(source);
            m.logic(value, 8U);
            m.db(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xed90U: { // e940: shift-word
            next = 0xed92U;
            scene_timing::shift_word(t, m, pc, 0U, 4U, true, true);
            break;
        }
        case 0xed92U: { // 43f900026f1c: lea-absolute-long
            next = 0xed98U;
            const auto value = 0x26f1cU;
            r.address[1] = (r.address[1] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[1] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed98U: { // 30310000: move-memory-to-register
            next = 0xed9cU;
            t.clocks(2U);
            const auto source = (r.address[1] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])) + 0x0U);
            t.prefetch(pc + 4U);
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xed9cU: { // 61000eaa: bsr
            next = 0xeda0U;
            const auto target = 0xfc48U;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::bsr);
            if (result.status != TranslationStatus::complete || result.control != 1U) {
                outcome = result;
                return true;
            }
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0xeda0U: { // 323c07f0: move-to-data-register
            next = 0xeda4U;
            t.prefetch(pc + 4U);
            m.logic(0x7f0U, 16U);
            m.dw(1U, 0x7f0U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xeda4U: { // 41f90020c100: lea-absolute-long
            next = 0xedaaU;
            const auto value = 0x20c100U;
            r.address[0] = (r.address[0] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[0] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xedaaU: { // d0ed0078: adda-word-memory
            next = 0xedaeU;
            const auto address = (r.address[5] + 0x78U);
            t.prefetch(pc + 4U);
            const auto source = static_cast<std::uint32_t>(static_cast<std::int16_t>(t.word(address)));
            scene_timing::address_add(t, r, 0U, source, pc + 6U);
            break;
        }
        case 0xedaeU: { // 7037: moveq
            next = 0xedb0U;
            r.data[0] = 0x37U;
            m.logic(0x37U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xedb0U: { // 3081: move-to-indirect
            next = 0xedb2U;
            const auto destination = r.address[0];
            m.logic(r.data[1], 16U);
            t.word(destination, r.data[1]);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xedb2U: { // 5048: quick-word-address
            next = 0xedb4U;
            scene_timing::address_add(t, r, 0U, 0x8U, pc + 4U);
            break;
        }
        case 0xedb4U: { // 51c8fffa: dbf
            next = 0xedb8U;
            next = t.dbf(pc, 0xedb0U, 0U);
            if ((r.data[0] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_mode_dispatch_ecdc_detail

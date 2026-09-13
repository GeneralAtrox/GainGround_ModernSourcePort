// GENERATED original loader repair. Implemented but unverified.
// 1B994 is MOVEA.L A6,A5: pass the next actor slot, never the code address.
// Every child continuation has an explicit PC case, including 1B99A.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult cpu_b_select_and_initialize_descriptor_batch(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1b922U: { // 7000 moveq #$0, d0
            next = 0x1b924U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1b924U: { // 30380c02 move.w $c02.w, d0
            next = 0x1b928U;
            const auto source_address = 0xc02U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1b928U: { // 81fc0028 divs.w #$28, d0
            next = 0x1b92cU;
            const auto divisor = 0x28U;
            if ((divisor & 0xffffU) == 0U) {
                const auto result = m.exception(c, 5U, 0x1b928U, 0x1b92cU);
                if (result.status != TranslationStatus::complete || result.control != 2U) return result;
                next = r.program_counter;
            } else {
                m.divide(0U, divisor, true);
            }
            break;
        }
        case 0x1b92cU: { // 4840 swap d0
            next = 0x1b92eU;
            const auto value = (r.data[0] << 16U) | (r.data[0] >> 16U);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1b92eU: { // e540 asl.w #$2, d0
            next = 0x1b930U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x1b930U: { // 3200 move.w d0, d1
            next = 0x1b932U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1b932U: { // d040 add.w d0, d0
            next = 0x1b934U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1b934U: { // d041 add.w d1, d0
            next = 0x1b936U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1b936U: { // 7200 moveq #$0, d1
            next = 0x1b938U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1b938U: { // 12380821 move.b $821.w, d1
            next = 0x1b93cU;
            const auto source_address = 0x821U;
            const auto value = m.byte(source_address);
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1b93cU: { // 6702 beq.b $1b940
            next = 0x1b93eU;
            if ((r.status & 4U) != 0U) { next = 0x1b940U; transfer_kind = 1U; }
            break;
        }
        case 0x1b93eU: { // 5341 subq.w #$1, d1
            next = 0x1b940U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1b940U: { // d241 add.w d1, d1
            next = 0x1b942U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1b942U: { // d241 add.w d1, d1
            next = 0x1b944U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1b944U: { // d041 add.w d1, d0
            next = 0x1b946U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1b946U: { // 41f900027060 lea.l $27060.l, a0
            next = 0x1b94cU;
            const auto source_address = 0x27060U;
            r.address[0] = source_address;
            break;
        }
        case 0x1b94cU: { // 20700000 movea.l (a0, d0.w), a0
            next = 0x1b950U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1b950U: { // 31d80c14 move.w (a0)+, $c14.w
            next = 0x1b954U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = 0xc14U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1b954U: { // 3e18 move.w (a0)+, d7
            next = 0x1b956U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            m.dw(7U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1b956U: { // 61000044 bsr.w $1b99c
            next = 0x1b95aU;
            const auto result = m.call(c, 324U, 0x1b956U, 0x1b99cU, 0x1b95aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1b95aU: { // 7c00 moveq #$0, d6
            next = 0x1b95cU;
            r.data[6] = 0x0U;
            m.logic(r.data[6], 32U);
            break;
        }
        case 0x1b95cU: { // 4df83400 lea.l $3400.w, a6
            next = 0x1b960U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x1b960U: { // 4a47 tst.w d7
            next = 0x1b962U;
            const auto value = r.data[7];
            m.logic(value, 16U);
            break;
        }
        case 0x1b962U: { // 6b1a bmi.b $1b97e
            next = 0x1b964U;
            if ((r.status & 8U) != 0U) { next = 0x1b97eU; transfer_kind = 1U; }
            break;
        }
        case 0x1b964U: { // 3cbc8000 move.w #$8000, (a6)
            next = 0x1b968U;
            const auto value = 0x8000U;
            const auto destination_address = r.address[6];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1b968U: { // 2d580002 move.l (a0)+, $2(a6)
            next = 0x1b96cU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1b96cU: { // dcfc0080 adda.w #$80, a6
            next = 0x1b970U;
            const auto source_value = 0x80U;
            r.address[6] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1b970U: { // 61000048 bsr.w $1b9ba
            next = 0x1b974U;
            const auto result = m.call(c, 325U, 0x1b970U, 0x1b9baU, 0x1b974U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1b974U: { // dcfc0080 adda.w #$80, a6
            next = 0x1b978U;
            const auto source_value = 0x80U;
            r.address[6] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1b978U: { // 5246 addq.w #$1, d6
            next = 0x1b97aU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[6];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(6U, value);
            break;
        }
        case 0x1b97aU: { // 51cffff4 dbra d7, $1b970
            next = 0x1b97eU;
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x1b970U;
            break;
        }
        case 0x1b97eU: { // 30380c02 move.w $c02.w, d0
            next = 0x1b982U;
            const auto source_address = 0xc02U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1b982U: { // d040 add.w d0, d0
            next = 0x1b984U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1b984U: { // d040 add.w d0, d0
            next = 0x1b986U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1b986U: { // 41f900027240 lea.l $27240.l, a0
            next = 0x1b98cU;
            const auto source_address = 0x27240U;
            r.address[0] = source_address;
            break;
        }
        case 0x1b98cU: { // 20300000 move.l (a0, d0.w), d0
            next = 0x1b990U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1b990U: { // 6602 bne.b $1b994
            next = 0x1b992U;
            if ((r.status & 4U) == 0U) { next = 0x1b994U; transfer_kind = 1U; }
            break;
        }
        case 0x1b992U: { // 4e75 rts 
            next = 0x1b994U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1b992U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1b994U: { // 2a4e movea.l a6, a5
            next = 0x1b996U;
            const auto value = r.address[6];
            r.address[5] = value;
            break;
        }
        case 0x1b996U: { // 2040 movea.l d0, a0
            next = 0x1b998U;
            const auto value = r.data[0];
            r.address[0] = value;
            break;
        }
        case 0x1b998U: { // 4e90 jsr (a0)
            next = 0x1b99aU;
            const auto target_address = r.address[0];
            const auto result = m.indirect_call(c, 0x1b998U, target_address, 0x1b99aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1b99aU: { // 4e75 rts 
            next = 0x1b99cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1b99aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1b922U:
        case 0x1b924U:
        case 0x1b928U:
        case 0x1b92cU:
        case 0x1b92eU:
        case 0x1b930U:
        case 0x1b932U:
        case 0x1b934U:
        case 0x1b936U:
        case 0x1b938U:
        case 0x1b93cU:
        case 0x1b93eU:
        case 0x1b940U:
        case 0x1b942U:
        case 0x1b944U:
        case 0x1b946U:
        case 0x1b94cU:
        case 0x1b950U:
        case 0x1b954U:
        case 0x1b956U:
        case 0x1b95aU:
        case 0x1b95cU:
        case 0x1b960U:
        case 0x1b962U:
        case 0x1b964U:
        case 0x1b968U:
        case 0x1b96cU:
        case 0x1b970U:
        case 0x1b974U:
        case 0x1b978U:
        case 0x1b97aU:
        case 0x1b97eU:
        case 0x1b982U:
        case 0x1b984U:
        case 0x1b986U:
        case 0x1b98cU:
        case 0x1b990U:
        case 0x1b992U:
        case 0x1b994U:
        case 0x1b996U:
        case 0x1b998U:
        case 0x1b99aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}

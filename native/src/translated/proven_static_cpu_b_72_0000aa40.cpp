// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0000aa40(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xaa40U: { // 41f900200000 lea.l $200000.l, a0
            next = 0xaa46U;
            r.address[0] = 0x200000U;
            break;
        }
        case 0xaa46U: { // 43f88402 lea.l $8402.w, a1
            next = 0xaa4aU;
            r.address[1] = 0xffff8402U;
            break;
        }
        case 0xaa4aU: { // 323c002f move.w #$2f, d1
            next = 0xaa4eU;
            m.dw(1U, 0x2fU);
            m.logic(r.data[1], 16U);
            break;
        }
        case 0xaa4eU: { // 4258 clr.w (a0)+
            next = 0xaa50U;
            const auto destination_address = r.address[0];
            const auto value = m.word(destination_address);
            r.address[0] += 2U;
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xaa50U: { // 5048 addq.w #$8, a0
            next = 0xaa52U;
            const auto source_value = 0x8U;
            r.address[0] += source_value;
            break;
        }
        case 0xaa52U: { // 4298 clr.l (a0)+
            next = 0xaa54U;
            const auto destination_address = r.address[0];
            const auto value = m.lng(destination_address);
            r.address[0] += 4U;
            (void)value;
            m.word(destination_address + 2U, 0U);
            m.word(destination_address, (0U) >> 16U);
            m.logic(0U, 32U);
            break;
        }
        case 0xaa54U: { // 4298 clr.l (a0)+
            next = 0xaa56U;
            const auto destination_address = r.address[0];
            const auto value = m.lng(destination_address);
            r.address[0] += 4U;
            (void)value;
            m.word(destination_address + 2U, 0U);
            m.word(destination_address, (0U) >> 16U);
            m.logic(0U, 32U);
            break;
        }
        case 0xaa56U: { // 30d9 move.w (a1)+, (a0)+
            next = 0xaa58U;
            const auto source_address = r.address[1];
            const auto value = m.word(source_address);
            r.address[1] += 2U;
            const auto destination_address = r.address[0];
            m.word(destination_address, value);
            r.address[0] += 2U;
            m.logic(value, 16U);
            break;
        }
        case 0xaa58U: { // 7019 moveq #$19, d0
            next = 0xaa5aU;
            r.data[0] = 0x19U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xaa5aU: { // 20d9 move.l (a1)+, (a0)+
            next = 0xaa5cU;
            const auto source_address = r.address[1];
            const auto value = m.lng(source_address);
            r.address[1] += 4U;
            const auto destination_address = r.address[0];
            m.lng(destination_address, value);
            r.address[0] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0xaa5cU: { // 51c8fffc dbra d0, $aa5a
            next = 0xaa60U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0xaa5aU;
            break;
        }
        case 0xaa60U: { // 5848 addq.w #$4, a0
            next = 0xaa62U;
            const auto source_value = 0x4U;
            r.address[0] += source_value;
            break;
        }
        case 0xaa62U: { // 51c9ffea dbra d1, $aa4e
            next = 0xaa66U;
            m.dw(1U, r.data[1] - 1U);
            if ((r.data[1] & 0xffffU) != 0xffffU) next = 0xaa4eU;
            break;
        }
        case 0xaa66U: { // 41f900204000 lea.l $204000.l, a0
            next = 0xaa6cU;
            r.address[0] = 0x204000U;
            break;
        }
        case 0xaa6cU: { // 43f897e2 lea.l $97e2.w, a1
            next = 0xaa70U;
            r.address[1] = 0xffff97e2U;
            break;
        }
        case 0xaa70U: { // 323c002f move.w #$2f, d1
            next = 0xaa74U;
            m.dw(1U, 0x2fU);
            m.logic(r.data[1], 16U);
            break;
        }
        case 0xaa74U: { // 701e moveq #$1e, d0
            next = 0xaa76U;
            r.data[0] = 0x1eU;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xaa76U: { // 20d9 move.l (a1)+, (a0)+
            next = 0xaa78U;
            const auto source_address = r.address[1];
            const auto value = m.lng(source_address);
            r.address[1] += 4U;
            const auto destination_address = r.address[0];
            m.lng(destination_address, value);
            r.address[0] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0xaa78U: { // 51c8fffc dbra d0, $aa76
            next = 0xaa7cU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0xaa76U;
            break;
        }
        case 0xaa7cU: { // 5848 addq.w #$4, a0
            next = 0xaa7eU;
            const auto source_value = 0x4U;
            r.address[0] += source_value;
            break;
        }
        case 0xaa7eU: { // 51c9fff4 dbra d1, $aa74
            next = 0xaa82U;
            m.dw(1U, r.data[1] - 1U);
            if ((r.data[1] & 0xffffU) != 0xffffU) next = 0xaa74U;
            break;
        }
        case 0xaa82U: { // 4e75 rts 
            next = 0xaa84U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xaa82U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0xaa40U:
        case 0xaa46U:
        case 0xaa4aU:
        case 0xaa4eU:
        case 0xaa50U:
        case 0xaa52U:
        case 0xaa54U:
        case 0xaa56U:
        case 0xaa58U:
        case 0xaa5aU:
        case 0xaa5cU:
        case 0xaa60U:
        case 0xaa62U:
        case 0xaa66U:
        case 0xaa6cU:
        case 0xaa70U:
        case 0xaa74U:
        case 0xaa76U:
        case 0xaa78U:
        case 0xaa7cU:
        case 0xaa7eU:
        case 0xaa82U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}

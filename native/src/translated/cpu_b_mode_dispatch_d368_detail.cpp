#include "cpu_b_mode_dispatch_d368_detail.h"

namespace gain_ground::translated::cpu_b_mode_dispatch_d368_detail {

bool dispatch_alternate_paths(FunctionContext &, CpuRegisters &r,
    unverified::Machine &m, CpuBIrqTiming &t, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind)
{
    switch (pc) {
        case 0xd472U: { // 42787416: clear-memory
            next = 0xd476U;
            const auto address = 0x7416U;
            t.prefetch(pc + 4U);
            (void)t.word(address);
            m.logic(0U, 16U);
            t.prefetch(pc + 6U);
            t.word(address, 0U);
            break;
        }
        case 0xd476U: { // 42787420: clear-memory
            next = 0xd47aU;
            const auto address = 0x7420U;
            t.prefetch(pc + 4U);
            (void)t.word(address);
            m.logic(0U, 16U);
            t.prefetch(pc + 6U);
            t.word(address, 0U);
            break;
        }
        case 0xd47aU: { // 4278742a: clear-memory
            next = 0xd47eU;
            const auto address = 0x742aU;
            t.prefetch(pc + 4U);
            (void)t.word(address);
            m.logic(0U, 16U);
            t.prefetch(pc + 6U);
            t.word(address, 0U);
            break;
        }
        case 0xd47eU: { // 42787434: clear-memory
            next = 0xd482U;
            const auto address = 0x7434U;
            t.prefetch(pc + 4U);
            (void)t.word(address);
            m.logic(0U, 16U);
            t.prefetch(pc + 6U);
            t.word(address, 0U);
            break;
        }
        case 0xd482U: { // 42b8082e: clear-memory
            next = 0xd486U;
            const auto address = 0x82eU;
            t.prefetch(pc + 4U);
            (void)t.word(address);
            (void)t.word(address + 2U);
            m.logic(0U, 32U);
            t.prefetch(pc + 6U);
            t.word(address + 2U, 0U);
            t.word(address, 0U);
            break;
        }
        case 0xd486U: { // 42380403: clear-memory
            next = 0xd48aU;
            const auto address = 0x403U;
            t.prefetch(pc + 4U);
            (void)t.byte(address);
            m.logic(0U, 8U);
            t.prefetch(pc + 6U);
            t.byte(address, 0U);
            break;
        }
        case 0xd48aU: { // 41f87b6c: lea
            next = 0xd48eU;
            r.address[0] = 0x7b6cU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd48eU: { // 4298: clear-memory
            next = 0xd490U;
            const auto address = r.address[0];
            (void)t.word(address);
            r.address[0] += 4U;
            (void)t.word(address + 2U);
            m.logic(0U, 32U);
            t.prefetch(pc + 4U);
            t.word(address + 2U, 0U);
            t.word(address, 0U);
            break;
        }
        case 0xd490U: { // 4298: clear-memory
            next = 0xd492U;
            const auto address = r.address[0];
            (void)t.word(address);
            r.address[0] += 4U;
            (void)t.word(address + 2U);
            m.logic(0U, 32U);
            t.prefetch(pc + 4U);
            t.word(address + 2U, 0U);
            t.word(address, 0U);
            break;
        }
        case 0xd492U: { // 4298: clear-memory
            next = 0xd494U;
            const auto address = r.address[0];
            (void)t.word(address);
            r.address[0] += 4U;
            (void)t.word(address + 2U);
            m.logic(0U, 32U);
            t.prefetch(pc + 4U);
            t.word(address + 2U, 0U);
            t.word(address, 0U);
            break;
        }
        case 0xd494U: { // 2b7c0000d4ae0002: move-long-register-or-immediate-to-memory
            next = 0xd49cU;
            const auto value = 0xd4aeU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = (r.address[5] + 0x2U);
            t.prefetch(pc + 8U);
            scene_timing::nz_high(r, static_cast<std::uint16_t>(value >> 16U));
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); t.word(destination + 2U, static_cast<std::uint16_t>(value));
            m.logic(value, 32U);
            t.prefetch(pc + 10U);
            break;
        }
        case 0xd49cU: { // 3b7c02580018: move-immediate-16-to-displacement
            next = 0xd4a2U;
            const auto value = 0x258U;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[5] + 0x18U);
            t.prefetch(pc + 6U);
            m.logic(value, 16U);
            t.word(destination, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd4a2U: { // 30380c02: move-to-data-register
            next = 0xd4a6U;
            t.prefetch(pc + 4U);
            const auto source = 0xc02U;
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd4a6U: { // c0fc0708: multiply-unsigned-immediate
            next = 0xd4aaU;
            scene_timing::multiply_unsigned(t, m, pc, 0U, 0x708U);
            break;
        }
        case 0xd4aaU: { // 3b400026: move-to-displacement
            next = 0xd4aeU;
            const auto destination = (r.address[5] + 0x26U);
            t.prefetch(pc + 4U);
            m.logic(r.data[0], 16U);
            t.word(destination, r.data[0]);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd52eU: { // 2b7c0000d54e0002: move-long-register-or-immediate-to-memory
            next = 0xd536U;
            const auto value = 0xd54eU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = (r.address[5] + 0x2U);
            t.prefetch(pc + 8U);
            scene_timing::nz_high(r, static_cast<std::uint16_t>(value >> 16U));
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); t.word(destination + 2U, static_cast<std::uint16_t>(value));
            m.logic(value, 32U);
            t.prefetch(pc + 10U);
            break;
        }
        case 0xd536U: { // 41f80c1a: lea
            next = 0xd53aU;
            r.address[0] = 0xc1aU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd53aU: { // 7003: moveq
            next = 0xd53cU;
            r.data[0] = 0x3U;
            m.logic(0x3U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xd53cU: { // 317c00010000: move-immediate-16-to-displacement
            next = 0xd542U;
            const auto value = 0x1U;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[0] + 0x0U);
            t.prefetch(pc + 6U);
            m.logic(value, 16U);
            t.word(destination, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd542U: { // 5048: quick-word-address
            next = 0xd544U;
            scene_timing::address_add(t, r, 0U, 0x8U, pc + 4U);
            break;
        }
        case 0xd544U: { // 51c8fff6: dbf
            next = 0xd548U;
            next = t.dbf(pc, 0xd53cU, 0U);
            if ((r.data[0] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0xd548U: { // 3b7c001e0018: move-immediate-16-to-displacement
            next = 0xd54eU;
            const auto value = 0x1eU;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[5] + 0x18U);
            t.prefetch(pc + 6U);
            m.logic(value, 16U);
            t.word(destination, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd5e4U: { // 11f8041a0403: move-memory-to-extended-memory
            next = 0xd5eaU;
            const auto source = 0x41aU;
            t.prefetch(pc + 4U);
            const auto value = t.byte(source);
            const auto destination = 0x403U;
            m.logic(value, 8U);
            t.prefetch(pc + 6U);
            t.byte(destination, value);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd5eaU: { // 3b7c001e0018: move-immediate-16-to-displacement
            next = 0xd5f0U;
            const auto value = 0x1eU;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[5] + 0x18U);
            t.prefetch(pc + 6U);
            m.logic(value, 16U);
            t.word(destination, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd5f0U: { // 41fa0008: lea
            next = 0xd5f4U;
            r.address[0] = 0xd5faU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd5f4U: { // 2b480002: move-long-register-or-immediate-to-memory
            next = 0xd5f8U;
            const auto value = r.address[0];
            const auto destination = (r.address[5] + 0x2U);
            t.prefetch(pc + 4U);
            scene_timing::nz_high(r, static_cast<std::uint16_t>(value >> 16U));
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); t.word(destination + 2U, static_cast<std::uint16_t>(value));
            m.logic(value, 32U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd5f8U: { // 4e71: nop
            next = 0xd5faU;
            t.prefetch(pc + 4U);
            break;
        }
        default:
            return false;
    }
    return true;
}

} // namespace gain_ground::translated::cpu_b_mode_dispatch_d368_detail

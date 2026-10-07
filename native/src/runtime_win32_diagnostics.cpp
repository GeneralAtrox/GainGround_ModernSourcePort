#include "runtime_win32_internal.h"
namespace runtime_win32_detail {
std::uint16_t RuntimeWindow::stage_index() const{
        const auto bytes = host.region_bytes(2U);
        return bytes.size() > 0xc03U ? static_cast<std::uint16_t>((bytes[0xc02U] << 8U) | bytes[0xc03U]) : 0U;
    }

void RuntimeWindow::start_sweep(){
        const auto keep = sweep;
        sweep = Sweep{};
        sweep.frames_per_cell = keep.frames_per_cell;
        sweep.min_x = keep.min_x; sweep.max_x = keep.max_x; sweep.min_y = keep.min_y; sweep.max_y = keep.max_y;
        sweep.active = true;
        sweep.stage = stage_index();
        diag_log("sweep start stage %u record %05x", unsigned(sweep.stage), unsigned(host.player_record(0)));
    }

void RuntimeWindow::stop_sweep(const char *why){
        if (!sweep.active) return;
        sweep.active = false;
        diag_log("sweep %s stage %u teleports %u skipped %u", why, unsigned(sweep.stage), sweep.teleports, sweep.skipped);
    }

bool RuntimeWindow::cell_clear(int x, int y) const{
        const auto shared = host.region_bytes(3U);
        if (shared.size() < 0x3bb20U || x - 10 < 0 || x + 20 > 383 || y - 9 < 0 || y + 18 > 495) return false;
        for (int px : {x, x + 20})
            for (int py : {y, y + 18})
                if (shared[0x3af22U + static_cast<std::size_t>(px / 8) * 64U + static_cast<std::size_t>((495 - py) / 8)] != 0U) return false; // any attribute: solid, exit, hazard
        return true;
    }

void RuntimeWindow::hold_stage_clock(){
        host.write_memory_word(2U, 0xd2cU, 0U, 0x00ffU);
    }

void RuntimeWindow::sweep_step(){
        if (host.faulted()) return;
        // After a sweep the clock has long expired underneath; keep holding the
        // time-up until the harness moves the game to another stage.
        if (!sweep.active) { if (sweep.teleports && stage_index() == sweep.stage) hold_stage_clock(); return; }
        if (stage_index() != sweep.stage) { stop_sweep("left stage during"); return; }
        hold_stage_clock();
        if (++sweep.frames < sweep.frames_per_cell) return;
        sweep.frames = 0;
        for (;;) {
            if (++sweep.cell >= Sweep::width * Sweep::height) { stop_sweep("done"); return; }
            const int x = (sweep.cell % Sweep::width) * Sweep::step + 4;
            const int y = (sweep.cell / Sweep::width) * Sweep::step + 4;
            if (x < sweep.min_x || x > sweep.max_x || y < sweep.min_y || y > sweep.max_y || !cell_clear(x, y)) { ++sweep.skipped; continue; }
            {
                // Where the game left the player since the previous placement, and its lifecycle state.
                const auto bytes = host.region_bytes(2U); const auto rec = host.player_record(0U);
                if (rec && rec + 0x70U < bytes.size())
                    diag_log("sweep before frame %llu at %d,%d active %02x mode %04x +3f %02x +44 %04x +46 %04x",
                             static_cast<unsigned long long>(devices.frame()),
                             int(std::int16_t((bytes[rec + 0x12U] << 8) | bytes[rec + 0x13U])), int(std::int16_t((bytes[rec + 0x1aU] << 8) | bytes[rec + 0x1bU])),
                             unsigned(bytes[rec]), (unsigned(bytes[rec + 0x44U]) << 8) | bytes[rec + 0x45U], unsigned(bytes[rec + 0x3fU]),
                             (unsigned(bytes[rec + 0x44U]) << 8) | bytes[rec + 0x45U], (unsigned(bytes[rec + 0x46U]) << 8) | bytes[rec + 0x47U]);
            }
            if (!host.teleport_player(0U, x, y)) { stop_sweep("no player record during"); return; }
            ++sweep.teleports;
            diag_log("sweep teleport frame %llu cell %d x %d y %d", static_cast<unsigned long long>(devices.frame()), sweep.cell, x, y);
            return;
        }
    }
}

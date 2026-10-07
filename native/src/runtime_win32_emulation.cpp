#include "runtime_win32_internal.h"
namespace runtime_win32_detail {
void RuntimeWindow::post(std::function<void()> command){
        { std::lock_guard lock(commands_lock); commands.push_back(std::move(command)); }
        SetEvent(wake);
    }

void RuntimeWindow::run_commands(){
        std::vector<std::function<void()>> pending;
        { std::lock_guard lock(commands_lock); pending.swap(commands); }
        for (auto &command : pending) command();
    }

void RuntimeWindow::stop_emulation(){
        if (!emulation.joinable()) return;
        stopping = true;
        SetEvent(wake);
        emulation.join();
    }

double RuntimeWindow::busy_percent(){
        FILETIME created{}, exited{}, kernel{}, user{};
        if (!GetThreadTimes(GetCurrentThread(), &created, &exited, &kernel, &user)) return -1.0;
        const auto cpu = (std::uint64_t(kernel.dwHighDateTime) << 32U | kernel.dwLowDateTime) +
                         (std::uint64_t(user.dwHighDateTime) << 32U | user.dwLowDateTime);
        const auto now = std::chrono::steady_clock::now();
        const double wall = std::chrono::duration<double>(now - busy_wall).count();
        const double percent = wall > 0.0 ? double(cpu - busy_cpu_100ns) / 1e5 / wall : 0.0;
        busy_cpu_100ns = cpu;
        busy_wall = now;
        return percent;
    }

void RuntimeWindow::emulation_main(){
        gain_ground::HostSampler sampler;
        if (!sample_path.empty()) sampler.start(sample_path);
        // Audio is produced here, so busy background work must not preempt it,
        // and a hybrid CPU must not park it on an efficiency core.
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
        THREAD_POWER_THROTTLING_STATE throttling{THREAD_POWER_THROTTLING_CURRENT_VERSION, THREAD_POWER_THROTTLING_EXECUTION_SPEED, 0U};
        SetThreadInformation(GetCurrentThread(), ThreadPowerThrottling, &throttling, sizeof(throttling));
        scheduler_fiber = ConvertThreadToFiberEx(nullptr, FIBER_FLAG_FLOAT_SWITCH);
        if (scheduler_fiber)
            game_fiber = CreateFiberEx(0, 8U * 1024U * 1024U, FIBER_FLAG_FLOAT_SWITCH, &RuntimeWindow::execute, this);
        if (!game_fiber) error = L"Cannot create the native game continuation";
        host.set_checkpoint(&RuntimeWindow::checkpoint, this);
        // Scheduling only; device clocks use their own source frequencies.
        auto timer = CreateWaitableTimerExW(nullptr, nullptr, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS);
        const bool coarse = !timer; // Before Windows 10 1803: raise the system timer resolution instead.
        if (coarse) { timeBeginPeriod(1U); timer = CreateWaitableTimerW(nullptr, FALSE, nullptr); }
        LARGE_INTEGER due{};
        due.QuadPart = -10000LL * kSliceMs;
        if (timer && !SetWaitableTimer(timer, &due, kSliceMs, nullptr, nullptr, FALSE)) { CloseHandle(timer); timer = nullptr; }
        epoch = std::chrono::steady_clock::now();
        publish_state();
        auto woke = std::chrono::steady_clock::now();
        while (!stopping) {
            const HANDLE handles[]{wake, timer};
            WaitForMultipleObjects(timer ? 2U : 1U, handles, FALSE, timer ? INFINITE : DWORD(kSliceMs));
            const auto now = std::chrono::steady_clock::now();
            max_gap_ms = std::max(max_gap_ms, std::chrono::duration<double, std::milli>(now - woke).count());
            woke = now;
            run_commands();
            if (stopping) break;
            poll_controllers();
            advance();
            save_failure();
            if (host.take_start_stage_applied()) select_stage(-1);
            publish_frame();
        }
        if (timer) { CancelWaitableTimer(timer); CloseHandle(timer); }
        if (coarse) timeEndPeriod(1U);
        if (game_fiber) { DeleteFiber(game_fiber); game_fiber = nullptr; }
        if (scheduler_fiber) { ConvertFiberToThread(); scheduler_fiber = nullptr; }
        sampler.stop();
    }

void RuntimeWindow::publish_frame(){
        const auto stopped = host.faulted() || !error.empty() ? status() : std::wstring{};
        if (!frame_dirty && stopped == published_status) return;
        {
            std::lock_guard lock(shared_lock);
            if (frame_dirty) shared.pixels = video.pixels();
            shared.status = stopped;
        }
        frame_dirty = false;
        published_status = stopped;
        InvalidateRect(window, nullptr, FALSE);
    }

void RuntimeWindow::publish_state(){
        {
            std::lock_guard lock(shared_lock);
            shared.paused = paused;
            shared.unlimited_credits = host.unlimited_credits();
            shared.selected_stage = selected_stage;
        }
        PostMessageW(window, kStateMessage, 0, 0);
    }

void RuntimeWindow::record_input(unsigned key, bool pressed){
        input_history.push_back({emulated_ns, devices.frame(), key, pressed});
    }

void RuntimeWindow::apply_bindings(const gain_ground::RuntimeBindings &chosen){
        release_input();
        keyboard.set_bindings(chosen.keys);
        for(auto &pad:controllers.pads)pad.set_bindings(chosen.pad);
    }

void RuntimeWindow::release_input(){
        keyboard.release_all(devices);
        for(auto &pad:controllers.pads)pad.release();
        record_input(0U,false);
    }

void RuntimeWindow::poll_controllers(){
        const bool focused=GetForegroundWindow()==window;
        const auto now=GetTickCount64();bool pause=false;
        for(unsigned p=0;p<3;++p){
            gain_ground::RuntimePadState sample;
            if(controllers.get && now>=controllers.retry_at[p]){
                XINPUT_STATE state{};
                if(controllers.get(p,&state)==ERROR_SUCCESS)
                    sample={true,state.Gamepad.wButtons,state.Gamepad.sThumbLX,state.Gamepad.sThumbLY};
                else controllers.retry_at[p]=now+1000;
            }
            pause|=controllers.pads[p].update(sample,focused,!paused,[&](unsigned bit,bool pressed){
                const auto key=gain_ground::RuntimeKeyboard::gamepad_key(p,bit);
                keyboard.key(devices,key,pressed);record_input(key,pressed);
            });
        }
        if(pause)toggle_pause();
    }

void RuntimeWindow::checkpoint(void *argument){
        auto &app = *static_cast<RuntimeWindow *>(argument);
        if (GetCurrentFiber() != app.game_fiber) return;
        if (app.host.faulted() || app.devices.time_ns() >= app.yield_at_ns ||
            (!app.host.timed_execution() && app.host.operations_since_switch() >= 65536U))
            SwitchToFiber(app.scheduler_fiber);
    }

void WINAPI RuntimeWindow::execute(void *argument){
        auto &app = *static_cast<RuntimeWindow *>(argument);
        if (!app.game.start(app.assets)) {
            if (!app.host.faulted()) app.error = L"Native startup failed";
        } else {
            while (!app.host.faulted()) {
                if (!app.game.frame()) {
                    if (!app.host.faulted()) app.error = L"Native frame failed";
                    break;
                }
            }
        }
        // The scheduler never resumes a failed continuation.
        for (;;) SwitchToFiber(app.scheduler_fiber);
    }

void RuntimeWindow::advance(){
        if (paused || !error.empty() || host.faulted()) return;
        if (!title_started) {
            // Approved replacement loading sequence: display the original logo
            // resources for two seconds, then enter direct-loaded game startup.
            if (std::chrono::steady_clock::now() - epoch < std::chrono::seconds(2)) return;
            title_started = true;
            epoch = std::chrono::steady_clock::now();
        }
        auto target_ns = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - epoch).count() * speed);
        if (speed <= 1.0 && target_ns > emulated_ns + kMaxLagNs) {
            max_lag_ms = std::max(max_lag_ms, double(target_ns - emulated_ns) / 1e6);
            epoch += std::chrono::nanoseconds(static_cast<long long>(double(target_ns - emulated_ns - kMaxLagNs) / speed));
            target_ns = emulated_ns + kMaxLagNs;
            ++slipped;
        }
        // Allow enough work to catch up between UI timer messages (~16 ms).
        // This is a work ceiling, not a delay: stop at the wall-clock target.
        // Above real speed the same ceiling scales with the speed, so a tick may
        // run several emulated frames; the window only needs to stay responsive.
        const auto host_deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(static_cast<long long>(20 * std::max(1.0, speed)));
        yield_at_ns = target_ns;
        while (emulated_ns < target_ns && !host.faulted()) {
        host.reset_yield_window();
        SwitchToFiber(game_fiber);
        if (!error.empty() || host.faulted()) return;
        emulated_ns = devices.time_ns();
        if (last_frame != devices.frame()) {
            const auto now = std::chrono::steady_clock::now();
            if (speed <= 1.0 || now - last_render >= std::chrono::milliseconds(16)) {
                video.render(host);
                last_render = now;
                frame_dirty = true;
            }
            last_frame = devices.frame();
            sweep_step();
            // Determinism probe for performance work: the execution checkpoint
            // count at fixed frames must not move when only host code changes.
            if (diag && last_frame % 600U == 0U) {
                diag_log("progress frame %llu checkpoints %llu updates %llu depth %zu/%zu max %zu/%zu audio min %.1f max %.1f ms starved %u dropped %u gap %.1f lag %.1f ms slipped %u busy %.0f%%", static_cast<unsigned long long>(last_frame),
                         static_cast<unsigned long long>(host.execution_checkpoints()), static_cast<unsigned long long>(game.updates()),
                         host.call_depth(0U), host.call_depth(1U), host.max_call_depth(0U), host.max_call_depth(1U),
                         output.min_queued_ms, output.max_queued_ms, output.starved, output.dropped,
                         max_gap_ms, max_lag_ms, slipped, busy_percent());
                max_gap_ms = max_lag_ms = 0.0;
                output.min_queued_ms = output.max_queued_ms = -1.0;
            }
        }
        if (std::chrono::steady_clock::now() >= host_deadline) break;
        }
        if (target_ns > emulated_ns) max_lag_ms = std::max(max_lag_ms, double(target_ns - emulated_ns) / 1e6);
        auto samples = devices.audio.take_samples();
        if (speed > 1.0) return; // Faster than real time: keep the mixer drained, play nothing.
        const auto starved = output.starved;
        if (!output.submit(std::move(samples))) { error = L"Audio output failed"; return; }
        if (diag && output.starved != starved)
            diag_log("audio starved frame %llu lag %.1f ms slipped %u", static_cast<unsigned long long>(last_frame),
                     target_ns > emulated_ns ? double(target_ns - emulated_ns) / 1e6 : 0.0, slipped);
    }
}

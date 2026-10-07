cmake_minimum_required(VERSION 3.24)
project(gain_ground_native LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(ZLIB REQUIRED)
option(GAIN_GROUND_RESEARCH_TESTS "Register tests requiring privately captured ROM data" OFF)
set(GAIN_GROUND_CHARACTER_SNAPSHOT "" CACHE PATH "Private runtime failure directory for all-character smoke tests")

if(MSVC)
    add_compile_options(/MP /utf-8)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/generated/gground_implementation_sources.cmake)

add_library(gain_ground_contracts STATIC
    src/asset_checkpoint_writer.cpp
    src/checkpoint_writer.cpp
    src/fixture_bundle.cpp
    src/fixture_compare.cpp
    src/fixture_compare_host_memory.cpp
    src/fixture_compare_host_hardware.cpp
    src/fixture_compare_host_interrupts.cpp
    src/fixture_compare_host_calls.cpp
    src/fixture_compare_host_compatibility_early.cpp
    src/fixture_compare_host_compatibility_current.cpp
    src/fixture_compare_host_compatibility_partition.cpp
    src/fixture_compare_host_finish.cpp
    generated/gground_fixture_marker_compatibility.cpp
    generated/gground_function_stubs.cpp
    generated/gground_stage_callbacks.cpp
    generated/gground_enemy_definitions.cpp
    src/gameplay/enemy.cpp
    src/translated/cpu_b_state72_default_trap_return.cpp
    src/translated/cpu_b_irq_redirect_wait.cpp
    src/translated/cpu_b_stage_object_callbacks.cpp
    src/translated/cpu_b_stage_gap_callbacks.cpp
    src/translated/cpu_b_initials_entry.cpp
    src/translated/cpu_b_initials_entry_region_f094_f0ff.cpp
    src/translated/cpu_b_initials_entry_region_f100_f1ff.cpp
    src/translated/cpu_b_initials_entry_region_f200_f2ff.cpp
    src/translated/cpu_b_initials_entry_region_f300_f3aa.cpp
    src/translated/cpu_b_72_000121f4_descriptor_setup.cpp
    src/translated/cpu_b_72_000121f4_record_fields.cpp
    src/translated/cpu_b_72_000121f4_tail_fields.cpp
    src/translated/cpu_b_advance_phase_callback_region_d734_d7d2.cpp
    src/translated/cpu_b_advance_phase_callback_region_d7d4_d85e.cpp
    src/translated/cpu_b_advance_phase_callback_region_d862_d8d4.cpp
    src/translated/cpu_b_advance_phase_callback_region_d8da_d952.cpp
    src/translated/cpu_b_advance_phase_callback_region_d958_d9e8.cpp
    src/translated/cpu_b_advance_phase_callback_region_d9ea_da8e.cpp
    src/translated/cpu_b_advance_phase_callback_region_da92_db0a.cpp
    src/translated/cpu_b_advance_phase_callback_region_db10_db9e.cpp
    src/translated/cpu_b_advance_phase_palette_lookup_setup.cpp
    src/translated/cpu_b_advance_phase_palette_overlay_updates.cpp
    src/translated/cpu_b_build_sprite_lists_range_016d16_016d8a.cpp
    src/translated/cpu_b_build_sprite_lists_range_016d8c_016df2.cpp
    src/translated/cpu_b_build_sprite_lists_range_016df6_016e60.cpp
    src/translated/cpu_b_build_sprite_lists_range_016e66_016ed2.cpp
    src/translated/cpu_b_build_sprite_lists_range_016ed6_016f3c.cpp
    src/translated/cpu_b_build_sprite_lists_range_016f40_016f6a.cpp
    src/translated/cpu_b_callback_state_table_region_ee9e_ef26.cpp
    src/translated/cpu_b_callback_state_table_region_ef30_ef90.cpp
    src/translated/cpu_b_callback_state_table_region_ef96_f04e.cpp
    src/translated/cpu_b_callback_state_table_region_f05a_f08c.cpp
    src/translated/cpu_b_callback_state_table_region_f23e_f25e.cpp
    src/translated/cpu_b_state72_record_region_f3c0_f58c.cpp
    src/translated/cpu_b_state72_record_region_f590_f608.cpp
    src/translated/cpu_b_state72_record_region_f60c_f69e.cpp
    src/translated/cpu_b_state72_record_region_f6a4_f728.cpp
    src/translated/cpu_b_state72_record_region_f72e_f7d4.cpp
    src/translated/cpu_b_state72_record_region_f7d8_f86e.cpp
    src/translated/cpu_b_state72_record_region_f870_f90c.cpp
    src/translated/cpu_b_state72_record_region_f910_fe3a.cpp
    src/translated/cpu_b_advance_phase_callback_dispatch.cpp
    src/translated/cpu_b_advance_phase_palette_overlay_dispatch.cpp
    src/translated/cpu_b_build_sprite_lists.cpp
    src/translated/cpu_b_callback_state_table_dispatch.cpp
    src/translated/cpu_b_compute_record_deltas_detail.cpp
    src/translated/cpu_b_compute_record_deltas.cpp
    src/translated/cpu_b_dispatch_state_step.cpp
    src/translated/cpu_b_dispatch_state_step_detail.cpp
    src/translated/cpu_b_mode_dispatch_d368_detail.cpp
    src/translated/cpu_b_mode_dispatch_ecdc_detail.cpp
    src/translated/cpu_b_mode_dispatch_d368.cpp
    src/translated/cpu_b_mode_dispatch_ecdc.cpp
    src/translated/cpu_b_record_flag1_gate_detail.cpp
    src/translated/cpu_b_record_flag1_gate_variant_detail.cpp
    src/translated/cpu_b_record_flag1_gate.cpp
    src/translated/cpu_b_record_flag1_gate_variant.cpp
    src/translated/cpu_b_state72_dispatch_record_phase.cpp
    ${GAIN_GROUND_IMPLEMENTATION_SOURCES}
)
target_include_directories(gain_ground_contracts
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/generated
)
target_link_libraries(gain_ground_contracts PUBLIC ZLIB::ZLIB)

add_library(gain_ground_ymfm STATIC third_party/ymfm/ymfm_opm.cpp)
target_include_directories(gain_ground_ymfm SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/ymfm)
add_library(gain_ground_runtime_host STATIC
    src/runtime_host.cpp
    src/native_game_loop.cpp
    src/runtime_host_boot.cpp
    src/runtime_host_clock.cpp
    src/runtime_host_execution.cpp
    src/runtime_host_gameplay.cpp
    src/runtime_host_hardware.cpp
    src/runtime_host_memory.cpp
    src/direct_asset_loader.cpp
    src/direct_boot_audio.cpp
    src/system24_video.cpp
    src/system24_audio.cpp
    src/system24_devices.cpp
    src/rom_archive.cpp
    src/rom_import.cpp
)
target_sources(gain_ground_runtime_host PRIVATE
    src/gameplay/character.cpp
    src/gameplay/projectile.cpp
    src/gameplay/character_definition.cpp
    src/gameplay/legacy_gameplay_bridge.cpp
    src/gameplay/legacy_enemy_bridge.cpp
    src/gameplay/enemy_navigation.cpp
    src/gameplay/legacy_enemy_navigation.cpp
    src/gameplay/runtime_game_definitions.cpp
    src/gameplay/legacy_character_movement.cpp
    src/gameplay/legacy_character_collision.cpp
    src/gameplay/legacy_character_exit.cpp
    src/gameplay/legacy_character_damage.cpp
    src/gameplay/legacy_projectile.cpp
    src/gameplay/legacy_attack_phase.cpp
)
target_link_libraries(gain_ground_runtime_host PUBLIC gain_ground_ymfm)
target_link_libraries(gain_ground_runtime_host PUBLIC gain_ground_contracts)
if(MSVC)
    target_compile_options(gain_ground_runtime_host PRIVATE /W4 /permissive-)
else()
    target_compile_options(gain_ground_runtime_host PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

if(WIN32)
    target_sources(gain_ground_runtime_host PRIVATE src/rom_hash_win32.cpp)
    target_link_libraries(gain_ground_runtime_host PUBLIC bcrypt)
    add_executable(gain_ground_rom_import tests/rom_import_main.cpp)
    target_link_libraries(gain_ground_rom_import PRIVATE gain_ground_runtime_host)
    add_executable(gain_ground_runtime WIN32
        src/runtime_win32.cpp
        src/runtime_win32_controls.cpp
        src/runtime_win32_emulation.cpp
        src/runtime_win32_input.cpp
        src/runtime_win32_diagnostics.cpp
        src/runtime_win32_presentation.cpp
        src/runtime_startup_win32.cpp
        src/host_sampler_win32.cpp
    )
    target_link_libraries(gain_ground_runtime PRIVATE gain_ground_runtime_host user32 gdi32 shell32 bcrypt winmm comdlg32 ole32 uuid psapi dbghelp)
    target_compile_definitions(gain_ground_runtime PRIVATE UNICODE _UNICODE)
    if(MINGW)
        target_link_options(gain_ground_runtime PRIVATE -municode)
    endif()
    if(MSVC)
        target_compile_options(gain_ground_runtime PRIVATE /W4 /permissive-)
    else()
        target_compile_options(gain_ground_runtime PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
endif()

if(MSVC)
    target_compile_options(gain_ground_contracts PRIVATE /W4 /permissive-)
else()
    target_compile_options(gain_ground_contracts PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_executable(gain_ground_fixture_smoke tests/fixture_bundle_smoke.cpp)
add_executable(gain_ground_native_loop_probe tests/native_loop_probe.cpp)
target_link_libraries(gain_ground_native_loop_probe PRIVATE gain_ground_runtime_host)
if(WIN32)
    target_link_libraries(gain_ground_native_loop_probe PRIVATE winmm)
endif()
add_executable(gain_ground_native_loop_depth tests/native_loop_depth.cpp)
add_executable(gain_ground_native_service_clock tests/native_service_clock.cpp)
target_link_libraries(gain_ground_native_service_clock PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_native_service_clock COMMAND gain_ground_native_service_clock)
target_link_libraries(gain_ground_native_loop_depth PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_native_loop_depth COMMAND gain_ground_native_loop_depth)
set_tests_properties(gain_ground_native_loop_depth PROPERTIES LABELS "native-child-boundary")
target_link_libraries(gain_ground_fixture_smoke PRIVATE gain_ground_contracts)
if(MSVC)
    target_compile_options(gain_ground_fixture_smoke PRIVATE /W4 /permissive-)
else()
    target_compile_options(gain_ground_fixture_smoke PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_executable(gain_ground_fixture_compare src/fixture_compare_main.cpp)
target_link_libraries(gain_ground_fixture_compare PRIVATE gain_ground_contracts)
if(MSVC)
    target_compile_options(gain_ground_fixture_compare PRIVATE /W4 /permissive-)
else()
    target_compile_options(gain_ground_fixture_compare PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_executable(gain_ground_checkpoint_writer_smoke tests/checkpoint_writer_smoke.cpp)
target_link_libraries(gain_ground_checkpoint_writer_smoke PRIVATE gain_ground_contracts)
if(MSVC)
    target_compile_options(gain_ground_checkpoint_writer_smoke PRIVATE /W4 /permissive-)
else()
    target_compile_options(gain_ground_checkpoint_writer_smoke PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_executable(gain_ground_asset_checkpoint_writer_smoke tests/asset_checkpoint_writer_smoke.cpp)
target_link_libraries(gain_ground_asset_checkpoint_writer_smoke PRIVATE gain_ground_contracts)
if(MSVC)
    target_compile_options(gain_ground_asset_checkpoint_writer_smoke PRIVATE /W4 /permissive-)
else()
    target_compile_options(gain_ground_asset_checkpoint_writer_smoke PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_executable(gain_ground_function34_carry_path tests/function34_carry_path.cpp)
target_link_libraries(gain_ground_function34_carry_path PRIVATE gain_ground_contracts)
if(MSVC)
    target_compile_options(gain_ground_function34_carry_path PRIVATE /W4 /permissive-)
else()
    target_compile_options(gain_ground_function34_carry_path PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_executable(gain_ground_fixture_marker_compatibility_test
    tests/fixture_marker_compatibility.cpp)
target_link_libraries(gain_ground_fixture_marker_compatibility_test PRIVATE gain_ground_contracts)
if(MSVC)
    target_compile_options(gain_ground_fixture_marker_compatibility_test PRIVATE /W4 /permissive-)
else()
    target_compile_options(gain_ground_fixture_marker_compatibility_test PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

enable_testing()
add_test(NAME gain_ground_source_line_limit
    COMMAND ${CMAKE_COMMAND} -DSOURCE_ROOT=${CMAKE_CURRENT_SOURCE_DIR}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/CheckSourceLineLimit.cmake)
add_executable(gain_ground_video_transparency tests/video_transparency.cpp)
target_link_libraries(gain_ground_video_transparency PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_video_transparency COMMAND gain_ground_video_transparency)
set_tests_properties(gain_ground_video_transparency PROPERTIES TIMEOUT 10)
add_executable(gain_ground_title_music tests/title_music.cpp src/system24_audio.cpp)
target_include_directories(gain_ground_title_music PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(gain_ground_title_music PRIVATE gain_ground_ymfm)
add_test(NAME gain_ground_title_music COMMAND gain_ground_title_music)
add_executable(gain_ground_system24_timer_deadline tests/system24_timer_deadline.cpp)
target_link_libraries(gain_ground_system24_timer_deadline PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_system24_timer_deadline COMMAND gain_ground_system24_timer_deadline)
set_tests_properties(gain_ground_system24_timer_deadline PROPERTIES LABELS "native-device-timing" TIMEOUT 30)
add_executable(gain_ground_audio_busy_deadline tests/audio_busy_deadline.cpp)
target_link_libraries(gain_ground_audio_busy_deadline PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_audio_busy_deadline COMMAND gain_ground_audio_busy_deadline)
set_tests_properties(gain_ground_audio_busy_deadline PROPERTIES LABELS "native-child-boundary" TIMEOUT 30)
add_executable(gain_ground_scene_child_timing tests/scene_child_timing.cpp)
target_link_libraries(gain_ground_scene_child_timing PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_scene_child_timing COMMAND gain_ground_scene_child_timing)
set_tests_properties(gain_ground_scene_child_timing PROPERTIES LABELS "native-child-boundary" TIMEOUT 30)
add_executable(gain_ground_sound_irq_timing tests/sound_irq_timing.cpp)
target_link_libraries(gain_ground_sound_irq_timing PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_sound_irq_timing COMMAND gain_ground_sound_irq_timing)
set_tests_properties(gain_ground_sound_irq_timing PROPERTIES LABELS "native-child-boundary" TIMEOUT 30)
add_executable(gain_ground_saved_actor_callback tests/saved_actor_callback.cpp)
add_executable(gain_ground_title_coin_music tests/title_coin_music.cpp)
add_executable(gain_ground_enemy_navigation tests/enemy_navigation.cpp)
target_link_libraries(gain_ground_enemy_navigation PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_enemy_navigation COMMAND gain_ground_enemy_navigation)
set_tests_properties(gain_ground_enemy_navigation PROPERTIES LABELS "native-child-boundary" TIMEOUT 30)
add_executable(gain_ground_enemy_navigation_combat tests/enemy_navigation_combat.cpp)
target_link_libraries(gain_ground_enemy_navigation_combat PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_enemy_navigation_combat COMMAND gain_ground_enemy_navigation_combat)
set_tests_properties(gain_ground_enemy_navigation_combat PROPERTIES LABELS "native-child-boundary" TIMEOUT 30)
add_executable(gain_ground_stage_select tests/stage_select.cpp)
target_link_libraries(gain_ground_stage_select PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_stage_select COMMAND gain_ground_stage_select)
set_tests_properties(gain_ground_stage_select PROPERTIES TIMEOUT 30)
add_executable(gain_ground_enemy_attack_direction tests/enemy_attack_direction.cpp)
target_link_libraries(gain_ground_enemy_attack_direction PRIVATE gain_ground_runtime_host)
if(GAIN_GROUND_RESEARCH_TESTS)
add_test(NAME gain_ground_enemy_attack_direction COMMAND gain_ground_enemy_attack_direction
    "${CMAKE_CURRENT_SOURCE_DIR}/../capture/cpu_b_state72_data.bin")
endif()
if(GAIN_GROUND_RESEARCH_TESTS)
set_tests_properties(gain_ground_enemy_attack_direction PROPERTIES LABELS "native-child-boundary" TIMEOUT 30)
endif()
target_link_libraries(gain_ground_title_coin_music PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_title_coin_music COMMAND gain_ground_title_coin_music)
set_tests_properties(gain_ground_title_coin_music PROPERTIES TIMEOUT 30)
add_executable(gain_ground_nonlocal_actor_callers tests/nonlocal_actor_callers.cpp)
target_link_libraries(gain_ground_nonlocal_actor_callers PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_nonlocal_actor_callers COMMAND gain_ground_nonlocal_actor_callers)
set_tests_properties(gain_ground_nonlocal_actor_callers PROPERTIES LABELS "native-child-boundary" TIMEOUT 30)
add_executable(gain_ground_runtime_dispatch_repairs tests/runtime_dispatch_repairs.cpp)
add_executable(gain_ground_character_smoke tests/character_smoke.cpp)
target_link_libraries(gain_ground_character_smoke PRIVATE gain_ground_runtime_host)
if(GAIN_GROUND_CHARACTER_SNAPSHOT)
    foreach(character RANGE 0 19)
        foreach(mode movement primary secondary)
            add_test(NAME character_${character}_${mode} COMMAND gain_ground_character_smoke
                "${GAIN_GROUND_CHARACTER_SNAPSHOT}" ${character} ${mode})
            set_tests_properties(character_${character}_${mode} PROPERTIES LABELS "character-smoke;native-child-boundary" TIMEOUT 5)
        endforeach()
    endforeach()
    add_test(NAME character_saved_crash COMMAND gain_ground_saved_actor_callback "${GAIN_GROUND_CHARACTER_SNAPSHOT}")
    set_tests_properties(character_saved_crash PROPERTIES LABELS "character-crash;native-child-boundary" TIMEOUT 5)
    add_custom_target(check_characters
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure --no-tests=error -L character-smoke -j 4
        DEPENDS gain_ground_character_smoke
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
endif()
target_link_libraries(gain_ground_runtime_dispatch_repairs PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_runtime_dispatch_repairs COMMAND gain_ground_runtime_dispatch_repairs)
set_tests_properties(gain_ground_runtime_dispatch_repairs PROPERTIES LABELS "native-child-boundary" TIMEOUT 30)
target_link_libraries(gain_ground_saved_actor_callback PRIVATE gain_ground_runtime_host)
if(GAIN_GROUND_RESEARCH_TESTS)
add_test(NAME gain_ground_saved_actor_callback COMMAND gain_ground_saved_actor_callback
    "${CMAKE_CURRENT_SOURCE_DIR}/../capture/repro-stage2-crash-20260912")
endif()
if(GAIN_GROUND_RESEARCH_TESTS)
set_tests_properties(gain_ground_saved_actor_callback PROPERTIES LABELS "native-child-boundary" TIMEOUT 30)
endif()
if(GAIN_GROUND_RESEARCH_TESTS)
add_test(NAME gain_ground_saved_actor_nonlocal_return COMMAND gain_ground_saved_actor_callback
    ${CMAKE_CURRENT_SOURCE_DIR}/../capture/repro-stage2-nonlocal-return-20260912 --retry-removed-actor)
endif()
if(GAIN_GROUND_RESEARCH_TESTS)
set_tests_properties(gain_ground_saved_actor_nonlocal_return PROPERTIES LABELS "native-child-boundary" TIMEOUT 30)
endif()
add_executable(gain_ground_coin_child_return tests/coin_child_return.cpp)
target_link_libraries(gain_ground_coin_child_return PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_coin_child_return COMMAND gain_ground_coin_child_return)
add_executable(gain_ground_irq_child_return tests/irq_child_return.cpp)
target_link_libraries(gain_ground_irq_child_return PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_irq_child_return COMMAND gain_ground_irq_child_return)
add_executable(gain_ground_frame_wait_child_return tests/frame_wait_child_return.cpp)
target_link_libraries(gain_ground_frame_wait_child_return PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_frame_wait_child_return COMMAND gain_ground_frame_wait_child_return)
add_executable(gain_ground_input_sampler_child_return tests/input_sampler_child_return.cpp)
target_link_libraries(gain_ground_input_sampler_child_return PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_input_sampler_child_return COMMAND gain_ground_input_sampler_child_return)
add_executable(gain_ground_player_start_input tests/player_start_input.cpp)
add_executable(gain_ground_controller_input tests/controller_input.cpp)
add_executable(gain_ground_player_strip_bytes tests/player_strip_bytes.cpp)
target_link_libraries(gain_ground_player_strip_bytes PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_player_strip_bytes COMMAND gain_ground_player_strip_bytes)
target_link_libraries(gain_ground_controller_input PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_controller_input COMMAND gain_ground_controller_input)
target_link_libraries(gain_ground_player_start_input PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_player_start_input COMMAND gain_ground_player_start_input)
set_tests_properties(gain_ground_coin_child_return gain_ground_irq_child_return gain_ground_frame_wait_child_return gain_ground_input_sampler_child_return gain_ground_player_start_input gain_ground_controller_input gain_ground_player_strip_bytes
    PROPERTIES LABELS "native-child-boundary" TIMEOUT 30)
# Build and run real-child integration before fixture replay. An empty selection
# is an error: fixture-only success must never silently replace this gate.
add_custom_target(check_native_child_boundaries
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure --no-tests=error
        --label-regex "^native-child-boundary$"
    DEPENDS gain_ground_coin_child_return gain_ground_irq_child_return gain_ground_frame_wait_child_return gain_ground_input_sampler_child_return gain_ground_player_start_input gain_ground_controller_input gain_ground_player_strip_bytes gain_ground_sound_irq_timing gain_ground_scene_child_timing gain_ground_audio_busy_deadline gain_ground_enemy_navigation gain_ground_enemy_navigation_combat gain_ground_enemy_attack_direction
        gain_ground_nonlocal_actor_callers gain_ground_runtime_dispatch_repairs gain_ground_saved_actor_callback gain_ground_character_smoke gain_ground_native_loop_depth
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    VERBATIM
    USES_TERMINAL)
add_executable(gain_ground_interrupt_entry_timing tests/interrupt_entry_timing.cpp)
target_link_libraries(gain_ground_interrupt_entry_timing PRIVATE gain_ground_contracts)
add_executable(gain_ground_frame_wait_timing tests/frame_wait_timing.cpp)
target_link_libraries(gain_ground_frame_wait_timing PRIVATE gain_ground_contracts)
add_test(NAME gain_ground_frame_wait_timing COMMAND gain_ground_frame_wait_timing)
add_test(NAME gain_ground_interrupt_entry_timing COMMAND gain_ground_interrupt_entry_timing)
add_executable(gain_ground_sound_instruction_timing tests/sound_instruction_timing.cpp)
add_executable(gain_ground_state_step_child_return tests/state_step_child_return.cpp)
target_link_libraries(gain_ground_state_step_child_return PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_state_step_child_return COMMAND gain_ground_state_step_child_return)
target_link_libraries(gain_ground_sound_instruction_timing PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_sound_instruction_timing COMMAND gain_ground_sound_instruction_timing)
add_executable(gain_ground_direct_boot_audio tests/direct_boot_audio.cpp)
add_executable(gain_ground_system24_startup_devices tests/system24_startup_devices.cpp)
target_link_libraries(gain_ground_system24_startup_devices PRIVATE gain_ground_runtime_host)
add_test(NAME gain_ground_system24_startup_devices COMMAND gain_ground_system24_startup_devices)
target_link_libraries(gain_ground_direct_boot_audio PRIVATE gain_ground_runtime_host)
if(GAIN_GROUND_RESEARCH_TESTS)
add_test(NAME gain_ground_direct_boot_audio COMMAND gain_ground_direct_boot_audio
    ${CMAKE_CURRENT_SOURCE_DIR}/../capture/cpu_a_program.bin)
endif()
if(GAIN_GROUND_RESEARCH_TESTS)
set_tests_properties(gain_ground_direct_boot_audio PROPERTIES LABELS "native-device-timing" TIMEOUT 30)
endif()
if(WIN32)
    add_executable(gain_ground_board_startup_timing tests/board_startup_timing.cpp)
    target_link_libraries(gain_ground_board_startup_timing PRIVATE gain_ground_runtime_host bcrypt)
    if(GAIN_GROUND_RESEARCH_TESTS)
add_test(NAME gain_ground_board_startup_timing COMMAND gain_ground_board_startup_timing
        "${CMAKE_CURRENT_SOURCE_DIR}/../capture/cpu_a_program.bin"
        "${CMAKE_CURRENT_SOURCE_DIR}/../analysis/assets")
endif()
    if(GAIN_GROUND_RESEARCH_TESTS)
set_tests_properties(gain_ground_board_startup_timing PROPERTIES LABELS "native-device-timing" TIMEOUT 30)
endif()
endif()
if(GAIN_GROUND_RESEARCH_TESTS)
add_test(
    NAME gain_ground_fixture_smoke
    COMMAND gain_ground_fixture_smoke
        ${CMAKE_CURRENT_SOURCE_DIR}/../fixtures/corpus-expanded-v1/gground-function-fixtures.ggfz
)
endif()
add_test(
    NAME gain_ground_checkpoint_writer_smoke
    COMMAND gain_ground_checkpoint_writer_smoke
        ${CMAKE_CURRENT_BINARY_DIR}/checkpoint-smoke-a.ggcap
        ${CMAKE_CURRENT_BINARY_DIR}/checkpoint-smoke-b.ggcap
)
if(GAIN_GROUND_RESEARCH_TESTS)
add_test(
    NAME gain_ground_asset_checkpoint_writer_smoke
    COMMAND gain_ground_asset_checkpoint_writer_smoke
        ${CMAKE_CURRENT_SOURCE_DIR}/../analysis/assets
        ${CMAKE_CURRENT_BINARY_DIR}/asset-checkpoints-a
        ${CMAKE_CURRENT_BINARY_DIR}/asset-checkpoints-b
)
endif()
if(GAIN_GROUND_RESEARCH_TESTS)
add_test(
    NAME gain_ground_all_implemented_fixtures
    COMMAND gain_ground_fixture_compare
        ${CMAKE_CURRENT_SOURCE_DIR}/../fixtures/corpus-expanded-v1/gground-function-fixtures.ggfz
        --all-implemented
)
add_executable(gain_ground_fixture_enclosing_wait tests/fixture_enclosing_wait.cpp)
target_link_libraries(gain_ground_fixture_enclosing_wait PRIVATE gain_ground_contracts)
add_test(NAME gain_ground_fixture_enclosing_wait
    COMMAND gain_ground_fixture_enclosing_wait
        ${CMAKE_CURRENT_SOURCE_DIR}/../fixtures/corpus-expanded-v1/gground-function-fixtures.ggfz)
set_tests_properties(gain_ground_fixture_enclosing_wait PROPERTIES TIMEOUT 30)
add_executable(gain_ground_fixture_input_partition tests/fixture_input_partition.cpp)
target_link_libraries(gain_ground_fixture_input_partition PRIVATE gain_ground_contracts)
add_test(NAME gain_ground_fixture_input_partition
    COMMAND gain_ground_fixture_input_partition
        ${CMAKE_CURRENT_SOURCE_DIR}/../fixtures/corpus-expanded-v1/gground-function-fixtures.ggfz)
set_tests_properties(gain_ground_fixture_input_partition PROPERTIES TIMEOUT 30)
endif()
add_test(
    NAME gain_ground_function34_carry_path
    COMMAND gain_ground_function34_carry_path
)
add_test(
    NAME gain_ground_fixture_marker_compatibility_test
    COMMAND gain_ground_fixture_marker_compatibility_test
)
if(GAIN_GROUND_RESEARCH_TESTS)
add_test(
    NAME gain_ground_function151_irq_state_handoff
    COMMAND gain_ground_fixture_compare
        ${CMAKE_CURRENT_SOURCE_DIR}/../fixtures/corpus-expanded-v1/gground-function-fixtures.ggfz
        --record 67688
)
endif()
if(GAIN_GROUND_RESEARCH_TESTS)
add_test(
    NAME gain_ground_function151_irq_before_child
    COMMAND gain_ground_fixture_compare
        ${CMAKE_CURRENT_SOURCE_DIR}/../fixtures/corpus-expanded-v1/gground-function-fixtures.ggfz
        --record 67742
)
endif()

if(WIN32)
    target_link_libraries(gain_ground_runtime PRIVATE comctl32)
endif()

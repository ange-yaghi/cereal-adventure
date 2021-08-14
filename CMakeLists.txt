cmake_minimum_required(VERSION 3.10)

# Enable group projects in folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

project(cereal-adventure)

set(CMAKE_CXX_STANDARD 11)

add_executable(cereal-adventure WIN32
    # Source files
    src/asset_loader.cpp
    src/blur_stage.cpp
    src/cabinet.cpp
    src/ceiling_light_source.cpp
    src/cereal_adventure_app.cpp
    src/clock.cpp
    src/collectible_item.cpp
    src/colors.cpp
    src/cooldown_timer.cpp
    src/counter.cpp
    src/death_component.cpp
    src/debug_camera_controller.cpp
    src/demo_lighting_controller.cpp
    src/demo_shader_controls.cpp
    src/fan.cpp
    src/fire_damage_component.cpp
    src/fridge.cpp
    src/fruit_bowl.cpp
    src/fruit_projectile.cpp
    src/game_object.cpp
    src/jitter_filter.cpp
    src/ledge.cpp
    src/light_object.cpp
    src/main.cpp
    src/math_utilities.cpp
    src/microwave.cpp
    src/milk_carton.cpp
    src/os_utilities.cpp
    src/oven.cpp
    src/player.cpp
    src/player_arms_fsm.cpp
    src/player_legs_fsm.cpp
    src/projectile_damage_component.cpp
    src/realm.cpp
    src/scene_lighting_controller.cpp
    src/shaders.cpp
    src/shelves.cpp
    src/single_shelf.cpp
    src/sink.cpp
    src/spring_connector.cpp
    src/ssao.cpp
    src/static_art.cpp
    src/stool_1.cpp
    src/stove_hood.cpp
    src/table.cpp
    src/test_obstacle.cpp
    src/toaster.cpp
    src/toast_projectile.cpp
    src/turn_table_camera.cpp
    src/ui.cpp
    src/vase.cpp
    src/walk_component.cpp
    src/wave_generator.cpp
    src/window_light_source.cpp
    src/world.cpp
    src/wrapping_timer.cpp

    # Include files
    include/aabb.h
    include/asset_loader.h
    include/blur_stage.h
    include/cabinet.h
    include/ceiling_light_source.h
    include/cereal_adventure_app.h
    include/clock.h
    include/collectible_item.h
    include/colors.h
    include/cooldown_timer.h
    include/counter.h
    include/death_component.h
    include/debug_camera_controller.h
    include/delta.h
    include/demo_lighting_controller.h
    include/demo_shader_controls.h
    include/fan.h
    include/fire_damage_component.h
    include/fridge.h
    include/fruit_bowl.h
    include/fruit_projectile.h
    include/game_object.h
    include/game_objects.h
    include/jitter_filter.h
    include/ledge.h
    include/light_object.h
    include/math_utilities.h
    include/microwave.h
    include/milk_carton.h
    include/os_utilities.h
    include/oven.h
    include/player.h
    include/player_arms_fsm.h
    include/player_legs_fsm.h
    include/projectile_damage_component.h
    include/realm.h
    include/scene_lighting_controller.h
    include/shaders.h
    include/shader_controls.h
    include/shelves.h
    include/single_shelf.h
    include/sink.h
    include/spring_connector.h
    include/ssao.h
    include/static_art.h
    include/stool_1.h
    include/stove_hood.h
    include/table.h
    include/test_obstacle.h
    include/toaster.h
    include/toast_projectile.h
    include/turn_table_camera.h
    include/ui.h
    include/vase.h
    include/walk_component.h
    include/wave_generator.h
    include/window_light_source.h
    include/world.h
    include/wrapping_timer.h
)

target_link_libraries(cereal-adventure
    delta-basic)

target_include_directories(cereal-adventure
    PUBLIC dependencies/submodules)

add_subdirectory(dependencies)

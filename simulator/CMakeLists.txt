cmake_minimum_required(VERSION 3.10)
project(StreamDeck_LVGL_Simulator)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# LVGL source files (adjust path as needed)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lvgl)

# Source files
set(SOURCES
    src/main.c
    src/ui_events.c
    src/simulator_input.c
    src/debug_panel.c
    # Add LVGL core files
    # lvgl/src/lv_core.c, etc...
)

# Create executable
add_executable(streamdeck_simulator ${SOURCES})
target_link_libraries(streamdeck_simulator ${SDL2_LIBRARIES} m)

# Compiler flags
target_compile_options(streamdeck_simulator PRIVATE -Wall -Wextra)

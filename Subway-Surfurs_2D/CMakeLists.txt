cmake_minimum_required(VERSION 3.10)
project(SubwaySurfers_2D VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Locate SFML Library
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

# Include the "include" directory so #include "Player.h" works inside src/
include_directories(include)

# Automatically find all .cpp files in src/
file(GLOB SOURCES "src/*.cpp")

# Create the Executable
add_executable(SubwaySurfers ${SOURCES})

# Link SFML
target_link_libraries(SubwaySurfers sfml-graphics sfml-window sfml-system sfml-audio)

# (Optional) Copy assets if you had an assets folder
# --- NEW: Copy Assets to Build Directory ---
add_custom_command(TARGET SubwaySurfers POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/assets)
# file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

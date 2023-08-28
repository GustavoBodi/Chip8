cmake_minimum_required(VERSION 3.25)
project(emu)

set(CMAKE_CXX_STANDARD 23)

find_package(SDL2 REQUIRED)
include_directories(emu ${SDL2_INCLUDE_DIRS})

add_executable(emu src/main.cpp src/Chip8.cpp include/Chip8.hpp include/Renderer.hpp src/Fonts.cpp include/Fonts.hpp include/Keyboard.hpp include/Screen.hpp)
target_link_libraries(emu ${SDL2_LIBRARIES})

cmake_minimum_required(VERSION 3.25)
project(emu)

set(CMAKE_CXX_STANDARD 23)

find_package(SDL2 REQUIRED)
include_directories(emu ${SDL2_INCLUDE_DIRS})

add_executable(emu main.cpp Chip8.cpp Chip8.hpp Renderer.hpp Fonts.cpp Fonts.hpp Keyboard.cpp Keyboard.hpp Screen.hpp)
target_link_libraries(emu ${SDL2_LIBRARIES})

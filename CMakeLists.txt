cmake_minimum_required (VERSION 3.10.2)
set(PROJECT1 CHIP_8)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_executable(${PROJECT1} main.cpp chip8.h chip8.cpp )

include(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

target_link_libraries(${PROJECT1} ${SDL2_LIBRARIES})

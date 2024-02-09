# cmake版本
cmake_minimum_required(VERSION 3.5.1)
# 项目名称
project(xtinygui)
set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE "Release")
file(GLOB xtinygui_src xtinygui/src/*.c)
# file(GLOB app app/src/*.c)
file(GLOB nes NES_Simulator/*.c)

add_executable(
    xtinygui 
    ${xtinygui_src}
    ${app}
    ${nes}
    app.c
    main.c
)

include_directories(
    ${CMAKE_SOURCE_DIR}/xtinygui/inc
    ${CMAKE_SOURCE_DIR}/app/inc
    ${CMAKE_SOURCE_DIR}/NES_Simulator
    ${CMAKE_SOURCE_DIR}/SDL2-devel-2.30.0-mingw/SDL2-2.30.0/x86_64-w64-mingw32/include/SDL2
)
target_link_directories(
    xtinygui
    PUBLIC
    ${CMAKE_SOURCE_DIR}/SDL2-devel-2.30.0-mingw/SDL2-2.30.0/x86_64-w64-mingw32/lib
    
)
target_link_libraries(
    xtinygui
    SDL2
    SDL2main
)
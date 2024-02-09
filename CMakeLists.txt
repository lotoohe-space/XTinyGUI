# cmake版本
cmake_minimum_required(VERSION 3.5.1)
# 项目名称
project(xtinygui)
set(CMAKE_C_STANDARD 99)
# set(CMAKE_BUILD_TYPE "Release")
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
)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")  
    message(STATUS "Running on Linux操作系统")  
    # 在这里添加适用于Linux的操作系统的特定配置  
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")  
    message(STATUS "Running on Windows操作系统")  
    # 在这里添加适用于Windows的操作系统的特定配置  
    include_directories(
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
        SDL2test
    )
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")  
    message(STATUS "Running on macOS/iOS操作系统")  
    # 在这里添加适用于macOS/iOS的操作系统的特定配置  
    find_package(SDL2 REQUIRED COMPONENTS SDL2)
    target_link_libraries(xtinygui PRIVATE SDL2::SDL2)
else()  
    message(STATUS "Running on an unknown operating system")  
    # 在这里添加适用于其他操作系统的通用配置  
endif()

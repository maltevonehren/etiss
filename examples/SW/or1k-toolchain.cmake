cmake_minimum_required(VERSION 3.10)

set(OR1K_TOOLCHAIN_PREFIX   ""              CACHE STRING "optional prefix for the or1k toolchain in case it is not on the path")
set(OR1K_TOOLCHAIN_BASENAME "or1k-elf"      CACHE STRING "base name of the toolchain executables")


if("${OR1K_TOOLCHAIN_PREFIX}" STREQUAL "")
    set(OR1K_TOOLCHAIN "${OR1K_TOOLCHAIN_BASENAME}")
else()
    set(OR1K_TOOLCHAIN "${OR1K_TOOLCHAIN_PREFIX}/${OR1K_TOOLCHAIN_BASENAME}")
endif()
if(WIN32)
    set(EXE_EXT ".exe")
endif()
set(CMAKE_C_COMPILER   "${OR1K_TOOLCHAIN}-gcc${EXE_EXT}")
set(CMAKE_CXX_COMPILER "${OR1K_TOOLCHAIN}-g++${EXE_EXT}")


set(CMAKE_C_FLAGS          "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS        "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_ASM_FLAGS        "${CMAKE_ASM_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "")


set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR OR1K)

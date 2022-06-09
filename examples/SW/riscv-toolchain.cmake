cmake_minimum_required(VERSION 3.10)


SET(RISCV_ARCH               "rv32gc"              CACHE STRING "RISC-V architecture (-march)")
SET(RISCV_ABI                "ilp32d"              CACHE STRING "RISC-V ABI (-mabi)")

SET(RISCV_TOOLCHAIN_PREFIX   ""                    CACHE STRING "optional prefix for the riscv toolchain in case it is not on the path")
SET(RISCV_TOOLCHAIN_BASENAME "riscv64-unknown-elf" CACHE STRING "base name of the toolchain executables")

if("${RISCV_TOOLCHAIN_PREFIX}" STREQUAL "")
    SET(CMAKE_C_COMPILER   "${RISCV_TOOLCHAIN_BASENAME}-gcc")
    SET(CMAKE_CXX_COMPILER "${RISCV_TOOLCHAIN_BASENAME}-g++")
else()
    SET(CMAKE_C_COMPILER   "${RISCV_TOOLCHAIN_PREFIX}/${RISCV_TOOLCHAIN_BASENAME}-gcc")
    SET(CMAKE_CXX_COMPILER "${RISCV_TOOLCHAIN_PREFIX}/${RISCV_TOOLCHAIN_BASENAME}-g++")
endif()



SET(CMAKE_C_FLAGS          "-D__riscv__ -march=${RISCV_ARCH} -mabi=${RISCV_ABI} -specs=semihost.specs")
SET(CMAKE_CXX_FLAGS        "-D__riscv__ -march=${RISCV_ARCH} -mabi=${RISCV_ABI} -specs=semihost.specs")
SET(CMAKE_EXE_LINKER_FLAGS "-march=${RISCV_ARCH} -mabi=${RISCV_ABI}")


SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR ${RISCV_ARCH})

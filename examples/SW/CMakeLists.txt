include(ExternalProject)


SET(EXAMPLES_BUILD_RISCV OFF CACHE BOOL "build the riscv examples")
SET(EXAMPLES_ALWAYS_BUILD OFF CACHE BOOL "setting this variable two 1 will rebuild the target software every time. Useful when changing software in examples/SW/target_code")

SET(RISCV_TOOLCHAIN_PREFIX   ""                    CACHE STRING "optional prefix for the riscv toolchain in case it is not on the path")
SET(RISCV_TOOLCHAIN_BASENAME "riscv64-unknown-elf" CACHE STRING "base name of the toolchain executables")



set(RISCV_TOOLCHAIN_FILE "${CMAKE_BINARY_DIR}/examples/SW/riscv-toolchain.cmake")
SET(EXAMPLES_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/target_code)

configure_file(
    riscv-toolchain.cmake
    ${RISCV_TOOLCHAIN_FILE}
    COPYONLY
)

install(FILES
    ${RISCV_TOOLCHAIN_FILE}
    DESTINATION "lib/CMake/toolchains" COMPONENT dev
)

if (EXAMPLES_BUILD_RISCV)
    ExternalProject_Add(example_code_rv32gc
        SOURCE_DIR ${EXAMPLES_SOURCE}
        BUILD_ALWAYS ${EXAMPLES_ALWAYS_BUILD}
        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/examples/SW/rv32gc
            -DRISCV_ARCH=rv32gc
            -DRISCV_ABI=ilp32d
            -DRISCV_TOOLCHAIN_PREFIX=${RISCV_TOOLCHAIN_PREFIX}
            -DRISCV_TOOLCHAIN_BASENAME=${RISCV_TOOLCHAIN_BASENAME}
            -DCMAKE_TOOLCHAIN_FILE=${RISCV_TOOLCHAIN_FILE}
    )

    ExternalProject_Add(example_code_rv64gc
        SOURCE_DIR ${EXAMPLES_SOURCE}
        BUILD_ALWAYS ${EXAMPLES_ALWAYS_BUILD}
        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/examples/SW/rv64gc
            -DRISCV_ARCH=rv64gc
            -DRISCV_ABI=lp64d
            -DRISCV_TOOLCHAIN_PREFIX=${RISCV_TOOLCHAIN_PREFIX}
            -DRISCV_TOOLCHAIN_BASENAME=${RISCV_TOOLCHAIN_BASENAME}
            -DCMAKE_TOOLCHAIN_FILE=${RISCV_TOOLCHAIN_FILE}
    )
endif()

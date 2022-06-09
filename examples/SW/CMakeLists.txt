include(ExternalProject)

set(RISCV_TOOLCHAIN_PATH "${CMAKE_BINARY_DIR}/examples/SW/riscv-toolchain.cmake")

configure_file(
    riscv-toolchain.cmake
    ${RISCV_TOOLCHAIN_PATH}
    COPYONLY
)

install(FILES
    ${RISCV_TOOLCHAIN_PATH}
    DESTINATION "lib/CMake/toolchains" COMPONENT dev
)

SET(EXAMPLES_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/target_code)
SET(EXAMPLES_BUILD_ALWAYS 0 CACHE BOOL "setting this variable two 1 will rebuild the target software every time. Useful when changing software in examples/SW/target_code")

ExternalProject_Add(example_code_rv32gc
    SOURCE_DIR ${EXAMPLES_SOURCE}
    BUILD_ALWAYS ${EXAMPLES_BUILD_ALWAYS}
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/examples/SW/rv32gc
        -DRISCV_ARCH=rv32gc
        -DRISCV_ABI=ilp32d
        -DCMAKE_TOOLCHAIN_FILE=${RISCV_TOOLCHAIN_PATH}
)

ExternalProject_Add(example_code_rv64gc
    SOURCE_DIR ${EXAMPLES_SOURCE}
    BUILD_ALWAYS ${EXAMPLES_BUILD_ALWAYS}
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/examples/SW/rv64gc
        -DRISCV_ARCH=rv64gc
        -DRISCV_ABI=lp64d
        -DCMAKE_TOOLCHAIN_FILE=${RISCV_TOOLCHAIN_PATH}
)

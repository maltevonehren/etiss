include(ExternalProject)


set(EXAMPLES_BUILD_RISCV OFF CACHE BOOL "build the riscv examples")
set(EXAMPLES_BUILD_OR1K OFF CACHE BOOL "build the or1k examples")
set(EXAMPLES_ALWAYS_BUILD OFF CACHE BOOL "setting this variable two 1 will rebuild the target software every time. Useful when changing software in examples/SW/target_code")


set(RISCV_TOOLCHAIN_PREFIX   ""                    CACHE STRING "optional prefix for the RISC-V toolchain in case it is not on the path")
set(RISCV_TOOLCHAIN_BASENAME "riscv64-unknown-elf" CACHE STRING "base name of the RISC-V toolchain executables")

set(RISCV_TOOLCHAIN_FILE "${CMAKE_CURRENT_BINARY_DIR}/riscv-toolchain.cmake")
configure_file(riscv-toolchain.cmake ${RISCV_TOOLCHAIN_FILE} COPYONLY)
install(FILES  ${RISCV_TOOLCHAIN_FILE} DESTINATION "lib/CMake/toolchains")

set(RISCV_TOOLCHAIN_ARGS 
    -DRISCV_TOOLCHAIN_PREFIX=${RISCV_TOOLCHAIN_PREFIX}
    -DRISCV_TOOLCHAIN_BASENAME=${RISCV_TOOLCHAIN_BASENAME}
    -DCMAKE_TOOLCHAIN_FILE=${RISCV_TOOLCHAIN_FILE}
)


set(OR1K_TOOLCHAIN_PREFIX   ""                    CACHE STRING "optional prefix for the OR1K toolchain in case it is not on the path")
set(OR1K_TOOLCHAIN_BASENAME "or1k-elf"            CACHE STRING "base name of the OR1K toolchain executables")

set(OR1K_TOOLCHAIN_FILE "${CMAKE_CURRENT_BINARY_DIR}/or1k-toolchain.cmake")
configure_file(or1k-toolchain.cmake ${OR1K_TOOLCHAIN_FILE} COPYONLY)
install(FILES  ${OR1K_TOOLCHAIN_FILE} DESTINATION "lib/CMake/toolchains")

set(OR1K_TOOLCHAIN_ARGS 
    -DOR1K_TOOLCHAIN_PREFIX=${OR1K_TOOLCHAIN_PREFIX}
    -DOR1K_TOOLCHAIN_BASENAME=${OR1K_TOOLCHAIN_BASENAME}
    -DCMAKE_TOOLCHAIN_FILE=${OR1K_TOOLCHAIN_FILE}
)


# this macro build the startup code from the directory `startup_${arch}`
# and the example code from `target_code`.
# It will be installed into `${CMAKE_INSTALL_PREFIX}/examples/SW/${subarch}`
MACRO (BUILD_FOR_SUBARCH arch subarch)
    set(CUSTOM_CMAKE_ARGS "${ARGN}")

    set(CUSTOM_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/examples/SW/${subarch})

    # builds the startup code, installs it and custom cmake macro
    ExternalProject_Add(startup_code_${subarch}
        BUILD_ALWAYS ${EXAMPLES_ALWAYS_BUILD}
        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/${arch}
        CMAKE_GENERATOR "Unix Makefiles"
        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${CUSTOM_INSTALL_PREFIX}
            ${CUSTOM_CMAKE_ARGS}
    )

    # builds example code using macro generated before
    ExternalProject_Add(example_code_${subarch}
        DEPENDS startup_code_${subarch}
        BUILD_ALWAYS ${EXAMPLES_ALWAYS_BUILD}
        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/target_code
        CMAKE_GENERATOR "Unix Makefiles"
        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${CUSTOM_INSTALL_PREFIX}
            ${CUSTOM_CMAKE_ARGS}
            -DETISS_CMAKE=${CUSTOM_INSTALL_PREFIX}/CMake/etiss.cmake
    )
ENDMACRO()


if (EXAMPLES_BUILD_RISCV)
    BUILD_FOR_SUBARCH(riscv rv32gc ${RISCV_TOOLCHAIN_ARGS} -DHAS_TIME=ON -DRISCV_ARCH=rv32gc -DRISCV_ABI=ilp32d)
    BUILD_FOR_SUBARCH(riscv rv64gc ${RISCV_TOOLCHAIN_ARGS} -DHAS_TIME=ON -DRISCV_ARCH=rv64gc -DRISCV_ABI=lp64d)
endif()


if (EXAMPLES_BUILD_OR1K)
    BUILD_FOR_SUBARCH(or1k or1k ${OR1K_TOOLCHAIN_ARGS})
endif()

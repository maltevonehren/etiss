# Target Software and Toolchain Files

Example target software for simulation is located in the `./target_code` directory
(see its [README](target_code/ReadME.md) for explanations on the different programs).
It can be automatically build for different architectures during the regular build process (see [explanation below](#build-examples-during-regular-build-process)).

## Build Examples during regular Build Process

The `target_code` folder is a standalone CMake project.
The same project can build multiple times with a different configuration,
resulting in binaries for the different architectures.
These are installed into the `<etiss_install_dir>/examples/SW/<arch>/bin` directory.

- Setting `EXAMPLES_BUILD_RISCV` to `ON` will build the software for rv32gc and rv64gc
 
(The corresponding toolchain has to be installed. See [Specifying compiler location](#specifying-compiler-location))

## Build Custom Software using provided Toolchain Files

`riscv-toolchain.cmake` is a simple toolchain file for compiling code to RISC-V with semihosting.
It is used for building the example target software but can also be used to build your own software for simulation. It is copied to `<etiss_install_dir>/lib/CMake/toolchains/`.

## Specifying compiler location

The toolchain files contain a default compiler setup that should work most of the time.
It can be customized using CMake cache variables.

- The toolchain basename can be overwritten using `<ARCH>_TOOLCHAIN_BASENAME`.
- In case the toolchain is not on the path its location get be specified with `<ARCH>_TOOLCHAIN_PREFIX`

The main cmake project will pass these variables on to the external projects.
See [this section of the main README](../../README.md#example-target-software) for an example.

The defaults are:
- `RISCV_TOOLCHAIN_BASENAME` = `riscv64-unknown-elf`

## Changing code in `examples/SW/target_code`

Due to the nature of CMakes ExternalProject_Add, changes to the source files in `target_code` do not trigger a build of the target software.
When changing code in the `target_code` folder, make sure to set `EXAMPLES_ALWAYS_BUILD` to `ON`.
This will trigger a build for the target software during every ETISS build.
(During this build only the changed code will be compiled again)

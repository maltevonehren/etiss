# Target Software and Toolchain Files

The folder `target_code` contains code examples to be compiled for multiple architectures to be used in ETISS.
See its [README](target_code/README.md) for details on the individual examples
and see the [explanation below](#build-examples-during-regular-build-process) on how to build them during the regular build process.

Since the code is running without an operating system custom startup code and linker scripts are required for each architecture.
The custom startup code is located in the `<arch>` folders.
In addition to the startup code there is also a CMake file with a macro to build the code using the custom startup code and linker script.

## Build Examples during regular Build

The `target_code` and `<arch>` folders are standalone CMake projects.
The same projects can build multiple times with different configurations, resulting in binaries for the different architectures.
The examples  are then installed into the `<etiss_install_dir>/examples/SW/<subarch>/bin` directory.

- Setting the CMake cache variable `EXAMPLES_BUILD_RISCV` to `ON` will build the software for `rv32gc` and `rv64gc`
- Setting the CMake cache variable `EXAMPLES_BUILD_OR1K` to `ON` will build the software for `or1k`
 
(The corresponding toolchain has to be installed. See [Specifying compiler location](#specifying-compiler-location))

### Detailed Description of the Build Process

For each enabled subarch, the following steps are completed:

1.  The CMake project in the `<arch>` folder gets build first.
    It installs the file `<etiss_install_dir>/examples/SW/<subarch>/CMake/etiss.cmake`
    which contains a macro for building target code using the custom startup code and linker script.
    It also build and installs the startup code as well as installing required files such as the linker script.

2.  Afterwards the project in the `target_code` folder builds all examples using the macro from before.

## Run the Examples

The example binaries are located in the `<etiss_install_dir>/examples/SW/<subarch>/bin` directory.
A configuration file for telling ETISS about the memory layout is `<etiss_install_dir>/examples/SW/<subarch>/etc/memsegs.ini`.

To run the `hello_world` example for rv32gc execute the following command inside `<etiss_install_dir>`

    $ bin/run_helper.sh examples/SW/rv32gc/bin/hello_world config examples/SW/rv32gc/etc/memsegs.ini

## Build Custom Target Code

When the examples get build, the file `<etiss_install_dir>/examples/SW/<subarch>/CMake/etiss.cmake` is installed for each subarch. It contains a macro for building target code. Simply include this file in your CMake project and use `add_executable_etiss` instead of `add_executable`. 

To use the correct compiler for each architecture you can use the toolchain files installed with the examples. They can be found at `<etiss_install_dir>/lib/CMake/toolchains/<arch>-toolchain.cmake`. Use them with `-DCMAKE_TOOLCHAIN_FILE=<path_to_toolchain_file>`

## Specifying compiler location

The toolchain files contain a default toolchain setup that should work most of the time.
It can be customized using CMake cache variables.

- The toolchain basename can be overwritten using `<ARCH>_TOOLCHAIN_BASENAME`.
- In case the toolchain is not on the path its location get be specified with `<ARCH>_TOOLCHAIN_PREFIX`

The main CMake project will pass these variables on to the external projects.
See [this section of the main README](/README.md#example-target-software) for an example.

The defaults are:
- `RISCV_TOOLCHAIN_BASENAME` = `riscv64-unknown-elf`
- `OR1K_TOOLCHAIN_BASENAME` = `or1k-elf`

## Changing code in `examples/SW/target_code`

When working on code in the `target_code` folder, make sure to set the CMake cache variable `EXAMPLES_ALWAYS_BUILD` to `ON`.
This will trigger a build for the external projects during every ETISS build.
(During these builds only the changed code will be build/installed again)

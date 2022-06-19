
# Target Software

This folder contains a standalone CMake project that can get build for different (sub)architectures.

- The `examples` folder contains different example programs (see its [README](examples/README.md) for explanations on the different programs)
- The `startup_<arch>` folders contain custom startup code for the respective architectures. These can for example include setting the stackpointer or initializing a trap vector.

The `CMakeLists.txt` contain the macro `add_executable_etiss` for building an executable using the custom linker script and custom startup code.

## cpp
## dhry
## hello_world
## interactive
## test_cases

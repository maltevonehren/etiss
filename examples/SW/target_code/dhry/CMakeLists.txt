
add_executable(dhry dhry_1.c dhry_2.c riscv_timepatch.c)

target_compile_options(dhry PRIVATE 
    -DTIME
    -Wno-implicit-int -Wno-implicit-function-declaration -Wno-pointer-to-int-cast
)

install(TARGETS dhry DESTINATION bin)

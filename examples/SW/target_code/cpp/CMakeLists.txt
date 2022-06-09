
ADD_EXECUTABLE(example_cpp main.cpp riscv_timepatch.c)

install(TARGETS example_cpp DESTINATION bin)

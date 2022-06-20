cmake_minimum_required(VERSION 3.10)

project(etiss_riscv_startup C CXX ASM)

# values for linker script and memory_layout.ini
set(MEM_ROM_ORIGIN 0x10000000)
set(MEM_ROM_LENGTH 0x00400000)
set(MEM_RAM_ORIGIN 0x20000000)
set(MEM_RAM_LENGTH 0x00100000)

set(MIN_STACK_SIZE 0x1000)
set(MIN_HEAP_SIZE 0x1000)


set(LIB_NAME etiss_crt0)

add_library(etiss_crt0 STATIC crt0.s fixes.c trap_handler.c)
install(TARGETS etiss_crt0 DESTINATION lib)

configure_file(etiss.specs.in etiss.specs @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/etiss.specs DESTINATION lib)

configure_file(etiss.ld.in etiss.ld @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/etiss.ld DESTINATION etc)

configure_file(memory_layout.ini.in memory_layout.ini @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/memory_layout.ini DESTINATION etc)

configure_file(etiss.cmake.in etiss.cmake @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/etiss.cmake DESTINATION CMake)

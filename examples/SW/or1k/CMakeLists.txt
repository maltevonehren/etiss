cmake_minimum_required(VERSION 3.10)

project(etiss_or1k_startup NONE)

set(LOGGER_ADDR 0x80000000)
set(MEM_ROM_ORIGIN 0x00003000)
set(MEM_ROM_LENGTH 0x0007D000)
set(MEM_RAM_ORIGIN 0x00080000)
set(MEM_RAM_LENGTH 0x00080000)

set(MIN_STACK_SIZE 0x1000)
set(MIN_HEAP_SIZE 0x1000)

# set(LIB_NAME etiss_crt0)

# add_library(etiss_crt0 STATIC board.S irq_tt.c)
# install(TARGETS etiss_crt0 DESTINATION lib)

configure_file(etiss.ld.in etiss.ld @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/etiss.ld DESTINATION etc)

configure_file(memsegs.ini.in memsegs.ini @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/memsegs.ini DESTINATION etc)

configure_file(etiss.cmake.in etiss.cmake @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/etiss.cmake DESTINATION CMake)

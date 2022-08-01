cmake_minimum_required(VERSION 3.10)

project(etiss_or1k_startup C)

set(OR1K_BOARD or1ksim-uart) # use or1ksim-uart.S
set(LOGGER_ADDR 0x90000000) # taken from or1ksim-uart.S
set(LOGGER_MASK 0xFFFFFFFF)
set(MEM_ORIGIN 0x00000000) # taken from or1ksim-uart.S
set(MEM_LENGTH 0x00800000) # taken from or1ksim-uart.S


set(LIB_NAME etiss_fixes)
add_library(etiss_fixes STATIC fixes.c)
install(TARGETS etiss_fixes DESTINATION lib)

configure_file(memsegs.ini.in memsegs.ini @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/memsegs.ini DESTINATION etc)

configure_file(etiss.cmake.in etiss.cmake @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/etiss.cmake DESTINATION CMake)

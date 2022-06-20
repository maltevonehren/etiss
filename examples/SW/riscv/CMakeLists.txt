cmake_minimum_required(VERSION 3.10)

project(etiss_riscv_startup C CXX ASM)

set(LIB_NAME etiss_crt0)

add_library(etiss_crt0 STATIC crt0.s fixes.c trap_handler.c)
install(TARGETS etiss_crt0 DESTINATION lib)

configure_file(etiss.specs.in etiss.specs @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/etiss.specs DESTINATION lib)

configure_file(etiss.ld etiss.ld COPYONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/etiss.ld DESTINATION CMake)

configure_file(etiss.cmake.in etiss.cmake @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/etiss.cmake DESTINATION CMake)

cmake_minimum_required(VERSION 3.10)

project(etiss_example_code)

include(${ETISS_CMAKE})

add_subdirectory(hello_world)
add_subdirectory(test_cases)

if (HAS_SEMIHOSTING)
    add_subdirectory(interactive)
endif()

if (HAS_SEMIHOSTING AND HAS_TIME)
    add_subdirectory(cpp)
endif()

if(HAS_TIME)
    add_subdirectory(dhry)
endif()


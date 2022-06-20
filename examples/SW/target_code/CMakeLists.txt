cmake_minimum_required(VERSION 3.10)

project(etiss_example_code)

include(${ETISS_CMAKE})

add_subdirectory(hello_world)
add_subdirectory(test_cases)
add_subdirectory(interactive)

if(HAS_TIME)
    add_subdirectory(cpp)
    add_subdirectory(dhry)
endif()


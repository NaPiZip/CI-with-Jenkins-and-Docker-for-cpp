cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)

set(EXE_NAME "Runner")

project("Example" LANGUAGES CXX)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -std=c++14")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "/W4 /WX /EHsc")
endif()

INCLUDE_DIRECTORIES("googletest/googletest/include")
INCLUDE_DIRECTORIES("googletest/googlemock/include")

add_subdirectory(googletest)

file(GLOB project_SRC
    "project/*.h"
    "project/*.cpp"
)

add_executable(${EXE_NAME} ${project_SRC})
target_link_libraries(${EXE_NAME} gtest gmock)

# tell CMake to use CTest extension
enable_testing()


add_test(NAME RunnerTest
        COMMAND ${EXE_NAME}.exe)
#[[
Note:
The following command is used to list all cmake vars
$ cmake -LAH
]]

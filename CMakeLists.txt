cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME "Example")
set(EXE_NAME "Runner")

project(${PROJECT_NAME} LANGUAGES CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -pedantic-errors")

INCLUDE_DIRECTORIES("googletest/googletest/include")
INCLUDE_DIRECTORIES("googletest/googlemock/include")
LINK_DIRECTORIES("/work/googletest/build/lib")

file(GLOB project_SRC
    "PROJECT_NAME/*.h"
    "project/*.cpp"
)

add_executable(${EXE_NAME} ${project_SRC})
target_link_libraries(${EXE_NAME} gtest gmock)

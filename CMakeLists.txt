cmake_minimum_required(VERSION 3.14)
project(RaylibTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

add_subdirectory(external/raylib)
set_target_properties(raylib PROPERTIES COMPILE_OPTIONS "")

add_subdirectory(external/fmt)

if(CMAKE_BUILD_TYPE MATCHES Release)
  add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wno-sign-compare -O3)
else()
  add_compile_options(-Wall -Wextra -Wno-sign-compare -O3)
endif()

set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")

file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")
list(REMOVE_ITEM SOURCES "${SOURCE_DIR}/main.cpp")
add_library(RaylibTest ${SOURCES})
target_include_directories(RaylibTest PUBLIC include external/raygui/src)
target_link_libraries(RaylibTest raylib fmt::fmt)

add_executable(app src/main.cpp)
target_link_libraries(app RaylibTest)

find_package(GTest REQUIRED)
enable_testing()
file(GLOB TEST_SRCS "tests/*.cpp")
add_executable(tests ${TEST_SRCS})
target_link_libraries(tests GTest::GTest GTest::Main RaylibTest)
add_test(NAME Tests COMMAND tests)

cmake_minimum_required(VERSION 3.14)
project(TDBS LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

add_subdirectory(external/raylib)
set_target_properties(raylib PROPERTIES COMPILE_OPTIONS "")

# imgui
set(IMGUI_PATH external/imgui)
set(IMGUI_SOURCES
    ${IMGUI_PATH}/imgui.cpp
    ${IMGUI_PATH}/imgui_demo.cpp
    ${IMGUI_PATH}/imgui_draw.cpp
    ${IMGUI_PATH}/imgui_tables.cpp
    ${IMGUI_PATH}/imgui_widgets.cpp
)
# rlImGui
add_library(rlImGui
    external/rlImGui/rlImGui.cpp
    ${IMGUI_SOURCES}
)
target_include_directories(rlImGui PUBLIC
    external/rlImGui
    ${IMGUI_PATH}
)
target_link_libraries(rlImGui PUBLIC raylib)

add_subdirectory(external/fmt)

if(CMAKE_BUILD_TYPE MATCHES Release)
  add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wno-sign-compare -O3)
else()
  add_compile_options(-Wall -Wextra -Wno-sign-compare -O3)
endif()

set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")

file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")
list(REMOVE_ITEM SOURCES "${SOURCE_DIR}/main.cpp")
add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include external/raygui/src)
target_link_libraries(${PROJECT_NAME} raylib rlImGui fmt::fmt)

add_executable(tdbs src/main.cpp)
target_link_libraries(tdbs ${PROJECT_NAME})

find_package(GTest REQUIRED)
enable_testing()
file(GLOB TEST_SRCS "tests/*.cpp")
add_executable(tests ${TEST_SRCS})
target_link_libraries(tests GTest::GTest GTest::Main ${PROJECT_NAME})
add_test(NAME Tests COMMAND tests)

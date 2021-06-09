cmake_minimum_required(VERSION 3.19)

if (DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

if (DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}")
endif()

set(MY_PROJECT "StreamlineVisualization")
set(MY_LIBRARY "StreamlineVisualizationLib")

set(INCLUDE_DIRS "include")
set(SOURCE_DIRS "src/*.cpp" "src/extern/*.cpp")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project (${MY_PROJECT} LANGUAGES CXX C)
message("CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")

file(GLOB_RECURSE MY_SOURCE CONFIGURE_DEPENDS ${SOURCE_DIRS})

add_library(${MY_LIBRARY} ${MY_SOURCE})
target_include_directories(${MY_LIBRARY} PUBLIC ${INCLUDE_DIRS})

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
target_link_libraries(${MY_LIBRARY} PUBLIC glfw glm::glm glad::glad imgui::imgui)

add_executable(${MY_PROJECT} src/main.cpp)
target_link_libraries(${MY_PROJECT} PUBLIC ${MY_LIBRARY})

add_custom_command(TARGET ${MY_PROJECT} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/ ${CMAKE_BINARY_DIR}/assets/)
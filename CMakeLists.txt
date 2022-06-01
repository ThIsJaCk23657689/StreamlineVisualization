cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

set(MY_PROJECT "StreamlineVisualizer")
set(MY_EXECUTABLE "streamline_visualizer")

project(${MY_PROJECT} VERSION 0.7.0)

add_executable(${MY_EXECUTABLE})

set_target_properties(${MY_EXECUTABLE}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

# 顯示相關資訊
message(STATUS)
message(STATUS "========== Build Information ==========")
message(STATUS "\tHost System Name:         ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "\tHost System Version:      ${CMAKE_HOST_SYSTEM_VERSION}")
message(STATUS "\tHost System Processor:    ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS)
message(STATUS "\tC++ Compiler:             ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "\tC++ Compiler Path:        ${CMAKE_CXX_COMPILER}")
message(STATUS)

if (MSVC)
    message(STATUS "\tMSVC Version:             ${MSVC_VERSION}")
    message(STATUS "\tMSVC Toolset:             ${MSVC_TOOLSET_VERSION}")
endif ()
message(STATUS)

message(STATUS "\tVCPKG Path:               ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS)
message(STATUS "========================================")

# 引入 vcpkg 套件
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(glad REQUIRED)
find_package(imgui REQUIRED)
find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")

# Program Version
configure_file(VERSION.hpp.in VERSION.hpp)

# 指定標頭檔資料夾、找尋所有原始檔、並將【原始碼】加入到【執行檔目標】中
target_include_directories(${MY_EXECUTABLE} PRIVATE
    "include"
    ${STB_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

file(GLOB MY_SOURCE CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/extern/*.cpp"
)
target_sources(${MY_EXECUTABLE} PRIVATE ${MY_SOURCE})

# 將 vcpkg 的套件（函式庫）連結到【執行檔目標】
target_link_libraries(${MY_EXECUTABLE} PRIVATE
    OpenGL::GL
    glfw
    glad::glad
    glm::glm
    imgui::imgui
)

# 針對不同的編譯器有不同的引入設定
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
        target_link_libraries(${MY_EXECUTABLE} PRIVATE stdc++fs) # C++ filesystem
    endif ()
endif ()

# 建立 Symlink 到 assets 資料夾
add_custom_command(TARGET ${MY_EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${MY_EXECUTABLE}>/assets"
    DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    COMMENT
        "Creating symlinks to project resources..."
    VERBATIM
)
cmake_minimum_required(VERSION 3.12)
project(graphics_programming_hw1)

include_directories(include)
link_directories(lib)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(
    GLOB
    SOURCE_FILES
    src/*.cpp
)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui)
add_library(IMGUI STATIC)

target_sources(IMGUI
    PRIVATE
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/imgui_impl_glfw.cpp
)

add_compile_definitions(NDEBUG)
add_executable(graphics_programming_hw1 ${SOURCE_FILES})

set(LIBS opengl32.lib glew32.lib glfw3dll.lib IMGUI)
target_link_libraries(graphics_programming_hw1 ${LIBS})

# copy graphics_programming dll to bin dir
add_custom_command(TARGET graphics_programming_hw1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/dll"
        $<TARGET_FILE_DIR:graphics_programming_hw1>)

# copy shader
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/shader"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shader)

# copy model
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/model"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/model)

# copy texture
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/texture"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/texture)
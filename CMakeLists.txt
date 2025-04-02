cmake_minimum_required(VERSION 3.30)
project(VisualSorterProjectV2)

set(CMAKE_CXX_STANDARD 26)

set(LIB_PATH "C:/__Libraries__")
set(SDL3_DIR "${LIB_PATH}/SDL3/cmake")
set(CMAKE_PREFIX_PATH "${LIB_PATH}/SDL3/cmake")

find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
        "${LIB_PATH}/glfw-3.4.bin.WIN64/include"
        "include"
        "algorithms"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends"
        "${LIB_PATH}/SDL3/i686-w64-mingw32/include"
)
link_directories(
        "${LIB_PATH}/glfw-3.4.bin.WIN64/lib-mingw-w64"
        "${LIB_PATH}/SDL3/x86_64-w64-mingw32/lib"
)

file(GLOB SOURCES "src/*.cpp" "algorithms/*.cpp")

file(GLOB IMGUI_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp"
)

add_library(imgui ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends"
)

add_executable(VisualSorterProjectV2 main.cpp ${SOURCES})
target_link_libraries(VisualSorterProjectV2 ${OPENGL_LIBRARIES} SDL3::SDL3 glfw3 glu32 opengl32 winmm imgui sdl3)

add_custom_command(TARGET VisualSorterProjectV2 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LIB_PATH}/SDL3/x86_64-w64-mingw32/bin/SDL3.dll"
        $<TARGET_FILE_DIR:VisualSorterProjectV2>)

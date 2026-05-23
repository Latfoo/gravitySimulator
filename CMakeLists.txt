cmake_minimum_required(VERSION 3.14)
project(GravitySim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project sources (exclude benchmark which has its own main)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
list(REMOVE_ITEM SOURCES ${CMAKE_SOURCE_DIR}/src/benchmark.cpp)

# Third-party directories
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/include/imgui)
set(IMPLOT_DIR ${CMAKE_SOURCE_DIR}/include/implot)

# ImGui / ImPlot sources
list(APPEND SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl2.cpp
    ${IMPLOT_DIR}/implot.cpp
    ${IMPLOT_DIR}/implot_items.cpp
)

# Main executable
add_executable(GravitySim ${SOURCES})

# Headless benchmark executable (no GL / ImGui)
set(BENCHMARK_SOURCES
    src/benchmark.cpp
    src/physics.cpp
    src/NumericalIntegrators.cpp
    src/SceneLoader.cpp
)
add_executable(Benchmark ${BENCHMARK_SOURCES})

# Include paths
target_include_directories(GravitySim PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${IMPLOT_DIR}
)

target_include_directories(Benchmark PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Dependencies
find_package(glfw3 REQUIRED)

if(APPLE)
    target_link_libraries(GravitySim glfw "-framework OpenGL")
    target_link_libraries(Benchmark m)
else() # Linux
    find_package(OpenGL REQUIRED)
    target_link_libraries(GravitySim glfw OpenGL::GL GLU dl m pthread)
    target_link_libraries(Benchmark m pthread)
endif()

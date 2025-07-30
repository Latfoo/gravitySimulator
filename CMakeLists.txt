cmake_minimum_required(VERSION 3.14)
project(GravitySim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
include_directories(include)

add_executable(GravitySim ${SOURCES})

find_package(glfw3 REQUIRED)

if(APPLE)
    target_link_libraries(GravitySim
        glfw
        "-framework OpenGL"
    )
else()  # Linux
    find_package(OpenGL REQUIRED)
    target_link_libraries(GravitySim
        glfw
        OpenGL::GL
        GLU
        dl
        m
        pthread
    )
    endif()
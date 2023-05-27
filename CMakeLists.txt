cmake_minimum_required(VERSION 3.10)

project(minecraft)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Libraries
set(LIB_DIR ${PROJECT_SOURCE_DIR}/libs)

add_subdirectory(${LIB_DIR}/glfw)
add_subdirectory(${LIB_DIR}/glad)
add_subdirectory(${LIB_DIR}/glm)
add_subdirectory(${LIB_DIR}/stb_image)

# Executables
add_subdirectory(src)

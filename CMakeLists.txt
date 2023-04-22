cmake_minimum_required(VERSION 3.10)

project(minecraft)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-private-field -Wno-unused-local-typedefs -Wno-missing-braces -Wno-missing-field-initializers -Wno-unknown-pragmas -Wno-unknown-warni")

# Libraries
set(LIB_DIR ${PROJECT_SOURCE_DIR}/libs)

add_subdirectory(${LIB_DIR}/glfw)
add_subdirectory(${LIB_DIR}/glad)
add_subdirectory(${LIB_DIR}/glm)
add_subdirectory(${LIB_DIR}/stb_image)

# Executables
add_subdirectory(src)

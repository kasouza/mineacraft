cmake_minimum_required(VERSION 3.0)

project(stb_image)

add_library(stb_image STATIC stb_image.c)
target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

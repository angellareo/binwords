cmake_minimum_required(VERSION 2.8)
project(binWordsBuffer C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake.modules/")

set ( EXECUTABLE_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)


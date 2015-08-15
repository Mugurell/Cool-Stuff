cmake_minimum_required(VERSION 3.2)
project(Table_of_Read_Words)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} \
    -std=c11 -Wall -Werror -Wextra -pedantic")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

set(SOURCE_FILES main.c)
add_executable(Table_of_Read_Words ${SOURCE_FILES})
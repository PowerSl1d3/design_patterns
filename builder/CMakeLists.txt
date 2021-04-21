cmake_minimum_required(VERSION 3.17)
project(builder)

set(CMAKE_CXX_STANDARD 17)

add_executable(builder main.cpp)

target_compile_options(builder PUBLIC -Wall -Wextra -pedantic -Werror)
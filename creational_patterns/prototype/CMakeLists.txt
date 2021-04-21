cmake_minimum_required(VERSION 3.17)
project(prototype)

set(CMAKE_CXX_STANDARD 17)

add_executable(prototype main.cpp)

target_compile_options(prototype PUBLIC -Wall -Wextra -pedantic -Werror)
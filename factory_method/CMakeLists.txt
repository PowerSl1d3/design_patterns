cmake_minimum_required(VERSION 3.17)
project(factory_method)

set(CMAKE_CXX_STANDARD 17)

add_executable(factory_method main.cpp)

target_compile_options(factory_method PUBLIC -Wall -Wextra -pedantic -Werror)
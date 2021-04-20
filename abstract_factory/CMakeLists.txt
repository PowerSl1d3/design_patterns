cmake_minimum_required(VERSION 3.17)
project(abstract_factory)

set(CMAKE_CXX_STANDARD 17)

add_executable(abstract_factory main.cpp)

target_compile_options(abstract_factory PUBLIC -Wall -Wextra -pedantic -Werror)
cmake_minimum_required(VERSION 4.2)
project(interpreter_yuliia_shapoval)

set(CMAKE_CXX_STANDARD 20)

add_executable(interpreter_yuliia_shapoval main.cpp
        Lexer.cpp
        Lexer.h)

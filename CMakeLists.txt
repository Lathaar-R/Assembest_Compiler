cmake_minimum_required(VERSION 3.10)
project(ASBCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(header)

add_executable(ASBCompiler main.cpp src/CodeGenerator.cpp src/Lexer.cpp src/Parser.cpp)
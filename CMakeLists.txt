cmake_minimum_required(VERSION 3.17)
project(Funtime)

set(CMAKE_CXX_STANDARD 20)


set(CANDIL_PARSER_INCLUDES candil_parser/token.hpp)

add_executable(Funtime main.cpp ${CANDIL_PARSER_INCLUDES})
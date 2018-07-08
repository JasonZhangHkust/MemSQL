cmake_minimum_required(VERSION 3.10)
project(MemSQL)

set(CMAKE_CXX_STANDARD 11)

add_executable(MemSQL main.cpp Query.h Query.cpp TableField.h  TableField.cpp TableBase.h Table.h Table.cpp)
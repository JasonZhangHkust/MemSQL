cmake_minimum_required(VERSION 3.10)
project(MemSQL)

set(CMAKE_CXX_STANDARD 11)

add_executable(MemSQL main.cpp Query.h)
        #TableField.h TableBase.h Table.h)
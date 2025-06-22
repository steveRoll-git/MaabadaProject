cmake_minimum_required(VERSION 3.30)
project(MaabadaProject C)

set(CMAKE_C_STANDARD 90)

include_directories(preprocess/libs)

add_executable(MaabadaProject
        common/linked_list.c
        common/linked_list.h
        preprocess/libs/parser.c
        preprocess/libs/parser.h
        preprocess/macroprocessor.c)

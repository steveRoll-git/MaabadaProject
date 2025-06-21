cmake_minimum_required(VERSION 3.30)
project(MaabadaProject C)

set(CMAKE_C_STANDARD 90)

include_directories(preprocess/libs)

add_executable(MaabadaProject
        preprocess/libs/datatypes.h
        preprocess/libs/datatypes.c
        preprocess/libs/parser.c
        preprocess/libs/parser.h
        preprocess/macroprocessor.c)

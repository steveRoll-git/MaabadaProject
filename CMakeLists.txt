cmake_minimum_required(VERSION 3.30)
project(MaabadaProject C)

set(CMAKE_C_STANDARD 90)

include_directories(preprocess/libs)

add_executable(MaabadaProject
        datatypes/linked_list.c
        datatypes/linked_list.h
        preprocess/parser.c
        preprocess/parser.h
        preprocess/macroprocessor.c
        preprocess/macroprocessor.h
        external/data.h
        firstpass/firstpass.c
        external/utils.c
        external/utils.h
        firstpass/parser.c
        main.c
)

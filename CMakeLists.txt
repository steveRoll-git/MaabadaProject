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
        common/data.h
        firstpass/firstpass.c
        common/utils.c
        common/utils.h
        firstpass/parser.c
        firstpass/parser.h
        main.c
        datatypes/growing_array.c
        datatypes/growing_array.h
)

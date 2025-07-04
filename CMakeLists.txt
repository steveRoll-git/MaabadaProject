cmake_minimum_required(VERSION 3.30)
project(MaabadaProject C)

set(CMAKE_C_STANDARD 90)

include_directories(preprocess/libs)
include(CTest)

set(project_files
        datatypes/linked_list.c
        datatypes/linked_list.h
        preprocess/macroprocessor.c
        preprocess/macroprocessor.h
        common/data.h
        firstpass/firstpass.c
        common/utils.c
        common/utils.h
        firstpass/parser.c
        firstpass/parser.h
        datatypes/growing_array.c
        datatypes/growing_array.h
        datatypes/assembler.c
        datatypes/assembler.h
        firstpass/firstpass.h
)

enable_testing()

add_executable(MaabadaProject
        ${project_files}
        main.c
)

function(new_test testFile)
    string(REPLACE .c "" EXE_NAME ${testFile})
    add_executable(${EXE_NAME} ${project_files} tests/${testFile})
    add_test(NAME ${testFile} COMMAND ${EXE_NAME})
endfunction()

file(GLOB_RECURSE SRC_LIST "tests/*.c")

foreach (src ${SRC_LIST})
    string(REPLACE ${PROJECT_SOURCE_DIR}/tests/ "" ACTUAL_NAME ${src})
    new_test(${ACTUAL_NAME})
endforeach ()

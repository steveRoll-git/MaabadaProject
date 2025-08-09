cmake_minimum_required(VERSION 3.30)
project(MaabadaProject C)

set(CMAKE_C_STANDARD 90)

include_directories(preprocess/libs)
include(CTest)

set(project_files
        headers/codegen.h
        headers/context.h
        headers/data.h
        headers/errors.h
        headers/list.h
        headers/output.h
        headers/parser.h
        headers/preprocessor.h
        headers/table.h
        headers/utils.h
        source/codegen.c
        source/context.c
        source/errors.c
        source/list.c
        source/output.c
        source/parser.c
        source/preprocessor.c
        source/table.c
        source/utils.c
)

enable_testing()

add_executable(MaabadaProject
        ${project_files}
        source/assembler.c
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

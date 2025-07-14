cmake_minimum_required(VERSION 3.30)
project(MaabadaProject C)

set(CMAKE_C_STANDARD 90)

include_directories(preprocess/libs)
include(CTest)

set(project_files
        include/assembler.h
        include/codegen.h
        include/data.h
        include/errors.h
        include/list.h
        include/output.h
        include/parser.h
        include/preprocessor.h
        include/table.h
        include/utils.h
        source/assembler.c
        source/codegen.c
        source/data.c
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

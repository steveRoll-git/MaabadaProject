cmake_minimum_required(VERSION 3.30)
project(MaabadaProject C)

set(CMAKE_C_STANDARD 90)

include_directories(preprocess/libs)
include(CTest)

set(project_files
        include/assembler.h
        include/data.h
        include/errors.h
        include/firstpass.h
        include/list.h
        include/macroprocessor.h
        include/parser.h
        include/table.h
        source/assembler.c
        source/data.c
        source/firstpass.c
        source/list.c
        source/macroprocessor.c
        source/parser.c
        source/table.c
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

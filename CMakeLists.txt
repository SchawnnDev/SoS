cmake_minimum_required(VERSION 3.14)
project(sos C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(lib)

add_executable(
        sos

        main.c
        src/listIden.c
        src/utils.c

        include/listIden.h
        include/listRangeVar.h
        include/listTmp.h
        include/utils.h
        include/variable.h
)

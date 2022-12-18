cmake_minimum_required(VERSION 3.10.2)
project(sos C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(lib)

add_library(
        libsos

        STATIC

        src/listIden.c
        src/listRangeVar.c
        src/listTmp.c
        src/utils.c

        include/listIden.h
        include/listRangeVar.h
        include/listTmp.h
        include/utils.h
        include/variable.h)

target_link_libraries(libsos PUBLIC argtable3::argtable3 logc)
target_include_directories(libsos PUBLIC "${LIBS_DIR}/argtable3/src/" src include)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)

    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()


add_executable(sos main.c)

target_link_libraries(sos libsos)


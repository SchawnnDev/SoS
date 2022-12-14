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
        include/variable.h)

target_link_libraries(sos PRIVATE argtable3::argtable3 logc)
target_include_directories(sos PRIVATE "${LIBS_DIR}/argtable3/src/")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)

    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()



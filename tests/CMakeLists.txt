set(
        TEST_SRC_LIST
        "listIden-test.c"
        "listRangeVar-test.c"
        "compilation-test.c"
        "listInstructionMips-test.c"
)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    set(WARN_FLAGS "-Wall -Wextra -Wunused")
    string(APPEND CMAKE_C_FLAGS " ${WARN_FLAGS}")
    string(APPEND CMAKE_CXX_FLAGS " ${WARN_FLAGS}")
endif()

foreach(SRC IN LISTS TEST_SRC_LIST)
    get_filename_component(TARGET "${SRC}" NAME_WE)

    add_executable("${TARGET}" "${SRC}")
    target_link_libraries("${TARGET}" PRIVATE greatest libsos)
    target_include_directories("${TARGET}" PRIVATE ./)

    if(UNIX AND NOT APPLE)
        target_compile_definitions("${TARGET}" PRIVATE _GNU_SOURCE)
    endif()

    # Conditionally enable Code Coverage under GNU/Linux only
    if(CODE_COVERAGE AND UNIX AND (NOT APPLE))
        target_compile_options("${TARGET}" PRIVATE -fprofile-arcs -ftest-coverage)
        target_link_options("${TARGET}" PRIVATE -fprofile-arcs)
    endif()


    add_test(
            NAME "${TARGET}" COMMAND "${TARGET}"
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )

endforeach()
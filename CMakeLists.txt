cmake_minimum_required(VERSION 3.10.2)
project(sos C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(lib)

find_package(FLEX 2.6 REQUIRED)
find_package(BISON 3.0 REQUIRED)

set(SRC_DIR src)
set(BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(PARSER_DIR gen)
set(LEXER_OUT "${BIN_DIR}/lexer.c")
set(PARSER_OUT "${BIN_DIR}/parser.c")

# Create bin folder if not exists
add_custom_target(build-time-make-directory ALL COMMAND ${CMAKE_COMMAND} -E make_directory ${BIN_DIR})
file(MAKE_DIRECTORY ${BIN_DIR})

flex_target(LEXER "${PARSER_DIR}/lexer.lex" "${LEXER_OUT}" DEFINES_FILE "${BIN_DIR}/lexer.h")
bison_target(PARSER "${PARSER_DIR}/parser.y" "${PARSER_OUT}" DEFINES_FILE "${BIN_DIR}/parser.h" COMPILE_FLAGS "-v")
add_flex_bison_dependency(LEXER PARSER)

add_library(
        libsos

        STATIC

        src/compilation.c
        src/listIden.c
        src/listRangeVar.c
        src/listTmp.c
        src/utils.c
        src/listInstructionMips.c
        src/asm.c
        src/labels.c
        mips/header.asm.c
        mips/functions/atoi.asm.c
        mips/functions/bufferWrite.asm.c
        mips/functions/bufferLen.asm.c
        mips/functions.asm.c
        mips/functions/saveRegistersToStack.asm.c
        mips/functions/loadRegistersFromStack.asm.c
        "${LEXER_OUT}"
        "${PARSER_OUT}"


        include/compilation.h
        include/listIden.h
        include/listRangeVar.h
        include/listTmp.h
        include/utils.h
        include/variable.h
        include/boolExpr.h
        include/listInstructionMips.h
        include/asm.h
        include/labels.h
        mips/functions/atoi.asm.h
        mips/header.asm.h
        mips/functions/bufferWrite.asm.h
        mips/functions/bufferLen.asm.h
        mips/functions.asm.h
        mips/functions/saveRegistersToStack.asm.h
        mips/functions/loadRegistersFromStack.asm.h)

target_link_libraries(libsos PUBLIC argtable3::argtable3 logc)
target_include_directories(libsos PUBLIC "${LIBS_DIR}/argtable3/src/" src include mips mips/functions "${BIN_DIR}")

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)

    if (BUILD_TESTING)
        add_subdirectory(tests)
    endif ()
endif ()


add_executable(sos main.c)

target_link_libraries(sos libsos)


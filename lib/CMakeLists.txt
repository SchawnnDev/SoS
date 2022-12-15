set(LIBS_DIR "${CMAKE_SOURCE_DIR}/lib/" CACHE STRING "(Must be a cache variable, do not change)")

# Logc
add_library(logc STATIC "${LIBS_DIR}/log.c/src/log.c")
target_include_directories(logc PUBLIC "${LIBS_DIR}/log.c/src")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(logc PUBLIC "RXI_LOGC_DEFAULT_LEVEL=LOG_DEBUG")
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(logc PUBLIC "RXI_LOGC_DEFAULT_LEVEL=LOG_TRACE")
endif ()

# Greatest
add_library(greatest INTERFACE)
target_include_directories(greatest INTERFACE "${LIBS_DIR}/greatest/")

# Argtable3

set(ARGTABLE3_ENABLE_CONAN OFF CACHE BOOL "Enable Conan dependency manager")
set(ARGTABLE3_ENABLE_TESTS OFF CACHE BOOL "Enable unit tests")
set(ARGTABLE3_ENABLE_EXAMPLES OFF CACHE BOOL "Enable examples")

add_subdirectory(argtable3)
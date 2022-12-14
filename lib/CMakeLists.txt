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
add_subdirectory(argtable3)
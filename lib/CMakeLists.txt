# Logc
add_library(logc STATIC "${DEPS_DIR}/log.c/src/log.c")
target_include_directories(logc PUBLIC "${DEPS_DIR}/log.c/src")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(logc PUBLIC "RXI_LOGC_DEFAULT_LEVEL=LOG_DEBUG")
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(logc PUBLIC "RXI_LOGC_DEFAULT_LEVEL=LOG_TRACE")
endif ()

# Greatest
add_library(greatest INTERFACE)
target_include_directories(greatest INTERFACE "${DEPS_DIR}/greatest/")

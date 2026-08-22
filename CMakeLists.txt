cmake_minimum_required(VERSION 3.20)
project(klib
    VERSION 0.1.0
    LANGUAGES C
)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Sources
file(GLOB_RECURSE KLIB_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/types/*.c
)

# Headers
file(GLOB_RECURSE KLIB_HEADERS
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/types/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

# Library
add_library(klib STATIC ${KLIB_SOURCES})

if(WIN32)
    target_link_libraries(klib PUBLIC ws2_32)
endif()

target_include_directories(klib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/types>
        $<INSTALL_INTERFACE:include>
)

# Warnings
target_compile_options(klib PRIVATE
    $<$<C_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<C_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

# Tests
file(GLOB TEST_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/t/*.c
)
foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} PRIVATE klib)
    target_include_directories(${TEST_NAME}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/types
    )
endforeach()

# Installation
install(TARGETS klib
    ARCHIVE DESTINATION lib
)
install(FILES ${KLIB_HEADERS}
    DESTINATION include/klib
)

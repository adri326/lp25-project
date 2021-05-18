cmake_minimum_required(VERSION 3.0.0)

# Options
set(NO_TEST "false" CACHE STRING "true or false")

project(lp25-project)

set(SOURCES
    src/md5sum.c
    src/save.c
    src/scan.c
    src/tree.c
)

set(HEADERS
    src/defs.h
    src/md5sum.h
    src/save.h
    src/scan.h
    src/tree.h
)

find_package(OpenSSL REQUIRED)

add_executable(lp25-project src/main.c ${SOURCES} ${HEADERS})
target_link_libraries(lp25-project PUBLIC OpenSSL::Crypto)

if(NO_TEST STREQUAL "true")
    message("INFO: Tests disabled, enable them by setting NO_TEST to false: cmake .. -DNO_TEST=false")
    add_compile_definitions("NO_TEST")
else()
    message("INFO: Tests enabled, disable them by setting NO_TEST to true: cmake .. -DNO_TEST=true")
    add_compile_definitions("TEST")
    add_compile_definitions("TEST_DIR=\"${CMAKE_SOURCE_DIR}/test\"")
    project(lp25-project-test)

    set(TEST_SOURCES
        test/main.c
    )

    find_package(check REQUIRED)
    add_executable(lp25-project-test ${TEST_SOURCES} ${HEADERS})
    add_library(lp25-project-lib ${SOURCES} ${HEADERS})
    target_link_libraries(lp25-project-test PUBLIC check lp25-project-lib OpenSSL::Crypto)
endif()

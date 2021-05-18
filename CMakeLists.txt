cmake_minimum_required(VERSION 3.0.0)

project(lp25-project)

set(SOURCES
    src/main.c
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

add_executable(lp25-project ${SOURCES} ${HEADERS})

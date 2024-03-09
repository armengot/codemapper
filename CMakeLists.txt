cmake_minimum_required(VERSION 3.0)
project(codemapper)

set(GRAPHVIZ_INCLUDE_DIR /usr/local/include/graphviz/)
#   ___ ___ _____ 
#  / __|_ _|_   _|
# | (_ || |  | |  
#  \___|___| |_| 
# ---------------------------------------------------------------------------------------------------------
# Definir el script de CMake para generar el archivo gitinfo.h
set(GENERATE_GITINFO_SCRIPT ${CMAKE_SOURCE_DIR}/generate_gitinfo.cmake)

file(WRITE ${GENERATE_GITINFO_SCRIPT}
    "execute_process(COMMAND git describe --abbrev=0 --tags OUTPUT_VARIABLE VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)\n"
    "execute_process(COMMAND git rev-parse --short HEAD OUTPUT_VARIABLE HASH OUTPUT_STRIP_TRAILING_WHITESPACE)\n"
    "execute_process(COMMAND git log -1 --format='%ae' OUTPUT_VARIABLE LAST_AUTHOR_COMMIT OUTPUT_STRIP_TRAILING_WHITESPACE)\n"
    "file(WRITE ${CMAKE_SOURCE_DIR}/lib/gitinfo.h \"#ifndef GITINFO_H\\n\")\n"
    "file(APPEND ${CMAKE_SOURCE_DIR}/lib/gitinfo.h \"#define GITINFO_H\\n\")\n"
    "file(APPEND ${CMAKE_SOURCE_DIR}/lib/gitinfo.h \"\\n\")\n"
    "file(APPEND ${CMAKE_SOURCE_DIR}/lib/gitinfo.h \"#define VERSION \\\"${VERSION}\\\"\\n\")\n"
    "file(APPEND ${CMAKE_SOURCE_DIR}/lib/gitinfo.h \"#define HASH \\\"${HASH}\\\"\\n\")\n"
    "file(APPEND ${CMAKE_SOURCE_DIR}/lib/gitinfo.h \"#define LAST_AUTHOR_COMMIT \\\"${LAST_AUTHOR_COMMIT}\\\"\\n\")\n"
    "file(APPEND ${CMAKE_SOURCE_DIR}/lib/gitinfo.h \"\\n\")\n"
    "file(APPEND ${CMAKE_SOURCE_DIR}/lib/gitinfo.h \"#endif // GITINFO_H\\n\")\n"
)

# Agregar la regla de CMake para generar gitinfo.h antes de la construcción del proyecto
add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/lib/gitinfo.h
    COMMAND ${CMAKE_COMMAND} -P ${GENERATE_GITINFO_SCRIPT}
    DEPENDS ${GENERATE_GITINFO_SCRIPT}
    COMMENT "Generating git information header"
)

add_custom_target(generate_gitinfo_header DEPENDS ${CMAKE_SOURCE_DIR}/lib/gitinfo.h)
# ---------------------------------------------------------------------------------------------------------
# graphviz library usage
execute_process(COMMAND pkg-config libgvc --cflags OUTPUT_VARIABLE CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND pkg-config libgvc --libs OUTPUT_VARIABLE LDFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)

# include graphbiz and own libraries
include_directories(${GRAPHVIZ_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/lib/)
include_directories(${CMAKE_SOURCE_DIR}/lan/)

# main project source 
set(SOURCES exe/main.cpp)

# codemapper own libraries
add_library(cm_tools STATIC lib/tools.cpp)
add_library(cm_graph STATIC lib/cm_edge.cpp lib/cm_node.cpp lib/cm_graph.cpp)
add_library(cm_lan STATIC lan/cm_lan.cpp) 
add_library(cm_clan STATIC lan/cm_clan.cpp)

# main executable
add_executable(${PROJECT_NAME} ${SOURCES})
# first test for compilation and graphviz demo
add_executable(simple exe/simple.c)
add_executable(test exe/testlib.cpp)
# git
add_dependencies(${PROJECT_NAME} generate_gitinfo_header)

# compile options and linker commands
target_link_libraries(cm_clan PRIVATE cm_graph cm_lan cm_tools)
target_compile_options(${PROJECT_NAME} PUBLIC ${CFLAGS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LDFLAGS} PRIVATE cm_graph cm_clan cm_tools)
target_compile_options(test PUBLIC ${CFLAGS})
target_link_libraries(test PUBLIC ${LDFLAGS} PRIVATE cm_graph cm_tools)

# tests and examples too
target_compile_options(simple PUBLIC ${CFLAGS})
target_link_libraries(simple PUBLIC ${LDFLAGS})
target_compile_options(test PUBLIC ${CFLAGS})
target_link_libraries(test PUBLIC ${LDFLAGS})
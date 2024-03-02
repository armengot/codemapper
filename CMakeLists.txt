cmake_minimum_required(VERSION 3.0)
project(codemapper)

set(GRAPHVIZ_INCLUDE_DIR /usr/local/include/graphviz/)

# graphviz library usage
execute_process(COMMAND pkg-config libgvc --cflags OUTPUT_VARIABLE CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND pkg-config libgvc --libs OUTPUT_VARIABLE LDFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)

# include graphbiz and own libraries
include_directories(${GRAPHVIZ_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/lib/)

# main project source and simple tests for compilation
set(SOURCES exe/main.cpp)

# codemapper own libraries
add_library(cm_graph STATIC lib/cm_edge.cpp lib/cm_node.cpp lib/cm_graph.cpp)

# main executable
add_executable(${PROJECT_NAME} ${SOURCES})
# first test for compilation
add_executable(simple exe/simple.c)
add_executable(test exe/testlib.cpp)

# compile options and linker commands
target_compile_options(${PROJECT_NAME} PUBLIC ${CFLAGS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LDFLAGS} PRIVATE cm_graph)
target_compile_options(test PUBLIC ${CFLAGS})
target_link_libraries(test PUBLIC ${LDFLAGS} PRIVATE cm_graph)

# tests and examples too
target_compile_options(simple PUBLIC ${CFLAGS})
target_link_libraries(simple PUBLIC ${LDFLAGS})
target_compile_options(test PUBLIC ${CFLAGS})
target_link_libraries(test PUBLIC ${LDFLAGS})
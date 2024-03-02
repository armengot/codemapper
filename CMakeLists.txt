cmake_minimum_required(VERSION 3.0)
project(codemapper)

# steps to get graphviz as library
# git clone https://gitlab.com/graphviz/graphviz
# cd graphviz
# mkdir build
# cmake ..
# make
# make install
set(GRAPHVIZ_INCLUDE_DIR /usr/local/include/graphviz/)

# graphviz library usage
execute_process(COMMAND pkg-config libgvc --cflags OUTPUT_VARIABLE CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND pkg-config libgvc --libs OUTPUT_VARIABLE LDFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)

# include graphbiz and own libraries
include_directories(${GRAPHVIZ_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/lib/)

# main project source and simple test for compilation
set(SOURCES exe/main.cpp)
set(SIMPLE exe/simple.c)

# codemapper own libraries
add_library(cm_graph STATIC lib/cm_edge.cpp lib/cm_node.cpp lib/cm_graph.cpp)

# main executable
add_executable(${PROJECT_NAME} ${SOURCES})
# first test for compilation
add_executable(simple ${SIMPLE})

# compile options and linker commands
target_compile_options(${PROJECT_NAME} PUBLIC ${CFLAGS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LDFLAGS} PRIVATE cm_graph)

# test example too
target_compile_options(simple PUBLIC ${CFLAGS})
target_link_libraries(simple PUBLIC ${LDFLAGS})
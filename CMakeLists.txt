cmake_minimum_required(VERSION 3.0)
project(codemapper)

set(GRAPHVIZ_INCLUDE_DIR /usr/local/include/graphviz/)

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
add_library(cm_graph STATIC lib/cm_edge.cpp lib/cm_node.cpp lib/cm_graph.cpp)
add_library(cm_lan STATIC lan/cm_lan.cpp lib/tools.cpp) 
add_library(cm_clan STATIC lan/cm_clan.cpp)

# main executable
add_executable(${PROJECT_NAME} ${SOURCES})
# first test for compilation and graphviz demo
add_executable(simple exe/simple.c)
add_executable(test exe/testlib.cpp)

# compile options and linker commands
target_link_libraries(cm_clan PRIVATE cm_graph cm_lan)
target_compile_options(${PROJECT_NAME} PUBLIC ${CFLAGS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LDFLAGS} PRIVATE cm_graph cm_clan)
target_compile_options(test PUBLIC ${CFLAGS})
target_link_libraries(test PUBLIC ${LDFLAGS} PRIVATE cm_graph)

# tests and examples too
target_compile_options(simple PUBLIC ${CFLAGS})
target_link_libraries(simple PUBLIC ${LDFLAGS})
target_compile_options(test PUBLIC ${CFLAGS})
target_link_libraries(test PUBLIC ${LDFLAGS})
cmake_minimum_required(VERSION 3.0)
project(codemapper)

# git clone https://gitlab.com/graphviz/graphviz
# cd graphviz
# mkdir build
# cmake ..
# make
# make install
set(GRAPHVIZ_INCLUDE_DIR /usr/local/include/graphviz/)

execute_process(COMMAND pkg-config libgvc --cflags OUTPUT_VARIABLE CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND pkg-config libgvc --libs OUTPUT_VARIABLE LDFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)

include_directories(${GRAPHVIZ_INCLUDE_DIR})

set(SOURCES exe/main.cpp)
set(SIMPLE exe/simple.c)

add_library(cm_graph STATIC lib/cm_edge.cpp lib/cm_node.cpp lib/cm_graph.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
add_executable(simple ${SIMPLE})

target_compile_options(${PROJECT_NAME} PUBLIC ${CFLAGS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LDFLAGS})

target_compile_options(simple PUBLIC ${CFLAGS})
target_link_libraries(simple PUBLIC ${LDFLAGS})
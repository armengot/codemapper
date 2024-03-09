cmake_minimum_required(VERSION 3.0)
project(codemapper)
# ---------------------------------------------------------------------------------------------------------
set(GRAPHVIZ_INCLUDE_DIR /usr/local/include/graphviz/)
# ---------------------------------------------------------------------------------------------------------
string(ASCII 27 Esc)
set(RESETCOLOR "${Esc}[39m${Esc}[0m")
set(GREENCOLOR "${Esc}[92m")
set(REDCOLOR "${Esc}[91m")
set(REDBOLD "${Esc}[91m${Esc}[1m")
# ---------------------------------------------------------------------------------------------------------
#   ___ ___ _____ 
#  / __|_ _|_   _|
# | (_ || |  | |  
#  \___|___| |_| 
# ---------------------------------------------------------------------------------------------------------
find_package(Git REQUIRED)
set(GIT_COMMIT_HEADER_FILE "${CMAKE_SOURCE_DIR}/lib/gitinfo.h")
execute_process(COMMAND git log -1 --pretty=format:"%H" OUTPUT_VARIABLE GIT_HASH
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND hostname OUTPUT_VARIABLE HOSTNAME
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND date "+%F-%T" OUTPUT_VARIABLE GIT_TIME
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND git log -1 --pretty=format:%ae
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GITAUTHORLST
  OUTPUT_STRIP_TRAILING_WHITESPACE)    
execute_process(COMMAND git config user.email OUTPUT_VARIABLE GIT_USER_CXX
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_STRIP_TRAILING_WHITESPACE)
if("${GIT_USER_CXX}" STREQUAL "")
  message(STATUS "${REDCOLOR}Warning: Commits without ${REDBOLD}NAME${REDCOLOR} not allowed${RESETCOLOR}")
else()
  message(STATUS "Please ${REDCOLOR}check${RESETCOLOR} the GIT NAME recorded within the executable binary: ${GREENCOLOR}${GITAUTHORLST}${RESETCOLOR}")
endif()
#execute_process(COMMAND git fetch --tags)
execute_process(COMMAND git describe --tags --abbrev=0 OUTPUT_VARIABLE GIT_LASTAG
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_STRIP_TRAILING_WHITESPACE)
file(WRITE ${GIT_COMMIT_HEADER_FILE} "// Get GIT info\n"
    "#ifndef GITINFO_H\n"
    "#define GITINFO_H\n"
    "#define GITAUTHORID \"${GITAUTHORLST}\"\n"
    "#define GIT_MACHINE \"${HOSTNAME}\"\n"
    "#define GIT_DTETIME \"${GIT_TIME}\"\n"
    "#define GIT_LNGHASH ${GIT_HASH}\n"    
    "#define GIT_USERCXX \"${GIT_USER_CXX}\"\n"
    "#define GIT_OFFICIAL_VERSION \"${GIT_LASTAG}\"\n"
    "#endif\n")
add_custom_target(git_commit_header_target DEPENDS ${GIT_COMMIT_HEADER_FILE})
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
add_dependencies(${PROJECT_NAME} git_commit_header_target)

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
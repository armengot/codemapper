cmake_minimum_required(VERSION 3.0)
# ---------------------------------------------------------------------------------------------------------
project(codemapper)
set(${PROJECT_NAME}_AUTHOR "Marcelo Armengot")
set(${PROJECT_NAME}_URL "https://github.com/armengot/codemapper")
# ---------------------------------------------------------------------------------------------------------
set(GUI_CODEMAPPER cmgui)
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
if("${GIT_LASTAG}" STREQUAL "")
  message(STATUS "${REDCOLOR}Warning: unknown version")
else()
  message(STATUS "Checking the compilation of ${REDCOLOR}${GIT_LASTAG}${RESETCOLOR} release, visit our repo if want more info about other releases.")
endif()  
file(WRITE ${GIT_COMMIT_HEADER_FILE} "// Get GIT info\n"
    "#ifndef GITINFO_H\n"
    "#define GITINFO_H\n"
    "#define GITAUTHORID \"${GITAUTHORLST}\"\n"
    "#define GIT_MACHINE \"${HOSTNAME}\"\n"
    "#define GIT_DTETIME \"${GIT_TIME}\"\n"
    "#define GIT_LNGHASH ${GIT_HASH}\n"    
    "#define GIT_USERCXX \"${GIT_USER_CXX}\"\n"
    "#define GIT_OFFICIAL_VERSION \"${GIT_LASTAG}\"\n"
    "#define CODEMAPPER_AUTHOR \"${${PROJECT_NAME}_AUTHOR}\"\n"
    "#define CODEMAPPER_URL \"${${PROJECT_NAME}_URL}\"\n"
    "#endif\n")
add_custom_target(git_commit_header_target DEPENDS ${GIT_COMMIT_HEADER_FILE})
# ---------------------------------------------------------------------------------------------------------
# Qt5 requirements to compile the GUI side
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5 COMPONENTS Svg REQUIRED)
# ---------------------------------------------------------------------------------------------------------
# graphviz library usage
execute_process(COMMAND pkg-config libgvc --cflags OUTPUT_VARIABLE CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND pkg-config libgvc --libs OUTPUT_VARIABLE LDFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
# ---------------------------------------------------------------------------------------------------------
# include graphbiz and own libraries
# ---------------------------------------------------------------------------------------------------------
include_directories(${GRAPHVIZ_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/lib/)
include_directories(${CMAKE_SOURCE_DIR}/lan/)
include_directories(${CMAKE_SOURCE_DIR}/qcm/)
include_directories(${Qt5Svg_INCLUDE_DIRS})
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# Meta-Object Compiler (MOC) : See Qt5 documentation: https://doc.qt.io/qt-5/moc.html
# CMAKE AUTOMOC ON = no linker problems between Q_OBJECT qcanvas-cqtgui
set(CMAKE_AUTOMOC ON)
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)
# main project source 
set(SOURCES exe/main.cpp)
# ---------------------------------------------------------------------------------------------------------
# codemapper own libraries
# ---------------------------------------------------------------------------------------------------------
add_library(cm_tools STATIC lib/tools.cpp)
add_library(cm_graph STATIC lib/cm_edge.cpp lib/cm_node.cpp lib/cm_graph.cpp)
add_library(cm_lan STATIC lan/cm_lan.cpp ) 
add_library(cm_clan STATIC lan/cm_clan.cpp)
add_library(cm_pylan STATIC lan/cm_pylan.cpp)
add_library(cm_qcanvas STATIC qcm/qcanvas.cpp)
add_library(cm_qtlib STATIC qcm/cm_qt5_gui.cpp)
# ---------------------------------------------------------------------------------------------------------
# main executable tool command line
add_executable(${PROJECT_NAME} ${SOURCES})
# ---------------------------------------------------------------------------------------------------------
# first test for compilation and graphviz demo
add_executable(simple exe/simple.c)
add_executable(test exe/testlib.cpp)
# ---------------------------------------------------------------------------------------------------------
# main GUI (beta) in progress
add_executable(${GUI_CODEMAPPER} gui/main.cpp)
# ---------------------------------------------------------------------------------------------------------
# git
add_dependencies(${PROJECT_NAME} git_commit_header_target)
add_dependencies(${GUI_CODEMAPPER} git_commit_header_target)
# ---------------------------------------------------------------------------------------------------------
# compile options and linker commands
target_compile_options(cm_tools PUBLIC ${CFLAGS})
target_link_libraries(cm_tools PUBLIC ${LDFLAGS})
target_link_libraries(cm_clan PRIVATE cm_graph cm_lan cm_tools stdc++fs)
target_link_libraries(cm_pylan PRIVATE cm_graph cm_lan cm_tools stdc++fs) # stdc++fs links experimental/filesystem library
target_compile_options(${PROJECT_NAME} PUBLIC ${CFLAGS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LDFLAGS} PRIVATE cm_graph cm_clan cm_pylan cm_tools)
target_compile_options(test PUBLIC ${CFLAGS})
target_link_libraries(test PUBLIC ${LDFLAGS} PRIVATE cm_graph cm_tools)
target_compile_options(${GUI_CODEMAPPER} PUBLIC ${CFLAGS})
# ---------------------------------------------------------------------------------------------------------
# Qt5 link
target_link_libraries(cm_qcanvas Qt5::Gui)
target_link_libraries(cm_qcanvas Qt5::Widgets)
target_link_libraries(cm_qtlib cm_qcanvas)
target_link_libraries(cm_qtlib Qt5::Widgets)
target_link_libraries(cm_qtlib Qt5::Gui)
target_link_libraries(cm_qtlib Qt5::Core)
target_link_libraries(cm_qtlib ${Qt5Svg_LIBRARIES})
target_link_libraries(${GUI_CODEMAPPER} PUBLIC ${LDFLAGS} cm_qtlib cm_qcanvas cm_graph cm_clan cm_pylan cm_tools)
# ---------------------------------------------------------------------------------------------------------
# tests and examples too
target_compile_options(simple PUBLIC ${CFLAGS})
target_link_libraries(simple PUBLIC ${LDFLAGS})
target_compile_options(test PUBLIC ${CFLAGS})
target_link_libraries(test PUBLIC ${LDFLAGS})
# ---------------------------------------------------------------------------------------------------------
#  ___  ___ ___ 
# |   \| __| _ )
# | |) | _|| _ \
# |___/|___|___/    
# ---------------------------------------------------------------------------------------------------------
# .deb package generation, steps:
# (1) cmake .. # inside build folder
# (2) cpack -G DEB
if (GIT_LASTAG MATCHES "^DEV")
  string(SUBSTRING ${GIT_LASTAG} 4 2 VERSION_MAJOR)
  string(SUBSTRING ${GIT_LASTAG} 7 2 VERSION_MINOR)
  string(SUBSTRING ${GIT_LASTAG} 10 2 VERSION_PATCH)
  set(DEB_TAG "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-beta")
else ()
  string(SUBSTRING ${GIT_LASTAG} 4 2 VERSION_MAJOR)
  string(SUBSTRING ${GIT_LASTAG} 7 2 VERSION_MINOR)
  string(SUBSTRING ${GIT_LASTAG} 10 2 VERSION_PATCH)
  if (VERSION_PATCH STREQUAL "")
    set(DEB_TAG "${VERSION_MAJOR}.${VERSION_MINOR}-master")
  else ()
    set(DEB_TAG "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-master")
  endif ()
endif ()
# out pack folder
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/pck")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${DEB_TAG})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "graphviz graphs automatic generation from source code projects")
set(CPACK_PACKAGE_CONTACT "${${PROJECT_NAME}_AUTHOR} <${GIT_USER_CXX}>")
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
else()
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
endif()
# empty postinst and prerm files
execute_process(COMMAND mkdir -p "${CPACK_PACKAGE_DIRECTORY}")
execute_process(COMMAND mkdir -p "${CPACK_PACKAGE_DIRECTORY}/deb")
execute_process(COMMAND touch "${CPACK_PACKAGE_DIRECTORY}/deb/postinst")
execute_process(COMMAND touch "${CPACK_PACKAGE_DIRECTORY}/deb/prerm")
execute_process(COMMAND chmod +x "${CPACK_PACKAGE_DIRECTORY}/deb/postinst")
execute_process(COMMAND chmod +x "${CPACK_PACKAGE_DIRECTORY}/deb/prerm")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CPACK_PACKAGE_DIRECTORY}/deb/postinst;${CPACK_PACKAGE_DIRECTORY}/deb/prerm")
# DEPS COMPLETE TREE
# ------------------
set(CM_DEPS libgvc6)
set(${CM_DEPS} ${CM_DEPS},gcc-snapshot)
set(${CM_DEPS} ${CM_DEPS},libcgraph6)
set(${CM_DEPS} ${CM_DEPS},lib32stdc++6)
set(${CM_DEPS} ${CM_DEPS},gcc-snapshot)
set(${CM_DEPS} ${CM_DEPS},libc6)
#set(${CM_DEPS} ${CM_DEPS},bison)
#set(${CM_DEPS} ${CM_DEPS},flex)
set(${CM_DEPS} ${CM_DEPS},libltdl7)
set(${CM_DEPS} ${CM_DEPS},zlib1g)
set(${CM_DEPS} ${CM_DEPS},libpathplan4)
set(${CM_DEPS} ${CM_DEPS},libcdt5)
set(${CM_DEPS} ${CM_DEPS},libxdot4)
set(${CM_DEPS} ${CM_DEPS},libexpat1)
# ------------------------------------------------------
set(CPACK_DEBIAN_PACKAGE_DEPENDS ${CM_DEPS})
# ------------------------------------------------------
install(DIRECTORY ${CMAKE_INSTALL_PREFIX}/ DESTINATION /usr/local/${PROJECT_NAME})
set(CPACK_GENERATOR "DEB")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY FALSE)
include(CPack)
# ---------------------------------------------------------------------------------------------------------
# ==============================================================================================================================
#  __  __   _   _  _____ ___ ___ _    ___    ___ ___  __  __ __  __ ___ _  _ _____ ___ 
# |  \/  | /_\ | |/ / __| __|_ _| |  | __|  / __/ _ \|  \/  |  \/  | __| \| |_   _/ __|
# | |\/| |/ _ \| ' <| _|| _| | || |__| _|  | (_| (_) | |\/| | |\/| | _|| .` | | | \__ \
# |_|  |_/_/ \_\_|\_\___|_| |___|____|___|  \___\___/|_|  |_|_|  |_|___|_|\_| |_| |___/
# ==============================================================================================================================
add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND echo -n
    COMMENT "${GREENCOLOR}${PROJECT_NAME}${RESETCOLOR} is just command line tool to parse source code projects."
    VERBATIM
)
add_custom_command(TARGET test
    POST_BUILD
    COMMAND echo -n
    COMMENT "${GREENCOLOR}test${RESETCOLOR} is a example of cm_graph library usage."
    VERBATIM
)
add_custom_command(TARGET ${GUI_CODEMAPPER}
    POST_BUILD
    COMMAND echo -n
    COMMENT "${GREENCOLOR}${GUI_CODEMAPPER}${RESETCOLOR} is a Qt5 frontend to render graphs parsed by codemapper libs and edit the result (beta, in progress)."
    VERBATIM
)
# ---------------------------------------------------------------------------------------------------------

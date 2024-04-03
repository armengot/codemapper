# Requirements details

## Introduction

The ```CMakeLists.txt``` file available needs some requirements in your system to compile ```codemapper```
```
set(checking_paths_unix CMAKE_C_COMPILER CMAKE_CXX_COMPILER CMAKE_MAKE_PROGRAM Qt5_DIR GRAPHVIZ_INCLUDE_DIR)
set(checking_paths_windows ${checking_paths_unix} GRAPHVIZ_LIBRARY_DIR PKGCONFIG_WIN)
```
If everything is OK your cmake call will be:

### Linux

Call cmake with:
```
cmake ..
```
And obtain something like:
- [Ok] CMAKE_C_COMPILER: /usr/bin/cc
- [Ok] CMAKE_CXX_COMPILER: /usr/bin/c++
- [Ok] CMAKE_MAKE_PROGRAM: /usr/bin/gmake
- [Ok] Qt5_DIR: /usr/lib/x86_64-linux-gnu/cmake/Qt5/
- [Ok] GRAPHVIZ_INCLUDE_DIR: /usr/local/include/graphviz/

If everything was found, call make:
```
make
```

### Win32/64

In windows version I usually did (inside ```C:\~\~\codemapper\build>``` folder):
```
cmake -DCMAKE_MAKE_PROGRAM=C:/msys64/mingw32.exe -G "MinGW Makefiles" -Wno-dev ..
```
And obtain:
- [Ok] CMAKE_C_COMPILER: C:/msys64/ucrt64/bin/gcc.exe
- [Ok] CMAKE_CXX_COMPILER: C:/msys64/ucrt64/bin/g++.exe
- [Ok] CMAKE_MAKE_PROGRAM: C:/msys64/mingw32.exe
- [Ok] Qt5_DIR: C:/Qt/Qt5.12.12/5.12.12/mingw73_64/lib/cmake/Qt5
- [Ok] GRAPHVIZ_INCLUDE_DIR: C:/Users/marmengot/dev/Graphviz-10.0.1-win64/include/graphviz
- [Ok] GRAPHVIZ_LIBRARY_DIR: C:/Users/marmengot/dev/Graphviz-10.0.1-win64/lib
- [Ok] PKGCONFIG_WIN: C:/pkg-config-lite-0.28-1/bin/pkg-config.exe

If everything was found, call make:
```
C:\make-3.81-bin\bin\make.exe
```

## Solving requirements to compile Windows version

### (1) GCC for Windows
For tests I used [MSYS 64](https://www.msys2.org/) on Windows and [include](https://www.computerhope.com/issues/ch000549.htm) it in the path environment variable.

### (2) Install CMake
On windows download [cmake](https://cmake.org/download/). And check [Ninja](https://github.com/ninja-build/ninja/releases) is available or copy in the same path than cmake.

### (3) Make
Other Windows unexistent tools can be downloaded from [sourceforge](https://sourceforge.net/projects/gnuwin32/files/) as ```make.exe``` for Windows.

### (4) Qt5
The ```codemapper``` project is a set of tools and libraries, may be you only need the basics, but if you want use the GUI tool (```cmgui```) you will need Qt5 libraries in your system.
For test this version I downloaded ```qt-opensource-windows-x86-5.12.12.exe``` for Windows from Qt [web](https://www.qt.io/offline-installers).

### (5) Graphviz
In Linux versions, install ```graphviz``` package is available from ```apt-get``` (or whatever package manager) utilities.
In Windows I downloaded ```windows_10_cmake_Release_Graphviz-10.0.1-win64.zip``` file from [Graphviz](https://graphviz.org/download/) pages and unzipped next to my sources.
Headers and dynamic libraries will be required by ```codemapper```.

### (6) Pkg-config
Can be found in [sourceforge](https://sourceforge.net/projects/pkgconfiglite/) too.

## Linux

The previous packages lists are usual in most of Linux distribution, and can be installed installing packages like:

```
graphviz qt5 build-essentials flex bison 
```



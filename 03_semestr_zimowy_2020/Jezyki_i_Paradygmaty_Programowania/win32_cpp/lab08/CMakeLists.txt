
cmake_minimum_required ( VERSION 3.1 )

project ( Lab08 CXX )


# Zależności (Biblioteki OpenGL)
find_package ( OpenGL REQUIRED )


# Pliki nagłówkowe
include_directories (

    ./include
    ./libs/GLFW/include
    ${OPENGL_INCLUDE_DIRS}
)


# Standard "c++11" (obsługa wątków)
set (
    CMAKE_CXX_FLAGS "-std=gnu++0x -U__STRICT_ANSI__"
)


# Biblioteki statyczne (MinGW)
set (

    CMAKE_CXX_STANDARD_LIBRARIES
    "-static-libgcc -static-libstdc++"
)


# Kompilacja w systemie Windows
set (

    CMAKE_EXE_LINKER_FLAGS
    "-Wl,--subsystem,windows"
)


# Pliki źródłowe
add_executable (

    Lab08 WIN32

    ./src/Lab08/PhilosophersCore.cpp
    ./src/Lab08/PhilosophersLogic.cpp
    ./src/Lab08/PhilosophersDrawing.cpp
)


# Ustalenie katalogu docelowego dla pliku wykonywalnego
# oraz wymuszenie kompilacji programu 32-bitowego
set_target_properties (

    Lab08 PROPERTIES

    RUNTIME_OUTPUT_DIRECTORY ../bin/

    COMPILE_FLAGS "-m32"
    LINK_FLAGS "-m32"
)


# Biblioteki dynamiczne
target_link_libraries (

    Lab08

    ${CMAKE_CURRENT_SOURCE_DIR}/libs/GLFW/libglfw3.a
    ${OPENGL_LIBRARIES}
)

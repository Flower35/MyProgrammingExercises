
cmake_minimum_required ( VERSION 3.0 )

project ( Tetris )

# Zależności
find_package(OpenGL REQUIRED)

# Pliki nagłówkowe
include_directories (

    ./include
    ./libs/GLFW/include
    ${OPENGL_INCLUDE_DIRS}
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

    Tetris
    WIN32

    ./src/Tetris/TetrisCore.cpp
    ./src/Tetris/TetrisGame.cpp
    ./src/Tetris/TetrisLogic.cpp
    ./src/Tetris/TetrisDrawing.cpp
)


# Ustalenie katalogu docelowego dla pliku wykonywalnego
# oraz wymuszenie kompilacji programu 32-bitowego
set_target_properties (

    Tetris PROPERTIES

    RUNTIME_OUTPUT_DIRECTORY ../bin/

    COMPILE_FLAGS "-m32"
    LINK_FLAGS "-m32"
)


# Biblioteki dynamiczne
target_link_libraries (

    Tetris

    ${CMAKE_CURRENT_SOURCE_DIR}/libs/GLFW/libglfw3.a
    ${OPENGL_LIBRARIES}
)

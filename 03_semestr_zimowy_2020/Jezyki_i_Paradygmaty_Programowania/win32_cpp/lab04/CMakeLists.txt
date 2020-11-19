
cmake_minimum_required ( VERSION 3.0 )

project ( Lab04 )


# Pliki nag³ówkowe
include_directories (

    ./include
)


# Biblioteki statyczne
set (

    CMAKE_CXX_STANDARD_LIBRARIES
    "-static-libgcc -static-libstdc++"
)


# Kompilacja w systemie Windows
# (tryb konsoli zamiast trybu okienkowego)
set (

    CMAKE_EXE_LINKER_FLAGS
    "-Wl,--subsystem,console"
)


# Pliki Ÿród³owe
add_executable (

    Lab04
    WIN32

    ./src/Lab04/Lab04.cpp
    ./src/Lab04/MyException.cpp
    ./src/Lab04/Matrix.cpp
)

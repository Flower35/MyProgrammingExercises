
cmake_minimum_required ( VERSION 3.0 )

project ( Lab05 )


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

    Lab05
    WIN32

    ./src/Lab05/Lab05.cpp
    ./src/Lab05/MyException.cpp
    ./src/Lab05/Matrix.cpp
)


# Wymuszenie kompilacji programu 32-bitowego
set_target_properties (

    Lab05 PROPERTIES
    COMPILE_FLAGS "-m32"
    LINK_FLAGS "-m32"
)

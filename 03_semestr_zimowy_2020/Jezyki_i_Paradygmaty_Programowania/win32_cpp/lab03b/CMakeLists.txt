
cmake_minimum_required ( VERSION 3.0 )

project ( Lab03b )


# Pliki nag³ówkowe
include_directories (

    ./include
    ./libs/sqlite3/include
)


# Kompilacja w systemie Windows (tryb konsolowy)
set (

    CMAKE_EXE_LINKER_FLAGS
    "-Wl,--subsystem,console"
)


# Pliki Ÿród³owe (".cpp" -> ".exe")
add_executable (

    Lab03b
    WIN32

    ./src/Lab03b/Lab03b.cpp
    ./src/Lab03b/MyException.cpp
    ./src/Lab03b/Matrix.cpp
    ./src/Lab03b/MatrixDatabase.cpp
)


# Wymuszenie kompilacji programu 32-bitowego
set_target_properties (

    Lab03b PROPERTIES
    COMPILE_FLAGS "-m32"
    LINK_FLAGS "-m32"
)


# Biblioteki dynamiczne (".lib" -> ".dll")
target_link_libraries (

    Lab03b

    ${CMAKE_CURRENT_SOURCE_DIR}/libs/sqlite3/sqlite3.lib
)

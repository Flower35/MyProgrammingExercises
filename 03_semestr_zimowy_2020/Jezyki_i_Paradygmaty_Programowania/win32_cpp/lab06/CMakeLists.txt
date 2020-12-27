
cmake_minimum_required ( VERSION 3.0 )

project ( Lab06 )


# Pliki nagłówkowe
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


# Pliki źródłowe
add_executable (

    Lab06
    WIN32

    ./src/Lab06/Tests.cpp
    ./src/Lab06/Lab06.cpp
    ./src/Lab06/HRMSException.cpp
    ./src/Lab06/HRMS.cpp
    ./src/Lab06/HRMS_Employees.cpp
    ./src/Lab06/HRMS_DepPosSal.cpp
    ./src/Lab06/HRMS_SortPrint.cpp
    ./src/Lab06/Employee.cpp
    ./src/Lab06/IndexedText.cpp
)


# Ustalenie katalogu docelowego dla pliku wykonywalnego
# oraz wymuszenie kompilacji programu 32-bitowego
set_target_properties (

    Lab06 PROPERTIES

    RUNTIME_OUTPUT_DIRECTORY ../bin/

    COMPILE_FLAGS "-m32"
    LINK_FLAGS "-m32"
)

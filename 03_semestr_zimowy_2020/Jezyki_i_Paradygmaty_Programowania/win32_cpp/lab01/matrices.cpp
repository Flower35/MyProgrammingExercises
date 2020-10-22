/****************************************************************
  [LAB 01, ZADANIE 05]
    Napisz program, który doda do siebie
    dwie macierze o wymiarach 2 x 3.

****************************************************************/

#include <iostream>
#include <iomanip>
#include <string>
#include <new>

#include <cstdint>
#include <cassert>


////////////////////////////////////////////////////////////////
// DEFINICJE
////////////////////////////////////////////////////////////////

#define MATRIX_ROWS 2
#define MATRIX_COLS 3
#define MATRIX_COUNT 2

#define PRINTING_PRECISION 3
#define PRINTING_WIDTH (10 + 1 + PRINTING_PRECISION)


////////////////////////////////////////////////////////////////
// Pobieranie wartoœci liczbowej
// (zmiennoprzecinkowej) od u¿ytkownika
////////////////////////////////////////////////////////////////
bool get_value_from_stdin(float &result)
{
    std::string test;

    try
    {
        std::cout << ">> ";

        std::getline(std::cin, test);

        result = std::stof(test);
    }
    catch (std::exception)
    {
        return false;
    }

    return true;
}


////////////////////////////////////////////////////////////////
// Wydrukowanie odstêpu na ekranie
////////////////////////////////////////////////////////////////
void print_dashed_line()
{
    std::cout.width(64);
    std::cout.fill('-');

    std::cout << "" << std::endl;

    std::cout.fill(' ');
}


////////////////////////////////////////////////////////////////
// Funkcja wyœwietlaj¹ca wiersz dowolnej macierzy na ekranie
////////////////////////////////////////////////////////////////
void matrix_show_row(float* matrix_row, int32_t cols, int32_t pos)
{
    int32_t col;

    if ((nullptr != matrix_row) && (cols > 0))
    {
        for (col = 0; col < cols; col++)
        {
            std::cout << " [";

            if (col < pos)
            {
                std::cout.precision(PRINTING_PRECISION);
                std::cout.width(PRINTING_WIDTH);
                std::cout.setf(std::ios::fixed);

                std::cout << matrix_row[col];
            }
            else
            {
                std::cout.width(PRINTING_WIDTH);

                if (col > pos)
                {
                    std::cout.fill(' ');
                }
                else
                {
                    std::cout.fill('x');
                }

                std::cout << "";
            }

            std::cout << ']';
        }

        std::cout << std::endl;
    }
}


////////////////////////////////////////////////////////////////
//  Funkcja wyœwietlaj¹ca dowoln¹ macierz na ekranie
////////////////////////////////////////////////////////////////
void matrix_show(float** matrix, int32_t rows, int32_t cols, int32_t elements)
{
    int32_t row;

    if ((nullptr != matrix) && (rows > 0) && (cols > 0))
    {
        for (row = 0; row < rows; row++)
        {
            matrix_show_row(matrix[row], cols, elements);

            if (elements >= 0)
            {
                elements -= cols;
            }
        }
    }
}


////////////////////////////////////////////////////////////////
// Funkcja wype³niaj¹ca dowoln¹ macierz
////////////////////////////////////////////////////////////////
bool matrix_fill(float** matrix, int32_t rows, int32_t cols)
{
    int32_t row, col, elements = 0;

    if ((nullptr != matrix) && (rows > 0) && (cols > 0))
    {
        for (row = 0; row < rows; row++)
        {
            for (col = 0; col < cols; col++)
            {
                matrix_show(matrix, rows, cols, elements);

                std::cout << std::endl;

                if (!get_value_from_stdin(matrix[row][col]))
                {
                    return false;
                }

                std::cout << std::endl;

                elements++;
            }
        }

        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////////
// Funkcja wype³niaj¹ca wiele macierzy
////////////////////////////////////////////////////////////////
bool matrix_fill_multiple(float** matrices[], int32_t size, int32_t rows, int32_t cols)
{
    int32_t a;

    for (a = 0; a < size; a++)
    {
        if (!matrix_fill(matrices[a], rows, cols))
        {
            return false;
        }
    }

    return true;
}


////////////////////////////////////////////////////////////////
// Funkcja dodaje do siebie dwie macierze.
// Wynik zapisywany w pierwszej macierzy (nadpisanie zawartoœci!)
////////////////////////////////////////////////////////////////
void matrix_add(float** matrix_a, float** matrix_b, int32_t rows, int32_t cols)
{
    int32_t row, col;

    if ((nullptr != matrix_a) && (nullptr != matrix_b))
    {
        for (row = 0; row < rows; row++)
        {
            if ((nullptr != matrix_a[row]) && (nullptr != matrix_b[row]))
            {
                for (col = 0; col < cols; col++)
                {
                    matrix_a[row][col] += matrix_b[row][col];
                }
            }
        }
    }
}


////////////////////////////////////////////////////////////////
// Wykasowanie pamiêci zaalokowanej dla macierzy
////////////////////////////////////////////////////////////////
void matrix_destroy(float** matrix, int32_t rows)
{
    int32_t row;

    if (nullptr != matrix)
    {
        for (row = 0; row < rows; row++)
        {
            if (nullptr != matrix[row])
            {
                delete[](matrix[row]);
            }
        }

        delete[](matrix);
    }
}


////////////////////////////////////////////////////////////////
// Dynamiczna alokacja pamiêci dla jednej macierzy
////////////////////////////////////////////////////////////////
float** matrix_create(int32_t rows, int32_t cols)
{
    int32_t row;
    float** matrix = nullptr;

    if ((rows > 0) && (cols > 0))
    {
        try
        {
            matrix = new float* [rows];

            for (row = 0; row < rows; row++)
            {
                matrix[row] = nullptr;
            }

            for (row = 0; row < rows; row++)
            {
                matrix[row] = new float [cols];
            }
        }
        catch (std::bad_alloc)
        {
            matrix_destroy(matrix, rows);
            matrix = nullptr;
        }
    }

    return matrix;
}


////////////////////////////////////////////////////////////////
// Dynamiczna alokacja pamiêci dla wielu macierzy
////////////////////////////////////////////////////////////////
bool matrix_create_multiple(float** matrices[], int32_t size, int32_t rows, int32_t cols)
{
    int32_t a;

    for (a = 0; a < size; a++)
    {
        matrices[a] = nullptr;
    }

    for (a = 0; a < size; a++)
    {
        if (nullptr == (matrices[a] = matrix_create(rows, cols)))
        {
            return false;
        }
    }

    return true;
}


////////////////////////////////////////////////////////////////
// POCZ¥TEK PROGRAMU
////////////////////////////////////////////////////////////////
int32_t main()
{
    int32_t a, result = 0;
    float** matrices[MATRIX_COUNT];

    /********************************/
    /* Przygotowanie miejsca na dane. */

    assert((MATRIX_COUNT >= 2) && "Nieprawidlowa definicja liczby macierzy!");

    if (matrix_create_multiple(matrices, MATRIX_COUNT, MATRIX_ROWS, MATRIX_COLS))
    {
        print_dashed_line();
        std::cout << "Ten program zsumuje ze soba dwie macierze o wymiarach 2 x 3." << std::endl;
        print_dashed_line();

        std::cout << std::endl;

        /********************************/
        /* Poproszenie u¿ytkownika o wype³nienie macierzy. */
        /* Wyœwietlenie ich na ekranie. */

        if (matrix_fill_multiple(matrices, MATRIX_COUNT, MATRIX_ROWS, MATRIX_COLS))
        {
            print_dashed_line();

            for (a = 0; a < MATRIX_COUNT; a++)
            {
                std::cout << std::endl;
                matrix_show(matrices[a], MATRIX_ROWS, MATRIX_COLS, (MATRIX_ROWS * MATRIX_COLS));
            }

            /********************************/
            /* Dodanie do siebie dwóch macierzy i wyœwietlenie wyniku operacji. */

            matrix_add(matrices[0], matrices[1], MATRIX_ROWS, MATRIX_COLS);

            std::cout << std::endl;
            matrix_show(matrices[0], MATRIX_ROWS, MATRIX_COLS, (MATRIX_ROWS * MATRIX_COLS));

            std::cout << std::endl;
            std::cout << "Dzieki. Do zobaczenia!" << std::endl;
        }
        else
        {
            std::cout << std::endl;
            std::cout << "Podano niepoprawne dane! Do widzenia." << std::endl;

            result = 1;
        }
    }
    else
    {
        std::cerr << "Niewystarczajaca ilosc pamieci na uruchomienie tej aplikacji! :(" << std::endl;

        result = 1;
    }

    /********************************/
    /* Zwolnienie pamiêci i zamkniêcie programu. */

    for (a = 0; a < MATRIX_COUNT; a++)
    {
        matrix_destroy(matrices[a], MATRIX_ROWS);
    }

    return result;
}

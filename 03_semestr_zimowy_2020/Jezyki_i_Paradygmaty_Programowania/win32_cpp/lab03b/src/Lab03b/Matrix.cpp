#include <Lab03b/Matrix.h>
#include <Lab03b/MyException.h>

namespace Lab03b
{

    ////////////////////////////////////////////////////////////////
    // [Matrix] Alokowanie pamiêci
    // dla macierzy o zadanych wymiarach.
    ////////////////////////////////////////////////////////////////
    void Matrix::create()
    {
        int n, m;

        if ((rows <= 0) || (columns <= 0))
        {
            throw MyException
            (
                "Matrix::create():\n" \
                "    Niedodatnie wymiary macierzy! [%d][%d].",
                rows,
                columns
            );
        }

        try
        {
            elements = new double* [rows];

            for (n = 0; n < rows; n++)
            {
                elements[n] = nullptr;
            }

            for (n = 0; n < rows; n++)
            {
                elements[n] = new double [columns];

                for (m = 0; m < columns; m++)
                {
                    elements[n][m] = 0;
                }
            }
        }
        catch (std::bad_alloc)
        {
            throw MyException
            (
                "Matrix::create():\n" \
                "    Blad alokacji pamieci!"
            );
        }
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Dealokacja pamiêci
    // i resetowanie macierzy.
    ////////////////////////////////////////////////////////////////
    void Matrix::destroy()
    {
        int n;

        if (nullptr != elements)
        {
            for (n = 0; n < rows; n++)
            {
                if (nullptr != elements[n])
                {
                    delete[](elements[n]);
                }
            }

            delete[](elements);
            elements = nullptr;
        }

        rows = 0;
        columns = 0;
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Kopiowanie wymiarów i elementów
    // z ju¿ istniej¹cej macierzy.
    ////////////////////////////////////////////////////////////////
    void Matrix::copy(const Matrix &m2)
    {
        int n, m;

        rows = m2.rows;
        columns = m2.columns;

        create();

        for (n = 0; n < rows; n++)
        {
            for (m = 0; m < columns; m++)
            {
                elements[n][m] = m2.elements[n][m];
            }
        }
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Sprawdzenie poprawnoœci indeksów
    // w trakcie pobierania lub nadpisywania elementu.
    ////////////////////////////////////////////////////////////////
    void Matrix::validateRowAndColumn(const char* func_name, int n, int m) const
    {
        if ((n < 0) || (n > rows))
        {
            throw MyException
            (
                "Matrix::%s():\n" \
                "    Nieprawidlowy indeks wiersza (%d)!" \
                "    Oczekiwano indesku od [0] do [%d].",
                func_name,
                n,
                (rows - 1)
            );
        }

        if ((m < 0) || (m > columns))
        {
            throw MyException
            (
                "Matrix::%s():\n" \
                "    Nieprawidlowy indeks kolumny (%d)!" \
                "    Oczekiwano indesku od [0] do [%d].",
                func_name,
                m,
                (columns - 1)
            );
        }
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Konstruktor przyjmuj¹cy dwa wymiary.
    ////////////////////////////////////////////////////////////////
    Matrix::Matrix(int new_rows, int new_columns)
    {
        rows = new_rows;
        columns = new_columns;

        create();
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Konstruktor przyjmuj¹cy jeden wymiar.
    ////////////////////////////////////////////////////////////////
    Matrix::Matrix(int new_dimension)
    {
        rows = new_dimension;
        columns = new_dimension;

        create();
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Konstruktor kopii.
    ////////////////////////////////////////////////////////////////
    Matrix::Matrix(const Matrix &m2)
    {
        copy(m2);
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Operator przypisania,
    ////////////////////////////////////////////////////////////////
    Matrix & Matrix::operator = (const Matrix &m2)
    {
        if (this != &m2)
        {
            destroy();

            copy(m2);
        }

        return (*this);
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Destruktor,
    ////////////////////////////////////////////////////////////////
    Matrix::~Matrix()
    {
        destroy();
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Ustawienie elementu
    // w "n"-tym wierszu na "m"-tej kolumnie.
    ////////////////////////////////////////////////////////////////
    void Matrix::set(int n, int m, double val)
    {
        validateRowAndColumn("set", n, m);

        elements[n][m] = val;
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Pobranie elementu
    // z "n"-tego wiersza i "m"-tej kolumny.
    ////////////////////////////////////////////////////////////////
    double Matrix::get(int n, int m) const
    {
        validateRowAndColumn("get", n, m);

        return elements[n][m];
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Zwraca liczbê kolumn.
    ////////////////////////////////////////////////////////////////
    int Matrix::getCols() const
    {
        return columns;
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Zwraca liczbê wierszy.
    ////////////////////////////////////////////////////////////////
    int Matrix::getRows() const
    {
        return rows;
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Wypisuje zawartoœæ macierzy na ekranie.
    ////////////////////////////////////////////////////////////////
    void Matrix::print() const
    {
        int n, m, o = columns - 1;

        for (n = 0; n < rows; n++)
        {
            std::cout << "    [ ";

            for (m = 0; m < o; m++)
            {
                std::cout.width(4);
                std::cout << elements[n][m] << " , ";
            }

            if (m < columns)
            {
                std::cout.width(4);
                std::cout << elements[n][m];
            }

            std::cout << " ]" << std::endl;
        }
    }

}

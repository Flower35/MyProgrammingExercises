#include <Lab05/Matrix.h>
#include <Lab05/MyException.h>

namespace Lab05
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
                "Matrix::Matrix(): Niedodatnie wymiary macierzy! [%d][%d].",
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
            throw MyException("Matrix::create(): Blad alokacji pamieci!");
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
    // [Matrix] Sprawdzenie poprawnoœci indeksu wiersza
    // w trakcie pobierania lub nadpisywania elementu.
    ////////////////////////////////////////////////////////////////
    void Matrix::validateRow(const char* func_name, int n) const
    {
        if ((n < 0) || (n > rows))
        {
            throw MyException
            (
                "Matrix::%s(): Nieprawidlowy indeks wiersza (%d)!" \
                " Oczekiwano indesku od [0] do [%d].",
                func_name,
                n,
                (rows - 1)
            );
        }
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Porównanie wymiarów dwóch macierzy.
    ////////////////////////////////////////////////////////////////
    void Matrix::validateDimensions(const char* func_name, const Matrix &m2, bool multiplication) const
    {
        bool test = multiplication
            ? (m2.rows != columns)
            : ((m2.rows != rows) || (m2.columns != columns));

        if (test)
        {
            throw MyException
            (
                "Matrix::%s(): niezgodne wymiary macierzy!" \
                " [%d][%d], [%d][%d].",
                func_name,
                rows,
                columns,
                m2.rows,
                m2.columns
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
    // [Matrix] Ustawienie elementów w "n"-tym wierszu.
    // (wykorzystanie operatora nawiasów kwadratowych)
    ////////////////////////////////////////////////////////////////
    double* Matrix::operator [] (int n) const
    {
        validateRow("set", n);

        return elements[n];
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Pobranie elementów z "n"-tego wiersza.
    // (wykorzystanie operatora nawiasów kwadratowych)
    ////////////////////////////////////////////////////////////////
    double* & Matrix::operator [] (int n)
    {
        validateRow("get", n);

        return elements[n];
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Dodawanie do siebie dwóch macierzy
    // o tych samych wymiarach.
    // (wykorzystanie operatora plusa)
    ////////////////////////////////////////////////////////////////
    Matrix Matrix::operator + (const Matrix &m2) const
    {
        validateDimensions("add", m2, false);

        int n, m;
        Matrix result(rows, columns);

        for (n = 0; n < rows; n++)
        {
            for (m = 0; m < columns; m++)
            {
                result[n][m] = elements[n][m] + m2.elements[n][m];
            }
        }

        return result;
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Odejmowanie od siebie dwóch macierzy
    // o tych samych wymiarach.
    // (wykorzystanie operatora minusa)
    ////////////////////////////////////////////////////////////////
    Matrix Matrix::operator - (const Matrix &m2) const
    {
        validateDimensions("subtract", m2, false);

        int n, m;
        Matrix result(rows, columns);

        for (n = 0; n < rows; n++)
        {
            for (m = 0; m < columns; m++)
            {
                result[n][m] = elements[n][m] - m2.elements[n][m];
            }
        }

        return result;
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Mno¿enie dwóch macierzy:
    // liczba kolumn pierwszej macierzy musi
    // równaæ siê liczbie wierszy drugiej macierzy.
    // (wykorzystanie operatora gwiazdki)
    ////////////////////////////////////////////////////////////////
    Matrix Matrix::operator * (const Matrix &m2) const
    {
        validateDimensions("multiply", m2, true);

        int m, n, o;
        double val;
        Matrix result(rows, m2.columns);

        for (m = 0; m < rows; m++)
        {
            for (n = 0; n < m2.columns; n++)
            {
                val = 0;

                for (o = 0; o < columns; o++)
                {
                    val += elements[m][o] * m2.elements[o][n];
                }

                result[m][n] = val;
            }
        }

        return result;
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Porównanie dwóch macierzy.
    // (wykorzystanie operatora porównania)
    ////////////////////////////////////////////////////////////////
    bool Matrix::operator == (const Matrix &m2) const
    {
        if ((rows != m2.rows) || (columns != m2.columns))
        {
            return false;
        }

        int n, m;

        for (n = 0; n < rows; n++)
        {
            for (m = 0; m < columns; m++)
            {
                if (elements[n][m] != m2.elements[n][m])
                {
                    return false;
                }
            }
        }

        return true;
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Zwrócenie macierzy transponowanej.
    ////////////////////////////////////////////////////////////////
    Matrix Matrix::transposed() const
    {
        Matrix result(columns, rows);
        int a, b;

        for (a = 0; a < rows; a++)
        {
            for (b = 0; b < columns; b++)
            {
                result[b][a] = elements[a][b];
            }
        }

        return result;
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
            std::cout << "[";

            for (m = 0; m < o; m++)
            {
                std::cout.width(3);
                std::cout << elements[n][m] << ", ";
            }

            if (m < columns)
            {
                std::cout.width(3);
                std::cout << elements[n][m];
            }

            std::cout.width(0);
            std::cout << "]" << std::endl;
        }
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Zapisuje macierz do pliku tekstowego.
    ////////////////////////////////////////////////////////////////
    Matrix & Matrix::operator >> (std::ofstream &file) const
    {
        int n, m, o = columns - 1;

        file.exceptions(std::ios::failbit | std::ios::badbit);

        try
        {
            file << columns << " " << rows << std::endl;

            for (n = 0; n < rows; n++)
            {
                for (m = 0; m < o; m++)
                {
                    file << elements[n][m] << " ";
                }

                if (m < columns)
                {
                    file << elements[n][m];
                }

                file << std::endl;
            }
        }
        catch (std::exception)
        {
            throw MyException("Nie udalo sie zapisac danych do pliku!");
        }

        return (Matrix &)(*this);
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Konstruktor odczytuj¹cy macierz z pliku tekstowego.
    ////////////////////////////////////////////////////////////////
    Matrix::Matrix(const std::string &fullpath)
    {
        int n, m;

        std::string line;

        std::stringstream sstream;
        sstream.exceptions(std::ios::failbit | std::ios::badbit);

        std::ifstream file;
        file.exceptions(std::ios::goodbit);
        file.open(fullpath, std::ios::in);

        if (!file.is_open())
        {
            throw MyException
            (
                "Nie mozna otworzyc pliku \"%s\" do odczytu!",
                fullpath.c_str()
            );
        }

        file.exceptions(std::ios::failbit | std::ios::badbit);

        try
        {
            std::getline(file, line);
            sstream.str(line);

            sstream >> columns >> rows;
            sstream.clear();

            create();

            for (n = 0; n < rows; n++)
            {
                std::getline(file, line);
                sstream.str(line);

                for (m = 0; m < columns; m++)
                {
                    if (std::getline(sstream, line, ' '))
                    {
                        elements[n][m] = std::stod(line);
                    }
                    else
                    {
                        throw std::exception();
                    }
                }

                sstream.clear();
            }
        }
        catch (std::ios::failure)
        {
            throw MyException("Wystapil blad podczas odczytywania danych z pliku!");
        }
        catch (std::exception)
        {
            throw MyException("Niepoprawny format danych w pliku!");
        }

        file.close();
    }

}

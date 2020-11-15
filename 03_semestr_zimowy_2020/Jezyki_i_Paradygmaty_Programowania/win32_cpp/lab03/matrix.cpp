/****************************************************************
  [LAB 03, ZADANIE 01]
    Nale¿y zaimplementowaæ klasê reprezentuj¹c¹ macierz
    elementów `double`. Klasa ma mieæ nastêpuj¹ce metody:

    * konstruktor przyjmuj¹cy dwa argumenty typu `int`
    * konstruktor przyjmuj¹cy jeden argument typu `int`
    * konstruktor przyjmuj¹cy jako argument œcie¿kê do pliku
    * `set(n, m, val)`
    * `get(n, m)`
    * `add(Matrix m2)`
    * `subtract(Matrix m2)`
    * `multiply(Matrix m2)`
    * `getCols()` => zmiana nazwy funkcji z `cols()`
    * `getRows()` => `rows` zarezerwowane dla nazwy w³asnoœci klasy
    * `print()`
    * `store(std::string filename, std::string path)`

****************************************************************/

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <new>
#include <cstdarg>


////////////////////////////////////////////////////////////////
// Definicje
////////////////////////////////////////////////////////////////
#define LARGE_BUFFER_SIZE 256
#define NAZWA_PLIKU "matrix.txt"


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
// Klasa wyj¹tku
////////////////////////////////////////////////////////////////
class MyException
{
    /* Properties */

    private:

        std::string message;

    /* Methods */

    public:

        MyException(const char* format, ...)
        {
            char test[LARGE_BUFFER_SIZE];

            va_list args;
            va_start(args, format);

            vsprintf(test, format, args);
            message = test;

            va_end(args);
        }

        void printMessage() const
        {
            std::cout << "[EXCEPTION] " << message << std::endl;
        }
};


////////////////////////////////////////////////////////////////
// Klasa macierzy
////////////////////////////////////////////////////////////////
class Matrix
{
    /* Properties */

    private:

        double** elements;
        int rows;
        int columns;

    /* Methods */

    private:

        /* Alokowanie pamiêci dla macierzy o zadanych wymiarach */
        void create()
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

        /* Dealokacja pamiêci i resetowanie macierzy */
        void destroy()
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

        /* Kopiowanie wymiarów i elementów z ju¿ istniej¹cej macierzy */
        void copy(const Matrix &m2)
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

        /* Sprawdzenie poprawnoœci indeksów (odczyt i zapis) */
        void validateRowAndColumn(const char* func_name, int n, int m) const
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

            if ((m < 0) || (m > columns))
            {
                throw MyException
                (
                    "Matrix::%s(): Nieprawidlowy indeks kolumny (%d)!" \
                    " Oczekiwano indesku od [0] do [%d].",
                    func_name,
                    m,
                    (columns - 1)
                );
            }
        }

        /* Porównanie wymiarów dwóch macierzy */
        void validateDimensions(const char* func_name, const Matrix &m2, bool multiplication) const
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

    public:

        /* Konstruktor przyjmuj¹cy dwa wymiary */
        Matrix(int new_rows, int new_columns)
        {
            rows = new_rows;
            columns = new_columns;

            create();
        }

        /* Konstruktor przyjmuj¹cy jeden wymiar */
        Matrix(int new_dimension)
        {
            rows = new_dimension;
            columns = new_dimension;

            create();
        }

        /* Konstruktor kopii */
        Matrix(const Matrix &m2)
        {
            copy(m2);
        }

        /* Operator przypisania */
        Matrix &operator = (const Matrix &m2)
        {
            if (this != &m2)
            {
                destroy();

                copy(m2);
            }

            return (*this);
        }

        /* Destruktor */
        ~Matrix()
        {
            destroy();
        }

        /* Ustawienie elementu w "n"-tym wierszu na "m"-tej kolumnie */
        void set(int n, int m, double val)
        {
            validateRowAndColumn("set", n, m);

            elements[n][m] = val;
        }

        /* Pobranie elementu z "n"-tego wiersza i "m"-tej kolumny */
        double get(int n, int m) const
        {
            validateRowAndColumn("get", n, m);

            return elements[n][m];
        }

        /* Dodawanie do siebie dwóch macierzy o tych samych wymiarach */
        Matrix add(const Matrix &m2) const
        {
            validateDimensions("add", m2, false);

            int n, m;
            Matrix result(rows, columns);

            for (n = 0; n < rows; n++)
            {
                for (m = 0; m < columns; m++)
                {
                    result.set(n, m, (elements[n][m] + m2.elements[n][m]));
                }
            }

            return result;
        }

        /* Odejmowanie od siebie dwóch macierzy o tych samych wymiarach */
        Matrix subtract(const Matrix &m2) const
        {
            validateDimensions("subtract", m2, false);

            int n, m;
            Matrix result(rows, columns);

            for (n = 0; n < rows; n++)
            {
                for (m = 0; m < columns; m++)
                {
                    result.set(n, m, (elements[n][m] - m2.elements[n][m]));
                }
            }

            return result;
        }

        /* Mno¿enie dwóch macierzy: liczba kolumn pierwszej macierzy */
        /* musi równaæ siê liczbie wierszy drugiej macierzy */
        Matrix multiply(const Matrix &m2) const
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

                    result.set(m, n, val);
                }
            }

            return result;
        }

        /* Zwraca liczbê kolumn */
        int getCols() const
        {
            return columns;
        }

        /* Zwraca liczbê wierszy */
        int getRows() const
        {
            return rows;
        }

        /* Wypisuje zawartoœæ macierzy na ekranie */
        void print() const
        {
            int n, m, o = columns - 1;

            for (n = 0; n < rows; n++)
            {
                std::cout << "[";

                for (m = 0; m < o; m++)
                {
                    std::cout << elements[n][m] << ", ";
                }

                if (m < columns)
                {
                    std::cout << elements[n][m];
                }

                std::cout << "]" << std::endl;
            }
        }

        /* Zapisuje macierz do pliku tekstowego */
        void store(const std::string &filename, const std::string &path) const
        {
            int n, m, o = columns - 1;

            std::string fullpath = path + "/" + filename;

            std::ofstream file(fullpath, (std::ios::out | std::ios::trunc));

            if (!file.is_open())
            {
                throw MyException
                (
                    "Nie mozna otworzyc pliku \"%s\" do zapisu!",
                    fullpath.c_str()
                );
            }

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

            file.close();
        }

        /* Konstruktor odczytuj¹cy macierz z pliku tekstowego */
        Matrix(const std::string &fullpath)
        {
            int n, m;

            std::string line;

            std::stringstream sstream;
            sstream.exceptions(std::ios::failbit | std::ios::badbit);

            std::ifstream file(fullpath, std::ios::in);
            file.exceptions(std::ios::failbit | std::ios::badbit);

            if (!file.is_open())
            {
                throw MyException
                (
                    "Nie mozna otworzyc pliku \"%s\" do odczytu!",
                    fullpath.c_str()
                );
            }

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
};


////////////////////////////////////////////////////////////////
// Testowanie zadania
////////////////////////////////////////////////////////////////
int main()
{
    int a, b;
    double val;

    try
    {
        Matrix m1(4);
        Matrix m2(4);
        Matrix vec(4, 1);

        /* Macierz jednostkowa */
        print_dashed_line();
        std::cout << "Macierz m1 [" << m1.getRows() << "][" << m1.getCols()
            << "] (jednostkowa):" << std::endl;

        for (a = 0; a < 4; a++)
        {
            for (b = 0; b < 4; b++)
            {
                val = (a != b) ? 0 : 1.0;

                m1.set(a, b, val);

                if (m1.get(a, b) != val)
                {
                    throw MyException("Niepowodzenie sprawdzenia `m1.get()`!");
                }
            }
        }

        m1.print();

        /* Macierz wype³niona liczbami od 1 do 16 */
        print_dashed_line();
        std::cout << "Macierz m2 [" << m2.getRows() << "][" << m2.getCols()
            << "] (wypelniona liczbami od 1 do 16):" << std::endl;

        val = 0;
        for (a = 0; a < 4; a++)
        {
            for (b = 0; b < 4; b++)
            {
                val++;
                m2.set(a, b, val);

                if (m2.get(a, b) != val)
                {
                    throw MyException("Niepowodzenie sprawdzenia `m2.get()`!");
                }
            }
        }

        m2.print();

        /* Dodawanie */
        print_dashed_line();
        std::cout << "Dodawanie (m3 := m1 + m2):" << std::endl;
        m2 = m1.add(m2);
        m2.print();

        /* Odejmowanie */
        print_dashed_line();
        std::cout << "Odejmowanie (m2 := m3 - m1):" << std::endl;
        m2 = m2.subtract(m1);
        m2.print();

        /* Wektor pionowy */
        print_dashed_line();
        std::cout << "Wektor [" << vec.getRows() << "][" << vec.getCols()
            << "] (pionowy):" << std::endl;

        val = 0;
        for (a = 0; a < 4; a++)
        {
            val += 10;
            vec.set(a, 0, val);

            if (vec.get(a, 0) != val)
            {
                throw MyException("Niepowodzenie sprawdzenia `vec.get()`!");
            }
        }

        vec.print();

        /* Mno¿enie macierzy jednostkowej przez inn¹ macierz */
        print_dashed_line();
        std::cout << "Mnozenie (m2 := m1 x m2):" << std::endl;
        m2 = m1.multiply(m2);
        m2.print();

        /* Mno¿enie macierzy jednostkowej przez wektor */
        print_dashed_line();
        std::cout << "Mnozenie (vec := m1 x vec):" << std::endl;
        vec = m1.multiply(vec);
        vec.print();

        /* Mno¿enie innej macierzy przez wektor*/
        print_dashed_line();
        std::cout << "Mnozenie (vec := m2 x vec):" << std::endl;
        vec = m2.multiply(vec);
        vec.print();

        /* Jeszcze jedno mno¿enie */
        print_dashed_line();
        std::cout << "Mnozenie (m2 := m2 x m2):" << std::endl;
        m2 = m2.multiply(m2);
        m2.print();

        /* Zapis macierzy do pliku */
        print_dashed_line();
        std::cout << "Zapis do pliku \"" << NAZWA_PLIKU << "\":" << std::endl;
        m2.store(NAZWA_PLIKU, ".");

        /* Odczyt macierzy z pliku */
        print_dashed_line();
        std::cout << "Odczyt z pliku \"" << NAZWA_PLIKU << "\":" << std::endl;
        Matrix m3(std::string("./") + NAZWA_PLIKU);
        m3.print();

        a = 0;
    }
    catch (MyException &error)
    {
        print_dashed_line();
        error.printMessage();

        a = 1;
    }

    print_dashed_line();
    return a;
}

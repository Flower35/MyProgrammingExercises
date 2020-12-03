/****************************************************************
  [LAB 03, ZADANIE DODATKOWE]
    Program z "lab03/matrix" z funkcj¹ zapisu macierzy
    do bazy danych z wykorzystaniem biblioteki "sqlite3".

****************************************************************/

#include <Lab03b/Matrix.h>
#include <Lab03b/MyException.h>

namespace Lab03b
{

    ////////////////////////////////////////////////////////////////
    // Wydrukowanie odstêpu na ekranie.
    ////////////////////////////////////////////////////////////////
    void print_dashed_line()
    {
        std::cout.width(64);
        std::cout.fill('-');

        std::cout << "" << std::endl;

        std::cout.fill(' ');
    }


    ////////////////////////////////////////////////////////////////
    // Testowanie zadania.
    ////////////////////////////////////////////////////////////////
    int testuj_zadanie()
    {
        int a, b;
        double val;
        sqlite3_int64 index_in_table;
        const std::string path = std::string("./") + DB_FILENAME;

        try
        {
            Matrix m1(4);

            /* Testowa macierz */

            print_dashed_line();
            std::cout << " * Macierz m1 ["
                << m1.getRows() << "][" << m1.getCols()
                << "]:" << std::endl;

            val = 160;

            for (a = 0; a < 4; a++)
            {
                for (b = 0; b < 4; b++)
                {
                    m1.set(a, b, val);

                    if (m1.get(a, b) != val)
                    {
                        throw MyException("Niepowodzenie sprawdzenia `m1.get()`!");
                    }

                    val -= 10.0;
                }
            }

            m1.print();

            /* Zapis macierzy do pliku bazodanowego */

            print_dashed_line();
            std::cout << " * Zapis do pliku \"" << path << "\"..." << std::endl;

            index_in_table = m1.store(path);

            print_dashed_line();
            std::cout << " * Identyfikator z tabelki \"" << DB_TABLE_NAMES[0]
                << "\": " << index_in_table
                << "." << std::endl;

            /* Odczyt macierzy z pliku bazodanowego */

            print_dashed_line();
            std::cout << " * Odczyt z pliku \"" << path << "\"..." << std::endl;

            m1 = Matrix(path, index_in_table);

            print_dashed_line();
            std::cout << " * Nadpisana macierz:" << std::endl;
            m1.print();

            a = SUCCESS;
        }
        catch (MyException &error)
        {
            print_dashed_line();
            error.printMessage();

            a = FAILURE;
        }

        print_dashed_line();
        return a;
    }

}


////////////////////////////////////////////////////////////////
// Procedura wejœcia programu
////////////////////////////////////////////////////////////////
int main()
{
    return Lab03b::testuj_zadanie();
}

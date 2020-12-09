/****************************************************************
  [LAB 04, ZADANIE 01]
    Program z "lab03/matrix" w formie projektu "CMake".

****************************************************************/

#include <Lab04/Lab04.h>
#include <Lab04/MyException.h>
#include <Lab04/Matrix.h>

namespace Lab04
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
        catch (std::exception &error)
        {
            print_dashed_line();

            std::cout << error.what() << std::endl;

            a = 1;
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
    return Lab04::testuj_zadanie();
}

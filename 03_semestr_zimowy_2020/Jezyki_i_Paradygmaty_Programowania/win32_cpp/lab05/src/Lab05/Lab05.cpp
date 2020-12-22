/****************************************************************
  [LAB 05, ZADANIE 01]
    Program z "lab03/matrix" z wykorzystaniem
    przeci¹¿enia operatorów ('+', '-', '*', '>>', '==', '[]').

****************************************************************/

#include <Lab05/Lab05.h>
#include <Lab05/MyException.h>
#include <Lab05/Matrix.h>

namespace Lab05
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

        std::ofstream text_file;

        const std::string text_file_path = std::string("../") + NAZWA_PLIKU;

        const int SIZE_A = 4;
        const int SIZE_B = 5;

        try
        {
            Matrix
                m1(SIZE_A),
                m2(SIZE_A, SIZE_B),
                m3(SIZE_A, SIZE_B),
                m4(1),
                vec(SIZE_A, 1);

            /********************************/
            /* Macierz jednostkowa */

            print_dashed_line();
            std::cout << "Macierz m1 [" << m1.getRows() << "][" << m1.getCols()
                << "] (jednostkowa):" << std::endl;

            for (a = 0; a < SIZE_A; a++)
            {
                m1[a][a] = 1.0;

                if (m1[a][a] != 1.0)
                {
                    throw MyException("Niepowodzenie sprawdzenia `m1.get()`!");
                }
            }

            m1.print();

            /********************************/
            /* Macierz (m2) wype³niona wybranymi liczbami */

            print_dashed_line();
            std::cout << "Macierz m2 [" << m2.getRows() << "][" << m2.getCols()
                << "] (wypelniona liczbami od 1 do "
                << (SIZE_A * SIZE_B) << "):" << std::endl;

            val = 0;

            for (a = 0; a < SIZE_A; a++)
            {
                for (b = 0; b < SIZE_B; b++)
                {
                    val++;
                    m2[a][b] = val;

                    if (m2[a][b] != val)
                    {
                        throw MyException("Niepowodzenie sprawdzenia `m2.get()`!");
                    }
                }
            }

            m2.print();

            /********************************/
            /* Macierz (m3) wype³niona wybranymi liczbami */

            print_dashed_line();
            std::cout << "Macierz m3 [" << m3.getRows() << "][" << m3.getCols()
                << "] (wypelniona liczbami od " << (SIZE_A * SIZE_B)
                << " do 1):" << std::endl;

            val = (double)(SIZE_A * SIZE_B);

            for (a = 0; a < SIZE_A; a++)
            {
                for (b = 0; b < SIZE_B; b++)
                {
                    m3[a][b] = val;

                    if (m3[a][b] != val)
                    {
                        throw MyException("Niepowodzenie sprawdzenia `m3.get()`!");
                    }

                     val--;
                }
            }

            m3.print();

            /********************************/
            /* Dodawanie */

            print_dashed_line();
            std::cout << "Dodawanie (m4 := m2 + m3):" << std::endl;

            m4 = m2 + m3;

            m4.print();

            /********************************/
            /* Odejmowanie */

            print_dashed_line();
            std::cout << "Odejmowanie (m3 := m4 - m2):" << std::endl;

            m3 = m4 - m2;

            m3.print();

            /********************************/
            /* Wektor pionowy */

            print_dashed_line();
            std::cout << "Wektor [" << vec.getRows() << "][" << vec.getCols()
                << "] (pionowy):" << std::endl;

            val = 0;

            for (a = 0; a < 4; a++)
            {
                val += 10.0;
                vec[a][0] = val;

                if (vec[a][0] != val)
                {
                    throw MyException("Niepowodzenie sprawdzenia `vec.get()`!");
                }
            }

            vec.print();

            /********************************/
            /* Mno¿enie macierzy jednostkowej przez inn¹ macierz */

            print_dashed_line();
            std::cout << "Mnozenie (m3 := m1 x m2):" << std::endl;

            m3 = m1 * m2;

            m3.print();

            /********************************/
            /* Porównanie obiektów (oczekiwana prawda) */

            print_dashed_line();
            std::cout << "Porownanie macierzy \"m2\" i \"m3:\" "
                << (m2 == m3) << " (oczekiwane \"1\")" << std::endl;

            /********************************/
            /* Mno¿enie macierzy jednostkowej przez wektor */

            print_dashed_line();
            std::cout << "Mnozenie (vec := m1 x vec):" << std::endl;

            vec = m1 * vec;

            vec.print();

            /********************************/
            /* Transponowanie macierzy [4x5 -> 5x4] */

            print_dashed_line();
            std::cout << "Transponowanie macierzy (m3 <- m2):" << std::endl;

            m3 = m2.transposed();

            m3.print();

            /********************************/
            /* Mno¿enie innej macierzy przez wektor */

            print_dashed_line();
            std::cout << "Mnozenie (m4 := m3 x vec):" << std::endl;

            m4 = m3 * vec;

            m4.print();

            /********************************/
            /* Porównanie obiektów (oczekiwany fa³sz) */

            print_dashed_line();
            std::cout << "Porownanie macierzy \"vec\" i \"m4\": "
                << (vec == m4) << " (oczekiwane \"0\")" << std::endl;

            /********************************/
            /* Zapis macierzy do pliku */

            print_dashed_line();
            std::cout << "Zapis macierzy \"m3\" do pliku \""
                << text_file_path << "\"." << std::endl;

            text_file.exceptions(std::ios::goodbit);
            text_file.open
            (
                text_file_path,
                std::ios::out | std::ios::trunc
            );

            if (!text_file.is_open())
            {
                throw MyException
                (
                    "Nie udalo sie otworzyc pliku \"%s\"!",
                    text_file_path.c_str()
                );
            }

            m3 >> text_file;

            text_file.close();

            /********************************/
            /* Odczyt macierzy z pliku */

            print_dashed_line();
            std::cout << "Odczyt macierzy \"m5\" z pliku \""
                << text_file_path << "\":" << std::endl;

            Matrix m5(text_file_path);

            m5.print();

            a = SUCCESS;
        }
        catch (std::exception &error)
        {
            print_dashed_line();

            std::cout << error.what() << std::endl;

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
    return Lab05::testuj_zadanie();
}

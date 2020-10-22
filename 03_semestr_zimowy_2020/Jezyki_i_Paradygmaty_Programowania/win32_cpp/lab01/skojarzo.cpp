/****************************************************************
  [LAB 01, ZADANIE 04]
    Dwie r�ne liczby ca�kowite "a" i "b" wi�ksze od 1 nazwiemy skojarzonymi,
    je�li suma wszystkich r�nych dodatnich dzielnik�w "a" mniejszych od "a"
    jest r�wna "b+1", a suma wszystkich r�nych dodatnich dzielnik�w "b"
    mniejszych od "b" jest r�wna "a+1".

  Ten program wypisze wszystkie mo�liwe pary
  liczb skojarzonych dla [2 >= n => 1048576].

****************************************************************/

#include <iostream>

#include <cstdint>


////////////////////////////////////////////////////////////////
// DEFINICJE
////////////////////////////////////////////////////////////////

#define MAIN_LOOP_LIMIT (1024 * 1024)


////////////////////////////////////////////////////////////////
// Wyliczenie pierwiastka kwadratowego liczby naturalnej
////////////////////////////////////////////////////////////////
uint32_t sqrt_uint32(uint32_t n)
{
    uint32_t result = 0, test = (-1);

    while (0 != n)
    {
        test += 2;
        result++;

        if (n >= test)
        {
            n -= test;
        }
        else
        {
            n = 0;
            result--;
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////
// Obliczenie sumy dzielnik�w w�a�ciwych danej liczby naturalnej
// (bez jedynki)
////////////////////////////////////////////////////////////////
uint32_t sum_of_divisors(uint32_t n)
{
    uint32_t i, remainder, quotient, root, result = 0;

    if (n > 2)
    {
        root = sqrt_uint32(n);

        for (i = 2; i <= root; i++)
        {
            remainder = n % i;

            if (0 == remainder)
            {
                quotient = n / i;
                result += quotient;

                /* Nie dodawaj dwa razy pierwiastka kwadratowego! */

                if (quotient != root)
                {
                    result += (n / quotient);
                }
            }
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////
// POCZ�TEK PROGRAMU
////////////////////////////////////////////////////////////////
int32_t main()
{
    uint32_t b, a, n = 2;

    /********************************/
    /* Wyszykiwanie i wypisywanie liczb skojarzonych. */

    do
    {
        a = sum_of_divisors(n);

        if (a > n)
        {
            /********************************/
            /* "n" nie jest liczb� pierwsz�, bo "a" jest r�ne od zera. */
            /* Przy za�o�eniu "a > n" nie wyst�pi� duplikaty par. */

            b = sum_of_divisors(a);

            if (b == n)
            {
                /********************************/
                /* Znaleziono par� liczb skojarzonych! */

                std::cout << " -> (" << n << ", " << a << ")" << std::endl;
            }
        }

        n++;

        if (n > MAIN_LOOP_LIMIT)
        {
            n = 0;
        }
    }
    while (0 != n);

    /********************************/
    /* Zamkni�cie programu po wyczerpaniu limitu sprawdze�. */

    return 0;
}

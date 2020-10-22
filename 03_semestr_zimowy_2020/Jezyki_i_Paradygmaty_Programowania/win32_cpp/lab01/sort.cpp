/****************************************************************
  [LAB 01, ZADANIE 01]
    Napisz program sortuj�cy rosn�co ci�g liczb
    podanych jako argument programu.

****************************************************************/

#include <iostream>
#include <string>
#include <new>

#include <cstdint>


////////////////////////////////////////////////////////////////
// POCZ�TEK PROGRAMU
////////////////////////////////////////////////////////////////
int32_t main(int32_t argc, char** argv)
{
    int32_t a, b, dest, current;
    int32_t* sorted_numbers;

    /********************************/
    /* Program nale�y uruchomi� z argumentami wiersza polece�. */

    if (argc <= 1)
    {
        std::cerr << "BLAD: Nie podano zadnych argumentow!" << std::endl;

        return 1;
    }

    /********************************/
    /* Alokacja pami�ci dla wszystkich liczb. */

    argc--;

    try
    {
        sorted_numbers = new int [argc];
    }
    catch (std::bad_alloc &err)
    {
        std::cerr << "BLAD: Niewystarczajaca ilosc pamieci do uruchomienia programu!" << std::endl;

        return 1;
    }

    /********************************/
    /* Do��czanie kolejnych element�w */
    /* do tworzonej stopniowo listy posortowanej. */

    try
    {
        for (a = 0; a < argc; a++)
        {
            current = std::stoi(argv[1 + a]);

            dest = (a - 1);

            while ((dest >= 0) && (sorted_numbers[dest] > current))
            {
                dest--;
            }

            dest++;

            for (b = (a - 1); b >= dest; b--)
            {
                sorted_numbers[b + 1] = sorted_numbers[b];
            }

            sorted_numbers[dest] = current;
        }
    }
    catch (std::exception)
    {
        std::cerr << "BLAD: Nieprawidlowy argument #" << (1 + a) << "!" << std::endl;
    }

    b = a;

    /********************************/
    /* Wy�wietlenie wyniku. */

    if (b > 0)
    {
        std::cout << sorted_numbers[0];

        for (a = 1; a < b; a++)
        {
            std::cout << " " << sorted_numbers[a];
        }

        std::cout << std::endl;
    }

    /********************************/
    /* Zwolnienie pami�ci i zamkni�cie programu. */

    delete[](sorted_numbers);

    return 0;
}

/****************************************************************
  [LAB 01, ZADANIE 02]
    Napisz program, kt�ry okre�li czy wczytany ci�g
    (argument programu) jest palindromem tekstowym.

****************************************************************/

#include <iostream>
#include <new>

#include <cstdint>
#include <cstring>


////////////////////////////////////////////////////////////////
// POCZ�TEK PROGRAMU
////////////////////////////////////////////////////////////////
int32_t main(int32_t argc, char** argv)
{
    int32_t a, b, dest, length;
    char* text;
    char c;

    /********************************/
    /* Program nale�y uruchomi� z argumentami wiersza polece�. */

    if (argc <= 1)
    {
        std::cout << "Nie." << std::endl;

        return 1;
    }

    /********************************/
    /* Alokacja pami�ci na jeden ci�g�y tekst. */

    length = 0;

    for (a = 1; a < argc; a++)
    {
        length += std::strlen(argv[a]);
    }

    try
    {
        text = new char [length];
    }
    catch (std::bad_alloc &err)
    {
        std::cerr << "BLAD: Niewystarczajaca ilosc pamieci do uruchomienia programu!" << std::endl;

        std::cout << "Nie." << std::endl;

        return 1;
    }

    /********************************/
    /* Wybieranie tylko liter lub cyfr z przekazanego argumentu. */
    /* Zamiana ma�ych liter na du�e. Ignorowanie pozosta�ych znak�w. */

    dest = 0;

    for (a = 1; a < argc; a++)
    {
        length = std::strlen(argv[a]);

        for (b = 0; b < length; b++)
        {
            c = argv[a][b];

            if ( ((c >= 'A') && (c <= 'Z'))
              || ((c >= '0') && (c <= '9'))
              || ((c >= 'a') && (c <= 'z')) )
            {
                if (c >= 'a')
                {
                    c = c - 'a' + 'A';
                }

                text[dest] = argv[a][b];
                dest++;
            }
        }
    }

    /********************************/
    /* Okre�lenie, czy tekst przekazany do programu jest palindromem. */

    a = 0;
    b = dest - 1;

    dest = 1;
    while ((1 == dest) && (a < b))
    {
        if (text[a] != text[b])
        {
            dest = 0;
        }
        else
        {
            a++;
            b--;
        }
    }

    /********************************/
    /* Wy�wietlenie wyniku. */

    std::cout << (dest ? "Tak." : "Nie.") << std::endl;

    /********************************/
    /* Zwolnienie pami�ci i zamkni�cie programu. */

    delete[](text);

    return 0;
}

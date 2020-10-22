/****************************************************************
  [LAB 01, ZADANIE 02]
    Napisz program, który okreœli czy wczytany ci¹g
    (argument programu) jest palindromem tekstowym.

****************************************************************/

#include <iostream>
#include <new>

#include <cstdint>
#include <cstring>


////////////////////////////////////////////////////////////////
// POCZ¥TEK PROGRAMU
////////////////////////////////////////////////////////////////
int32_t main(int32_t argc, char** argv)
{
    int32_t a, b, dest, length;
    char* text;
    char c;

    /********************************/
    /* Program nale¿y uruchomiæ z argumentami wiersza poleceñ. */

    if (argc <= 1)
    {
        std::cout << "Nie." << std::endl;

        return 1;
    }

    /********************************/
    /* Alokacja pamiêci na jeden ci¹g³y tekst. */

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
    /* Zamiana ma³ych liter na du¿e. Ignorowanie pozosta³ych znaków. */

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
    /* Okreœlenie, czy tekst przekazany do programu jest palindromem. */

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
    /* Wyœwietlenie wyniku. */

    std::cout << (dest ? "Tak." : "Nie.") << std::endl;

    /********************************/
    /* Zwolnienie pamiêci i zamkniêcie programu. */

    delete[](text);

    return 0;
}

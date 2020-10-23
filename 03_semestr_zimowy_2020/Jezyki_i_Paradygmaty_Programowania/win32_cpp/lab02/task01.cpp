/****************************************************************
  [LAB 02, ZADANIE 01]
    Napisz funkcj� otrzymuj�c� jako argumenty
    wska�niki do dw�ch zmiennych typu "int",
    kt�ra zamienia ze sob� warto�ci wskazywanych zmiennych
    tylko wtedy, gdy wskazywana przez drugi argument zmienna
    jest mniejsza od zmiennej wskazywanej przez pierwszy argument.

  [LAB 02, ZADANIE 02]
    Napisz funkcj� otrzymuj�c� jako argumenty
    referencje do dw�ch zmiennych typu "int",
    kt�ra zamienia ze sob� warto�ci zmiennych,
    do kt�rych referencje dostali�my w argumentach.

  [LAB 02, ZADANIE 03]
    Napisz funkcj� otrzymuj�c� dwa argumenty:
    referencj� `a` oraz wska�nik `b` do zmiennych typu "int",
    kt�ra zamienia ze sob� warto�ci zmiennych,
    do kt�rych wska�nik i referencj� dosta�a w argumentach.

****************************************************************/

#include <iostream>

#include <cstdint>


////////////////////////////////////////////////////////////////
// Dane do testowania zada�
////////////////////////////////////////////////////////////////

int32_t test_numbers[] =
{
    10, (-2), (-8), (-4), 6, 5, (-7), 3, 9, (-1)
};


////////////////////////////////////////////////////////////////
// Funkcja opisywana w tre�ci zadania 1.
////////////////////////////////////////////////////////////////
void task01_swap_function(int32_t* a, int32_t* b)
{
    if ((nullptr != a) && (nullptr != b) && ((*b) < (*a)))
    {
        (*a) ^= (*b);
        (*b) ^= (*a);
        (*a) ^= (*b);
    }
}


////////////////////////////////////////////////////////////////
// Funkcja opisywana w tre�ci zadania 2.
////////////////////////////////////////////////////////////////
void task02_swap_function(int32_t &a, int32_t &b)
{
    a ^= b;
    b ^= a;
    a ^= b;
}


////////////////////////////////////////////////////////////////
// Funkcja opisywana w tre�ci zadania 3.
////////////////////////////////////////////////////////////////
void task03_swap_function(int32_t &a, int32_t* b)
{
    if (nullptr != b)
    {
        a ^= (*b);
        (*b) ^= a;
        a ^= (*b);
    }
}


////////////////////////////////////////////////////////////////
// Wydrukowanie odst�pu na ekranie
////////////////////////////////////////////////////////////////
void print_dashed_line()
{
    std::cout.width(64);
    std::cout.fill('-');

    std::cout << "" << std::endl;

    std::cout.fill(' ');
}


////////////////////////////////////////////////////////////////
// Testowanie zada�
////////////////////////////////////////////////////////////////
int32_t main()
{
    int32_t a, b, c, d = sizeof(test_numbers) / sizeof(int32_t) - (2 - 1);

    print_dashed_line();
    std::cout << "W pierwszym zadaniu liczby sa zamieniane miejscami, gdy 'b' < 'a'." << std::endl;
    std::cout << "W kolejnych zadanich zamiana nastepuje zawsze i na rozne sposoby (wsk, ref)." << std::endl;

    /********************************/
    /* Zadanie 1. */

    print_dashed_line();

    for (c = 0; c < d; c += 2)
    {
        a = test_numbers[c];
        b = test_numbers[c + 1];

        std::cout << '(' << a << ", " << b << ')';

        task01_swap_function(&a, &b);

        std::cout << " -> (" << a << ", " << b << ')' << std::endl;
    }

    /********************************/
    /* Zadanie 2. */

    print_dashed_line();

    for (c = 0; c < d; c += 2)
    {
        a = test_numbers[c];
        b = test_numbers[c + 1];

        std::cout << '(' << a << ", " << b << ')';

        task02_swap_function(a, b);

        std::cout << " -> (" << a << ", " << b << ')' << std::endl;
    }

    /********************************/
    /* Zadanie 3. */

    print_dashed_line();

    for (c = 0; c < d; c += 2)
    {
        a = test_numbers[c];
        b = test_numbers[c + 1];

        std::cout << '(' << a << ", " << b << ')';

        task03_swap_function(a, &b);

        std::cout << " -> (" << a << ", " << b << ')' << std::endl;
    }

    return 0;
}

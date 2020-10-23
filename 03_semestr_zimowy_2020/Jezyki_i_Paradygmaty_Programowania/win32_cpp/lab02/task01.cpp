/****************************************************************
  [LAB 02, ZADANIE 01]
    Napisz funkcjê otrzymuj¹c¹ jako argumenty
    wskaŸniki do dwóch zmiennych typu "int",
    która zamienia ze sob¹ wartoœci wskazywanych zmiennych
    tylko wtedy, gdy wskazywana przez drugi argument zmienna
    jest mniejsza od zmiennej wskazywanej przez pierwszy argument.

  [LAB 02, ZADANIE 02]
    Napisz funkcjê otrzymuj¹c¹ jako argumenty
    referencje do dwóch zmiennych typu "int",
    która zamienia ze sob¹ wartoœci zmiennych,
    do których referencje dostaliœmy w argumentach.

  [LAB 02, ZADANIE 03]
    Napisz funkcjê otrzymuj¹c¹ dwa argumenty:
    referencjê `a` oraz wskaŸnik `b` do zmiennych typu "int",
    która zamienia ze sob¹ wartoœci zmiennych,
    do których wskaŸnik i referencjê dosta³a w argumentach.

****************************************************************/

#include <iostream>

#include <cstdint>


////////////////////////////////////////////////////////////////
// Dane do testowania zadañ
////////////////////////////////////////////////////////////////

int32_t test_numbers[] =
{
    10, (-2), (-8), (-4), 6, 5, (-7), 3, 9, (-1)
};


////////////////////////////////////////////////////////////////
// Funkcja opisywana w treœci zadania 1.
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
// Funkcja opisywana w treœci zadania 2.
////////////////////////////////////////////////////////////////
void task02_swap_function(int32_t &a, int32_t &b)
{
    a ^= b;
    b ^= a;
    a ^= b;
}


////////////////////////////////////////////////////////////////
// Funkcja opisywana w treœci zadania 3.
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
// Testowanie zadañ
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

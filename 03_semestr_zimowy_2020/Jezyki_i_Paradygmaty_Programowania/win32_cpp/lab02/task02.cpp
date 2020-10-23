/****************************************************************
  [LAB 02, ZADANIE 04]
    Napisz funkcjê, która pobiera dwa argumenty i zwraca dwa odrêbne wyniki.
    Jednym z wyników powinien byæ iloczyn obu argumentów, a drugim ich suma.
    Poniewa¿ funkcja mo¿e bezpoœrednio zwracaæ tylko jedn¹ wartoœæ,
    druga powinna byæ zwracana poprzez parametr wskaŸnikowy albo referencjê.

****************************************************************/

#include <iostream>

#include <cstdint>


////////////////////////////////////////////////////////////////
// Dane do testowania zadania
// ("INT32": od -2147483648 do +2147483647)
////////////////////////////////////////////////////////////////

int32_t test_numbers[] =
{
    2123456789, 2123456789,
    2123456789, (-2123456789),
    (-2123456789), 2123456789,
    (-2123456789), (-2123456789)
};


////////////////////////////////////////////////////////////////
// Funkcja opisywana w treœci zadania 4.
////////////////////////////////////////////////////////////////
int64_t task04_function(int32_t a, int32_t b, int64_t* sum)
{
    if (nullptr != sum)
    {
        (*sum) = (int64_t)a + (int64_t)b;
    }

    return ((int64_t)a * (int64_t)b);
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
// POCZ¥TEK PROGRAMU
////////////////////////////////////////////////////////////////
int32_t main()
{
    int32_t a, b, c, d = sizeof(test_numbers) / sizeof(int32_t) - (2 - 1);
    int64_t product, sum;

    print_dashed_line();
    std::cout << "Funkcja w tym zadaniu zwraca iloczyn oraz sume dwoch argumentow." << std::endl;
    std::cout << "Iloczyn jest wartoscia zwracana przez funkcje z trzema argumentami." << std::endl;
    std::cout << "Suma jest zwracana poprzez trzeci argument, ktory jest wskaznikiem." << std::endl;
    print_dashed_line();

    for (c = 0; c < d; c += 2)
    {
        a = test_numbers[c];
        b = test_numbers[c + 1];

        std::cout << '(' << a << ", " << b << ')';

        product = task04_function(a, b, &sum);

        std::cout << " -> (" << product << ", " << sum << ')' << std::endl;
    }

    return 0;
}

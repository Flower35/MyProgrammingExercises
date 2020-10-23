/****************************************************************
  [LAB 02, ZADANIE 05]
    Napisz funkcjê, która zamienia miejscami wartoœci swoich
    dwóch argumentów. Przeka¿ do funkcji argumenty przy u¿yciu
    wskaŸników albo referencji. Przygotuj odpowiednie testy
    dla zaproponowanego rozwi¹zania.

  [LAB 02, ZADANIE 06]
    Nale¿y wzi¹æ pod uwagê, ¿e funkcja `std::swap()` powoduje
    skopiowanie obiektu do zmiennej tymczasowej i dwa razy wywo³uje
    operator przypisania, wiêc mo¿e nie byæ efektywna dla typów,
    dla których wspomniane operacje s¹ kosztowne.
    W takie sytuacji rekomendowane jest przeci¹¿enie tej funkcji tak,
    by wykonywa³a zamianê w wydajniejszy sposób.

****************************************************************/

#include <iostream>
#include <new>

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <ctime>


////////////////////////////////////////////////////////////////
// Funkcja do zamiany miejscami swoich argumentów
// (traktowanych jako bloki danych o ustalonym rozmiarze),
// bez wywo³ywania konstruktora kopiowania i przypisywania.
////////////////////////////////////////////////////////////////
template <typename T>
void swap(T &a, T &b)
{
    int32_t i;
    const int32_t size = sizeof(T) >> 2;
    const int32_t tail = sizeof(T) - (size << 2);

    uint32_t temp[2];
    uint8_t* blocks[2] = {(uint8_t*)&a, (uint8_t*)&b};

    /* Zamienianie 32-bitowych (4-bajtowych) bloków */

    for (i = 0; i < size; i++)
    {
        /* Sztuczka: `b = (a xor b) xor b; a = (a xor b) xor a;` */

        *((uint32_t*)blocks[0]) ^= *((uint32_t*)blocks[1]);
        *((uint32_t*)blocks[1]) ^= *((uint32_t*)blocks[0]);
        *((uint32_t*)blocks[0]) ^= *((uint32_t*)blocks[1]);

        blocks[0] += 4;
        blocks[1] += 4;
    }

    /* Zamienianie koñcówki (od 0 do 3 bajtów) */

    if (tail > 0)
    {
        for (i = 0; i < tail; i++)
        {
            temp[0] = (temp[0] << 8) | *(blocks[0]);
            temp[1] = (temp[1] << 8) | *(blocks[1]);

            blocks[0] += 1;
            blocks[1] += 1;
        }

        blocks[0] -= tail;
        blocks[1] -= tail;

        /* Dosuwamy bajty od prawej do lewej */

        for (i = tail; i < 4; i++)
        {
            temp[0] <<= 8;
            temp[1] <<= 8;
        }

        temp[0] ^= temp[1];
        temp[1] ^= temp[0];
        temp[0] ^= temp[1];

        for (i = 0; i < tail; i++)
        {
            *(blocks[0]) = (uint8_t)(temp[0] >> 24);
            *(blocks[1]) = (uint8_t)(temp[1] >> 24);

            temp[0] <<= 8;
            temp[1] <<= 8;

            blocks[0] += 1;
            blocks[1] += 1;
        }
    }
}


////////////////////////////////////////////////////////////////
// Dane do testowania zadania.
// (klasa o du¿ym rozmiarze, niepodzielnym bez reszty przez 4)
////////////////////////////////////////////////////////////////

#define LARGE_CONTAINER_SIZE (4 * 1024 - 1)

class LargeContainer
{
    private:

        uint8_t contents[LARGE_CONTAINER_SIZE];

    public:

        void printAddress() const
        {
            std::printf("[0x%08X]", this);
        }

        void randomFill()
        {
            std::cout << "Wypelnianie obiektu ";
            printAddress();
            std::cout << " losowymi bajtami." << std::endl;

            for (int32_t i = 0; i < sizeof(contents); i++)
            {
                contents[i] = (uint8_t)std::rand();
            }
        }

        LargeContainer()
        {
            printAddress();
            std::cout << " Konstruktor obiektu testowego." << std::endl;
            std::memset(contents, 0x00, sizeof(contents));
        }

        LargeContainer& operator = (const LargeContainer &other)
        {
            std::cout << "Kopiowanie obiektu ";
            other.printAddress();
            std::cout << " do ";
            printAddress();
            std::cout << std::endl;

            std::memcpy(contents, other.contents, sizeof(contents));

            return (*this);
        }

        ~LargeContainer()
        {
            printAddress();
            std::cout << " Destruktor obiektu testowego." << std::endl;
        }
};


////////////////////////////////////////////////////////////////
// Porównanie obiektów po zamianie pierwszego z drugim
////////////////////////////////////////////////////////////////
bool compare_after_swap(LargeContainer* test_objects)
{
    if (0 != std::memcmp(&(test_objects[0]), &(test_objects[3]), sizeof(LargeContainer)))
    {
        return false;
    }

    if (0 != std::memcmp(&(test_objects[1]), &(test_objects[2]), sizeof(LargeContainer)))
    {
        return false;
    }

    return true;
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
    LargeContainer* test_objects;

    /********************************/
    /* Alokacja pamiêci: dwa obiekty do zamiany miêdzy sob¹ + ich wstêpne kopie */

    print_dashed_line();

    try
    {
        test_objects = new LargeContainer[4];
    }
    catch (std::bad_alloc)
    {
        std::cerr << "Niewystarczajaca ilosc pamieci na uruchomienie tej aplikacji! :(" << std::endl;

        return 1;
    }

    /********************************/
    /* Przygotowanie danych (losowe wype³nienie + skopiowanie pierwotnych obiektów) */

    print_dashed_line();

    std::srand(std::time(NULL));

    test_objects[0].randomFill();
    test_objects[1].randomFill();

    print_dashed_line();

    test_objects[2] = test_objects[0];
    test_objects[3] = test_objects[1];

    /********************************/
    /* Zamiana obiektów miejscami i sprawdzenie poprawnoœci zamiany. */

    print_dashed_line();

    swap(test_objects[0], test_objects[1]);

    if (compare_after_swap(test_objects))
    {
        std::cout << "OBIEKTY ZAMIENIONE MIEJSCAMI POMYSLNIE!" << std::endl;
    }
    else
    {
        std::cout << "NIEPOWODZENIE ZAMIANY OBIEKTOW MIEJSCAMI!" << std::endl;
    }

    /********************************/
    /* Zwolnienie pamiêci i zamkniêcie programu. */

    print_dashed_line();

    delete[](test_objects);

    return 0;
}

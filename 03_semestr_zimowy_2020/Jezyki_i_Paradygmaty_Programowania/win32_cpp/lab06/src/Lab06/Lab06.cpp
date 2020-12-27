/****************************************************************
  [LAB 06]
    Implementacja systemu dla dzia³u „Human Resources” w firmie.
    Klasa "HRMS" ma zawieraæ:

  * kontener w którym bêd¹ przechowywani wszyscy pracownicy

  * kontener w którym bêdzie przechowywane mapowanie pomiêdzy
   identyfikatorami departamentów a identyfikatorami wszystkich
   pracowników danego departamentu

  * kontener przechowywuj¹cy informacje o zarobkach pracowników

  * metoda `add(Employee employee, std::string departmentId, double salary)`
   dodaj¹ca u¿ytkownika do systemu, do danego departamentu,
   maj¹cego dane wynagrodzenie

  * metoda `printDepartment(std::string departmentId)` wypisuj¹ca
   wszystkich pracowników danego departamentu

  * metoda `changeSalary(std::string employeeId, double salary)`
   zmieniaj¹ca wynagrodzenie danego pracownika

  * metoda `printSalaries()` wypisuj¹ca wynagrodzenia
   wszystkich pracowników, wraz z informacjami o tych pracownikach

  * metoda `printSalariesSorted()` wypisuj¹ca wynagrodzenia
   wszystkich pracowników, wraz z informacjami o tych pracownikach,
   w kolejnoœci malej¹cego wynagrodzenia

    Nale¿y zaimplementowaæ metodê `main()` demonstruj¹c¹ dzia³anie
    powy¿szego kodu dla co najmniej 10 pracowników pracuj¹cych
    w co najmniej 3 departamentach.

****************************************************************/

#include <Lab06/Lab06.h>


namespace Lab06
{

    ////////////////////////////////////////////////////////////////
    // [funkcja pomocnicza]
    // Wyœwietlenie odstêpu w konsoli.
    ////////////////////////////////////////////////////////////////
    void printDashedLine()
    {
        std::cout.width(64);
        std::cout.fill('-');

        std::cout << "" << std::endl;

        std::cout.fill(' ');
    }


    ////////////////////////////////////////////////////////////////
    // [funkcja pomocnicza]
    // Porównanie dwóch tekstów.
    // (bez wzglêdu na wielkoœæ znaków)
    ////////////////////////////////////////////////////////////////
    bool compareStringsLowercase(const std::string & s1, const std::string & s2)
    {
        size_t len = s1.length();
        size_t a;

        if (s2.length() != len)
        {
            return false;
        }

        for (a = 0; a < len; a++)
        {
            if (std::tolower(s1.at(a)) != std::tolower(s2.at(a)))
            {
                return false;
            }
        }

        return true;
    }


    ////////////////////////////////////////////////////////////////
    // [funkcja pomocnicza]
    // Sprawdzenie czy wybrany napis znajduje siê
    // w tablicy i zwrócenie odpowiedniego indeksu.
    // Funkcja zwraca (-1) w przypadku niedopasowania.
    ////////////////////////////////////////////////////////////////
    int32_t findStringInArray(const std::string & s, const std::list<std::string> & v)
    {
        size_t a = 0;
        std::list<std::string>::const_iterator it;

        for (it = v.begin(); v.end() != it; it++)
        {
            if (compareStringsLowercase(s, (*it)))
            {
                return a;
            }

            a++;
        }

        return (-1);
    }


    ////////////////////////////////////////////////////////////////
    // [funkcja pomocnicza]
    // Czy tekst "s1" zawiera tekst "s2" ?
    // (bez wzglêdu na wielkoœæ znaków)
    ////////////////////////////////////////////////////////////////
    bool stringContainsLowercase(const std::string & s1, const std::string & s2)
    {
        size_t len1 = s1.length();
        size_t len2 = s2.length();
        size_t a, b;
        bool result;

        if (len2 > len1)
        {
            return false;
        }

        len1 = len1 - len2 + 1;

        for (a = 0; a < len1; a++)
        {
            result = true;

            for (b = 0; result && (b < len2); b++)
            {
                if (std::tolower(s1.at(a + b)) != std::tolower(s2.at(b)))
                {
                    result = false;
                }
            }

            if (result)
            {
                return true;
            }
        }

        return false;
    }


    ////////////////////////////////////////////////////////////////
    // [funkcja pomocnicza]
    // Sortowanie tekstów alfabetycznie.
    // (bez wzglêdu na wielkoœæ znaków)
    ////////////////////////////////////////////////////////////////
    // Funkcja zwraca PRAWDÊ, je¿eli pierwszy tekst
    // powinien znajdowaæ siê przed tekstem drugim.
    ////////////////////////////////////////////////////////////////
    bool sortStringsLowercaseAscending(const std::string & s1, const std::string & s2)
    {
        size_t len1 = s1.length();
        size_t len2 = s2.length();
        size_t len = (len1 < len2) ? len1 : len2;
        size_t a;
        int comp;

        for (a = 0; a < len; a++)
        {
            comp = std::tolower(s2.at(a)) - std::tolower(s1.at(a));

            if (comp < 0)
            {
                /* Nale¿y zmieniæ kolejnoœæ. */
                return false;
            }
            else if (comp > 0)
            {
                /* Drugi tekst za tekstem pierwszym. */
                return true;
            }
        }

        return (len1 < len2);
    }


    ////////////////////////////////////////////////////////////////
    // [funkcja pomocnicza]
    // Utworzene listy s³ów kluczowych rozdzielonych wybranym znakiem.
    ////////////////////////////////////////////////////////////////
    std::list<std::string> splitStringsBySpaces(const std::string & source, const char space)
    {
        std::list<std::string> result;
        size_t word_start = 0;
        size_t source_end = source.length();
        size_t word_end = 0;
        char last_char = space;
        char current_char;

        while (word_end < source_end)
        {
            current_char = source.at(word_end);

            if (space == last_char)
            {
                if (space != current_char)
                {
                    word_start = word_end;
                }
            }
            else if (space == current_char)
            {
                result.push_back
                (
                    source.substr(word_start, word_end - word_start)
                );
            }

            last_char = current_char;
            word_end++;
        }

        if (' ' != last_char)
        {
            result.push_back
            (
                source.substr(word_start, source_end - word_start)
            );
        }

        return result;
    }

}

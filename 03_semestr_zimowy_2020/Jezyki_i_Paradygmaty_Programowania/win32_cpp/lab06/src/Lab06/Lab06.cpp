////////////////////////////////////////////////////////////////
// Laboratorium 06.
////////////////////////////////////////////////////////////////

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

}


////////////////////////////////////////////////////////////////
// Testowanie aplikacji
////////////////////////////////////////////////////////////////

#include <Lab06/HRMS.h>

#include <vector>

const uint32_t NUM_DEPARTMENTS =  5;
const uint32_t NUM_POSITIONS   =  7;

const char * RANDOM_NAMES[NUM_DEPARTMENTS] =
{
    "lorem",
    "ipsum",
    "dolor",
    "sit",
    "amet"
};

const char * DEPARTMENT_NAMES[NUM_DEPARTMENTS] =
{
    "General Management",
    "Marketing",
    "Operations",
    "Finance",
    "Human Resource"
};

const char * POSITION_NAMES[NUM_POSITIONS] =
{
    "Administrator",
    "Gaming Director",

    "Advertising Executive",

    "Radio Station Manager",
    "General Superintendent",

    "Credit Manager",

    "Programmer"
};

int32_t main()
{
    try
    {
        Lab06::HRMS test_system;

        std::vector<uint32_t> departments;
        std::vector<uint32_t> positions;
        std::vector<uint32_t> employees;

        std::list<uint32_t> indices;

        size_t a, b;
        double c;

        /********************************/
        /* Tworzenie departamentów z "przerwami w identyfikatorach" */

        for (a = 0; a < NUM_DEPARTMENTS; a++)
        {
            test_system.removeDepartment
            (
                test_system.addDepartment(RANDOM_NAMES[a])
            );

            departments.push_back
            (
                test_system.addDepartment(DEPARTMENT_NAMES[a])
            );
        }

        /********************************/

        Lab06::printDashedLine();

        std::cout << " * Utworzono departamenty:" << std::endl;

        Lab06::printDashedLine();

        test_system.printAllDepartments();

        Lab06::printDashedLine();

        /********************************/
        /* Tworzenie stanowisk z "przerwami w identyfikatorach" */

        for (a = 0; a < NUM_POSITIONS; a++)
        {
            for (b = 0; b < 3; b++)
            {
                test_system.removePosition
                (
                    test_system.addPosition("")
                );
            }

            positions.push_back
            (
                test_system.addPosition(POSITION_NAMES[a])
            );
        }

        /********************************/

        Lab06::printDashedLine();

        std::cout << " * Utworzono stanowiska:" << std::endl;

        Lab06::printDashedLine();

        test_system.printAllPositions();

        Lab06::printDashedLine();

        /********************************/
        /* Dodawanie pracowników:     */
        /* Testowanie funkcji `add()` */

        employees.push_back
        (
            test_system.addEmployee
            (
                "Adam",
                "Mickiewicz",
                departments[0],
                positions[0],
                100.0
            )
        );

        employees.push_back
        (
            test_system.addEmployee
            (
                "Fryderyk",
                "Chopin",
                departments[0],
                positions[1],
                300.0
            )
        );

        employees.push_back
        (
            test_system.addEmployee
            (
                "Mikolaj",
                "Rej",
                departments[0],
                positions[1],
                1100.0
            )
        );

        employees.push_back
        (
            test_system.addEmployee
            (
                "Giuseppe",
                "Verdi",
                departments[1],
                positions[2],
                500.0
            )
        );

        employees.push_back
        (
            test_system.addEmployee
            (
                "Stanislaw",
                "Lem",
                departments[2],
                positions[3],
                1800.0
            )
        );

        employees.push_back
        (
            test_system.addEmployee
            (
                "Antonio",
                "Vivaldi",
                departments[2],
                positions[3],
                400.0
            )
        );

        employees.push_back
        (
            test_system.addEmployee
            (
                "Ignacy",
                "Krasicki",
                departments[2],
                positions[4],
                1200.0
            )
        );

        employees.push_back
        (
            test_system.addEmployee
            (
                "Ferenc",
                "Liszt",
                departments[2],
                positions[4],
                200.0
            )
        );

        employees.push_back
        (
            test_system.addEmployee
            (
                "Julian",
                "Tuwim",
                departments[3],
                positions[5],
                1900.0
            )
        );

        employees.push_back
        (
            test_system.addEmployee
            (
                "Richard",
                "Wagner",
                departments[3],
                positions[5],
                1500.0
            )
        );

        employees.push_back
        (
            test_system.addEmployee
            (
                "Juliusz",
                "Slowacki",
                departments[3],
                positions[5],
                1000.0
            )
        );

        employees.push_back
        (
            test_system.addEmployee
            (
                "Franz",
                "Schubert",
                departments[1],
                positions[2],
                1600.0
            )
        );

        employees.push_back
        (
            test_system.addEmployee
            (
                "Aleksander",
                "Fredro",
                departments[0],
                positions[0],
                2000.0
            )
        );

        employees.push_back
        (
            test_system.addEmployee
            (
                "Edvard",
                "Grieg",
                departments[0],
                positions[0],
                1700.0
            )
        );

        employees.push_back
        (
            test_system.addEmployee
            (
                "Stanislaw",
                "Wyspianski",
                departments[4],
                positions[6],
                700.0
            )
        );

        employees.push_back
        (
            test_system.addEmployee
            (
                "Piotr",
                "Czajkowski",
                departments[2],
                positions[4],
                1300.0
            )
        );

        employees.push_back
        (
            test_system.addEmployee
            (
                "Jan",
                "Kasprowicz",
                departments[4],
                positions[6],
                800.0
            )
        );

        employees.push_back
        (
            test_system.addEmployee
            (
                "Ludwig van",
                "Beethoven",
                departments[4],
                positions[6],
                600.0
            )
        );

        employees.push_back
        (
            test_system.addEmployee
            (
                "Wladyslaw",
                "Reymont",
                departments[2],
                positions[3],
                1400.0
            )
        );

        employees.push_back
        (
            test_system.addEmployee
            (
                "Wolfgang Amadeus",
                "Mozart",
                departments[4],
                positions[6],
                900.0
            )
        );

        /********************************/

        Lab06::printDashedLine();

        std::cout << " * Dodano pracownikow:" << std::endl;

        Lab06::printDashedLine();

        test_system.printAllEmployees();

        Lab06::printDashedLine();

        /********************************/
        /* Wypisywanie pracowników z wybranych dzia³ów,   */
        /* sortowanie listy alfabetycznie wed³ug nazwisk. */

        for (a = 0; a < NUM_DEPARTMENTS; a++)
        {
            Lab06::printDashedLine();

            std::cout
                << " * [" << (a + 1) << "/" << NUM_DEPARTMENTS
                << "] Pracownicy dzialu \""
                << test_system.getDepartment(departments[a]).getText()
                << "\" (sort po nazwiskach):" << std::endl;

            Lab06::printDashedLine();

            indices = test_system.findEmployeesByDepartment(departments[a]);
            test_system.sortEmployeesListBySurname(indices);
            test_system.printEmployees(indices);

            Lab06::printDashedLine();
        }

        /********************************/
        /* Wypisywanie pracowników z wybranych stanowisk, */
        /* sortowanie listy malej¹co wed³ug pensi.        */

        for (a = 0; a < NUM_POSITIONS; a++)
        {
            Lab06::printDashedLine();

            std::cout
                << " * [" << (a + 1) << "/" << NUM_POSITIONS
                << "] Pracownicy na pozycji \""
                << test_system.getPosition(positions[a]).getText()
                << "\" (sort po pensjach):" << std::endl;

            Lab06::printDashedLine();

            indices = test_system.findEmployeesByPosition(positions[a]);
            test_system.sortEmployeesListBySalary(indices);
            test_system.printEmployees(indices);

            Lab06::printDashedLine();
        }

        /********************************/
        /* Testowanie funkcji `changeSalary()` i `printSalariesSorted()` */

        b = employees.size();
        c = 125.30;
        for (a = 0; a < b; a++)
        {
            test_system.changeSalary(employees[a], c);
            c += 125.30;
        }

        Lab06::printDashedLine();

        std::cout
            << " * Pracownicy po zmianach pensji:"
            << std::endl;

        Lab06::printDashedLine();

        indices = test_system.getAllEmployeeIds();
        test_system.sortEmployeesListBySalary(indices);
        test_system.printEmployees(indices);

        Lab06::printDashedLine();

        /********************************/

        Lab06::printDashedLine();

        std::cout
            << " * Pracownicy, ktorzy maja w nazwisku 'ow', sort po imionach:"
            << std::endl;

        Lab06::printDashedLine();

        indices = test_system.findEmployeesBySurname("ow");
        test_system.sortEmployeesListByName(indices);
        test_system.printEmployees(indices);

        Lab06::printDashedLine();

        /********************************/

        Lab06::printDashedLine();

        std::cout
            << " * Departamenty, ktore maja w nazwie 'an', alfabetycznie:"
            << std::endl;

        Lab06::printDashedLine();

        indices = test_system.findDepartmentsContainingName("an");
        test_system.sortDepartmentsListByName(indices);
        test_system.printDepartments(indices);

        Lab06::printDashedLine();

        /********************************/

        Lab06::printDashedLine();

        std::cout
            << " *Stanowiska, ktore maja w nazwie 'in', alfabetycznie:"
            << std::endl;

        Lab06::printDashedLine();

        indices = test_system.findPositionsContainingName("in");
        test_system.sortPositionsListByName(indices);
        test_system.printPositions(indices);

        Lab06::printDashedLine();

        /********************************/
    }
    catch (std::exception &err)
    {
        Lab06::printDashedLine();

        std::cout
            << "[EXCEPTION!]" << std::endl
            << "  " << err.what() << std::endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

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


////////////////////////////////////////////////////////////////
// Generowanie przyk³adowych danych
////////////////////////////////////////////////////////////////
void generateExampleData
(
    Lab06::HRMS & hrms
)
{
    std::vector<uint32_t> departments;
    std::vector<uint32_t> positions;
    size_t a, b;

    /********************************/
    /* Tworzenie departamentów z "przerwami w identyfikatorach" */

    for (a = 0; a < NUM_DEPARTMENTS; a++)
    {
        hrms.removeDepartment
        (
            hrms.addDepartment(RANDOM_NAMES[a])
        );

        departments.push_back
        (
            hrms.addDepartment(DEPARTMENT_NAMES[a])
        );
    }

    /********************************/

    Lab06::printDashedLine();

    std::cout << " * Utworzono departamenty:" << std::endl;

    hrms.printAllDepartments();

    Lab06::printDashedLine();

    /********************************/
    /* Tworzenie stanowisk z "przerwami w identyfikatorach" */

    for (a = 0; a < NUM_POSITIONS; a++)
    {
        for (b = 0; b < 3; b++)
        {
            hrms.removePosition
            (
                hrms.addPosition("")
            );
        }

        positions.push_back
        (
            hrms.addPosition(POSITION_NAMES[a])
        );
    }

    /********************************/

    Lab06::printDashedLine();

    std::cout << " * Utworzono stanowiska:" << std::endl;

    hrms.printAllPositions();

    Lab06::printDashedLine();

    /********************************/
    /* Dodawanie pracowników... */

    hrms.addEmployee
    (
        "Adam",
        "Mickiewicz",
        departments[0],
        positions[0],
        100.0
    );

    hrms.addEmployee
    (
        "Fryderyk",
        "Chopin",
        departments[0],
        positions[1],
        300.0
    );

    hrms.addEmployee
    (
        "Mikolaj",
        "Rej",
        departments[0],
        positions[1],
        1100.0
    );

    hrms.addEmployee
    (
        "Giuseppe",
        "Verdi",
        departments[1],
        positions[2],
        500.0
    );

    hrms.addEmployee
    (
        "Stanislaw",
        "Lem",
        departments[2],
        positions[3],
        1800.0
    );

    hrms.addEmployee
    (
        "Antonio",
        "Vivaldi",
        departments[2],
        positions[3],
        400.0
    );

    hrms.addEmployee
    (
        "Ignacy",
        "Krasicki",
        departments[2],
        positions[4],
        1200.0
    );

    hrms.addEmployee
    (
        "Ferenc",
        "Liszt",
        departments[2],
        positions[4],
        200.0
    );

    hrms.addEmployee
    (
        "Julian",
        "Tuwim",
        departments[3],
        positions[5],
        1900.0
    );

    hrms.addEmployee
    (
        "Richard",
        "Wagner",
        departments[3],
        positions[5],
        1500.0
    );

    hrms.addEmployee
    (
        "Juliusz",
        "Slowacki",
        departments[3],
        positions[5],
        1000.0
    );

    hrms.addEmployee
    (
        "Franz",
        "Schubert",
        departments[1],
        positions[2],
        1600.0
    );

    hrms.addEmployee
    (
        "Aleksander",
        "Fredro",
        departments[0],
        positions[0],
        2000.0
    );

    hrms.addEmployee
    (
        "Edvard",
        "Grieg",
        departments[0],
        positions[0],
        1700.0
    );

    hrms.addEmployee
    (
        "Stanislaw",
        "Wyspianski",
        departments[4],
        positions[6],
        700.0
    );

    hrms.addEmployee
    (
        "Piotr",
        "Czajkowski",
        departments[2],
        positions[4],
        1300.0
    );

    hrms.addEmployee
    (
        "Jan",
        "Kasprowicz",
        departments[4],
        positions[6],
        800.0
    );

    hrms.addEmployee
    (
        "Ludwig van",
        "Beethoven",
        departments[4],
        positions[6],
        600.0
    );

    hrms.addEmployee
    (
        "Wladyslaw",
        "Reymont",
        departments[2],
        positions[3],
        1400.0
    );

    hrms.addEmployee
    (
        "Wolfgang Amadeusz",
        "Mozart",
        departments[4],
        positions[6],
        900.0
    );

    /********************************/

    Lab06::printDashedLine();

    std::cout << " * Dodano pracownikow:" << std::endl;

    hrms.printAllEmployees();

    Lab06::printDashedLine();
}


////////////////////////////////////////////////////////////////
// Wyra¿enia sta³e dla metod parsuj¹cych.
////////////////////////////////////////////////////////////////

const int32_t OBJECT_TYPE_EMPLOYEE   = 0;
const int32_t OBJECT_TYPE_DEPARTMENT = 1;
const int32_t OBJECT_TYPE_POSITION   = 2;

const std::list<std::string> OBJECT_TYPE_NAMES =
{
    "EMPLOYEE",
    "DEPARTMENT",
    "POSITION"
};

const int32_t FIND_METHOD_ID         = 0;
const int32_t FIND_METHOD_NAME       = 1;
const int32_t FIND_METHOD_SURNAME    = 2;
const int32_t FIND_METHOD_DEPARTMENT = 3;
const int32_t FIND_METHOD_POSITION   = 4;
const int32_t FIND_METHOD_SALARY     = 5;

const std::list<std::string> FIND_METHOD_NAMES =
{
    "ID",
    "NAME",
    "SURNAME",
    "DEPARTMENT",
    "POSITION",
    "SALARY"
};

const int32_t SORT_METHOD_NAME     = 0;
const int32_t SORT_METHOD_SURNAME  = 1;
const int32_t SORT_METHOD_POSITION = 2;
const int32_t SORT_METHOD_SALARY   = 3;

const std::list<std::string> SORT_METHOD_NAMES =
{
    "NAME",
    "SURNAME",
    "POSITION",
    "SALARY"
};


////////////////////////////////////////////////////////////////
// Sprawdzenie, czy istnieje kolejne s³owo kluczowe
// w tablicy, do której wstawiono podzielone polecenie.
////////////////////////////////////////////////////////////////
bool getNextKeyword
(
    const std::list<std::string> & keywords,
    std::list<std::string>::iterator & it
)
{
    if (keywords.end() != it)
    {
        it++;
        return (keywords.end() != it);
    }

    return false;
}


////////////////////////////////////////////////////////////////
// Parsowanie opcjonalnego polecenia "WITH"
// pojawiaj¹cego siê w wiêkszoœci poleceñ wyszukuj¹cych.
////////////////////////////////////////////////////////////////
// Funkcja zwraca 0, je¿eli napotkano b³¹d
// uniemo¿liwiaj¹cy dalsze parsowanie polecenia.
////////////////////////////////////////////////////////////////
// Funkcja zwraca 2, je¿eli zabrak³o kolejnych
// s³ów kluczowych, niezbêdnych do ustalenia metody wybierania.
////////////////////////////////////////////////////////////////
int32_t parseTheWithKeyword
(
    const char * current_keyword,
    const std::list<std::string> & keywords,
    std::list<std::string>::iterator & key_it,
    const int32_t object_type,
    int32_t & find_option,
    uint32_t * returned_indices,
    double * returned_salaries,
    std::string & returned_name
)
{
    std::list<std::string> dashed_keyword;
    std::list<std::string>::iterator dash_it;

    if (find_option < 0)
    {
        if (!getNextKeyword(keywords, key_it))
        {
            return 2;
        }

        find_option = Lab06::findStringInArray(*key_it, FIND_METHOD_NAMES);

        if
        (
            (find_option < 0)
            || ((object_type > OBJECT_TYPE_EMPLOYEE) && (find_option >= FIND_METHOD_SURNAME))
        )
        {
            std::cout << "\"" << current_keyword
                << "\": Niewlasciwa metoda wyszukiwania \""
                << *key_it << "\"!" << std::endl;
            Lab06::printDashedLine();
            return 0;
        }

        if (getNextKeyword(keywords, key_it))
        {
            switch (find_option)
            {
                case FIND_METHOD_ID:
                {
                    dashed_keyword = Lab06::splitStringsBySpaces(*key_it, ':');
                    dash_it = dashed_keyword.begin();

                    returned_indices[0] = std::stoi(*dash_it);

                    if (getNextKeyword(dashed_keyword, dash_it))
                    {
                        returned_indices[1] = std::stoi(*dash_it);
                    }
                    else
                    {
                        returned_indices[1] = returned_indices[0];
                    }
                }
                break;

                case FIND_METHOD_SALARY:
                {
                    dashed_keyword = Lab06::splitStringsBySpaces(*key_it, ':');
                    dash_it = dashed_keyword.begin();

                    returned_indices[0] = std::stoi(*dash_it);

                    if (getNextKeyword(dashed_keyword, dash_it))
                    {
                        returned_salaries[1] = std::stod(*dash_it);
                    }
                    else
                    {
                        returned_salaries[1] = returned_salaries[0];
                    }
                }
                break;

                case FIND_METHOD_DEPARTMENT:
                case FIND_METHOD_POSITION:
                {
                    returned_indices[0] = std::stoi(*key_it);
                    returned_indices[1] = returned_indices[0];
                }

                default:
                {
                    returned_name = *key_it;
                }
            }

            return 1;
        }
    }
    else
    {
        std::cout << "\"" << current_keyword
            << "\": Slowo kluczowe \"WITH\" uzyte wielokrotnie!" << std::endl;
        Lab06::printDashedLine();
        return 0;
    }

    return 2;
}


////////////////////////////////////////////////////////////////
// Zawê¿enie zbioru identyfikatorów na podstawie
// wczeœniej sparsowanego s³owa kluczowego "WITH".
////////////////////////////////////////////////////////////////
void runTheFindQuery
(
    const Lab06::HRMS & hrms,
    const int32_t object_type,
    const int32_t find_option,
    uint32_t * find_indices,
    double * find_salaries,
    std::string & find_name,
    std::list<uint32_t> & returned_indices,
    bool get_all_by_default
)
{
    switch (object_type)
    {
        case OBJECT_TYPE_EMPLOYEE:
            switch (find_option)
            {
                case FIND_METHOD_ID:
                    returned_indices = hrms.findEmployeesById(find_indices[0], find_indices[1]);
                    return;

                case FIND_METHOD_NAME:
                    returned_indices = hrms.findEmployeesByName(find_name);
                    return;

                case FIND_METHOD_SURNAME:
                    returned_indices = hrms.findEmployeesBySurname(find_name);
                    return;

                case FIND_METHOD_DEPARTMENT:
                    returned_indices = hrms.findEmployeesByDepartment(find_indices[0]);
                    return;

                case FIND_METHOD_POSITION:
                    returned_indices = hrms.findEmployeesByPosition(find_indices[0]);
                    return;

                case FIND_METHOD_SALARY:
                    returned_indices = hrms.findEmployeesBySalary(find_salaries[0], find_salaries[1]);
                    return;

                default:
                    if (get_all_by_default)
                    {
                        returned_indices = hrms.getAllEmployeeIds();
                    }
                    return;
            }
            break;

        case OBJECT_TYPE_DEPARTMENT:
            switch (find_option)
            {
                case FIND_METHOD_ID:
                    returned_indices = hrms.findDepartmentsById(find_indices[0], find_indices[1]);
                    return;

                case FIND_METHOD_NAME:
                    returned_indices = hrms.findDepartmentsContainingName(find_name);
                    return;

                default:
                    if (get_all_by_default)
                    {
                        returned_indices = hrms.getAllDepartmentIds();
                    }
                    return;
            }
            break;

        case OBJECT_TYPE_POSITION:
            switch (find_option)
            {
                case FIND_METHOD_ID:
                    returned_indices = hrms.findPositionsById(find_indices[0], find_indices[1]);
                    return;

                case FIND_METHOD_NAME:
                    returned_indices = hrms.findPositionsContainingName(find_name);
                    return;

                default:
                    if (get_all_by_default)
                    {
                        returned_indices = hrms.getAllPositionIds();
                    }
                    return;
            }
            break;
    }
}


////////////////////////////////////////////////////////////////
// Parsowanie ró¿nych poleceñ pozwalaj¹cych
// na zarz¹dzanie prost¹ baz¹ danych.
////////////////////////////////////////////////////////////////
int32_t parseCommand(Lab06::HRMS & hrms)
{
    std::list<std::string> keywords;
    std::list<std::string>::iterator key_it;
    std::list<std::string>::iterator quote_it;

    int32_t type_option = (-1);
    int32_t find_option = (-1);
    int32_t sort_option = (-1);

    bool dummy_state;
    uint32_t dummy_int[3] = {0};
    double dummy_double[3] = {0};
    std::string dummy_str[2];

    std::list<uint32_t> indices;
    std::list<uint32_t>::iterator idx_it;

    /********************************/

    Lab06::printDashedLine();

    std::cout << "Wpisz polecenie! (\"HELP\" aby wyswietlic liste polecen)" << std::endl
        << ">> ";

    std::getline(std::cin, dummy_str[0]);
    keywords = Lab06::splitStringsBySpaces(dummy_str[0], ' ');
    key_it = keywords.begin();

    while (keywords.end() != key_it)
    {
        if ('"' == key_it->front())
        {
            if ('"' == key_it->back())
            {
                *key_it = key_it->substr(1, key_it->length() - 2);
                key_it++;
            }
            else
            {
                dummy_state = true;
                dummy_str[0] = key_it->substr(1);
                quote_it = key_it;
                quote_it++;

                while (dummy_state && (keywords.end() != quote_it))
                {
                    dummy_str[0] += ' ';

                    if ('"' == quote_it->back())
                    {
                        dummy_str[0] += quote_it->substr(0, quote_it->length() - 1);
                        dummy_state = false;
                    }
                    else
                    {
                        dummy_str[0] += *quote_it;
                    }

                    quote_it = keywords.erase(quote_it);
                }

                *key_it = dummy_str[0];
                key_it = quote_it;
            }
        }
        else
        {
            key_it++;
        }
    }

    key_it = keywords.begin();

    Lab06::printDashedLine();

    if (keywords.size() > 0)
    {
        if (Lab06::compareStringsLowercase("EXIT", *key_it))
        {
            return 0;
        }
        else if (Lab06::compareStringsLowercase("HELP", *key_it))
        {
            std::cout
                << "Polecenia do obslugi prostej bazy danych:" << std::endl
                << std::endl;

            std::cout
                << ">> EXIT" << std::endl
                << "  -- wyjscie z programu;" << std::endl
                << std::endl;

            std::cout
                << ">> HELP" << std::endl
                << "  -- wyswietlenie dostepnych polecen z opisami;" << std::endl
                << std::endl;

            std::cout
                << ">> EXAMPLES" << std::endl
                << "  -- wyswietlenie przykladow korzystania z bazy;" << std::endl
                << std::endl;

            std::cout
                << ">> PRINT EMPLOYEE|DEPARTMENT|POSITION" << std::endl
                << "[WITH ID|NAME|SURNAME|DEPARTMENT|POSITION|SALARY ...]" << std::endl
                << "[SORT NAME|SURNAME|POSITION|SALARY]" << std::endl
                << "  -- wypisanie danych na ekranie; dla wyszukiwan \"ID\" i \"SALARY\"" << std::endl
                << "  -- mozna podac dwie liczby z dwukropkiem pomiedzy (jako zakres);" << std::endl
                << std::endl;

            std::cout
                << ">> ADD EMPLOYEE|DEPARTMENT|POSITION ..." << std::endl
                << "  -- dodanie nowego obiektu do bazy; dla pracownika podajemy:" << std::endl
                << "  -- imie, nazwisko, ID departamentu, ID stanowiska, pensje;" << std::endl
                << "  -- dla pozostalych typow podajemy tylko nazwe;" << std::endl
                << std::endl;

            std::cout
                << ">> REMOVE EMPLOYEE|DEPARTMENT|POSITION" << std::endl
                << "WITH ID|NAME|SURNAME|DEPARTMENT|POSITION|SALARY ..." << std::endl
                << "  -- usuniecie obiektow spelniajacych kryterium" << std::endl
                << std::endl;

            std::cout
                << ">> CHANGE EMPLOYEE|DEPARTMENT|POSITION" << std::endl
                << "WITH ID|NAME|SURNAME|DEPARTMENT|POSITION|SALARY ..." << std::endl
                << "PROP NAME|SURNAME|DEPARTMENT|POSITION|SALARY ..." << std::endl
                << "  -- zmiana wlasnosci obiektow spelniajacych kryterium" << std::endl
                << std::endl;

            std::cout
                << ">> SAVE ..." << std::endl
                << "  -- zapisuje HRMS do pliku binarnego o podanej sciezce" << std::endl
                << std::endl;

            std::cout
                << ">> LOAD ..." << std::endl
                << "  -- tworzy nowy HRMS z pliku binarnego o podanej sciezce" << std::endl
                << std::endl;

            std::cout
                << "Aby wpisac wieloczlonowa nazwe (np. departamentu lub sciezki do pliku)," << std::endl
                << "tj. nazwe ze spacjami, nalezy ujac taka nazwe w cudzyslow!" << std::endl
                << std::endl;

            Lab06::printDashedLine();
            return 1;
        }
        else if (Lab06::compareStringsLowercase("EXAMPLES", *key_it))
        {
            std::cout
                << "PRZYKLADOWE POLECENIA:" << std::endl
                << std::endl;

            std::cout
                << " * Dodanie nowego pracownika do bazy:" << std::endl
                << ">> ADD EMPLOYEE imie nazwisko 0 0 pensja" << std::endl
                << std::endl;

            std::cout
                << " * Wyswietlenie pracownikow z departamentu" << std::endl
                << "  o identyfikatorze X, sortowanie po nazwiskach:" << std::endl
                << ">> PRINT EMPLOYEE WITH DEPARTMENT x SORT SURNAME" << std::endl
                << std::endl;

            std::cout
                << " * Usuniecie pracownikow, ktorych pensja miesci sie" << std::endl
                << "  w przedziale od 500 PLN do 1500 PLN wlacznie:" << std::endl
                << ">> REMOVE EMPLOYEE WITH SALARY 500:1500" << std::endl
                << std::endl;

            std::cout
                << " * Awans pracownikow z jednego stanowiska na drugie:" << std::endl
                << ">> CHANGE EMPLOYEE WITH POSITION 1 PROP POSITION 2" << std::endl
                << std::endl;

            std::cout
                << " * Wyrzucenie wszystkich departamentow: (pracownicy NIE SA kasowani," << std::endl
                << "  ale przypisywany im jest zerowy identyfikator departamentu!)" << std::endl
                << ">> REMOVE DEPARTMENT WITH ID 0" << std::endl
                << std::endl;

            Lab06::printDashedLine();
            return 1;
        }
        else if (Lab06::compareStringsLowercase("PRINT", *key_it))
        {
            if (getNextKeyword(keywords, key_it))
            {
                type_option = Lab06::findStringInArray(*key_it, OBJECT_TYPE_NAMES);

                switch (type_option)
                {
                    case OBJECT_TYPE_EMPLOYEE:
                    case OBJECT_TYPE_DEPARTMENT:
                    case OBJECT_TYPE_POSITION:
                        break;

                    default:
                        std::cout << "\"PRINT\": Niewlasciwy typ obiektu!" << std::endl;
                        Lab06::printDashedLine();
                        return 1;
                }

               dummy_state = true;

                while (dummy_state && getNextKeyword(keywords, key_it))
                {
                    if (Lab06::compareStringsLowercase(*key_it, "WITH"))
                    {
                        dummy_int[2] = parseTheWithKeyword
                        (
                            "PRINT", keywords, key_it,
                            type_option, find_option,
                            dummy_int, dummy_double, dummy_str[0]
                        );

                        if (0 == dummy_int[2])
                        {
                            return 1;
                        }

                        dummy_state = (1 == dummy_int[2]);
                    }
                    else if (Lab06::compareStringsLowercase(*key_it, "SORT"))
                    {
                        if (sort_option < 0)
                        {
                            dummy_state = false;

                            if (getNextKeyword(keywords, key_it))
                            {
                                sort_option = Lab06::findStringInArray(*key_it, SORT_METHOD_NAMES);

                                if
                                (
                                    (sort_option < 0)
                                    || ((type_option > OBJECT_TYPE_EMPLOYEE) && (sort_option >= SORT_METHOD_SURNAME))
                                )
                                {
                                    std::cout << "\"PRINT\": Niewlasciwa metoda sortowania \"" << *key_it << "\"!" << std::endl;
                                    Lab06::printDashedLine();
                                    return 1;
                                }

                                dummy_state = true;
                            }
                        }
                        else
                        {
                            std::cout << "\"PRINT\": Slowo kluczowe \"SORT\" uzyte wielokrotnie!" << std::endl;
                            Lab06::printDashedLine();
                            return 1;
                        }
                    }
                    else
                    {
                        std::cout << "\"PRINT\": Nierozpoznane opcjonalne polecenie \"" << *key_it << "\"!" << std::endl;
                        Lab06::printDashedLine();
                        return 1;
                    }
                }

                if (dummy_state)
                {
                    runTheFindQuery
                    (
                        hrms, type_option, find_option,
                        dummy_int, dummy_double, dummy_str[0],
                        indices, true
                    );

                    switch (type_option)
                    {
                        case OBJECT_TYPE_EMPLOYEE:
                            switch (sort_option)
                            {
                                case SORT_METHOD_NAME:
                                    hrms.sortEmployeesListByName(indices);
                                    break;

                                case SORT_METHOD_SURNAME:
                                    hrms.sortEmployeesListBySurname(indices);
                                    break;

                                case SORT_METHOD_POSITION:
                                    hrms.sortEmployeesListByPosition(indices);
                                    break;

                                case SORT_METHOD_SALARY:
                                    hrms.sortEmployeesListBySalary(indices);
                                    break;
                            }

                            hrms.printEmployees(indices);
                            break;

                        case OBJECT_TYPE_DEPARTMENT:
                            if (SORT_METHOD_NAME == sort_option)
                            {
                                hrms.sortDepartmentsListByName(indices);
                            }

                            hrms.printDepartments(indices);
                            break;

                        case OBJECT_TYPE_POSITION:
                            if (SORT_METHOD_NAME == sort_option)
                            {
                                hrms.sortPositionsListByName(indices);
                            }

                            hrms.printPositions(indices);
                            break;
                    }

                    std::cout << "(" << indices.size() << " wpisow)"
                        << std::endl << std::endl;

                    Lab06::printDashedLine();
                    return 1;
                }
            }
        }
        else if (Lab06::compareStringsLowercase("ADD", *key_it))
        {
            if (getNextKeyword(keywords, key_it))
            {
                type_option = Lab06::findStringInArray(*key_it, OBJECT_TYPE_NAMES);

                dummy_state = false;

                switch (type_option)
                {
                    case OBJECT_TYPE_EMPLOYEE:
                    {
                        if (getNextKeyword(keywords, key_it))
                        {
                            dummy_str[0] = *key_it;

                            if (getNextKeyword(keywords, key_it))
                            {
                                dummy_str[1] = *key_it;

                                if (getNextKeyword(keywords, key_it))
                                {
                                    dummy_int[0] = std::stoi(*key_it);

                                    if (getNextKeyword(keywords, key_it))
                                    {
                                        dummy_int[1] = std::stoi(*key_it);

                                        if (getNextKeyword(keywords, key_it))
                                        {
                                            dummy_double[0] = std::stod(*key_it);

                                            dummy_state = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;

                    case OBJECT_TYPE_DEPARTMENT:
                    case OBJECT_TYPE_POSITION:
                    {
                        if (getNextKeyword(keywords, key_it))
                        {
                            dummy_str[0] = *key_it;

                            dummy_state = true;
                        }
                    }
                    break;

                    default:
                        std::cout << "\"ADD\": Niewlasciwy typ obiektu!" << std::endl;
                        Lab06::printDashedLine();
                        return 1;
                }

                if (dummy_state)
                {
                    switch (type_option)
                    {
                        case OBJECT_TYPE_EMPLOYEE:
                            dummy_int[2] = hrms.addEmployee
                            (
                                dummy_str[0], dummy_str[1],
                                dummy_int[0], dummy_int[1],
                                dummy_double[0]
                            );
                            break;

                        case OBJECT_TYPE_DEPARTMENT:
                            dummy_int[2] = hrms.addDepartment(dummy_str[0]);
                            break;

                        case OBJECT_TYPE_POSITION:
                            dummy_int[2] = hrms.addPosition(dummy_str[0]);
                            break;
                    }

                    std::cout << "(zwrocone ID: " << dummy_int[2] << ")"
                        << std::endl << std::endl;

                    Lab06::printDashedLine();
                    return 1;
                }
            }
        }
        else if (Lab06::compareStringsLowercase("REMOVE", *key_it))
        {
            if (getNextKeyword(keywords, key_it))
            {
                type_option = Lab06::findStringInArray(*key_it, OBJECT_TYPE_NAMES);

                switch (type_option)
                {
                    case OBJECT_TYPE_EMPLOYEE:
                    case OBJECT_TYPE_DEPARTMENT:
                    case OBJECT_TYPE_POSITION:
                        break;

                    default:
                        std::cout << "\"REMOVE\": Niewlasciwy typ obiektu!" << std::endl;
                        Lab06::printDashedLine();
                        return 1;
                }

                if (getNextKeyword(keywords, key_it))
                {
                    dummy_state = false;

                    if (Lab06::compareStringsLowercase(*key_it, "WITH"))
                    {
                        dummy_int[2] = parseTheWithKeyword
                        (
                            "REMOVE", keywords, key_it,
                            type_option, find_option,
                            dummy_int, dummy_double, dummy_str[0]
                        );

                        if (0 == dummy_int[2])
                        {
                            return 1;
                        }

                        dummy_state = (1 == dummy_int[2]);
                    }
                    else
                    {
                        std::cout << "\"REMOVE\": Oczekiwano na polecenie \"WITH\", otrzymano \"" << *key_it << "\"!" << std::endl;
                        Lab06::printDashedLine();
                        return 1;
                    }

                    if (dummy_state)
                    {
                        runTheFindQuery
                        (
                            hrms, type_option, find_option,
                            dummy_int, dummy_double, dummy_str[0],
                            indices, false
                        );

                        switch (type_option)
                        {
                            case OBJECT_TYPE_EMPLOYEE:
                            {
                                for (idx_it = indices.begin(); indices.end() != idx_it; idx_it++)
                                {
                                    hrms.removeEmployee(*idx_it);
                                }
                            }
                            break;

                            case OBJECT_TYPE_DEPARTMENT:
                            {
                                for (idx_it = indices.begin(); indices.end() != idx_it; idx_it++)
                                {
                                    hrms.removeDepartment(*idx_it);
                                }
                            }
                            break;

                            case OBJECT_TYPE_POSITION:
                            {
                                for (idx_it = indices.begin(); indices.end() != idx_it; idx_it++)
                                {
                                    hrms.removePosition(*idx_it);
                                }
                            }
                            break;
                        }

                        std::cout << "(usunieto " << indices.size() << " obiektow)"
                            << std::endl << std::endl;

                        Lab06::printDashedLine();
                        return 1;
                    }
                }
            }
        }
        else if (Lab06::compareStringsLowercase("CHANGE", *key_it))
        {
            if (getNextKeyword(keywords, key_it))
            {
                type_option = Lab06::findStringInArray(*key_it, OBJECT_TYPE_NAMES);

                switch (type_option)
                {
                    case OBJECT_TYPE_EMPLOYEE:
                    case OBJECT_TYPE_DEPARTMENT:
                    case OBJECT_TYPE_POSITION:
                        break;

                    default:
                        std::cout << "\"CHANGE\": Niewlasciwy typ obiektu!" << std::endl;
                        Lab06::printDashedLine();
                        return 1;
                }

                if (getNextKeyword(keywords, key_it))
                {
                    dummy_state = false;

                    if (Lab06::compareStringsLowercase(*key_it, "WITH"))
                    {
                        dummy_int[2] = parseTheWithKeyword
                        (
                            "CHANGE", keywords, key_it,
                            type_option, find_option,
                            dummy_int, dummy_double, dummy_str[0]
                        );

                        if (0 == dummy_int[2])
                        {
                            return 1;
                        }

                        dummy_state = (1 == dummy_int[2]);
                    }
                    else
                    {
                        std::cout << "\"REMOVE\": Oczekiwano na polecenie \"WITH\", otrzymano \"" << *key_it << "\"!" << std::endl;
                        Lab06::printDashedLine();
                        return 1;
                    }

                    if (dummy_state && getNextKeyword(keywords, key_it))
                    {
                        dummy_state = false;

                        if (Lab06::compareStringsLowercase(*key_it, "PROP"))
                        {
                            if (getNextKeyword(keywords, key_it))
                            {
                                sort_option = Lab06::findStringInArray(*key_it, FIND_METHOD_NAMES);

                                if
                                (
                                    (sort_option <= FIND_METHOD_ID)
                                    || ((type_option > OBJECT_TYPE_EMPLOYEE) && (sort_option >= FIND_METHOD_SURNAME))
                                )
                                {
                                    std::cout << "\"CHANGE\": Niewlasciwa wlasciwosc \"" << *key_it << "\"!" << std::endl;
                                    Lab06::printDashedLine();
                                    return 1;
                                }

                                if (getNextKeyword(keywords, key_it))
                                {
                                    switch (sort_option)
                                    {
                                        case FIND_METHOD_DEPARTMENT:
                                        case FIND_METHOD_POSITION:
                                            dummy_int[2] = std::stoi(*key_it);
                                            break;

                                        case FIND_METHOD_SALARY:
                                            dummy_double[2] = std::stod(*key_it);
                                            break;

                                        default:
                                            dummy_str[1] = *key_it;
                                    }

                                    dummy_state = true;
                                }
                            }
                        }
                        else
                        {
                            std::cout << "\"CHANGE\": Oczekiwano na polecenie \"PROP\", otrzymano \"" << *key_it << "\"!" << std::endl;
                            Lab06::printDashedLine();
                            return 1;
                        }

                        if (dummy_state)
                        {
                            runTheFindQuery
                            (
                                hrms, type_option, find_option,
                                dummy_int, dummy_double, dummy_str[0],
                                indices, false
                            );

                            switch (type_option)
                            {
                                case OBJECT_TYPE_EMPLOYEE:
                                    for (idx_it = indices.begin(); indices.end() != idx_it; idx_it++)
                                    {
                                        switch (sort_option)
                                        {
                                            case FIND_METHOD_NAME:
                                                hrms.changeEmployeeName(*idx_it, dummy_str[1]);
                                                break;

                                            case FIND_METHOD_SURNAME:
                                                hrms.changeEmployeeSurname(*idx_it, dummy_str[1]);
                                                break;

                                            case FIND_METHOD_DEPARTMENT:
                                                hrms.changeEmployeeDepartment(*idx_it, dummy_int[2]);
                                                break;

                                            case FIND_METHOD_POSITION:
                                                hrms.changeEmployeePosition(*idx_it, dummy_int[2]);
                                                break;

                                            case FIND_METHOD_SALARY:
                                                hrms.changeSalary(*idx_it, dummy_double[2]);
                                                break;
                                        }
                                    }
                                    break;

                                case OBJECT_TYPE_DEPARTMENT:
                                    for (idx_it = indices.begin(); indices.end() != idx_it; idx_it++)
                                    {
                                        switch (sort_option)
                                        {
                                            case FIND_METHOD_NAME:
                                                hrms.changeDepartmentName(*idx_it, dummy_str[1]);
                                                break;
                                        }
                                    }
                                    break;

                                case OBJECT_TYPE_POSITION:
                                    for (idx_it = indices.begin(); indices.end() != idx_it; idx_it++)
                                    {
                                        switch (sort_option)
                                        {
                                            case FIND_METHOD_NAME:
                                                hrms.changePositionName(*idx_it, dummy_str[1]);
                                                break;
                                        }
                                    }
                                    break;
                            }

                            std::cout << "(zmodyfikowano " << indices.size() << " obiektow)"
                                << std::endl << std::endl;

                            Lab06::printDashedLine();
                            return 1;
                        }
                    }
                }
            }
        }
        else if (Lab06::compareStringsLowercase("SAVE", *key_it))
        {
            if (getNextKeyword(keywords, key_it))
            {
                hrms.saveToFile(*key_it);

                std::cout << "(HRMS zapisany do pliku \"" << *key_it << "\")"
                    << std::endl << std::endl;
                Lab06::printDashedLine();
                return 1;
            }
        }
        else if (Lab06::compareStringsLowercase("LOAD", *key_it))
        {
            if (getNextKeyword(keywords, key_it))
            {
                hrms.loadFromFile(*key_it);

                std::cout << "(HRMS odczytany z pliku \"" << *key_it << "\")"
                    << std::endl << std::endl;
                Lab06::printDashedLine();
                return 1;
            }
        }
        else
        {
            std::cout << "Nierozpoznane polecenie \"" << *key_it << "\"!" << std::endl;
            Lab06::printDashedLine();
            return 1;
        }

        std::cout << "Za malo slow kluczowych dla tego polecenia!" << std::endl;
        Lab06::printDashedLine();
    }

    /********************************/
    /* Kontynuuj w g³ównej pêtli programu */

    return 1;
}


////////////////////////////////////////////////////////////////
// Procedura wejœcia aplikacji
////////////////////////////////////////////////////////////////
int32_t main()
{
    try
    {
        Lab06::HRMS hrms;
        int32_t x = 1;

        generateExampleData(hrms);

        while (x)
        {
            x = parseCommand(hrms);
        }
    }
    catch (std::exception &err)
    {
        Lab06::printDashedLine();

        std::cout
            << "[FATAL ERROR!]" << std::endl
            << "  " << err.what() << std::endl << std::endl;

        Lab06::printDashedLine();

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

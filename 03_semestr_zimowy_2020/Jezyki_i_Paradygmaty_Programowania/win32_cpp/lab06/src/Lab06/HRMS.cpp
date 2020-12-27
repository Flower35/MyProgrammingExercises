////////////////////////////////////////////////////////////////
// Klasa systemu zarz¹dzania pracownikami.
////////////////////////////////////////////////////////////////

#include <Lab06/HRMS.h>
#include <Lab06/HRMSException.h>


namespace Lab06
{

    ////////////////////////////////////////////////////////////////
    // [HRMS] Domyœlny konstruktor.
    ////////////////////////////////////////////////////////////////
    HRMS::HRMS()
    : _nextEmployeeId(1), _nextDepartmentId(1), _nextPositionId(1)
    {
        /* Zero zarezerwowane dla niepoprawnych */
        /* (np. usuniêtych) identyfikatorów...  */
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Kopiowanie danych z innej instancji systemu.
    ////////////////////////////////////////////////////////////////
    void HRMS::_assignFromOtherHRMS(const HRMS & other)
    {
        _nextEmployeeId   = other._nextEmployeeId;
        _nextDepartmentId = other._nextDepartmentId;
        _nextPositionId   = other._nextPositionId;

        _employees   = other._employees;
        _departments = other._departments;
        _departments = other._departments;

        _departmentsMap = other._departmentsMap;
        _positionsMap   = other._positionsMap;
        _salariesMap    = other._salariesMap;
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Konstruktor kopii.
    ////////////////////////////////////////////////////////////////
    HRMS::HRMS(const HRMS & other)
    {
        _assignFromOtherHRMS(other);
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Operator przypisania.
    ////////////////////////////////////////////////////////////////
    HRMS & HRMS::operator = (const HRMS & other)
    {
        _assignFromOtherHRMS(other);

        return (HRMS &)(*this);
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Destruktor.
    ////////////////////////////////////////////////////////////////
    HRMS::~HRMS()
    {}


    ////////////////////////////////////////////////////////////////
    // [HRMS] Zbieranie idektyfikatorów.
    ////////////////////////////////////////////////////////////////

    uint32_t HRMS::getLastEmployeeId() const
    {
        return _nextEmployeeId - 1;
    }

    uint32_t HRMS::getLastDepartmentId() const
    {
        return _nextDepartmentId - 1;
    }

    uint32_t HRMS::getLastPositionId() const
    {
        return _nextPositionId - 1;
    }

    std::list<uint32_t> HRMS::getAllEmployeeIds() const
    {
        std::list<uint32_t> result;
        std::map<uint32_t, Employee>::const_iterator it;

        for (it = _employees.begin(); _employees.end() != it; it++)
        {
            result.push_back(it->first);
        }

        return result;
    }

    std::list<uint32_t> HRMS::getAllDepartmentIds() const
    {
        std::list<uint32_t> result;
        std::map<uint32_t, IndexedText>::const_iterator it;

        for (it = _departments.begin(); _departments.end() != it; it++)
        {
            result.push_back(it->first);
        }

        return result;
    }

    std::list<uint32_t> HRMS::getAllPositionIds() const
    {
        std::list<uint32_t> result;
        std::map<uint32_t, IndexedText>::const_iterator it;

        for (it = _positions.begin(); _positions.end() != it; it++)
        {
            result.push_back(it->first);
        }

        return result;
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Zapis bazy do pliku binarnego.
    ////////////////////////////////////////////////////////////////
    void HRMS::saveToFile(const std::string & file_path) const
    {
        uint32_t x, y;
        double salary;
        std::map<uint32_t, Employee>::const_iterator it_a;
        std::map<uint32_t, IndexedText>::const_iterator it_b;

        std::map<uint32_t, uint32_t> continuous_departments;
        std::map<uint32_t, uint32_t> continuous_positions;
        std::map<uint32_t, uint32_t>::iterator it_c;

        std::ofstream file(file_path, std::ios::trunc | std::ios::binary);

        /********************************/

        if (!file.is_open())
        {
            throw HRMSException
            (
                "Nie mozna otworzyc pliku \"%s\"!",
                file_path.c_str()
            );
        }

        file.write((const char *)"HRMS", 0x04);

        /********************************/

        x = _departments.size();
        file.write((const char *)&x, sizeof(uint32_t));

        y = 1;
        for (it_b = _departments.begin(); _departments.end() != it_b; it_b++)
        {
            x = it_b->second._text.length();
            file.write((const char *)&x, sizeof(uint32_t));

            file.write(it_b->second._text.c_str(), x);

            if (false == continuous_departments.insert({it_b->first, y}).second)
            {
                throw HRMSException
                (
                    "Blad w trakcie zapisu bazy: " \
                    "nie udalo sie zmapowac identyfikatorow departamentow!"
                );
            }

            y++;
        }

        /********************************/

        x = _positions.size();
        file.write((const char *)&x, sizeof(uint32_t));

        y = 1;
        for (it_b = _positions.begin(); _positions.end() != it_b; it_b++)
        {
            x = it_b->second._text.length();
            file.write((const char *)&x, sizeof(uint32_t));

            file.write(it_b->second._text.c_str(), x);

            if (false == continuous_positions.insert({it_b->first, y}).second)
            {
                throw HRMSException
                (
                    "Blad w trakcie zapisu bazy: " \
                    "nie udalo sie zmapowac identyfikatorow stanowisk!"
                );
            }

            y++;
        }

        /********************************/

        x = _employees.size();
        file.write((const char *)&x, sizeof(uint32_t));

        for (it_a = _employees.begin(); _employees.end() != it_a; it_a++)
        {
            x = it_a->second._name.length();
            file.write((const char *)&x, sizeof(uint32_t));

            file.write(it_a->second._name.c_str(), x);

            x = it_a->second._surname.length();
            file.write((const char *)&x, sizeof(uint32_t));

            file.write(it_a->second._surname.c_str(), x);

            if (0 != (x = it_a->second._departmentId))
            {
                if (continuous_departments.end() == (it_c = continuous_departments.find(x)))
                {
                    throw HRMSException
                    (
                        "Blad w trakcie zapisu bazy: " \
                        "nie znaleziono departamentu o ID=%u! (pracownik ID=%u)",
                        x, it_a->first
                    );
                }

                x = it_c->second;
            }
            file.write((const char *)&x, sizeof(uint32_t));

            if (0 != (x = it_a->second._positionId))
            {
                if (continuous_positions.end() == (it_c = continuous_positions.find(x)))
                {
                    throw HRMSException
                    (
                        "Blad w trakcie zapisu bazy: " \
                        "nie znaleziono stanowiska o ID=%u! (pracownik ID=%u)",
                        x, it_a->first
                    );
                }

                x = it_c->second;
            }
            file.write((const char *)&x, sizeof(uint32_t));

            salary = getSalary(it_a->first);
            file.write((const char *)&salary, sizeof(double));
        }

        /********************************/

        file.close();
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Odczyt bazy z pliku binarnego.
    ////////////////////////////////////////////////////////////////
    void HRMS::loadFromFile(const std::string & file_path)
    {
        uint32_t a, b, c, d;
        double salary;
        char text[LARGE_BUFFER_SIZE];
        std::string dummy_str[2];

        std::map<uint32_t, uint32_t> continuous_departments;
        std::map<uint32_t, uint32_t> continuous_positions;
        std::map<uint32_t, uint32_t>::iterator it_c;

        std::ifstream file(file_path, std::ios::binary);

        /********************************/

        if (!file.is_open())
        {
            throw HRMSException
            (
                "Nie mozna otworzyc pliku \"%s\"!",
                file_path.c_str()
            );
        }

        file.read((char *)&a, sizeof(uint32_t));

        if (*(uint32_t*)"HRMS" != a)
        {
            throw HRMSException
            (
                "Blad w trakcie odczytu bazy: " \
                "oczekiwano naglowka \"HRMS\"!"
            );
        }

        _nextEmployeeId = 1;
        _nextDepartmentId = 1;
        _nextPositionId = 1;

        _employees.clear();
        _departments.clear();
        _positions.clear();

        _departmentsMap.clear();
        _positionsMap.clear();
        _salariesMap.clear();

        /********************************/

        file.read((char *)&b, sizeof(uint32_t));

        for (a = 1; a <= b; a++)
        {
            file.read((char *)&c, sizeof(uint32_t));

            if (c >= LARGE_BUFFER_SIZE)
            {
                throw HRMSException
                (
                    "Blad w trakcie odczytu bazy: " \
                    "nazwa departamentu %u/%u nie zmiesci sie na stosie!",
                    a, b
                );
            }

            file.read(text, c);
            text[c] = '\0';

            c = addDepartment(text);

            if (false == continuous_departments.insert({a, c}).second)
            {
                throw HRMSException
                (
                    "Blad w trakcie odczytu bazy: " \
                    "nie udalo sie zmapowac identyfikatorow departamentow!"
                );
            }
        }

        /********************************/

        file.read((char *)&b, sizeof(uint32_t));

        for (a = 1; a <= b; a++)
        {
            file.read((char *)&c, sizeof(uint32_t));

            if (c >= LARGE_BUFFER_SIZE)
            {
                throw HRMSException
                (
                    "Blad w trakcie odczytu bazy: " \
                    "nazwa stanowiska %u/%u nie zmiesci sie na stosie!",
                    a, b
                );
            }

            file.read(text, c);
            text[c] = '\0';

            c = addPosition(text);

            if (false == continuous_positions.insert({a, c}).second)
            {
                throw HRMSException
                (
                    "Blad w trakcie odczytu bazy: " \
                    "nie udalo sie zmapowac identyfikatorow stanowisk!"
                );
            }
        }

        /********************************/

        file.read((char *)&b, sizeof(uint32_t));

        for (a = 1; a <= b; a++)
        {
            file.read((char *)&c, sizeof(uint32_t));

            if (c >= LARGE_BUFFER_SIZE)
            {
                throw HRMSException
                (
                    "Blad w trakcie odczytu bazy: " \
                    "imie pracownika %u/%u nie zmiesci sie na stosie!",
                    a, b
                );
            }

            file.read(text, c);
            text[c] = '\0';
            dummy_str[0] = text;

            file.read((char *)&c, sizeof(uint32_t));

            if (c >= LARGE_BUFFER_SIZE)
            {
                throw HRMSException
                (
                    "Blad w trakcie odczytu bazy: " \
                    "nazwisko pracownika %u/%u nie zmiesci sie na stosie!",
                    a, b
                );
            }

            file.read(text, c);
            text[c] = '\0';
            dummy_str[1] = text;

            file.read((char *)&c, sizeof(uint32_t));
            if (0 != c)
            {
                if (continuous_departments.end() == (it_c = continuous_departments.find(c)))
                {
                    throw HRMSException
                    (
                        "Blad w trakcie odczytu bazy: " \
                        "nie znaleziono departamentu o ID=%u! (pracownik %u/%u)",
                        c, a, b
                    );
                }

                c = it_c->second;
            }

            file.read((char *)&d, sizeof(uint32_t));
            if (0 != d)
            {
                if (continuous_positions.end() == (it_c = continuous_positions.find(d)))
                {
                    throw HRMSException
                    (
                        "Blad w trakcie odczytu bazy: " \
                        "nie znaleziono stanowiska o ID=%u! (pracownik %u/%u)",
                        d, a, b
                    );
                }

                d = it_c->second;
            }

            file.read((char *)&salary, sizeof(double));

            addEmployee(dummy_str[0], dummy_str[1], c, d, salary);
        }

        /********************************/

        file.close();
    }

}

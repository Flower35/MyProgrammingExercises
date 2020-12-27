////////////////////////////////////////////////////////////////
// [HRMS] Departments, Positions, Salaries.
////////////////////////////////////////////////////////////////

#include <Lab06/HRMS.h>
#include <Lab06/HRMSException.h>


namespace Lab06
{

    ////////////////////////////////////////////////////////////////
    // [HRMS] Dodanie nowego departamentu o wybranej nazwie.
    ////////////////////////////////////////////////////////////////
    // Funkcja sprawdza, czy dany departament ju¿ istnieje.
    ////////////////////////////////////////////////////////////////
    // Zwracany jest identyfikator takiego departamentu.
    ////////////////////////////////////////////////////////////////
    uint32_t HRMS::addDepartment(const std::string & name)
    {
        std::map<uint32_t, IndexedText>::iterator it;

        /********************************/

        for (it = _departments.begin(); _departments.end() != it; it++)
        {
            if (it->second.matchesText(name))
            {
                return it->second.getIndex();
            }
        }

        /********************************/

        if (0 == _nextDepartmentId)
        {
            throw HRMSException
            (
                "Nie mozna dodac nowego departamentu: " \
                "Wyczerpano limit identyfikatorow!"
            );
        }

        /********************************/

        if (!(_departmentsMap.insert({_nextDepartmentId, {}}).second))
        {
            throw HRMSException
            (
                "Nie udalo sie wstawic pustego zbioru pracownikow dla departamentu \"%s\"!",
                name.c_str()
            );
        }

        /********************************/

        IndexedText dummy;
        dummy._hrms = this;
        dummy._id = _nextDepartmentId;
        dummy._text = name;

        if (!(_departments.insert({_nextDepartmentId, dummy}).second))
        {
            throw HRMSException
            (
                "Nie udalo sie wstawic departamentu \"%s\" do systemu!",
                name.c_str()
            );
        }

        /********************************/

        return _nextDepartmentId ++;
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Zwrócenie referencji do obiektu departamentu
    // przy u¿yciu znanego, unikatowego identyfikatora.
    ////////////////////////////////////////////////////////////////
    IndexedText & HRMS::getDepartment(uint32_t department_id) const
    {
        std::map<uint32_t, IndexedText>::const_iterator it;

        if
        (
            (0 == department_id)
            || (_departments.end() == (it = _departments.find(department_id)))
        )
        {
            throw HRMSException
            (
                "Nie znaleziono departamentu dla ID=%u!",
                department_id
            );
        }

        return (IndexedText &)(it->second);
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Usuniêcie departamentu przy u¿yciu
    // znanego, unikatowego identyfikatora.
    ////////////////////////////////////////////////////////////////
    // Modyfikowane s¹ równie¿ relacje miêdzy pracownikami
    // a departamentami o wybranym identyfikatorze.
    ////////////////////////////////////////////////////////////////
    void HRMS::removeDepartment(uint32_t department_id)
    {
        IndexedText & dummy = getDepartment(department_id);

        std::map<uint32_t, std::set<uint32_t>>::iterator it_map;
        std::set<uint32_t>::iterator it_set;
        std::map<uint32_t, Employee>::iterator it;

        /********************************/

        dummy._hrms = nullptr;
        dummy._id = 0;

        if (1 != _departments.erase(department_id))
        {
            throw HRMSException
            (
                "Nie udalo sie wykasowac departamentu o ID=%u!",
                department_id
            );
        }

        /********************************/

        if (_departmentsMap.end() == (it_map = _departmentsMap.find(department_id)))
        {
            throw HRMSException
            (
                "Nie znaleziono zbioru z identyfikatorami pracownikow dla departamentu ID=%u!",
                department_id
            );
        }

        for (it_set = it_map->second.begin(); it_map->second.end() != it_set; it_set++)
        {
            if (_employees.end() == (it = _employees.find(*it_set)))
            {
                throw HRMSException
                (
                    "Nie udalo sie wyrzucic pracownika ID=%u ze zbioru departamentu ID=%u!",
                    (*it_set),
                    department_id
                );
            }

            it->second._departmentId = 0;
        }

        /********************************/

        if (1 != _departmentsMap.erase(department_id))
        {
            throw HRMSException
            (
                "Nie udalo sie wykasowac zbioru z identyfikatorami pracownikow dla departamentu ID=%u!",
                department_id
            );
        }
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Zmiana nazwy wybranego departamentu.
    ////////////////////////////////////////////////////////////////
    void HRMS::changeDepartmentName(uint32_t department_id, const std::string & name)
    {
        IndexedText & dummy = getDepartment(department_id);

        dummy._text = name;
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Wybieranie departamentów, których identyfikatory
    // mieszcz¹ siê w wybranym przedziale.
    // Argument Zero ignoruje dan¹ granicê przedzia³u.
    ////////////////////////////////////////////////////////////////
    // Funkcja zwraca listê identyfikatorów, która mo¿e byæ pusta.
    ////////////////////////////////////////////////////////////////
    std::list<uint32_t> HRMS::findDepartmentsById(uint32_t min_id, uint32_t max_id) const
    {
        std::list<uint32_t> result;
        std::map<uint32_t, IndexedText>::const_iterator it;

        for (it = _departments.begin(); _departments.end() != it; it++)
        {
            if ((0 != min_id) && (it->first < min_id))
            {
                /* Nie klasyfikuje siê! */
            }
            else if ((0 != max_id) && (it->first > max_id))
            {
                /* Nie klasyfikuje siê! */
            }
            else
            {
                result.push_back(it->first);
            }
        }

        return result;
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Wyszukiwanie departamentów po czêœciowych nazwach.
    ////////////////////////////////////////////////////////////////
    // Funkcja zwraca listê identyfikatorów, która mo¿e byæ pusta.
    ////////////////////////////////////////////////////////////////
    std::list<uint32_t> HRMS::findDepartmentsContainingName(const std::string & name) const
    {
        std::list<uint32_t> result;
        std::map<uint32_t, IndexedText>::const_iterator it;

        for (it = _departments.begin(); _departments.end() != it; it++)
        {
            if (it->second.containsText(name))
            {
                result.push_back(it->first);
            }
        }

        return result;
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Dodanie nowego stanowiska o wybranej nazwie.
    ////////////////////////////////////////////////////////////////
    // Funkcja sprawdza, czy dane stanowisko ju¿ istnieje.
    ////////////////////////////////////////////////////////////////
    // Zwracany jest identyfikator takiego stanowiska.
    ////////////////////////////////////////////////////////////////
    uint32_t HRMS::addPosition(const std::string & name)
    {
        std::map<uint32_t, IndexedText>::iterator it;

        /********************************/

        for (it = _positions.begin(); _positions.end() != it; it++)
        {
            if (it->second.matchesText(name))
            {
                return it->second.getIndex();
            }
        }

        /********************************/

        if (0 == _nextPositionId)
        {
            throw HRMSException
            (
                "Nie mozna dodac nowego stanowiska: " \
                "Wyczerpano limit identyfikatorow!"
            );
        }

        /********************************/

        if (!(_positionsMap.insert({_nextPositionId, {}}).second))
        {
            throw HRMSException
            (
                "Nie udalo sie wstawic pustego zbioru pracownikow dla departamentu \"%s\"!",
                name.c_str()
            );
        }

        /********************************/

        IndexedText dummy;
        dummy._hrms = this;
        dummy._id = _nextPositionId;
        dummy._text = name;

        if (!(_positions.insert({_nextPositionId, dummy}).second))
        {
            throw HRMSException
            (
                "Nie udalo sie wstawic stanowiska \"%s\" do systemu!",
                name.c_str()
            );
        }

        /********************************/

        return _nextPositionId ++;
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Zwrócenie referencji do obiektu stanowiska
    // przy u¿yciu znanego, unikatowego identyfikatora.
    ////////////////////////////////////////////////////////////////
    IndexedText & HRMS::getPosition(uint32_t position_id) const
    {
        std::map<uint32_t, IndexedText>::const_iterator it;

        if(
            (0 == position_id)
            || (_positions.end() == (it = _positions.find(position_id)))
        )
        {
            throw HRMSException
            (
                "Nie znaleziono stanowiska dla ID=%u!",
                position_id
            );
        }

        return (IndexedText &)(it->second);
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Usuniêcie stanowiska przy u¿yciu
    // znanego, unikatowego identyfikatora.
    ////////////////////////////////////////////////////////////////
    // Modyfikowane s¹ równie¿ relacje miêdzy pracownikami
    // a stanowiskami o wybranym identyfikatorze.
    ////////////////////////////////////////////////////////////////
    void HRMS::removePosition(uint32_t position_id)
    {
        IndexedText & dummy = getPosition(position_id);

        std::map<uint32_t, std::set<uint32_t>>::iterator it_map;
        std::set<uint32_t>::iterator it_set;
        std::map<uint32_t, Employee>::iterator it;

        /********************************/

        dummy._hrms = nullptr;
        dummy._id = 0;

        if (1 != _positions.erase(position_id))
        {
            throw HRMSException
            (
                "Nie udalo sie wykasowac stanowiska o ID=%u!",
                position_id
            );
        }

        /********************************/

        if (_positionsMap.end() == (it_map = _positionsMap.find(position_id)))
        {
            throw HRMSException
            (
                "Nie znaleziono kontenera z identyfikatorami pracownikow dla stanowiska ID=%u!",
                position_id
            );
        }

        for (it_set = it_map->second.begin(); it_map->second.end() != it_set; it_set++)
        {
            if (_employees.end() == (it = _employees.find(*it_set)))
            {
                throw HRMSException
                (
                    "Nie udalo sie wyrzucic pracownika ID=%u ze zbioru stanowiska ID=%u!",
                    (*it_set),
                    position_id
                );
            }

            it->second._positionId = 0;
        }

        /********************************/

        if (1 != _positionsMap.erase(position_id))
        {
            throw HRMSException
            (
                "Nie udalo sie wykasowac zbioru z identyfikatorami pracownikow dla stanowiska ID=%u!",
                position_id
            );
        }
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Zmiana nazwy wybranego stanowiska.
    ////////////////////////////////////////////////////////////////
    void HRMS::changePositionName(uint32_t position_id, const std::string & name)
    {
        IndexedText & dummy = getPosition(position_id);

        dummy._text = name;
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Wybieranie stanowisk, których identyfikatory
    // mieszcz¹ siê w wybranym przedziale.
    // Argument Zero ignoruje dan¹ granicê przedzia³u.
    ////////////////////////////////////////////////////////////////
    // Funkcja zwraca listê identyfikatorów, która mo¿e byæ pusta.
    ////////////////////////////////////////////////////////////////
    std::list<uint32_t> HRMS::findPositionsById(uint32_t min_id, uint32_t max_id) const
    {
        std::list<uint32_t> result;
        std::map<uint32_t, IndexedText>::const_iterator it;

        for (it = _positions.begin(); _positions.end() != it; it++)
        {
            if ((0 != min_id) && (it->first < min_id))
            {
                /* Nie klasyfikuje siê! */
            }
            else if ((0 != max_id) && (it->first > max_id))
            {
                /* Nie klasyfikuje siê! */
            }
            else
            {
                result.push_back(it->first);
            }
        }

        return result;
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Wyszukiwanie stanowisk po czêœciowych nazwach.
    ////////////////////////////////////////////////////////////////
    // Funkcja zwraca listê identyfikatorów, która mo¿e byæ pusta.
    ////////////////////////////////////////////////////////////////
    std::list<uint32_t> HRMS::findPositionsContainingName(const std::string & name) const
    {
        std::list<uint32_t> result;
        std::map<uint32_t, IndexedText>::const_iterator it;

        for (it = _positions.begin(); _positions.end() != it; it++)
        {
            if (it->second.containsText(name))
            {
                result.push_back(it->first);
            }
        }

        return result;
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Pobranie wynagrodzenia danego pracownika.
    ////////////////////////////////////////////////////////////////
    double HRMS::getSalary(uint32_t employee_id) const
    {
        std::map<uint32_t, double>::const_iterator it;

        if (_salariesMap.end() == (it = _salariesMap.find(employee_id)))
        {
            throw HRMSException
            (
                "Nie znaleziono pensji dla pracownika o ID=%u!",
                employee_id
            );
        }

        return it->second;
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Zmiana wynagrodzenia danego pracownika.
    ////////////////////////////////////////////////////////////////
    void HRMS::changeSalary(uint32_t employee_id, double salary)
    {
        std::map<uint32_t, double>::iterator it;

        if (salary <= 0)
        {
            throw HRMSException
            (
                "Pensja pracownika musi byc dodatnia! (%lf)",
                salary
            );
        }

        if (_salariesMap.end() == (it = _salariesMap.find(employee_id)))
        {
            throw HRMSException
            (
                "Nie znaleziono pensji dla pracownika o ID=%u!",
                employee_id
            );
        }

        it->second = salary;
    }

}

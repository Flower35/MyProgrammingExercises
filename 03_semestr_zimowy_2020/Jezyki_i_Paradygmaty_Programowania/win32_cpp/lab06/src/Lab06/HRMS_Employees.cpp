////////////////////////////////////////////////////////////////
// [HRMS] Employees.
////////////////////////////////////////////////////////////////

#include <Lab06/HRMS.h>
#include <Lab06/HRMSException.h>


namespace Lab06
{

    ////////////////////////////////////////////////////////////////
    // [HRMS] Dodanie nowego pracownika z wybranymi parametrami.
    ////////////////////////////////////////////////////////////////
    // Musz¹ byæ wczeœniej stworzone odpowiednie departamenty
    // i stanowiska! (chyba ¿e identyfikatory wynosz¹ zero)
    ////////////////////////////////////////////////////////////////
    // Zwracany jest identyfikator dodanego pracownika.
    ////////////////////////////////////////////////////////////////
    uint32_t HRMS::addEmployee
    (
        const std::string & name,
        const std::string & surname,
        uint32_t department_id,
        uint32_t position_id,
        double salary
    )
    {
        std::map<uint32_t, std::set<uint32_t>>::iterator it;

        /********************************/

        if (0 == _nextEmployeeId)
        {
            throw HRMSException
            (
                "Nie mozna dodac nowego pracownika: " \
                "Wyczerpano limit identyfikatorow!"
            );
        }

        /********************************/

        if (0 != department_id)
        {
            if (_departmentsMap.end() == (it = _departmentsMap.find(department_id)))
            {
                throw HRMSException
                (
                    "Nie mozna dodac pracownika: "\
                    "departament o ID=%u nie istnieje!",
                    department_id
                );
            }

            it->second.insert(_nextEmployeeId);
        }

        /********************************/

        if (0 != position_id)
        {
            if (_positionsMap.end() == (it = _positionsMap.find(position_id)))
            {
                throw HRMSException
                (
                    "Nie mozna dodac pracownika: "\
                    "stanowisko o ID=%u nie istnieje!",
                    position_id
                );
            }

            it->second.insert(_nextEmployeeId);
        }

        /********************************/

        if (salary <= 0)
        {
            throw HRMSException
            (
                "Nie mozna dodac pracownika: " \
                "Pensja pracownika musi byc dodatnia! (%lf)",
                salary
            );
        }

        if (!(_salariesMap.insert({_nextEmployeeId, salary}).second))
        {
            throw HRMSException
            (
                "Nie udalo sie wstawic pensji pracownika do systemu!"
            );
        }

        /********************************/

        Employee dummy;
        dummy._hrms = this;
        dummy._id = _nextEmployeeId;
        dummy._name = name;
        dummy._surname = surname;
        dummy._departmentId = department_id;
        dummy._positionId = position_id;

        if (!(_employees.insert({_nextEmployeeId, dummy}).second))
        {
            throw HRMSException
            (
                "Nie udalo sie wstawic pracownika \"%s %s\" do systemu!",
                name.c_str(),
                surname.c_str()
            );
        }

        /********************************/

        return _nextEmployeeId ++;
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Zwrócenie referencji do obiektu pracownika
    // przy u¿yciu znanego, unikatowego identyfikatora.
    ////////////////////////////////////////////////////////////////
    Employee & HRMS::getEmployee(uint32_t employee_id) const
    {
        std::map<uint32_t, Employee>::const_iterator it;

        if
        (
            (0 == employee_id)
            || (_employees.end() == (it = _employees.find(employee_id)))
        )
        {
            throw HRMSException
            (
                "Nie znaleziono pracownika dla ID=%u!",
                employee_id
            );
        }

        return (Employee &)(it->second);
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Usuniêcie pracownika przy u¿yciu
    // znanego, unikatowego identyfikatora.
    ////////////////////////////////////////////////////////////////
    void HRMS::removeEmployee(uint32_t employee_id)
    {
        Employee & dummy = getEmployee(employee_id);

        std::map<uint32_t, std::set<uint32_t>>::iterator it_maps;

        uint32_t department_id;
        uint32_t position_id;

        /********************************/

        department_id = dummy._departmentId;
        position_id = dummy._positionId;

        dummy._hrms = nullptr;
        dummy._id = 0;

        if (1 != _employees.erase(employee_id))
        {
            throw HRMSException
            (
                "Nie udalo sie wykasowac pracownika o ID=%u!",
                employee_id
            );
        }

        /********************************/

        if (0 != department_id)
        {
            if (_departmentsMap.end() != (it_maps = _departmentsMap.find(department_id)))
            {
                it_maps->second.erase(employee_id);
            }
        }

        if (0 != position_id)
        {
            if (_positionsMap.end() != (it_maps = _positionsMap.find(position_id)))
            {
                it_maps->second.erase(employee_id);
            }
        }

        /********************************/

        _salariesMap.erase(employee_id);
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Zmiana imienia wybranego pracownika.
    ////////////////////////////////////////////////////////////////
    void HRMS::changeEmployeeName(uint32_t employee_id, const std::string & name)
    {
        Employee & dummy = getEmployee(employee_id);

        dummy._name = name;
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Zmiana nazwiska wybranego pracownika.
    ////////////////////////////////////////////////////////////////
    void HRMS::changeEmployeeSurname(uint32_t employee_id, const std::string & surname)
    {
        Employee & dummy = getEmployee(employee_id);

        dummy._surname = surname;
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Zmiana departamentu wybranego pracownika.
    ////////////////////////////////////////////////////////////////
    void HRMS::changeEmployeeDepartment(uint32_t employee_id, uint32_t department_id)
    {
        Employee & dummy = getEmployee(employee_id);

        std::map<uint32_t, std::set<uint32_t>>::iterator it;

        /********************************/

        if (0 != department_id)
        {
            if (_departments.end() == _departments.find(department_id))
            {
                throw HRMSException
                (
                    "Nie mozna zmienic departamentu pracownika ID=%u: "\
                    "Nie znaleziono departamentu dla ID=%u!",
                    employee_id,
                    department_id
                );
            }
        }

        /********************************/

        if (0 != dummy._departmentId)
        {
            if (_departmentsMap.end() != (it = _departmentsMap.find(dummy._departmentId)))
            {
                it->second.erase(employee_id);
            }
        }

        /********************************/

        dummy._departmentId = department_id;
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Zmiana stanowiska wybranego pracownika.
    ////////////////////////////////////////////////////////////////
    void HRMS::changeEmployeePosition(uint32_t employee_id, uint32_t position_id)
    {
        Employee & dummy = getEmployee(employee_id);

        std::map<uint32_t, std::set<uint32_t>>::iterator it;

        /********************************/

        if (0 != position_id)
        {
            if (_positions.end() == _positions.find(position_id))
            {
                throw HRMSException
                (
                    "Nie mozna zmienic stanowiska pracownika ID=%u: "\
                    "Nie znaleziono stanowiska dla ID=%u!",
                    employee_id,
                    position_id
                );
            }
        }

        /********************************/

        if (0 != dummy._positionId)
        {
            if (_positionsMap.end() != (it = _positionsMap.find(dummy._positionId)))
            {
                it->second.erase(employee_id);
            }
        }

        /********************************/

        dummy._positionId = position_id;
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Wyszukiwanie pracowników po fragmentach imion.
    ////////////////////////////////////////////////////////////////
    // Funkcja zwraca listê identyfikatorów, która mo¿e byæ pusta.
    ////////////////////////////////////////////////////////////////
    std::list<uint32_t> HRMS::findEmployeesByName(const std::string & name) const
    {
        std::list<uint32_t> result;
        std::map<uint32_t, Employee>::const_iterator it;

        for (it = _employees.begin(); _employees.end() != it; it++)
        {
            if (it->second.containsName(name))
            {
                result.push_back(it->first);
            }
        }

        return result;
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Wyszukiwanie pracowników po fragmentach nazwisk.
    ////////////////////////////////////////////////////////////////
    // Funkcja zwraca listê identyfikatorów, która mo¿e byæ pusta.
    ////////////////////////////////////////////////////////////////
    std::list<uint32_t> HRMS::findEmployeesBySurname(const std::string & surname) const
    {
        std::list<uint32_t> result;
        std::map<uint32_t, Employee>::const_iterator it;

        for (it = _employees.begin(); _employees.end() != it; it++)
        {
            if (it->second.containsSurname(surname))
            {
                result.push_back(it->first);
            }
        }

        return result;
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Wybieranie pracowników, których identyfikatory
    // mieszcz¹ siê w wybranym przedziale.
    // Argument Zero ignoruje dan¹ granicê przedzia³u.
    ////////////////////////////////////////////////////////////////
    // Funkcja zwraca listê identyfikatorów, która mo¿e byæ pusta.
    ////////////////////////////////////////////////////////////////
    std::list<uint32_t> HRMS::findEmployeesById(uint32_t min_id, uint32_t max_id) const
    {
        std::list<uint32_t> result;
        std::map<uint32_t, Employee>::const_iterator it;

        for (it = _employees.begin(); _employees.end() != it; it++)
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
    // [HRMS] Wybieranie pracowników z danego departamentu.
    ////////////////////////////////////////////////////////////////
    // Funkcja zwraca listê identyfikatorów, która mo¿e byæ pusta.
    ////////////////////////////////////////////////////////////////
    std::list<uint32_t> HRMS::findEmployeesByDepartment(uint32_t department_id) const
    {
        std::map<uint32_t, std::set<uint32_t>>::const_iterator it;

        if (_departmentsMap.end() != (it = _departmentsMap.find(department_id)))
        {
            return std::list<uint32_t>(it->second.begin(), it->second.end());
        }

        return std::list<uint32_t>();
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Wybieranie pracowników na danym stanowisku.
    ////////////////////////////////////////////////////////////////
    // Funkcja zwraca listê identyfikatorów, która mo¿e byæ pusta.
    ////////////////////////////////////////////////////////////////
    std::list<uint32_t> HRMS::findEmployeesByPosition(uint32_t position_id) const
    {
        std::map<uint32_t, std::set<uint32_t>>::const_iterator it;

        if (_positionsMap.end() != (it = _positionsMap.find(position_id)))
        {
            return std::list<uint32_t>(it->second.begin(), it->second.end());
        }

        return std::list<uint32_t>();
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Wybieranie pracowników, których pensja
    // mieœci siê w wybranym przedziale.
    // Argument Zero ignoruje dan¹ granicê przedzia³u.
    ////////////////////////////////////////////////////////////////
    // Funkcja zwraca listê identyfikatorów, która mo¿e byæ pusta.
    ////////////////////////////////////////////////////////////////
    std::list<uint32_t> HRMS::findEmployeesBySalary(double min_salary, double max_salary) const
    {
        std::list<uint32_t> result;
        std::map<uint32_t, double>::const_iterator it;

        for (it = _salariesMap.begin(); _salariesMap.end() != it; it++)
        {
            if ((0 != min_salary) && (it->second < min_salary))
            {
                /* Nie klasyfikuje siê! */
            }
            else if ((0 != max_salary) && (it->second > max_salary))
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

}

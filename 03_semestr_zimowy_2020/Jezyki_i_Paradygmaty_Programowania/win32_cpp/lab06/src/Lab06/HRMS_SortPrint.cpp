////////////////////////////////////////////////////////////////
// [HRMS] Sorting, Printing.
////////////////////////////////////////////////////////////////

#include <Lab06/HRMS.h>
#include <Lab06/HRMSException.h>


namespace Lab06
{

    ////////////////////////////////////////////////////////////////
    // [statyczna funkcja pomocnicza]
    // Sortowanie wskaŸników na obiekty pracowników
    // alfabetycznie wed³ug ich imion.
    ////////////////////////////////////////////////////////////////
    static bool sortEmployeePtrsByName(const Employee * & a, const Employee * & b)
    {
        return sortStringsLowercaseAscending(a->getName(), b->getName());
    }


    ////////////////////////////////////////////////////////////////
    // [statyczna funkcja pomocnicza]
    // Sortowanie wskaŸników na obiekty pracowników
    // alfabetycznie wed³ug ich nazwisk.
    ////////////////////////////////////////////////////////////////
    static bool sortEmployeePtrsBySurname(const Employee * & a, const Employee * & b)
    {
        return sortStringsLowercaseAscending(a->getSurname(), b->getSurname());
    }


    ////////////////////////////////////////////////////////////////
    // [statyczna funkcja pomocnicza]
    // Sortowanie wskaŸników na obiekty pracowników
    // alfabetycznie wed³ug ich stanowisk.
    ////////////////////////////////////////////////////////////////
    static bool sortEmployeePtrsByPosition(const Employee * & a, const Employee * & b)
    {
        return sortStringsLowercaseAscending(a->getPositionText(), b->getPositionText());
    }


    ////////////////////////////////////////////////////////////////
    // [statyczna funkcja pomocnicza]
    // Sortowanie wskaŸników na obiekty pracowników
    // malej¹ca wed³ug ich pensji.
    ////////////////////////////////////////////////////////////////
    static bool sortEmployeePtrsBySalary(const Employee * & a, const Employee * & b)
    {
        return a->getSalary() > b->getSalary();
    }


    ////////////////////////////////////////////////////////////////
    // [statyczna funkcja pomocnicza]
    // Sortowanie wskaŸników na obiekty tekstowe alfabetycznie.
    ////////////////////////////////////////////////////////////////
    static bool sortIndexedTextPtrs(const IndexedText * & a, const IndexedText * & b)
    {
        return sortStringsLowercaseAscending(a->getText(), b->getText());
    }


    ////////////////////////////////////////////////////////////////
    // [statyczna funkcja pomocnicza]
    // Sortowanie wskaŸników na obiekty tekstowe alfabetycznie.
    ////////////////////////////////////////////////////////////////
    static void sortIndexedTextsListUsingComparator
    (
        std::list<uint32_t> & idx_list,
        HrmsIndexedTextComparator comparator,
        const std::map<uint32_t, IndexedText> & text_map
    )
    {
        std::list<const IndexedText*> text_ptrs;

        std::list<uint32_t>::iterator it_idx;
        std::map<uint32_t, IndexedText>::const_iterator it;
        std::list<const IndexedText*>::iterator it_sorted;

        /********************************/

        for (it_idx = idx_list.begin(); idx_list.end() != it_idx; it_idx++)
        {
            if (text_map.end() != (it = text_map.find(*it_idx)))
            {
                text_ptrs.push_back(&(it->second));
            }
        }

        /********************************/

        text_ptrs.sort(comparator);

        idx_list.clear();

        for (it_sorted = text_ptrs.begin(); text_ptrs.end() != it_sorted; it_sorted++)
        {
            idx_list.push_back((*it_sorted)->getIndex());
        }
    }


    ////////////////////////////////////////////////////////////////
    // [statyczna funkcja pomocnicza]
    // Wypisz tylko wybrane teksty indeksowane.
    ////////////////////////////////////////////////////////////////
    static void printIndexedTexts
    (
        const std::list<uint32_t> & idx_list,
        const std::map<uint32_t, IndexedText> & text_map
    )
    {
        std::list<uint32_t>::const_iterator it_idx;
        std::map<uint32_t, IndexedText>::const_iterator it;
        uint32_t i = 1;

        for (it_idx = idx_list.begin(); idx_list.end() != it_idx; it_idx++)
        {
            if (text_map.end() != (it = text_map.find(*it_idx)))
            {
                std::cout << "  [" << i << "]";
                it->second.print(true);
                i++;
            }
        }
    }


    ////////////////////////////////////////////////////////////////
    // [statyczna funkcja pomocnicza]
    // Wypisz wszystkie teksty indeksowane.
    ////////////////////////////////////////////////////////////////
    static void printAllIndexedTexts
    (
        const std::map<uint32_t, IndexedText> & text_map
    )
    {
        std::map<uint32_t, IndexedText>::const_iterator it;

        for (it = text_map.begin(); text_map.end() != it; it++)
        {
            it->second.print(true);
        }
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS]Sortowanie listy identyfikatorów tekstowych
    // za pomoc¹ wybranego komparatora.
    ////////////////////////////////////////////////////////////////
    void HRMS::_sortEmployeesListUsingComparator
    (
        std::list<uint32_t> & idx_list,
        HrmsEmployeeComparator comparator
    )
    const
    {
        std::list<const Employee*> employee_ptrs;

        std::list<uint32_t>::iterator it_idx;
        std::map<uint32_t, Employee>::const_iterator it;
        std::list<const Employee*>::iterator it_sorted;

        /********************************/

        for (it_idx = idx_list.begin(); idx_list.end() != it_idx; it_idx++)
        {
            if (_employees.end() != (it = _employees.find(*it_idx)))
            {
                employee_ptrs.push_back(&(it->second));
            }
        }

        /********************************/

        employee_ptrs.sort(comparator);

        idx_list.clear();

        for (it_sorted = employee_ptrs.begin(); employee_ptrs.end() != it_sorted; it_sorted++)
        {
            idx_list.push_back((*it_sorted)->getIndex());
        }
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Sortowanie listy identyfikatorów pracowników
    // alfabetycznie wed³ug ich imion.
    ////////////////////////////////////////////////////////////////
    void HRMS::sortEmployeesListByName(std::list<uint32_t> & idx_list) const
    {
        _sortEmployeesListUsingComparator(idx_list, sortEmployeePtrsByName);
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Sortowanie listy identyfikatorów pracowników
    // alfabetycznie wed³ug ich nazwisk.
    ////////////////////////////////////////////////////////////////
    void HRMS::sortEmployeesListBySurname(std::list<uint32_t> & idx_list) const
    {
        _sortEmployeesListUsingComparator(idx_list, sortEmployeePtrsBySurname);
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Sortowanie listy identyfikatorów pracowników
    // alfabetycznie wed³ug ich stanowisk.
    ////////////////////////////////////////////////////////////////
    void HRMS::sortEmployeesListByPosition(std::list<uint32_t> & idx_list) const
    {
        _sortEmployeesListUsingComparator(idx_list, sortEmployeePtrsByPosition);
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Sortowanie listy identyfikatorów pracowników
    // malej¹ca wed³ug ich pensji.
    ////////////////////////////////////////////////////////////////
    void HRMS::sortEmployeesListBySalary(std::list<uint32_t> & idx_list) const
    {
        _sortEmployeesListUsingComparator(idx_list, sortEmployeePtrsBySalary);
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Sortowanie listy identyfikatorów departamentów
    // alfabetycznie.
    ////////////////////////////////////////////////////////////////
    void HRMS::sortDepartmentsListByName(std::list<uint32_t> & idx_list) const
    {
        sortIndexedTextsListUsingComparator
        (
            idx_list,
            sortIndexedTextPtrs,
            _departments
        );
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Sortowanie listy identyfikatorów stanowisk
    // alfabetycznie.
    ////////////////////////////////////////////////////////////////
    void HRMS::sortPositionsListByName(std::list<uint32_t> & idx_list) const
    {
        sortIndexedTextsListUsingComparator
        (
            idx_list,
            sortIndexedTextPtrs,
            _positions
        );
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Wypisz tylko wybranych pracowników.
    ////////////////////////////////////////////////////////////////
    void HRMS::printEmployees(const std::list<uint32_t> & idx_list) const
    {
        std::list<uint32_t>::const_iterator it_idx;
        std::map<uint32_t, Employee>::const_iterator it;
        uint32_t i = 1;

        for (it_idx = idx_list.begin(); idx_list.end() != it_idx; it_idx++)
        {
            if (_employees.end() != (it = _employees.find(*it_idx)))
            {
                std::cout << "  [" << i << "]";
                it->second.print(true);
                i++;
            }
        }
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Wypisz wszystkich pracowników.
    ////////////////////////////////////////////////////////////////
    void HRMS::printAllEmployees() const
    {
        std::map<uint32_t, Employee>::const_iterator it;

        for (it = _employees.begin(); _employees.end() != it; it++)
        {
            it->second.print(true);
        }
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Wypisz tylko wybrane departamenty.
    ////////////////////////////////////////////////////////////////
    void HRMS::printDepartments(const std::list<uint32_t> & idx_list) const
    {
        printIndexedTexts(idx_list, _departments);
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Wypisz wszystkie departamenty.
    ////////////////////////////////////////////////////////////////
    void HRMS::printAllDepartments() const
    {
        printAllIndexedTexts(_departments);
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Wypisz tylko wybrane stanowiska.
    ////////////////////////////////////////////////////////////////
    void HRMS::printPositions(const std::list<uint32_t> & idx_list) const
    {
        printIndexedTexts(idx_list, _positions);
    }


    ////////////////////////////////////////////////////////////////
    // [HRMS] Wypisz wszystkie stanowiska.
    ////////////////////////////////////////////////////////////////
    void HRMS::printAllPositions() const
    {
        printAllIndexedTexts(_positions);
    }

}

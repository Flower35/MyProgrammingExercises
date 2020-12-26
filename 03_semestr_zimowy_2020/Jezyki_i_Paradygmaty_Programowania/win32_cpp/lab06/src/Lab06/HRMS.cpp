////////////////////////////////////////////////////////////////
// Klasa systemu zarz¹dzania pracownikami.
////////////////////////////////////////////////////////////////

#include <Lab06/HRMS.h>


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

}

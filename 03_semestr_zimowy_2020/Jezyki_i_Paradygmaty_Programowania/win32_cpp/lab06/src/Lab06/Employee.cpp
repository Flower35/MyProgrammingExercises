////////////////////////////////////////////////////////////////
// Klasa pracownika.
////////////////////////////////////////////////////////////////

#include <Lab06/Employee.h>
#include <Lab06/IndexedText.h>

#include <Lab06/HRMS.h>
#include <Lab06/HRMSException.h>


namespace Lab06
{

    ////////////////////////////////////////////////////////////////
    // [Employee] Domyœlny konstruktor.
    ////////////////////////////////////////////////////////////////
    Employee::Employee()
    :
        _hrms(nullptr),
        _id(0),
        _departmentId(0),
        _positionId(0)
    {}


    ////////////////////////////////////////////////////////////////
    // [Employee] Kopiowanie danych z innej instancji.
    ////////////////////////////////////////////////////////////////
    void Employee::_assignFromOtherEmployee(const Employee & other)
    {
        _hrms = other._hrms;
        _id   = other._id;

        _name    = other._name;
        _surname = other._surname;

        _departmentId = other._departmentId;
        _positionId   = other._positionId;
    }


    ////////////////////////////////////////////////////////////////
    // [Employee] Konstruktor kopii.
    ////////////////////////////////////////////////////////////////
    Employee::Employee(const Employee & other)
    {
        _assignFromOtherEmployee(other);
    }


    ////////////////////////////////////////////////////////////////
    // [Employee] Operator przypisania.
    ////////////////////////////////////////////////////////////////
    Employee & Employee::operator = (const Employee & other)
    {
        _assignFromOtherEmployee(other);

        return (Employee &)(*this);
    }


    ////////////////////////////////////////////////////////////////
    // [Employee] Destruktor.
    ////////////////////////////////////////////////////////////////
    Employee::~Employee()
    {}


    ////////////////////////////////////////////////////////////////
    // [Employee] Pobieranie wartoœci wybranego obiektu.
    ////////////////////////////////////////////////////////////////

    void Employee::_validate() const
    {
        if ((nullptr == _hrms) || (0 == _id))
        {
            throw HRMSException
            (
                "Wybrany pracownik nie nalezy do zadnego systemu!"
            );
        }
    }

    uint32_t Employee::getIndex() const
    {
        _validate();
        return _id;
    }

    std::string Employee::getName() const
    {
        _validate();
        return _name;
    }

    std::string Employee::getSurname() const
    {
        _validate();
        return _surname;
    }

    uint32_t Employee::getDepartmentId() const
    {
        _validate();
        return _departmentId;
    }

    std::string Employee::getDepartmentText() const
    {
        _validate();

        if (0 == _departmentId)
        {
            return "";
        }

        return _hrms->getDepartment(_departmentId).getText();
    }

    uint32_t Employee::getPositionId() const
    {
        _validate();
        return _positionId;
    }

    std::string Employee::getPositionText() const
    {
        _validate();

        if (0 == _positionId)
        {
            return "";
        }

        return _hrms->getPosition(_positionId).getText();
    }

    double Employee::getSalary() const
    {
        _validate();
        return _hrms->getSalary(_id);
    }


    ////////////////////////////////////////////////////////////////
    // [Employee] Sprawdzenie, czy dany tekst nie zawiera siê
    // w imieniu b¹dŸ w nazwisku pracownika.
    ////////////////////////////////////////////////////////////////

    bool Employee::containsName(const std::string & str) const
    {
        return stringContainsLowercase(_name, str);
    }

    bool Employee::containsSurname(const std::string & str) const
    {
        return stringContainsLowercase(_surname, str);
    }


    ////////////////////////////////////////////////////////////////
    // [Employee] Wypisanie danych w konsoli.
    ////////////////////////////////////////////////////////////////
    void Employee::print(bool with_index) const
    {
        if (with_index)
        {
            std::cout << "  [ID: " << _id << "]";
        }

        std::cout
            << "  \""
            << _name << " "
            << _surname << "\";"
            << std::endl;

        std::cout
            << "    departament: " << getDepartmentText() << ";"
            << std::endl;

        std::cout
            << "    stanowisko: " << getPositionText() << ";"
            << std::endl;

        std::cout
            << "    pensja: " << getSalary() << " PLN;"
            << std::endl;
    }

}

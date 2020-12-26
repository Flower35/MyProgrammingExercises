#ifndef H_LAB06_EMPLOYEE
#define H_LAB06_EMPLOYEE

#include <Lab06/Lab06.h>

namespace Lab06
{
    ////////////////////////////////////////////////////////////////
    // Deklaracja typu klasy HRMS.
    ////////////////////////////////////////////////////////////////

    class HRMS;


    ////////////////////////////////////////////////////////////////
    // Klasa pracownika.
    ////////////////////////////////////////////////////////////////

    class Employee
    {
        /********************************/
        /*** Pola ***/

        private:

            HRMS * _hrms;
            uint32_t _id;
            std::string _name;
            std::string _surname;
            uint32_t _departmentId;
            uint32_t _positionId;

        /********************************/
        /*** Metody : Instancja pracownika ***/

        public:

            Employee();

        private:

            void _assignFromOtherEmployee(const Employee & other);

        public:

            Employee(const Employee & other);

            Employee & operator = (const Employee & other);

            ~Employee();

        /********************************/
        /*** Metody : Pobieranie wartoœci ***/

        private:

            void _validate() const;

        public:

            uint32_t getIndex() const;
            std::string getName() const;
            std::string getSurname() const;
            uint32_t getDepartmentId() const;
            std::string getDepartmentText() const;
            uint32_t getPositionId() const;
            std::string getPositionText() const;
            double getSalary() const;

        /********************************/
        /**** Metody : Sprawdzanie wartoœci ***/

        public:

            bool containsName(const std::string & str) const;
            bool containsSurname(const std::string & str) const;

        /********************************/
        /*** Metody : Wypisywanie danych ***/

        public:

            void print(bool with_index) const;

        /********************************/
        /*** Wyznaczenie przyjaŸni z systemem ***/
        /*** (tylko system mo¿e bezpoœrednio zmieniaæ wartoœci!) ***/

        friend class HRMS;
    };
}

#endif

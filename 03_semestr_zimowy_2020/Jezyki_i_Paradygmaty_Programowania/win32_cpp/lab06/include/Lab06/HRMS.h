#ifndef H_LAB06_HRMS
#define H_LAB06_HRMS

#include <map>
#include <set>
#include <list>
#include <fstream>

#include <Lab06/Employee.h>
#include <Lab06/IndexedText.h>

namespace Lab06
{
    ////////////////////////////////////////////////////////////////
    // Definicje pomocnicze.
    ////////////////////////////////////////////////////////////////

    typedef bool (*HrmsEmployeeComparator)(const Employee * &, const Employee * &);
    typedef bool (*HrmsIndexedTextComparator)(const IndexedText * &, const IndexedText * &);


    ////////////////////////////////////////////////////////////////
    // Klasa systemu zarz¹dzania pracownikami.
    ////////////////////////////////////////////////////////////////

    class HRMS
    {
        /********************************/
        /*** Pola ***/

        private:

            uint32_t _nextEmployeeId;
            uint32_t _nextDepartmentId;
            uint32_t _nextPositionId;

            std::map<uint32_t, Employee> _employees;
            std::map<uint32_t, IndexedText> _departments;
            std::map<uint32_t, IndexedText> _positions;

            std::map<uint32_t, std::set<uint32_t>> _departmentsMap;
            std::map<uint32_t, std::set<uint32_t>> _positionsMap;
            std::map<uint32_t, double> _salariesMap;

        /********************************/
        /*** Metody : Instancja systemu zarz¹dzania ***/

        public:

            HRMS();

        private:

            void _assignFromOtherHRMS(const HRMS & other);

        public:

            HRMS(const HRMS & other);

            HRMS & operator = (const HRMS & other);

            ~HRMS();

        /********************************/
        /*** Metody : Zbieranie identyfikatorów ***/

        public:

            uint32_t getLastEmployeeId() const;
            uint32_t getLastDepartmentId() const;
            uint32_t getLastPositionId() const;

            std::list<uint32_t> getAllEmployeeIds() const;
            std::list<uint32_t> getAllDepartmentIds() const;
            std::list<uint32_t> getAllPositionIds() const;

        /********************************/
        /*** Metody : Serializacja ***/

        public:

            void saveToFile(const std::string & file_path) const;
            void loadFromFile(const std::string & file_path);

        /********************************/
        /*** Metody : Pracownicy ***/

        public:

            uint32_t addEmployee
            (
                const std::string & name,
                const std::string & surname,
                uint32_t department_id,
                uint32_t position_id,
                double salary
            );

            Employee & getEmployee(uint32_t employee_id) const;

            void removeEmployee(uint32_t employee_id);

            void changeEmployeeName(uint32_t employee_id, const std::string & name);
            void changeEmployeeSurname(uint32_t employee_id, const std::string & surname);
            void changeEmployeeDepartment(uint32_t employee_id, uint32_t department_id);
            void changeEmployeePosition(uint32_t employee_id, uint32_t position_id);

            std::list<uint32_t> findEmployeesByName(const std::string & name) const;
            std::list<uint32_t> findEmployeesBySurname(const std::string & surname) const;

            std::list<uint32_t> findEmployeesById(uint32_t min_id, uint32_t max_id) const;
            std::list<uint32_t> findEmployeesByDepartment(uint32_t department_id) const;
            std::list<uint32_t> findEmployeesByPosition(uint32_t position_id) const;
            std::list<uint32_t> findEmployeesBySalary(double min_salary, double max_salary) const;

        /********************************/
        /*** Metody : Departamenty ***/

        public:

            uint32_t addDepartment(const std::string & name);

            IndexedText & getDepartment(uint32_t department_id) const;

            void removeDepartment(uint32_t department_id);

            void changeDepartmentName(uint32_t department_id, const std::string & name);

            std::list<uint32_t> findDepartmentsById(uint32_t min_id, uint32_t max_id) const;
            std::list<uint32_t> findDepartmentsContainingName(const std::string & name) const;

        /********************************/
        /*** Metody : Stanowiska ***/

        public:

            uint32_t addPosition(const std::string & name);

            IndexedText & getPosition(uint32_t position_id) const;

            void removePosition(uint32_t position_id);

            void changePositionName(uint32_t position_id, const std::string & name);

            std::list<uint32_t> findPositionsById(uint32_t min_id, uint32_t max_id) const;
            std::list<uint32_t> findPositionsContainingName(const std::string & name) const;

        /********************************/
        /*** Metody : Wyp³aty ***/

        public:

            double getSalary(uint32_t employee_id) const;
            void changeSalary(uint32_t employee_id, double salary);

        /********************************/
        /*** Metody : Sortowanie i wypisywanie danych ***/

        private:

            void _sortEmployeesListUsingComparator
            (
                std::list<uint32_t> & idx_list,
                HrmsEmployeeComparator comparator
            )
            const;

        public:

            void sortEmployeesListByName(std::list<uint32_t> & idx_list) const;
            void sortEmployeesListBySurname(std::list<uint32_t> & idx_list) const;
            void sortEmployeesListByPosition(std::list<uint32_t> & idx_list) const;
            void sortEmployeesListBySalary(std::list<uint32_t> & idx_list) const;

            void sortDepartmentsListByName(std::list<uint32_t> & idx_list) const;
            void sortPositionsListByName(std::list<uint32_t> & idx_list) const;

            void printEmployees(const std::list<uint32_t> & idx_list) const;
            void printAllEmployees() const;

            void printDepartments(const std::list<uint32_t> & idx_list) const;
            void printAllDepartments() const;

            void printPositions(const std::list<uint32_t> & idx_list) const;
            void printAllPositions() const;

    };
}

#endif

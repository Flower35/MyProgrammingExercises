#ifndef H_LAB06_HRMS_EXCEPTION
#define H_LAB06_HRMS_EXCEPTION

#include <Lab06/Lab06.h>
#include <exception>

namespace Lab06
{
    ////////////////////////////////////////////////////////////////
    // Klasa wyj¹tku dla systemu zarz¹dzania pracownikami.
    ////////////////////////////////////////////////////////////////

    class HRMSException : public std::exception
    {
        /********************************/
        /*** Pola ***/

        private:

            std::string _message;

        /********************************/
        /*** Metody ***/

        public:

            HRMSException(const char * format, ...);

            const char* what() const noexcept override;

    };
}

#endif

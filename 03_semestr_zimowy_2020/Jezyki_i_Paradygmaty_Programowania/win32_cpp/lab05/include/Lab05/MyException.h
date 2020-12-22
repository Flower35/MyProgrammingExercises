#ifndef H_LAB05_MYEXCEPTION
#define H_LAB05_MYEXCEPTION

#include <Lab05/Lab05.h>

namespace Lab05
{

    ////////////////////////////////////////////////////////////////
    // Klasa wyj¹tku
    ////////////////////////////////////////////////////////////////
    class MyException : public std::exception
    {
        /* Properties */

        private:

            std::string message;

        /* Methods */

        public:

            MyException(const char* format, ...);

            const char* what() const noexcept override;
    };

}

#endif

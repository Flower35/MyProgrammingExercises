#ifndef H_LAB03B_MYEXCEPTION
#define H_LAB03B_MYEXCEPTION

#include <Lab03b/Lab03b.h>

namespace Lab03b
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

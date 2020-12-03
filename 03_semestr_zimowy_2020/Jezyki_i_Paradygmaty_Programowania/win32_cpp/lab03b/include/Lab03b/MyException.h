#ifndef H_LAB03B_MYEXCEPTION
#define H_LAB03B_MYEXCEPTION

#include <Lab03b/Lab03b.h>

namespace Lab03b
{

    ////////////////////////////////////////////////////////////////
    // Klasa wyj¹tku
    ////////////////////////////////////////////////////////////////
    class MyException
    {
        /* Properties */

        private:

            std::string message;

        /* Methods */

        public:

            MyException(const char* format, ...);

            void printMessage() const;
    };

}

#endif

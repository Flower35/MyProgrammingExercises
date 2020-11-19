#ifndef H_LAB04_MYEXCEPTION
#define H_LAB04_MYEXCEPTION

#include <Lab04/Lab04.h>

namespace Lab04
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

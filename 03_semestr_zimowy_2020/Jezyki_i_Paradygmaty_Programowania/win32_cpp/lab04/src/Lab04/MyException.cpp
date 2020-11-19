#include <Lab04/MyException.h>

namespace Lab04
{

    ////////////////////////////////////////////////////////////////
    // [MyException] Konstruktor wyj¹tku.
    ////////////////////////////////////////////////////////////////
    MyException::MyException(const char* format, ...)
    {
        char test[LARGE_BUFFER_SIZE];

        va_list args;
        va_start(args, format);

        vsprintf(test, format, args);
        message = test;

        va_end(args);
    }


    ////////////////////////////////////////////////////////////////
    // [MyException] Wypisywanie wiadomoœci na ekranie konsoli.
    ////////////////////////////////////////////////////////////////
    void MyException::printMessage() const
    {
        std::cout << "[EXCEPTION] " << message << std::endl;
    }

}

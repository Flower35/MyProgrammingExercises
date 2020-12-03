#include <Lab03b/MyException.h>

namespace Lab03b
{

    ////////////////////////////////////////////////////////////////
    // [MyException] Konstruktor wyj¹tku.
    ////////////////////////////////////////////////////////////////
    MyException::MyException(const char* format, ...)
    {
        char test[LARGE_BUFFER_SIZE];

        std::va_list args;
        va_start(args, format);

        std::vsprintf(test, format, args);
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

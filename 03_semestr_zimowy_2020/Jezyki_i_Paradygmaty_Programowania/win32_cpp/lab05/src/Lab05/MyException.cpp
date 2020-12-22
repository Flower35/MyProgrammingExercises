#include <Lab05/MyException.h>

namespace Lab05
{

    ////////////////////////////////////////////////////////////////
    // [MyException] Konstruktor wyj�tku.
    ////////////////////////////////////////////////////////////////
    MyException::MyException(const char* format, ...)
    {
        char test[LARGE_BUFFER_SIZE];
        std::va_list args;

        va_start(args, format);
        std::vsprintf(test, format, args);
        va_end(args);

        message = std::string("[MY EXCEPTION - MOJ WYJATEK] ") + test;
    }


    ////////////////////////////////////////////////////////////////
    // [MyException] Zwr�cenie wiadomo�ci poprzez funkcj� przes�oni�t�.
    ////////////////////////////////////////////////////////////////
    const char* MyException::what() const noexcept
    {
        return message.c_str();
    }

}

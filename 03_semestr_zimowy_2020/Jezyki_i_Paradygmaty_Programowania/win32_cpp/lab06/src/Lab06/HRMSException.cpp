////////////////////////////////////////////////////////////////
// Klasa wyj¹tku dla systemu zarz¹dzania pracownikami.
////////////////////////////////////////////////////////////////

#include <Lab06/HRMSException.h>
#include <cstdarg>


namespace Lab06
{

    ////////////////////////////////////////////////////////////////
    // [HRMSException] Formatowanie tekstu
    // w momencie wyrzucenia wyj¹tku.
    ////////////////////////////////////////////////////////////////
    HRMSException::HRMSException(const char * format, ...)
    {
        char test[LARGE_BUFFER_SIZE];
        std::va_list args;

        va_start(args, format);
        std::vsprintf(test, format, args);
        va_end(args);

        _message = test;
    }


    ////////////////////////////////////////////////////////////////
    // [HRMSException] Wyœwietlenie treœci wyrzuconego wyj¹tku.
    ////////////////////////////////////////////////////////////////
    const char* HRMSException::what() const noexcept
    {
        return _message.c_str();
    }

}

#ifndef H_LAB06
#define H_LAB06


////////////////////////////////////////////////////////////////
// Podstawowe biblioteki
////////////////////////////////////////////////////////////////

#include <cstdint>
#include <cctype>

#include <string>
#include <utility>
#include <list>
#include <iostream>


////////////////////////////////////////////////////////////////
// Laboratorium 06
////////////////////////////////////////////////////////////////

namespace Lab06
{

    ////////////////////////////////////////////////////////////////
    // Definicje
    ////////////////////////////////////////////////////////////////

    static const uint32_t LARGE_BUFFER_SIZE = 512;


    ////////////////////////////////////////////////////////////////
    // Funkcje pomocnicze
    ////////////////////////////////////////////////////////////////

    void printDashedLine();

    bool compareStringsLowercase(const std::string & s1, const std::string & s2);

    int32_t findStringInArray(const std::string & s, const std::list<std::string> & v);

    bool stringContainsLowercase(const std::string & s1, const std::string & s2);

    bool sortStringsLowercaseAscending(const std::string & s1, const std::string & s2);

    std::list<std::string> splitStringsBySpaces(const std::string & source, const char space);

}

#endif

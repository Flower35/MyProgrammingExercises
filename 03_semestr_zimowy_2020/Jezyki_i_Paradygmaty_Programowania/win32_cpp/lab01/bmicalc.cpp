/****************************************************************
  [LAB 01, ZADANIE 03]
    Napisz kalkulator wyliczajacy wskaznik BMI (Body Mass Index).

****************************************************************/

#include <iostream>
#include <string>


////////////////////////////////////////////////////////////////
// Pobieranie wartoœci liczbowej
// (zmiennoprzecinkowej) od u¿ytkownika
////////////////////////////////////////////////////////////////
bool get_value_from_stdin(float &result)
{
    std::string test;

    try
    {
        std::cout << ">> ";

        std::getline(std::cin, test);

        result = std::stof(test);
    }
    catch (std::exception)
    {
        return false;
    }

    return true;
}


////////////////////////////////////////////////////////////////
// Wydrukowanie odstêpu na ekranie
////////////////////////////////////////////////////////////////
void print_dashed_line()
{
    std::cout.width(64);
    std::cout.fill('-');

    std::cout << "" << std::endl;

    std::cout.fill(' ');
}


////////////////////////////////////////////////////////////////
// Okreœlenie tekstowe wskaŸnika BMI
////////////////////////////////////////////////////////////////
const char* get_bmi_description(float bmi)
{
    if (bmi < 16.0f)
    {
        return "WYGLODZENIE";
    }
    else if (bmi < 17.0f)
    {
        return "WYCHUDZENIE";
    }
    else if (bmi < 18.5f)
    {
        return "NIEDOWAGA";
    }
    else if (bmi < 25.0f)
    {
        return "WARTOSC PRAWIDLOWA";
    }
    else if (bmi < 30.0f)
    {
        return "NADWAGA";
    }
    else if (bmi < 35.0f)
    {
        return "I STOPNIEN OTYLOSCI";
    }
    else if (bmi < 40.0f)
    {
        return "II STOPIEN OTYLOSCI";
    }

    return "OTYLOSC SKRAJNA";
}


////////////////////////////////////////////////////////////////
// Odczytanie danych od u¿ytkownika. Sprawdzenie poprawnoœci.
////////////////////////////////////////////////////////////////
bool get_bmi_data(float &height, float &weight)
{
    std::cout << std::endl;
    std::cout << "Podaj wzrost, w metrach (miedzy 0.20 a 3.00)" << std::endl;

    if (!get_value_from_stdin(height))
    {
        return false;
    }

    if ((height < 0.20f) || (height > 3.00f))
    {
        return false;
    }
    std::cout << std::endl;
    std::cout << "Podaj mase, w kilogramach (miedzy 2.00 a 250.00)" << std::endl;

    if (!get_value_from_stdin(weight))
    {
        return false;
    }

    if ((weight < 2.00f) || (weight > 250.00f))
    {
        return false;
    }

    return true;
}


////////////////////////////////////////////////////////////////
// POCZ¥TEK PROGRAMU
////////////////////////////////////////////////////////////////
int32_t main()
{
    float height, weight;

    /********************************/
    /* Powitanie. Proœba o wprowadzenie danych. */

    print_dashed_line();

    std::cout << "Prosty kalulator BMI (Body Mass Index)" << std::endl;

    print_dashed_line();

    if (get_bmi_data(height, weight))
    {
        /********************************/
        /* Wyœwietlenie odczytanych danych. Obliczenie wskaŸnika BMI. */

        std::cout << std::endl;
        print_dashed_line();

        std::cout << "Podsumowanie:" << std::endl
            << " -> twoj wzrost: " << height << " [m]" << std::endl
            << " -> twoja masa: " << weight << " [kg]" << std::endl;

        height = weight / (height * height);

        std::cout << std::endl;
        print_dashed_line();

        std::cout << "Twoje BMI wynosi: " << height << std::endl
            << " -> " << get_bmi_description(height) << std::endl;
    }
    else
    {
        /********************************/
        /* B³¹d w trakcie wprowadzania danych, lub dane */
        /* nie mieszcz¹ siê w dopuszczalnych granicach. */

        std::cout << std::endl;
        print_dashed_line();

        std::cout << "Podano niepoprawne dane! Do widzenia." << std::endl;

        return 1;
    }

    return 0;
}

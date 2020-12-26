////////////////////////////////////////////////////////////////
// Klasa tekstu z unikatowym indeksem.
////////////////////////////////////////////////////////////////

#include <Lab06/IndexedText.h>

#include <Lab06/HRMSException.h>


namespace Lab06
{

    ////////////////////////////////////////////////////////////////
    // [IndexedText] Domyœlny konstruktor.
    ////////////////////////////////////////////////////////////////
    IndexedText::IndexedText()
    :
        _hrms(nullptr),
        _id(0)
    {}


    ////////////////////////////////////////////////////////////////
    // [IndexedText] Kopiowanie danych z innej instancji.
    ////////////////////////////////////////////////////////////////
    void IndexedText::_assignFromOtherText(const IndexedText & other)
    {
        _hrms = other._hrms;
        _id   = other._id;

        _text = other._text;
    }


    ////////////////////////////////////////////////////////////////
    // [IndexedText] Konstruktor kopii.
    ////////////////////////////////////////////////////////////////
    IndexedText::IndexedText(const IndexedText & other)
    {
        _assignFromOtherText(other);
    }


    ////////////////////////////////////////////////////////////////
    // [IndexedText] Operator przypisania.
    ////////////////////////////////////////////////////////////////
    IndexedText & IndexedText::operator = (const IndexedText & other)
    {
        _assignFromOtherText(other);

        return (IndexedText &)(*this);
    }


    ////////////////////////////////////////////////////////////////
    // [IndexedText] Destruktor.
    ////////////////////////////////////////////////////////////////
    IndexedText::~IndexedText()
    {}


    ////////////////////////////////////////////////////////////////
    // [IndexedText] Pobieranie wartoœci wybranego obiektu.
    ////////////////////////////////////////////////////////////////

    void IndexedText::_validate() const
    {
        if ((nullptr == _hrms) || (0 == _id))
        {
            throw HRMSException
            (
                "Wybrany napis nie nalezy do zadnego systemu!"
            );
        }
    }

    uint32_t IndexedText::getIndex() const
    {
        _validate();
        return _id;
    }

    std::string IndexedText::getText() const
    {
        _validate();
        return _text;
    }


    ////////////////////////////////////////////////////////////////
    // [IndexedText] Porównywanie tekstów.
    ////////////////////////////////////////////////////////////////

    bool IndexedText::matchesText(const std::string & str) const
    {
        return compareStringsLowercase(_text, str);
    }

    bool IndexedText::containsText(const std::string & str) const
    {
        return stringContainsLowercase(_text, str);
    }


    ////////////////////////////////////////////////////////////////
    // [IndexedText] Wypisanie danych w konsoli.
    ////////////////////////////////////////////////////////////////
    void IndexedText::print(bool with_index) const
    {
        if (with_index)
        {
            std::cout << "  [ID: " << _id << "]";
        }

        std::cout << "  \"" << _text << "\";" << std::endl;
    }

}

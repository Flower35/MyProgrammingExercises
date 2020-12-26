#ifndef H_LAB06_INDEXED_TEXT
#define H_LAB06_INDEXED_TEXT

#include <Lab06/Lab06.h>

namespace Lab06
{
    ////////////////////////////////////////////////////////////////
    // Deklaracja typu klasy HRMS.
    ////////////////////////////////////////////////////////////////

    class HRMS;


    ////////////////////////////////////////////////////////////////
    // Klasa tekstu z unikatowym indeksem.
    ////////////////////////////////////////////////////////////////

    class IndexedText
    {
        /********************************/
        /*** Pola ***/

        private:

            HRMS * _hrms;
            uint32_t _id;
            std::string _text;

        /********************************/
        /* Metody : Instancja indeksowanego tekstu */

        public:

            IndexedText();

        private:

            void _assignFromOtherText(const IndexedText & other);

        public:

            IndexedText(const IndexedText & other);

            IndexedText & operator = (const IndexedText & other);

            ~IndexedText();

        /********************************/
        /*** Metody : Pobieranie wartoœci ***/

        private:

            void _validate() const;

        public:

            uint32_t getIndex() const;
            std::string getText() const;

        /********************************/
        /**** Metody : Sprawdzanie wartoœci ***/

        public:

            bool matchesText(const std::string & str) const;
            bool containsText(const std::string & str) const;

        /********************************/
        /*** Metody : Wypisywanie danych ***/

        public:

            void print(bool with_index) const;

        /********************************/
        /*** Wyznaczenie przyjaŸni z systemem ***/
        /*** (tylko system mo¿e bezpoœrednio zmieniaæ wartoœci!) ***/

        friend class HRMS;
    };
}

#endif

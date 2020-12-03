#ifndef H_LAB03B_MATRIX
#define H_LAB03B_MATRIX

#include <Lab03b/Lab03b.h>

/* Biblioteka dynamiczna */
#include <sqlite3/sqlite3.h>

namespace Lab03b
{

    ////////////////////////////////////////////////////////////////
    // Dodatkowe definicje.
    ////////////////////////////////////////////////////////////////

    extern const char* DB_TABLE_NAMES[2];


    ////////////////////////////////////////////////////////////////
    // Klasa macierzy
    ////////////////////////////////////////////////////////////////
    class Matrix
    {
        /* Properties */

        private:

            double** elements;
            int rows;
            int columns;

        /* Methods */

        private:

            void create();
            void destroy();
            void copy(const Matrix &m2);
            void validateRowAndColumn(const char* func_name, int n, int m) const;

        public:

            Matrix(int new_rows, int new_columns);
            Matrix(int new_dimension);

            Matrix(const Matrix &m2);
            Matrix &operator = (const Matrix &m2);

            ~Matrix();

            void set(int n, int m, double val);
            double get(int n, int m) const;

            int getCols() const;
            int getRows() const;

            void print() const;

            sqlite3_int64 store(const std::string &fullpath) const;
            Matrix(const std::string &fullpath, sqlite3_int64 index_in_table);
    };

}

#endif

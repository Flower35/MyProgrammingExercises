#ifndef H_LAB04_MATRIX
#define H_LAB04_MATRIX

#include <Lab04/Lab04.h>

namespace Lab04
{

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
            void validateDimensions(const char* func_name, const Matrix &m2, bool multiplication) const;

        public:

            Matrix(int new_rows, int new_columns);
            Matrix(int new_dimension);

            Matrix(const Matrix &m2);
            Matrix &operator = (const Matrix &m2);

            ~Matrix();

            void set(int n, int m, double val);
            double get(int n, int m) const;

            Matrix add(const Matrix &m2) const;
            Matrix subtract(const Matrix &m2) const;
            Matrix multiply(const Matrix &m2) const;

            int getCols() const;
            int getRows() const;

            void print() const;

            void store(const std::string &filename, const std::string &path) const;
            Matrix(const std::string &fullpath);
    };

}

#endif

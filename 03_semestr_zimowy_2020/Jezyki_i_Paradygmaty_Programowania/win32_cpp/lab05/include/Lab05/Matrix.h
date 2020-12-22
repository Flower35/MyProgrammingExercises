#ifndef H_LAB05_MATRIX
#define H_LAB05_MATRIX

#include <Lab05/Lab05.h>

namespace Lab05
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
            void validateRow(const char* func_name, int n) const;
            void validateDimensions(const char* func_name, const Matrix &m2, bool multiplication) const;

        public:

            Matrix(int new_rows, int new_columns);
            Matrix(int new_dimension);

            Matrix(const Matrix &m2);
            Matrix &operator = (const Matrix &m2);

            ~Matrix();

            double* operator [] (int n) const;
            double* &operator [] (int n);

            Matrix operator + (const Matrix &m2) const;
            Matrix operator - (const Matrix &m2) const;
            Matrix operator * (const Matrix &m2) const;
            bool operator == (const Matrix &m2) const;

            Matrix transposed() const;

            int getCols() const;
            int getRows() const;

            void print() const;

            Matrix &operator >> (std::ofstream &file) const;
            Matrix(const std::string &fullpath);
    };

}

#endif

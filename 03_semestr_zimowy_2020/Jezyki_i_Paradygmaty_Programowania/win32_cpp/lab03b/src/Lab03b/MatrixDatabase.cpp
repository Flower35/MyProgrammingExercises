#include <Lab03b/Matrix.h>
#include <Lab03b/MyException.h>

namespace Lab03b
{

    ////////////////////////////////////////////////////////////////
    // Dodatkowe definicje
    ////////////////////////////////////////////////////////////////

    const char* DB_TABLE_NAMES[2] =
    {
        "matrices",
        "mat_rows"
    };

    const int DB_KEYWORDS_COUNT = 2 + DB_MATRIX_MAX_DIMENSION;


    ////////////////////////////////////////////////////////////////
    // [Matrix] Funkcja zapisuje macierz do pliku bazodanowego.
    // Zwracany jest indeks nowego wpisu do tabelki z macierzami.
    ////////////////////////////////////////////////////////////////
    sqlite3_int64 Matrix::store(const std::string &fullpath) const
    {
        int test, a, b, c, d;

        char buffer[LARGE_BUFFER_SIZE];

        sqlite3* db = nullptr;

        const char* error_message = "";

        unsigned char* bytes;

        std::stringstream sstream;

        sqlite3_int64 row_indices[DB_MATRIX_MAX_DIMENSION];

        std::string queries[2];

        /* 1. Sprawdzenie rozmiaru tymczasowego bufora */
        /* oraz rozmiarów macierzy przed zapisem.      */

        assert
        (
            (LARGE_BUFFER_SIZE >= (1 + 2 * sizeof(double) * DB_MATRIX_MAX_DIMENSION))
            && "Zbyt maly rozmiar bufora tymczasowego -- nie zmiesci sie obiekt typu \"BLOB\"!"
        );

        if ((rows < 1) || (rows > DB_MATRIX_MAX_DIMENSION) || (columns < 1) || (columns > DB_MATRIX_MAX_DIMENSION))
        {
            throw MyException
            (
                "Matrix::store():\n" \
                "    Nieobslugiwane wymiary macierzy!\n" \
                "    Jest: [%d] wierszy na [%d] kolumn.\n" \
                "    Minimalny rozmiar: [1] x [1].\n" \
                "    Maksymalny rozmiar: [%d] x [%d].",
                rows, columns,
                DB_MATRIX_MAX_DIMENSION, DB_MATRIX_MAX_DIMENSION
            );
        }

        /* 2. Próba po³¹czenia siê z baz¹ danych. */

        test = sqlite3_open(fullpath.c_str(), &db);

        if (SQLITE_OK != test)
        {
            sqlite3_close(db);

            throw MyException
            (
                "Matrix::store():\n" \
                "    Nie mozna otworzyc pliku \"%s\" do zapisu!",
                fullpath.c_str()
            );
        }

        /* 3. Budowanie dynamicznych zapytañ do tworzenia tabelek. */

        sstream.clear();
        sstream.str("");

        sstream << "CREATE TABLE IF NOT EXISTS "
            << DB_TABLE_NAMES[0]
            << " (";

        sstream << "id INTEGER PRIMARY KEY AUTOINCREMENT"
            << ", ";

        sstream << "cols INTEGER NOT NULL"
            << ", ";

        sstream << "rows INTEGER NOT NULL";

        for (a = 0; a < DB_MATRIX_MAX_DIMENSION; a++)
        {
            std::sprintf(buffer, ", " "row%d_id INTEGER", a);
            sstream << buffer;
        }

        sstream << ");";

        queries[0] = sstream.str();

        sstream.clear();
        sstream.str("");

        sstream << "CREATE TABLE IF NOT EXISTS "
            << DB_TABLE_NAMES[1]
            << " (";

        sstream << "id INTEGER PRIMARY KEY AUTOINCREMENT"
            << ", ";

        sstream << "cells BLOB NOT NULL"
            << ");";

        queries[1] = sstream.str();

        /* 4. Utworzenie dwóch tabelek w bazie danych:             */
        /* (identyfikatory macierzy, dane poszczególnych wierszy) */

        for (a = 0; a < 2; a++)
        {
            print_dashed_line();
            std::cout << queries[a] << std::endl;

            test = sqlite3_exec
            (
                db,
                queries[a].c_str(),
                NULL,
                NULL,
                (char**)&error_message
            );

            if (SQLITE_OK != test)
            {
                sqlite3_close(db);

                throw MyException
                (
                    "Matrix::store():\n" \
                    "    Nie mozna utworzyc tabeli \"%s\" w bazie danych \"%s\"!\n" \
                    "    Komunikat od SQLite: %s",
                    DB_TABLE_NAMES[a],
                    fullpath.c_str(),
                    error_message
                );
            }
        }

        /* 5. Dodawanie kolejnych wierszy macierzy jako wiersze drugiej tabelki. */

        for (a = 0; a < rows; a++)
        {
            sstream.clear();
            sstream.str("");

            sstream << "INSERT INTO "
                << DB_TABLE_NAMES[1]
                << " (cells) VALUES";

            for (b = 0, d = 0; b < columns; b++)
            {
                bytes = (unsigned char*)&(elements[a][b]);

                for (c = 0; c < sizeof(double); c++)
                {
                    std::sprintf(&(buffer[d]), "%02X", bytes[c]);

                    d += 2;
                }
            }

            sstream << " (x'" << buffer << "');";

            print_dashed_line();
            std::cout <<sstream.str() << std::endl;

            test = sqlite3_exec
            (
                db,
                sstream.str().c_str(),
                NULL,
                NULL,
                (char**)&error_message
            );

            if (SQLITE_OK != test)
            {
                sqlite3_close(db);

                throw MyException
                (
                    "Matrix::store():\n" \
                    "    Nie mozna dodac rekordu do tabeli \"%s\" w bazie danych \"%s\"!\n" \
                    "    Komunikat od SQLite: %s",
                    DB_TABLE_NAMES[1],
                    fullpath.c_str(),
                    error_message
                );
            }

            row_indices[a] = sqlite3_last_insert_rowid(db);
        }

        /* 6. Dodawanie informacji o macierzy jako wiersz pierwszej tabelki. */

        sstream.clear();
        sstream.str("");

        sstream << "INSERT INTO "
            << DB_TABLE_NAMES[0]
            << " (cols, rows";

        for (a = 0; a < rows; a++)
        {
            std::sprintf(buffer, ", row%d_id", a);
            sstream << buffer;
        }

        sstream << ") VALUES (";

        sstream << columns
            << ", ";

        sstream << rows;

        for (a = 0; a < rows; a++)
        {
            std::sprintf(buffer, ", %" PRId64, row_indices[a]);
            sstream << buffer;
        }

        sstream << ");";

        print_dashed_line();
        std::cout <<sstream.str() << std::endl;

        test = sqlite3_exec
        (
            db,
            sstream.str().c_str(),
            NULL,
            NULL,
            (char**)&error_message
        );

        if (SQLITE_OK != test)
        {
            sqlite3_close(db);

            throw MyException
            (
                "Matrix::store():\n" \
                "    Nie mozna dodac rekordu do tabeli \"%s\" w bazie danych \"%s\"!\n" \
                "    Komunikat od SQLite: %s",
                DB_TABLE_NAMES[0],
                fullpath.c_str(),
                error_message
            );
        }

        /* 7. Zamkniêcie po³¹czenia z baz¹ danych      */
        /* oraz zwrócenie indeksu wstawionej macierzy. */

        row_indices[0] = sqlite3_last_insert_rowid(db);

        sqlite3_close(db);

        return row_indices[0];
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Struktura dla Callbacków od zapytañ "SELECT".
    ////////////////////////////////////////////////////////////////
    struct MatrixQueryData
    {
        int call_id;
        double** cells;

        struct MatrixQueryDataKeys
        {
            int64_t cols;
            int64_t rows;
            int64_t row_indices[DB_MATRIX_MAX_DIMENSION];
        };

        union
        {
            MatrixQueryDataKeys keys;
            int64_t values[DB_KEYWORDS_COUNT];
        };
    };


    ////////////////////////////////////////////////////////////////
    // [Matrix] Statyczny Callback po wywo³aniu
    // zapytania "SELECT" dla pierwszej tabelki bazy danych.
    ////////////////////////////////////////////////////////////////
    static int db_matrix_select_callback_01(void* arg1, int num_cols, char** row_data, char** names)
    {
        MatrixQueryData* data = (MatrixQueryData*)arg1;

        /* Callback uruchamiany jest tylko jeden raz! */

        if ((nullptr != data) && (0 == data->call_id))
        {
            int a, b;

            char buffer[DB_MATRIX_MAX_DIMENSION][16];

            const char* keywords[DB_KEYWORDS_COUNT];

            bool found_keyword[DB_KEYWORDS_COUNT] = {false};

            /* 1. Przygotuj nazwy poszukiwanych kolumn oraz adresy     */
            /* dla odpowiadaj¹cych s³owom kluczowym liczb ca³kowitych. */

            keywords[0] = "cols";
            keywords[1] = "rows";

            for (a = 0, b = 2; a < DB_MATRIX_MAX_DIMENSION; a++, b++)
            {
                std::sprintf(buffer[a], "row%d_id", a);
                keywords[b] = buffer[a];
            }

            /* 2. Przeszukaj kolumny zwrócone do tego callbacka. */

            for (a = 0; a < DB_KEYWORDS_COUNT; a++)
            {
                for (b = 0; !found_keyword[a] && (b < num_cols); b++)
                {
                    if (0 == std::strcmp(keywords[a], names[b]))
                    {
                        /* Pomijaj kolumny z wartoœci¹ "NULL". */

                        if (nullptr != row_data[b])
                        {
                            found_keyword[a] = true;

                            try
                            {
                                data->values[a] = std::stoll(row_data[b]);
                            }
                            catch (std::exception)
                            {
                                return FAILURE;
                            }
                        }
                    }
                }
            }

            /* 3. SprawdŸ, czy znaleziono ka¿de s³owo kluczowe. */

            if (!found_keyword[0] || !found_keyword[1])
            {
                return FAILURE;
            }

            if (data->keys.cols < 0)
            {
                data->keys.cols = 0;
            }
            else if (data->keys.cols > DB_MATRIX_MAX_DIMENSION)
            {
                data->keys.cols = DB_MATRIX_MAX_DIMENSION;
            }

            if (data->keys.rows < 0)
            {
                data->keys.rows = 0;
            }
            else if (data->keys.rows > DB_MATRIX_MAX_DIMENSION)
            {
                data->keys.rows = DB_MATRIX_MAX_DIMENSION;
            }

            b = 2 + data->keys.rows;

            for (a = 2; a < b; a++)
            {
                if (!found_keyword[a])
                {
                    return FAILURE;
                }
            }

            data->call_id += 1;
        }

        return SUCCESS;
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Statyczny Callback po wywo³aniu
    // zapytania "SELECT" dla drugiej tabelki bazy danych.
    ////////////////////////////////////////////////////////////////
    static int db_matrix_select_callback_02(void* arg1, int num_cols, char** row_data, char** names)
    {
        MatrixQueryData* data = (MatrixQueryData*)arg1;

        if (nullptr != data)
        {
            int a, b, c;

            /* 1. Do tego callbacka powinna byæ      */
            /* przekazana jedna kolumna typu "BLOB". */

            if ((1 != num_cols) || (nullptr == row_data[0]))
            {
                return FAILURE;
            }

            /* 2. Nie mamy ¿adnego okreœlenia d³ugoœci ci¹gu bajtów, */
            /* wiêc musimy zaufaæ, ¿e dostaliœmy block danych        */
            /* o rozmiarze odpowiadaj¹cym liczbie kolumn.            */

            a = data->call_id;
            b = sizeof(double) * data->keys.cols;

            std::memcpy(data->cells[a], row_data[0], b);

            /* 3. Przy nastêpnym uruchomieniu Callbacka      */
            /* bêdziemy uzupe³niaæ nastêpny wiersz macierzy. */

            data->call_id += 1;
        }

        return SUCCESS;
    }


    ////////////////////////////////////////////////////////////////
    // [Matrix] Konstruktor pobieraj¹cy macierz o konkretnym
    // indeksie z pliku bazodanowego.
    ////////////////////////////////////////////////////////////////
    Matrix::Matrix(const std::string &fullpath, sqlite3_int64 index_in_table)
    {
        int test, a;

        char buffer[LARGE_BUFFER_SIZE];

        MatrixQueryData query_data;

        sqlite3* db = nullptr;

        const char* error_message = "";

        std::stringstream sstream;

        /* 1. Próba po³¹czenia siê z baz¹ danych. */

        test = sqlite3_open(fullpath.c_str(), &db);

        if (SQLITE_OK != test)
        {
            sqlite3_close(db);

            throw MyException
            (
                "Matrix::Matrix(std::string, sqlite3_int64):\n" \
                "    Nie mozna otworzyc pliku \"%s\" do odczytu!",
                fullpath.c_str()
            );
        }

        /* 2. Zapytanie zbieraj¹ce wymiary macierzy */
        /* oraz indeksy do danych z drugiej tabelki. */

        query_data.call_id = 0;

        sstream.clear();
        sstream.str("");

        std::sprintf(buffer, "%" PRId64, index_in_table);

        sstream << "SELECT * FROM " << DB_TABLE_NAMES[0]
            << " WHERE id = " << buffer << ";";

        print_dashed_line();
        std::cout <<sstream.str() << std::endl;

        test = sqlite3_exec
        (
            db,
            sstream.str().c_str(),
            db_matrix_select_callback_01,
            (void*)&query_data,
            (char**)&error_message
        );

        if (SQLITE_OK != test)
        {
            sqlite3_close(db);

            throw MyException
            (
                "Matrix::Matrix(std::string, sqlite3_int64):\n" \
                "    Nie mozna wybrac rekordow z tabeli \"%s\" w bazie danych \"%s\"!\n" \
                "    Komunikat od SQLite: %s",
                DB_TABLE_NAMES[0],
                fullpath.c_str(),
                error_message
            );
        }

        /* 3. Dynamiczna alokacja pamiêci po pobraniu wymiarów. */

        columns = query_data.keys.cols;
        rows = query_data.keys.rows;
        create();

        query_data.call_id = 0;
        query_data.cells = elements;

        /* 4. Zapytania zbieraj¹ce dane binarne z drugiej tabelki. */

        for (a = 0; a < rows; a++)
        {
            sstream.clear();
            sstream.str("");

            std::sprintf(buffer, "%" PRId64, query_data.keys.row_indices[a]);

            sstream << "SELECT cells FROM " << DB_TABLE_NAMES[1]
                << " WHERE id = " << buffer << ";";

            print_dashed_line();
            std::cout <<sstream.str() << std::endl;

            test = sqlite3_exec
            (
                db,
                sstream.str().c_str(),
                db_matrix_select_callback_02,
                (void*)&query_data,
                (char**)&error_message
            );

            if (SQLITE_OK != test)
            {
                sqlite3_close(db);

                throw MyException
                (
                    "Matrix::Matrix(std::string, sqlite3_int64):\n" \
                    "    Nie mozna wybrac rekordu z tabeli \"%s\" w bazie danych \"%s\"!\n" \
                    "    Komunikat od SQLite: %s",
                    DB_TABLE_NAMES[1],
                    fullpath.c_str(),
                    error_message
                );
            }
        }

        /* 5. Zamkniêcie po³¹czenia z baz¹ danych. */

        sqlite3_close(db);
    }

}

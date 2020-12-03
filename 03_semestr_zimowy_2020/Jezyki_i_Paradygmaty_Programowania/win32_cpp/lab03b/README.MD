
# Laboratorium 3b

## Dodatkowe pliki wymagane przed kompilacją programu:
 * `"./libs/sqlite3/include/sqlite3/sqlite3.h"`
 * `"./libs/sqlite3/sqlite3.lib"`

## Dodatkowe pliki wymagane do uruchomienia programu:
 * `"./build/sqlite3.dll"`

## Bilbiotekę **"SQLite"** można pobrać ze strony:
 * https://www.sqlite.org/download.html
   * plik nagłówkowy znajdziemy w archiwum `"sqlite-amalgamation-???????.zip"`.
   * bibliotekę dynamiczną znajdziemy w archiwum `"sqlite-dll-win32-x86-???????.zip"`.
   * do stworzenia pliku **`"sqlite3.lib"`** wykorzystaj polecenie *`LIB /DEF:sqlite3.def /OUT:sqlite3.lib /MACHINE:X86`*, dostępne po włączeniu zestawu narzędzi konsolowych *Microsoft Visual Studio*: **`"x86 Native Tools Command Prompt for VS 201?"`**.

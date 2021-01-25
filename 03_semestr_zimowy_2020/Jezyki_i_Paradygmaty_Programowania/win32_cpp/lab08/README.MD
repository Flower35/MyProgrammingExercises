
# Laboratorium 8

## Dodatkowe pliki wymagane przed kompilacją programu:
 * `"./libs/GLFW/include/GLFW/glfw3.h"`
 * `"./libs/GLFW/libglfw3.a"`

## Dodatkowe pliki wymagane do uruchomienia programu:
 * `"./bin/glfw3.dll"`

## Bilbiotekę **"GLFW"** można pobrać ze strony:
 * https://www.glfw.org/download.html
   * plik nagłówkowy oraz bibliotekę dynamiczną znajdziemy w archiwum `"glfw-?.?.?.bin.WIN32.zip"`.

## W przypadku wystąpienia błędów w trakcie kompilacji projektu:
 * https://sourceforge.net/projects/mingwbuilds/files/host-windows/releases/4.8.1/32-bit/threads-posix/sjlj/
   * Kompilator **"MinGW"** (C++) z obsługą wątków *„POSIX”*.
   * Pozwala wyeliminować błąd kompilacji: `"'thread' is not a member of 'std'"`.

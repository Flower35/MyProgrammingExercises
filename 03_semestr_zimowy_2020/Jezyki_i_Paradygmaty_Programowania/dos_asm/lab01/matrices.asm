;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; "matrices.asm"
;; kompilator: "flat assembler g"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; [LAB 01, ZADANIE 05]
;;   Napisz program, który doda do siebie
;;   dwie macierze o wymiarach 2 x 3.
;;
;; Argumenty wymiarów i pozycji traktowane s¹
;; jako liczby "ze znakiem" (od -32768 do +32767).
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include 'align.inc'
include 'cpu/8086.inc'

format binary as 'com'

org 0x0100

    jmp ROZPOCZNIJ


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DEFINICJE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

MACIERZ_WIERSZE EQU 2
MACIERZ_KOLUMNY EQU 3
    ; MAX (ile zmieœci siê na ekranie): [6 x 8].

LIMIT_ALOKACJI EQU 0x8000


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; MAKRA
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

macro MAKRO_ODKLADANIE_PODSTAWOWYCH_ROZMIAROW_MACIERZY

    mov ax,MACIERZ_KOLUMNY
    push ax
    mov ax,MACIERZ_WIERSZE
    push ax

end macro

macro MAKRO_ODKLADANIE_ROZSZERZONYCH_ROZMIAROW_MACIERZY

    mov ax,(MACIERZ_WIERSZE * MACIERZ_KOLUMNY)
    push ax

    MAKRO_ODKLADANIE_PODSTAWOWYCH_ROZMIAROW_MACIERZY

end macro


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; FUNKCJE POMOCNICZE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include '../common/atoi_i16.inc'


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Wypisanie liczby 16-bitowej na sta³ej szerokoœci
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; INPUT:
;;  * Arg1 => INT16 (liczba "ze znakiem").
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

OUTPUT_INT16_STALA_SZEROKOSC:

    push bp
    mov bp,sp

    mov cx,0x2420
      ; Na stosie zostanie od³o¿one " $".
    push cx

    and cx,0xFBFF
      ; Odk³adanie szeœciu spacji.
      ; £¹cznie z koñcówk¹ " $" zaokr¹glamy do 3 pe³nych od³o¿eñ na stosie.
    push cx
    push cx
    push cx

    push bx

    push di
    mov di,bp
    sub di,0x02

    std

    mov ax,word[bp + 0x04]
    test ax,ax
      jz .argument_jest_zerem
      js .liczba_ujemna

    mov cl,'+'
    jmp .odlozenie_znaku_liczby

  .liczba_ujemna:

    neg ax
    mov cl,'-'

  .odlozenie_znaku_liczby:

    mov byte[bp - 0x07],cl
    jmp .rozpocznij_dzielenie

  .argument_jest_zerem:

    mov al,'0'
    stosb
    jmp .wypisz_i_wroc

  .rozpocznij_dzielenie:

    mov bx,0x0A

  .petla_dzielenia:

    xor dx,dx
    div bx

    mov cx,ax
    mov al,dl
    or al,'0'
    stosb

    mov ax,cx
    test ax,ax
      jnz .petla_dzielenia

  .wypisz_i_wroc:

    cld

    mov dx,bp
    sub dx,0x07
    mov ah,0x09
    int 0x21

    pop di
    pop bx

    mov sp,bp
    pop bp
    ret 0x02


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Funkcja wyœwietlaj¹ca wiersz dowolnej macierzy na ekranie.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; INPUT:
;;  * Arg1 => wskaŸnik na pocz¹tek wiersza macierzy
;;   (de facto jest to `int16_t*`, wskaŸnik pierwszego stopnia).
;;  * Arg2 => liczba elementów w wierszu (liczba kolumn).
;;  * Arg3 => liczba komórek do wyœwietlenia, a zarazem pozycja
;;   podkreœlonego elementu! (pusty wiersz dla Arg3 < 0).
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

WYSWIETLANIE_WIERSZA_MACIERZY:

    push bp
    mov bp,sp

    push bx
    push si

    xor bx,bx
    mov si,word[bp + 0x04]

  .petla:

    mov ax,word[bp + 0x06]
    cmp bx,ax
      jge .koniec_funkcji

    mov ah,0x02
    mov dl,' '
    int 0x21
    mov dl,'['
    int 0x21

    lodsw
      ; pobierz kolejny element wiersza w KA¯DEJ iteracji pêtli!

    mov cx,word[bp + 0x08]
    cmp bx,cx
      jg .pomin_element
      je .wstaw_znacznik

    push ax
    call OUTPUT_INT16_STALA_SZEROKOSC
    jmp .koniec_iteracji

  .wstaw_znacznik:

    mov dl,'x'
    jmp .wypisz_wypelniony_odstep

  .pomin_element:

    mov dl,' '

  .wypisz_wypelniony_odstep:

    mov ah,0x02
    mov cl,0x06

  .mini_petla_wypisywania_odstepu:

    int 0x21

    dec cl
      jnz .mini_petla_wypisywania_odstepu

  .koniec_iteracji:

    mov ah,0x02
    mov dl,']'
    int 0x21

    inc bx
    jmp .petla

  .koniec_funkcji:

    call WYSWIETL_CRLF

    pop si
    pop bx

    mov sp,bp
    pop bp
    ret 0x06


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Funkcja wyœwietlaj¹ca dowoln¹ macierz na ekranie.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; INPUT:
;;  * Arg1 => pocz¹tek macierzy, lista wskaŸników do wierszy.
;;   (de facto jest to `int16_t**`, wskaŸnik drugiego stopnia).
;;  * Arg2 => liczba wierszy.
;;  * Arg3 => liczba kolumn.
;;  * Arg4 => ³¹czna liczba jawnych elementów (wierszami, po kolumnach)
;;   (kiedy licznik spadnie do 0, oznacza to element podkreœlony).
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

WYSWIETLENIE_MACIERZY:

    push bp
    mov bp,sp

    push bx
    xor bx,bx

    push si
    mov si,word[bp + 0x04]

  .petla:

    mov ax,word[bp + 0x06]
    cmp bx,ax
      jge .koniec_funkcji

    mov ax,word[bp + 0x0A]
    push ax
      ; Nowy Arg3 = Stary Arg4 (modyfikowany)

    mov ax,word[bp + 0x08]
    push ax
      ; Nowy Arg2 = Stary Arg3

    lodsw
    push ax
      ; Nowy Arg1 = kolejny element z listy wskaŸników

    call WYSWIETLANIE_WIERSZA_MACIERZY

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Zmniejszenie licznika jawnych elementów
    ;; * o liczbê kolumn co ka¿dy wiersz.
    ;; * Krok pomijany dopiero kiedy licznik jest ujemny.

    mov ax,word[bp + 0x0A]
    test ax,ax
      jl .koniec_iteracji_petli

    mov cx,word[bp + 0x08]
    sub ax,cx
    mov word[bp + 0x0A],ax

  .koniec_iteracji_petli:

    inc bx
    jmp .petla

  .koniec_funkcji:

    pop si
    pop bx

    mov sp,bp
    pop bp
    ret 0x08


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Funkcja wype³niaj¹ca dowoln¹ macierz.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; INPUT:
;;  * Arg1 => pocz¹tek macierzy, lista wskaŸników do wierszy.
;;  * Arg2 => liczba wierszy.
;;  * Arg3 => liczba kolumn.
;; RETURNS:
;;  * CARRY BIT => ustawiony w przypadku wyst¹pienia b³êdu.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

WYPELNIJ_MACIERZ:

    push bp
    mov bp,sp

    xor cx,cx
    push cx
    push cx
    push cx
    sub sp,0x0A
      ; [bp - 0x02] = licznik wierszy (pêtla zewnêtrzna)
      ; [bp - 0x04] = licznik kolumn (pêtla wewnêtrzna)
      ; [bp - 0x06] = licznik elementów
      ; [bp - 0x10] = bufor (2B specjalne + 7B na tekst + Bajt CR)

    push bx

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Sprawdzenie, czy argumenty rozmiarów s¹ dodanie,
    ;; * oraz czy "WIERSZE x KOLUMNY" nie przekraczaj¹ 2 bajtów.

    mov ax,word[bp + 0x06]
    test ax,ax
      jle .nieprawidlowe_argumenty

    mov cx,word[bp + 0x08]
    test cx,cx
      jle .nieprawidlowe_argumenty

    mul cx
    test dx,dx
      jnz .nieprawidlowe_argumenty

    jmp .petla_wierszy

  .nieprawidlowe_argumenty:

    jmp .niepowodzenie__out_of_range

  .petla_wierszy:

    mov cx,word[bp - 0x02]
    mov ax,word[bp + 0x06]
    cmp cx,ax
      jl .petla_kolumn

    jmp .porzucenie_petli_wierszy

  .petla_kolumn:

    mov cx,word[bp - 0x04]
    mov ax,word[bp + 0x08]
    cmp cx,ax
      jge .porzucenie_petli_kolumn

    mov ax,word[bp - 0x06]
    push ax
      ; Nowy Arg4 = lokalny licznik.
      ; Pozosta³e 3 argumenty to kopia Arg tej funkcji.

    mov ax,word[bp + 0x08]
    push ax

    mov ax,word[bp + 0x06]
    push ax

    mov ax,word[bp + 0x04]
    push ax

    call WYSWIETLENIE_MACIERZY

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Prosty monit o wprowadzenie danych.

    mov ah,0x02
    mov dl,0x0A
    int 0x21
    mov dl,'>'
    int 0x21
    int 0x21
    mov dl,' '
    int 0x21

    mov bx,bp
    sub bx,0x10
    mov word[bx],0x0008
      ; bufor o rozmiarze 8, z pust¹ zawartoœci¹.

    mov dx,bx
    mov ah,0x0A
    int 0x21

    mov ah,0x02
    mov dl,0x0A
    int 0x21
    int 0x21

    mov dx,bx
    add dx,0x02
    call ATOI_INT16
      jc .niepowodzenie__out_of_range

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Wstawienie nowo odczytanej liczby na konkretne miejsce w macierzy.

    mov bx,word[bp + 0x04]
    mov cx,word[bp - 0x02]
    shl cx,0x01
    add bx,cx
    mov bx,word[bx]

    mov cx,word[bp - 0x04]
    shl cx,0x01
    add bx,cx
    mov word[bx],ax

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Koniec iteracji pêtli wewnêtrznej.

    mov ax,word[bp - 0x06]
    inc ax
    mov word[bp - 0x06],ax

    mov ax,word[bp - 0x04]
    inc ax
    mov word[bp - 0x04],ax
    jmp .petla_kolumn

  .porzucenie_petli_kolumn:

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Koniec iteracji pêtli zewnêtrznej.
    ;; * Resetujemy licznik pêtli wewnêtrznej.

    xor ax,ax
    mov word[bp - 0x04],ax

    mov ax,word[bp - 0x02]
    inc ax
    mov word[bp - 0x02],ax
    jmp .petla_wierszy

  .porzucenie_petli_wierszy:

    clc
    jmp .koniec_funkcji

  .niepowodzenie__out_of_range:

    stc

  .koniec_funkcji:

    pop bx

    mov sp,bp
    pop bp
    ret 0x06


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Funkcja dodaje do siebie dwie macierze.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; INPUT:
;;  * Arg1 => pierwsza lista wskaŸników do wierszy.
;;   Pe³ni równie¿ rolê wyniku, zatem uwa¿aj,
;;   bo elementy tej macierzy zostan¹ nadpisane!
;;  * Arg2 => druga lista wskaŸników do wierszy.
;;  * Arg3 => liczba wierszy.
;;  * Arg4 => liczba kolumn.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

DODAJ_DO_SIEBIE_MACIERZE:

    push bp
    mov bp,sp

    xor cx,cx
    push cx
    push cx
    sub sp,0x08
      ; [bp - 0x02] = licznik wierszy (pêtla zewnêtrzna)
      ; [bp - 0x04] = licznik kolumn (pêtla wewnêtrzna)
      ; [bp - 0x06] = wiersz pierwszej macierzy
      ; [bp - 0x08] = wiersz drugiej macierzy

    push bx

  .petla_wierszy:

    mov cx,word[bp - 0x02]
    mov ax,word[bp + 0x08]
    cmp cx,ax
      jge .porzucenie_petli_wierszy

    shl cx,0x01

    mov bx,word[bp + 0x04]
    add bx,cx
    mov ax,word[bx]
    mov word[bp - 0x06],ax

    mov bx,word[bp + 0x06]
    add bx,cx
    mov ax,word[bx]
    mov word[bp - 0x08],ax

  .petla_kolumn:

    mov cx,word[bp - 0x04]
    mov ax,word[bp + 0x0A]
    cmp cx,ax
      jge .porzucenie_petli_kolumn

    mov bx,word[bp - 0x08]
    mov dx,word[bx]
    add bx,0x02
    mov word[bp - 0x08],bx

    mov bx,word[bp - 0x06]
    mov ax,word[bx]
    add ax,dx
    mov word[bx],ax
    add bx,0x02
    mov word[bp - 0x06],bx

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Koniec iteracji pêtli wewnêtrznej.

    mov cx,word[bp - 0x04]
    inc cx
    mov word[bp - 0x04],cx
    jmp .petla_kolumn

  .porzucenie_petli_kolumn:

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Koniec iteracji pêtli zewnêtrznej.

    xor ax,ax
    mov word[bp - 0x04],ax

    mov ax,word[bp - 0x02]
    inc ax
    mov word[bp - 0x02],ax
    jmp .petla_wierszy

  .porzucenie_petli_wierszy:

    pop bx

    mov sp,bp
    pop bp
    ret 0x08


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Pseudo-dynamiczna alokacja pamiêci dla macierzy.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; INPUT:
;;  * Arg1 => referencja na "wolne" miejsce w pamiêci.
;;  * Arg2 => liczba wierszy.
;;  * Arg3 => liczba kolumn.
;; RETURNS:
;;  * reg AX => przydzielona lista wierszy.
;;   (de facto jest to `int16_t**`, wskaŸnik drugiego stopnia).
;;  * CARRY BIT => ustawiony w przypadku wyst¹pienia b³êdu.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

UTWORZ_DYNAMICZNA_MACIERZ:

    push bp
    mov bp,sp

    push bx
    push di

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Sprawdzenie, czy argumenty wymiarów s¹ dodatnie
    ;; * oraz czy macierz o zadanych wymiarach
    ;; * pomieœci siê na jednym segmencie.

    mov cx,word[bp + 0x06]
    test cx,cx
      jle .niepowodzenie

    mov ax,word[bp + 0x08]
    test ax,ax
      jle .niepowodzenie

    mul cx
    test dx,dx
      jnz .niepowodzenie
    shl ax,0x01
      jc .niepowodzenie

      ; "AX" jest teraz rozmiarem bloku: [2B x WIERSZE x KOLUMNY].

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Sprawdzenie, czy tworzona macierz
    ;; * pomieœci siê do koñca ustalonego limitu adresowego.
    ;; * Sprawdzamy wyra¿enie: [KONTEKST + ROZMIAR BLOKU + ROZMIAR LISTY].

    mov bx,word[bp + 0x04]
    mov dx,word[bx]
    add ax,dx
    mov di,ax
      ; "DX" wskazuje na pocz¹tek bloku danych.
      ; "AX" i "DI" wskazuj¹ na koniec bloku (a pocz¹tek listy).

    shl cx,0x01
    add ax,cx
    cmp ax,LIMIT_ALOKACJI
      ja .niepowodzenie

    mov word[bx],ax

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Wype³nianie listy adresami wierszy.

      ; Lista jest opcjonalnym elementem, poniewa¿ odliczony wczeœniej
      ; blok elementów macierzy ma te elementy ustawione bez odstêpów
      ; i we w³aœciwe kolejnoœci. Udajemy jednak, ¿e zwrócona lista
      ; jest dynamiczna i ma niepowi¹zane ze sob¹ adresy do wierszy
      ; (jak w przypadku wywo³ywania `malloc`).

    mov ax,dx
    mov dx,word[bp + 0x08]
    shl dx,0x01
      ; "AX" wskazuje na pocz¹tek bloku.
      ; "DX" sprowadza siê do wyra¿enia: [2B x KOLUMNY].

    mov cx,word[bp + 0x06]
    test cx,cx
      jz .alokacja_pomyslna

  .petla_wypelniania_listy:

    stosw

    add ax,dx
    dec cx
      jnz .petla_wypelniania_listy

  .alokacja_pomyslna:

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Po zakoñczeniu pêtli "AX" bêdzie wskazywaæ
    ;; * na koniec bloku, czyli na pocz¹tek listy!

    clc
    jmp .koniec

  .niepowodzenie:

    stc

  .koniec:

    pop di
    pop bx

    mov sp,bp
    pop bp
    ret 0x06


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; POZOSTA£E FUNKCJE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

WYSWIETL_CRLF:

    mov ah,0x02
    mov dl,0x0D
    int 0x21
    mov dl,0x0A
    int 0x21

    ret


WYSWIETL_PAUZE:

    mov ah,0x02
    mov cl,0x40
    mov dl,'-'

  .petla:

    int 0x21

    dec cl
      jnz .petla

    call WYSWIETL_CRLF

    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; POCZ¥TEK PROGRAMU
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ROZPOCZNIJ:

    mov bp,sp
    sub sp,0x04
    mov ax,POCZATEK_ALOKACJI
    push ax
      ; [bp - 0x02] = pierwsza macierz
      ; [bp - 0x04] = druga macierz
      ; [bp - 0x06] = pierwszy wolny adres do kolejnych alokacji

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Próba pseudo-dynamicznej alokacji pamiêci.

    mov bx,bp
    sub bx,0x06

    MAKRO_ODKLADANIE_PODSTAWOWYCH_ROZMIAROW_MACIERZY

    push bx
    call UTWORZ_DYNAMICZNA_MACIERZ
      jnc .pierwsza_alokacja_pomyslna

    jmp .nie_mozna_rozpoczac

  .pierwsza_alokacja_pomyslna:

    mov word[bp - 0x02],ax

    MAKRO_ODKLADANIE_PODSTAWOWYCH_ROZMIAROW_MACIERZY

    push bx
    call UTWORZ_DYNAMICZNA_MACIERZ
      jnc .druga_alokacja_pomyslna

    jmp .nie_mozna_rozpoczac

  .druga_alokacja_pomyslna:

    mov word[bp - 0x04],ax

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * U¿ytkownik wprowadza dane do swoich macierzy.

    call WYSWIETL_PAUZE

    mov dx,napis_powitanie
    mov ah,0x09
    int 0x21

    call WYSWIETL_CRLF

    call WYSWIETL_PAUZE

    call WYSWIETL_CRLF

    MAKRO_ODKLADANIE_PODSTAWOWYCH_ROZMIAROW_MACIERZY

    mov ax,word[bp - 0x02]
    push ax

    call WYPELNIJ_MACIERZ
      jc .nie_mozna_kontynuowac

    MAKRO_ODKLADANIE_PODSTAWOWYCH_ROZMIAROW_MACIERZY

    mov ax,word[bp - 0x04]
    push ax

    call WYPELNIJ_MACIERZ
      jc .nie_mozna_kontynuowac

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Wyœwietl dwie wype³nione macierze.

    call WYSWIETL_PAUZE

    call WYSWIETL_CRLF

    MAKRO_ODKLADANIE_ROZSZERZONYCH_ROZMIAROW_MACIERZY

    mov ax,word[bp - 0x02]
    push ax

    call WYSWIETLENIE_MACIERZY

    call WYSWIETL_CRLF

    MAKRO_ODKLADANIE_ROZSZERZONYCH_ROZMIAROW_MACIERZY

    mov ax,word[bp - 0x04]
    push ax

    call WYSWIETLENIE_MACIERZY

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Nastêpuje dodanie do siebie dwóch macierzy.

    MAKRO_ODKLADANIE_PODSTAWOWYCH_ROZMIAROW_MACIERZY

    mov ax,word[bp - 0x04]
    push ax

    mov ax,word[bp - 0x02]
    push ax

    call DODAJ_DO_SIEBIE_MACIERZE

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Wyœwietl wynik przeprowadzonej operacji na ekranie.

    call WYSWIETL_CRLF

    MAKRO_ODKLADANIE_ROZSZERZONYCH_ROZMIAROW_MACIERZY

    mov ax,word[bp - 0x02]
    push ax

    call WYSWIETLENIE_MACIERZY

    call WYSWIETL_CRLF

    mov dx,napis_podziekowanie
    jmp .wyswietl_tekst_i_zakoncz

  .nie_mozna_kontynuowac:

    mov dx,napis_blad_odczytu
    jmp .wyswietl_tekst_i_zakoncz

  .nie_mozna_rozpoczac:

    mov dx,napis_blad_alokacji

  .wyswietl_tekst_i_zakoncz:

    mov ah,0x09
    int 0x21

    call WYSWIETL_CRLF

    mov ax,0x4C00
    int 0x21


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DANE PROGRAMU
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

napis_powitanie:
  db "This program will add together two matrices with dimensions 2 x 3.",'$'

napis_blad_alokacji:
  db "Not enough memory to run this program! :(",'$'

napis_blad_odczytu:
  db "What have you done...",'$'

napis_podziekowanie:
  db "Thanks. See you soon!",'$'

align 0x02
  POCZATEK_ALOKACJI:


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

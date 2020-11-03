;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; "task03.asm"
;; kompilator: "flat assembler g"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; [LAB 02, ZADANIE 05]
;;   Napisz funkcjê, która zamienia miejscami wartoœci swoich
;;   dwóch argumentów. Przeka¿ do funkcji argumenty przy u¿yciu
;;   wskaŸników albo referencji. Przygotuj odpowiednie testy
;;   dla zaproponowanego rozwi¹zania.
;;
;; [LAB 02, ZADANIE 06]
;;   Nale¿y wzi¹æ pod uwagê, ¿e funkcja `std::swap()` powoduje
;;   skopiowanie obiektu do zmiennej tymczasowej i dwa razy wywo³uje
;;   operator przypisania, wiêc mo¿e nie byæ efektywna dla typów,
;;   dla których wspomniane operacje s¹ kosztowne.
;;   W takie sytuacji rekomendowane jest przeci¹¿enie tej funkcji tak,
;;   by wykonywa³a zamianê w wydajniejszy sposób.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include 'align.inc'
include 'cpu/8086.inc'

format binary as 'com'

org 0x0100

    jmp START


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DEFINICJE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ROZMIAR_POTEZNEGO_OBIEKTU EQU (8 * 1024 - 1)
TYPOWY_ROZMIAR_STOSU EQU 256


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; FUNKCJE POMOCNICZE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include '../common/memcpy.inc'
include '../common/memcmp.inc'

include '../common/out_h16.inc'


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Funkcja do zamiany miejscami swoich argumentów
;; (traktowanych jako bloki danych o ustalonym rozmiarze).
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; INPUT:
;;  * Arg1 => adres na pocz¹tek pierwszego obiektu.
;;  * Arg2 => adres na pocz¹tek drugiego obiektu.
;;  * Arg3 => rozmiar typu przekazanych obiektów (jak `sizeof`).
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ZAMIANA_MIEJSCAMI:

    push bp
    mov bp,sp
    sub sp,0x02
      ; [bp - 0x02] = licznik pêtli

    push bx
    push si
    push di

    mov cx,word[bp + 08]
    and cx,(not 0x0001)
      js .powrot
      jz .podmiana_ogona

  .petla_wordowska:

    mov word[bp - 0x02],cx

    mov si,word[bp + 0x04]
    mov di,si
    lodsw
    mov cx,ax

    mov si,word[bp + 0x06]
    mov bx,si
    lodsw

    stosw
    mov word[bp + 0x04],di

    mov di,bx
    mov ax,cx
    stosw
    mov word[bp + 0x06],di

    mov cx,word[bp - 0x02]
    dec cx
    dec cx
      jnz .petla_wordowska

  .podmiana_ogona:

    mov cx,word[bp + 08]
    shr cx,0x01
      jnc .powrot

    mov si,word[bp + 0x04]
    mov di,si
    lodsb
    mov ah,al

    mov si,word[bp + 0x06]
    mov bx,si
    lodsb

    stosb

    mov di,bx
    mov al,ah
    stosb

  .powrot:

    pop di
    pop si
    pop bx

    mov sp,bp
    pop bp
    ret 0x06


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Wyœwietlenie adresu obiektu w postaci hexadecymalnej.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; INPUT:
;;  * Arg1 => adres obiektu.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

@_OBIEKT_@_WYSWIETL_ADRES:

    push bp
    mov bp,sp

    mov dl,'['
    mov ah,0x02
    int 0x21
    mov dl,'0'
    int 0x21
    mov dl,'x'
    int 0x21

    mov ax,word[bp + 0x04]
    call OUTPUT_HEX16

    mov dl,']'
    mov ah,0x02
    int 0x21

    pop bp
    ret 0x02


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Wype³nianie obiektu pseudolosowymi znakami.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; INPUT:
;;  * Arg1 => adres obiektu.
;;  * reg BX => rozmiar obiektu (optymalizacja programu).
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

@_OBIEKT_@_LOSOWO_WYPELNIJ:

    push bp
    mov bp,sp

    push di
    mov di,word[bp + 0x04]

    mov dx,napis_wypelnianie_A
    mov ah,0x09
    int 0x21

    push di
    call @_OBIEKT_@_WYSWIETL_ADRES

    mov dx,napis_wypelnianie_B
    mov ah,0x09
    int 0x21
    mov dx,napis_CRLF
    int 0x21

    mov cx,bx
    and cx,(not 0x0001)
      js .powrot
      jz .wypelnianie_ogona

  .petla_wordowska:

    call LOSUJ_NUMEREK
    stosw

    dec cx
    dec cx
      jnz .petla_wordowska

  .wypelnianie_ogona:

    mov cx,bx
    shr cx,0x01
      jnc .powrot

    call LOSUJ_NUMEREK
    stosb

  .powrot:

    pop di

    pop bp
    ret 0x02


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Przypisanie jednego obiektu do drugiego.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; INPUT:
;;  * Arg1 => adres obiektu docelowego.
;;  * Arg2 => adres obiektu Ÿród³owego.
;;  * reg BX => rozmiar obiektu (optymalizacja programu).
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

@_OBIEKT_@_PRZYPISYWANIE:

    push bp
    mov bp,sp

    push si
    push di

    mov si,word[bp + 0x06]
    mov di,word[bp + 0x04]

    mov dx,napis_kopiowanie_A
    mov ah,0x09
    int 0x21

    push si
    call @_OBIEKT_@_WYSWIETL_ADRES

    mov dx,napis_kopiowanie_B
    mov ah,0x09
    int 0x21

    push di
    call @_OBIEKT_@_WYSWIETL_ADRES

    mov dl,'.'
    mov ah,0x02
    int 0x21

    mov dx,napis_CRLF
    mov ah,0x09
    int 0x21

    push bx
    push di
    push si
    call MEMCPY

    pop di
    pop si

    pop bp
    ret 0x04


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Generator liczb pseudolosowych.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; RETURNS:
;;  * reg AX => kolejny losowy numerek.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

LOSUJ_NUMEREK:

    mov ax,0x4E6D
    mul word[rng]
    add ax,0x3039
    mov word[rng],ax

    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; POZOSTA£E FUNKCJE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

WYSWIETL_LINIE_ODSTEPU:

    mov ah,0x02
    mov cl,0x40
    mov dl,'-'

  .petla:

    int 0x21

    dec cl
      jnz .petla

    mov dx,napis_CRLF
    mov ah,0x09
    int 0x21

    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; POCZ¥TEK PROGRAMU
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

START:

    ; Ziarno generatora liczb pseudolosowych (czas systemowy w sekundach)

    mov ah,0x00
    int 0x1A
    mov word[rng],dx

    ; "BX" bêdzie przechowywaæ wartoœæ sta³¹ (rozmiar testowego obiektu)

    mov bx,ROZMIAR_POTEZNEGO_OBIEKTU

    ; Wype³nianie dwóch obiektów losowymi znakami

    call WYSWIETL_LINIE_ODSTEPU

    mov ax,testowy_obiekt_A
    push ax
    call @_OBIEKT_@_LOSOWO_WYPELNIJ

    mov ax,testowy_obiekt_B
    push ax
    call @_OBIEKT_@_LOSOWO_WYPELNIJ

    ; Kopia pierwotnego stanu obiektów ("A" na "C", "B" na "D")

    call WYSWIETL_LINIE_ODSTEPU

    mov ax,testowy_obiekt_A
    push ax
    mov ax,testowy_obiekt_C
    push ax
    call @_OBIEKT_@_PRZYPISYWANIE

    mov ax,testowy_obiekt_B
    push ax
    mov ax,testowy_obiekt_D
    push ax
    call @_OBIEKT_@_PRZYPISYWANIE

    ; Zamiana obiektów miejscami i sprawdzenie poprawnoœci zamiany

    call WYSWIETL_LINIE_ODSTEPU

    push bx
    mov ax,testowy_obiekt_B
    push ax
    mov ax,testowy_obiekt_A
    push ax
    call ZAMIANA_MIEJSCAMI

    push bx
    mov ax,testowy_obiekt_D
    push ax
    mov ax,testowy_obiekt_A
    push ax
    call MEMCMP
      jc .niepowodzenie

    push bx
    mov ax,testowy_obiekt_C
    push ax
    mov ax,testowy_obiekt_B
    push ax
    call MEMCMP
      jc .niepowodzenie

    mov dx,napis_sukces
    jmp .wyswietl_tekst_i_zakoncz

  .niepowodzenie:

    mov dx,napis_niepowodzenie

  .wyswietl_tekst_i_zakoncz:

    mov ah,0x09
    int 0x21

    mov dx,napis_wykrzyknik_CRLF
    mov ah,0x09
    int 0x21

    mov ax,0x4C00
    int 0x21


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DANE PROGRAMU
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

napis_wypelnianie_A:
  db "Filling object ",'$'

napis_wypelnianie_B:
  db " width random bytes.",'$'

napis_kopiowanie_A:
  db "Copying object ",'$'

napis_kopiowanie_B:
  db " to ",'$'

napis_sukces:
  db "OBJECTS HAVE BEEN SWAPPED SUCCESSFULLY",'$'

napis_niepowodzenie:
  db "OBJECT SWAPPING FAILED",'$'

napis_wykrzyknik_CRLF:
  db "!"
napis_CRLF:
  db 0x0D,0x0A,'$'

align 0x02
rng: rb 0x02

align 0x02
testowy_obiekt_A: rb ROZMIAR_POTEZNEGO_OBIEKTU

align 0x02
testowy_obiekt_B: rb ROZMIAR_POTEZNEGO_OBIEKTU

align 0x02
testowy_obiekt_C: rb ROZMIAR_POTEZNEGO_OBIEKTU

align 0x02
testowy_obiekt_D: rb ROZMIAR_POTEZNEGO_OBIEKTU

assert ($ <= (0xFFFF + 0x01 - TYPOWY_ROZMIAR_STOSU))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

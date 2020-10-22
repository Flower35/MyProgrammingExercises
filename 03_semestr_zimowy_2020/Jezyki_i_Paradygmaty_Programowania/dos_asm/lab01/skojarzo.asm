;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; "skojarzo.asm"
;; kompilator: "flat assembler g"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; [LAB 01, ZADANIE 04]
;;   Dwie ró¿ne liczby ca³kowite "a" i "b" wiêksze od 1 nazwiemy skojarzonymi,
;;   jeœli suma wszystkich ró¿nych dodatnich dzielników "a" mniejszych od "a"
;;   jest równa "b+1", a suma wszystkich ró¿nych dodatnich dzielników "b"
;;   mniejszych od "b" jest równa "a+1".
;;
;; Ten program wypisze wszystkie mo¿liwe pary
;; liczb skojarzonych dla [2 >= n => 16384].
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include 'align.inc'
include 'cpu/8086.inc'

format binary as 'com'

org 0x0100

    jmp ROZPOCZNIJ


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DEFINICJE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

LIMIT_LICZB_SKOJARZONYCH EQU 0x4000
    ; ostatnia para: [9504, 20735].

ROZMIAR_TABLICY_POWTORZEN EQU ((1 shl 15) shr 3)
    ; rozmiar tablicy bitów (ka¿dy bit odpowiada jednemu numerowi).


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; FUNKCJE POMOCNICZE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include '../common/out_u16.inc'
include '../common/sqrt_u16.inc'


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Obliczenie sumy dzielników w³aœciwych danej liczby naturalnej.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; INPUT:
;;  * reg AX => UINT16 (liczba "bez znaku").
;; RETURNS:
;;  * reg AX => obliczona suma.
;; KONWENCJA:
;;  * zachowuje: BX.
;;  * niszczy: CX.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SUMA_DZIELNIKOW_WLASCIWYCH_BEZ_JEDYNKI:

    push bp
    mov bp,sp

    xor cx,cx

    push ax
    push cx
    push cx
      ; [bp - 0x02] = zadana liczba
      ; [bp - 0x04] = wynik
      ; [bp - 0x06] = licznik dzielnikow

    push bx

    cmp ax,0x02
      jb .niepowodzenie

    call SQRT_UINT16
    mov bx,ax

    mov cx,0x02

  .petla_sprawdzania_dzielnikow:

    cmp cx,bx
      ja .wyjscie_z_petli

    mov word[bp - 0x06],cx

    xor dx,dx
    mov ax,word[bp - 0x02]
    div cx

    test dx,dx
      jnz .nastepny_dzielnik

    add word[bp - 0x04],ax
      jc .niepowodzenie

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Nie dodawaj dwa razy pierwiastka kwadratowego!

    cmp ax,bx
      je .wyjscie_z_petli

    mov cx,ax
    xor dx,dx
    mov ax,word[bp - 0x02]
    div cx

    add word[bp - 0x04],ax
      jc .niepowodzenie

  .nastepny_dzielnik:

    mov cx,word[bp - 0x06]
    inc cx
    jmp .petla_sprawdzania_dzielnikow

  .niepowodzenie:

    xor ax,ax
    jmp .koniec_funkcji

  .wyjscie_z_petli:

    mov ax,word[bp - 0x04]

  .koniec_funkcji:

    pop bx

    mov sp,bp
    pop bp
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Wybranie bajtu z Tablicy Powtórzeñ.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; INPUT:
;;  * reg AX => sprawdzana liczba.
;; RETURNS:
;;  * reg AL => odczytany bajt z tablicy.
;;  * reg AH => maska bitu dla docelowego bajtu.
;;  * reg BX => pozycja bajtu w tablicy (0 w przypadku z³ego argumentu).
;; KONWENCJA:
;;  * niszczy: AX, CX.
;;  * zachowuje: BX.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

WYBIERZ_BAJT_Z_TABLICY_POWTORZEN:

    mov cl,al

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Podziel argument przez osiem
    ;; * i wybierz odpowiedni bajt z tablicy.

    shr ax,0x01
    shr ax,0x01
    shr ax,0x01

    cmp ax,ROZMIAR_TABLICY_POWTORZEN
      jb .kontynuuj

    xor bx,bx
    ret

  .kontynuuj:

    mov bx,tablica_powtorzen
    add bx,ax
    mov al,byte[bx]

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Ustaw odpowiedni bit maski.
    ;; * [0x00 -> 10000000b (0x80), 0x07 -> 00000001b (0x01)]

    mov ah,0x80
    and cl,0x07
      jz .powrot

  .petla_wyboru_bitu:

    shr ah,0x01

    dec cl
      jnz .petla_wyboru_bitu

  .powrot:

    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Ustawienie bitu w Tablicy Powtórzeñ.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; INPUT:
;;  * reg AX => sprawdzana liczba.
;; KONWENCJA:
;;  * zachowuje: AX, BX.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

USTAW_BIT_W_TABLICY_POWTORZEN:

    push ax
    push bx

    test ax,ax
      jz .powrot

    call WYBIERZ_BAJT_Z_TABLICY_POWTORZEN
    test bx,bx
      jz .powrot

    or al,ah
    mov byte[bx],al

  .powrot:

    pop bx
    pop ax
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Sprawdzenie bitu w Tablicy Powtórzeñ.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; INPUT:
;;  * reg AX => sprawdzana liczba.
;; RETURNS:
;;  * CARRY BIT => ustawiony, jeœli liczba ju¿ wystêpowa³a.
;; KONWENCJA:
;;  * zachowuje: AX, BX.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SPRAWDZ_BIT_W_TABLICY_POWTORZEN:

    push ax
    push bx

    test ax,ax
      jz .liczba_wystepowala

    call WYBIERZ_BAJT_Z_TABLICY_POWTORZEN
    test bx,bx
      jz .pozwalam_na_sprawdzenie

    and al,ah
      jnz .liczba_wystepowala

  .pozwalam_na_sprawdzenie:

    clc
    jmp .powrot

  .liczba_wystepowala:

    stc

  .powrot:

    pop bx
    pop ax
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; POCZ¥TEK PROGRAMU
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ROZPOCZNIJ:

    mov bp,sp
    sub sp,0x0A
      ; [bp - 0x02] = licznik
      ; [bp - 0x04] = suma dzielników licznika
      ; [bp - 0x06] = licznik animacji karetki

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Wyzerowanie Tablicy Powtórzeñ
    ;; * oraz przygotowanie licznika pêtli.

    xor ax,ax
    mov di,tablica_powtorzen
    mov cx,(ROZMIAR_TABLICY_POWTORZEN shr 0x01)
    rep stosw

    mov word[bp - 0x06],ax
    mov ax,0x02
    mov word[bp - 0x02],ax

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Wyczyœæ bufor klawiatury
    ;; * przed wejœciem do g³ównej pêtli programu.

  .czyszczenie_bufora_klawiatury:

    mov ah,0x01
    int 0x16
      jz .schowaj_migajacy_kursor

    mov ah,0x00
    int 0x16

    jmp .czyszczenie_bufora_klawiatury

  .schowaj_migajacy_kursor:

    mov ch,0x20
    mov ah,0x01
    int 0x10

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Naciœniêcie dowolnego klawisza
    ;; * spowoduje przerwanie wykonywania programu.

  .petla_sprawdzania_liczb_skojarzonych:

    mov ah,0x01
    int 0x16
      jz .nie_przerwano_programu

    mov ah,0x00
    int 0x16

    jmp .licznik_wyczerpany

  .nie_przerwano_programu:

    mov ax,word[bp - 0x06]
    inc ax
    and ax,0x03
    mov word[bp - 0x06],ax

    mov si,animacja_karetki
    add si,ax
    lodsb

    mov dl,al
    mov ah,0x02
    int 0x21
    mov dl,0x0D
    int 0x21

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Ustaw bit w Tablicy Powtórzeñ dla liczby z licznka pêtli.
    ;; * Oblicz sumê dzielników w³aœciwych bez jedynki.

    mov ax,word[bp - 0x02]

    call USTAW_BIT_W_TABLICY_POWTORZEN

    call SUMA_DZIELNIKOW_WLASCIWYCH_BEZ_JEDYNKI

    test ax,ax
      jz .kolejna_liczba
        ; skok na koniec, je¿eli wyst¹pi³ b³¹d
        ; lub gdy licznik jest liczb¹ pierwsz¹.

    mov word[bp - 0x04],ax

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * SprawdŸ, czy uzyskana liczba by³a ju¿ sprawdzana.
    ;; * Nastêpnie sprawdŸ, czy uzyskaliœmy liczby skojarzone.

    call SPRAWDZ_BIT_W_TABLICY_POWTORZEN
      jc .kolejna_liczba

    call SUMA_DZIELNIKOW_WLASCIWYCH_BEZ_JEDYNKI

    test ax,ax
      jnz .suma_nie_jest_liczba_pierwsza

    mov ax,word[bp - 0x04]

    call USTAW_BIT_W_TABLICY_POWTORZEN

    jmp .kolejna_liczba

  .suma_nie_jest_liczba_pierwsza:

    cmp ax,word[bp - 0x02]
      jne .kolejna_liczba

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Znaleziono parê skojarzonych liczb!

    mov ax,word[bp - 0x04]

    call USTAW_BIT_W_TABLICY_POWTORZEN

    mov dx,napis01
    mov ah,0x09
    int 0x21

    mov ax,word[bp - 0x02]
    call OUTPUT_UINT16

    mov dx,napis02
    mov ah,0x09
    int 0x21

    mov ax,word[bp - 0x04]
    call OUTPUT_UINT16

    mov dx,napis03
    mov ah,0x09
    int 0x21

  .kolejna_liczba:

    mov ax,word[bp - 0x02]

    add ax,0x01
      jc .licznik_wyczerpany

    cmp ax,LIMIT_LICZB_SKOJARZONYCH
      ja .licznik_wyczerpany

    mov word[bp - 0x02],ax

    call SPRAWDZ_BIT_W_TABLICY_POWTORZEN
      jc .kolejna_liczba

    jmp .petla_sprawdzania_liczb_skojarzonych

  .licznik_wyczerpany:

    mov cx,0x0607
    mov ah,0x01
    int 0x10
      ; przywrócono migaj¹cy kursor w konsoli

    mov ax,0x4C00
    int 0x21


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DANE PROGRAMU
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

animacja_karetki: db 0x2F,0x2D,0x5C,0x7C

napis01: db " -> (",'$'
napis02: db ", ",'$'
napis03: db ")",0x0D,0x0A,'$'

align 0x02
tablica_powtorzen: rb ROZMIAR_TABLICY_POWTORZEN


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

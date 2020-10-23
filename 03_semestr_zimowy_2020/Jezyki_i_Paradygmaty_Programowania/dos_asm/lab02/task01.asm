;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; "task01.asm"
;; kompilator: "flat assembler g"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; [LAB 02, ZADANIE 01]
;;   Napisz funkcjê otrzymuj¹c¹ jako argumenty
;;   wskaŸniki do dwóch zmiennych typu "int",
;;   która zamienia ze sob¹ wartoœci wskazywanych zmiennych
;;   tylko wtedy, gdy wskazywana przez drugi argument zmienna
;;   jest mniejsza od zmiennej wskazywanej przez pierwszy argument.
;;
;; [LAB 02, ZADANIE 02]
;;   Napisz funkcjê otrzymuj¹c¹ jako argumenty
;;   referencje do dwóch zmiennych typu "int",
;;   która zamienia ze sob¹ wartoœci zmiennych,
;;   do których referencje dostaliœmy w argumentach.
;;
;; [LAB 02, ZADANIE 03]
;;   Napisz funkcjê otrzymuj¹c¹ dwa argumenty:
;;   referencjê `a` oraz wskaŸnik `b` do zmiennych typu "int",
;;   która zamienia ze sob¹ wartoœci zmiennych,
;;   do których wskaŸnik i referencjê dosta³a w argumentach.
;;
;; Poniewa¿ w asemblerze nie ma rozró¿nienia miêdzy wskaŸnikiem a refk¹,
;; wobec tego zadania 2 i 3 zostan¹ poteraktowane jako jeden punkt.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include 'align.inc'
include 'cpu/8086.inc'

format binary as 'com'

org 0x0100

    jmp START


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; FUNKCJE POMOCNICZE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include '../common/out_i16.inc'


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Funkcja opisywana w treœci zadania 1.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; INPUT:
;;  * Arg1 => `a`, referencja na INT16 (liczba "ze znakiem").
;;  * Arg2 => `b`, referencja na ten sam typ.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

FUNKCJA_ZAMIANY_ZE_SPRAWDZENIEM:

    push bp
    mov bp,sp

    push bx

    mov bx,word[bp + 0x04]
    mov dx,bx
    mov ax,word[bx]

    mov bx,word[bp + 0x06]
    mov cx,word[bx]

    cmp cx,ax
      jge .powrot

    ; Argument drugi [BP+6] jest mniejszy od pierwszego [BP+4]

    push bx
    push dx
    call FUNKCJA_ZAMIANY

  .powrot:

    pop bx

    mov sp,bp
    pop bp
    ret 0x04


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Funkcja opisywana w treœci zadania 2.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; INPUT:
;;  * Arg1 => `a`, referencja na INT16.
;;  * Arg2 => `b`, referencja na INT16.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

FUNKCJA_ZAMIANY:

    push bp
    mov bp,sp

    push bx

    mov bx,word[bp + 0x04]
    mov dx,bx
    mov ax,word[bx]

    mov bx,word[bp + 0x06]
    mov cx,word[bx]

    mov word[bx],ax
    mov bx,dx
    mov word[bx],cx

    pop bx

    mov sp,bp
    pop bp
    ret 0x04


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Wyœwietlenie numerów w parze: `(a, b)`
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; INPUT:
;;  * Arg1 => pierwsza liczba, typu INT16.
;;  * Arg2 => druga liczba, tego samego typu.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

WYSWIETL_NUMERY_W_PARZE:

    push bp
    mov bp,sp

    push bx

    mov dl,'('
    mov ah,0x02
    int 0x21

    mov ax,word[bp + 0x04]
    call OUTPUT_INT16

    mov dl,','
    mov ah,0x02
    int 0x21
    mov dl,' '
    int 0x21

    mov ax,word[bp + 0x06]
    call OUTPUT_INT16

    mov dl,')'
    mov ah,0x02
    int 0x21

    pop bx

    mov sp,bp
    pop bp
    ret 0x04


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Wykonanie jednego z zadañ
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; INPUT:
;;  * Arg1 => adres funkcji do wykonania na ka¿dej parze liczb
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

WYKONAJ_ZADANIE:

    push bp
    mov bp,sp
    sub sp,0x04
      ; [bp - 0x02] = 'a'
      ; [bp - 0x04] = 'b'

    mov si,liczby_testowe
    mov di,(((liczby_testowe_koniec shr 1) shl 1) - (0x04 - 0x02))

  .petla:

    cmp si,di
      jae .powrot

    lodsw
    mov word[bp - 0x02],ax
    mov cx,ax
    lodsw
    mov word[bp - 0x04],ax

    push ax
    push cx
    call WYSWIETL_NUMERY_W_PARZE

    mov ax,bp
    sub ax,0x04
    push ax
    add ax,0x02
    push ax
    call word[bp + 0x04]

    mov dx,napis_strzalka
    mov ah,0x09
    int 0x21

    mov ax,word[bp - 0x04]
    push ax
    mov ax,word[bp - 0x02]
    push ax
    call WYSWIETL_NUMERY_W_PARZE

    call WYSWIETL_CRLF

    jmp .petla

  .powrot:

    mov sp,bp
    pop bp
    ret 0x02


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


WYSWIETL_LINIE_ODSTEPU:

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

START:

    ; Informacja o tym, co robi program

    call WYSWIETL_LINIE_ODSTEPU

    mov dx,napis_info
    mov ah,0x09
    int 0x21

    call WYSWIETL_CRLF

    ; Zadanie 1.

    call WYSWIETL_LINIE_ODSTEPU

    mov ax,FUNKCJA_ZAMIANY_ZE_SPRAWDZENIEM
    push ax
    call WYKONAJ_ZADANIE

    ; Zadanie 2.

    call WYSWIETL_LINIE_ODSTEPU

    mov ax,FUNKCJA_ZAMIANY
    push ax
    call WYKONAJ_ZADANIE

    ; Koniec

    mov ax,0x4C00
    int 0x21


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DANE PROGRAMU
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

napis_strzalka:
  db " -> ",'$'

napis_info:
  db "In the first task, the numbers are swapped if 'b' < 'a'.",0x0D,0x0A
  db "In the second task, the numbers are swapped by using references.",'$'

align 0x02
liczby_testowe: dw \
    10, (-2), (-8), (-4), 6, 5, (-7), 3, 9, (-1)
liczby_testowe_koniec:


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

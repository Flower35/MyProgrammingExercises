;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; "task02.asm"
;; kompilator: "flat assembler g"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; [LAB 02, ZADANIE 04]
;;   Napisz funkcjê, która pobiera dwa argumenty i zwraca dwa odrêbne wyniki.
;;   Jednym z wyników powinien byæ iloczyn obu argumentów, a drugim ich suma.
;;   Poniewa¿ funkcja mo¿e bezpoœrednio zwracaæ tylko jedn¹ wartoœæ,
;;   druga powinna byæ zwracana poprzez parametr wskaŸnikowy albo referencjê.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include 'align.inc'
include 'cpu/8086.inc'

format binary as 'com'

org 0x0100

    jmp START


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DEFINICJE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PRZYJAZNA_LICZBA EQU 31234
  ; ("INT16": od -32768 do +32767)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; FUNKCJE POMOCNICZE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include '../common/out_i16.inc'
include '../common/out_i32.inc'


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Funkcja opisywana w treœci zadania 4.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; INPUT:
;;  * Arg1 => pierwszy sk³adnik (INT16).
;;  * Arg2 => drugi sk³adnik (INT16).
;;  * Arg3 => referencja do oddania sumy argumentów (INT32).
;; OUTPUT:
;;  * regs DX:AX => iloczyn argumentów (INT32).
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ILOCZYN_I_SUMA:

    push bp
    mov bp,sp
    sub sp,0x04
      ; [bp - 0x02] = HIWORD sk³adnik
      ; [bp - 0x04] = LOWORD sk³adnik

    push di

    ; Dodawanie argumentów

    xor dx,dx
    mov ax,word[bp + 0x04]
    cwd

    mov word[bp - 0x02],dx
    mov word[bp - 0x04],ax

    xor dx,dx
    mov ax,word[bp + 0x06]
    cwd

    add ax,word[bp - 0x04]
    adc dx,word[bp - 0x02]

    mov di,word[bp + 0x08]

    stosw
    mov ax,dx
    stosw

    ; Mno¿enie argumentów

    xor dx,dx
    mov ax,word[bp + 0x04]
    cwd

    mov cx,word[bp + 0x06]
    imul cx

    pop di

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

    mov bp,sp
    sub sp,0x04
      ; [bp - 0x02] = HIWORD suma | pierwszy sk³adnik
      ; [bp - 0x04] = LOWORD suma | drugi sk³adnik

    mov bx,sp

    ; Informacja o tym, co robi program

    call WYSWIETL_LINIE_ODSTEPU

    mov dx,napis_info
    mov ah,0x09
    int 0x21

    call WYSWIETL_CRLF

    call WYSWIETL_LINIE_ODSTEPU

    ; Mno¿enie i dodawanie kilku par liczb
    ; ([INT16, INT16] => [INT32, INT32])

    mov si,liczby_testowe
    mov di,(((liczby_testowe_koniec shr 1) shl 1) - (0x04 - 0x02))

  .petla:

    cmp si,di
      jae .koniec

    mov dl,'('
    mov ah,0x02
    int 0x21

    lodsw
    mov word[bp - 0x02],ax
    call OUTPUT_INT16

    mov dx,napis_przecinek
    mov ah,0x09
    int 0x21

    lodsw
    mov word[bp - 0x04],ax
    call OUTPUT_INT16

    mov dx,napis_strzalka_z_nawiasami
    mov ah,0x09
    int 0x21

    push bx
    mov ax,word[bp - 0x04]
    push ax
    mov ax,word[bp - 0x02]
    push ax
    call ILOCZYN_I_SUMA

    call OUTPUT_INT32

    mov dx,napis_przecinek
    mov ah,0x09
    int 0x21

    mov ax,word[bp - 0x04]
    mov dx,word[bp - 0x02]
    call OUTPUT_INT32

    mov dl,')'
    mov ah,0x02
    int 0x21

    call WYSWIETL_CRLF

    jmp .petla

  .koniec:

    mov ax,0x4C00
    int 0x21


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DANE PROGRAMU
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

napis_strzalka_z_nawiasami:
  db ") -> (",'$'

napis_przecinek:
  db ", ",'$'

napis_info:
  db "A function in this task returns the product and the sum of two args.",0x0D,0x0A
  db "The product is the value returned from the function with three args.",0x0D,0x0A
  db "The sum is returned through the third argument, which is a pointer.",'$'

align 0x02
liczby_testowe: dw \
    PRZYJAZNA_LICZBA, PRZYJAZNA_LICZBA, \
    PRZYJAZNA_LICZBA, (-PRZYJAZNA_LICZBA), \
    (-PRZYJAZNA_LICZBA), PRZYJAZNA_LICZBA, \
    (-PRZYJAZNA_LICZBA), (-PRZYJAZNA_LICZBA)
liczby_testowe_koniec:


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

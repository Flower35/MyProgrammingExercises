
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Common function for single-segment executables.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DZIA�ANIE:
;;  * Odczytanie 16-bitowej liczby ca�kowitej z podanego ci�gu znak�w.
;;   Funkcja przerywa odczytywanie kiedy natrafi na inny znak ni�
;;   bia�e znaki (tabulatory, spacje), inny ni� cyfry, lub na
;;   powt�rzone znaki specjalne ('+', '-').
;; INPUT:
;;  * reg DX => wka�nik na pocz�tek ci�gu znak�w.
;; RETURNS:
;;  * reg AX => odczytana liczba ca�kowita.
;;  * reg DX => wska�nik za ostatni prawid�owy znak.
;;  * CARRY BIT => ustawiony w przypadku wyst�pienia b��du.
;; KONWENCJA:
;;  * niszczy: CX.
;;  * zachowuje: BX, SI.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ATOI_INT16:

    push bx
    push si

    mov bx,0x0A
    mov si,dx

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    xor ax,ax
    xor cl,cl
      ; CL = mini-stan sprawdzania
      ; (0x01 = wynik poprawny)
      ; (0x02 = sprawdzaj tylko cyfry)
      ; (0x04 = zaneguj wynik)

  .petla_znakow:

    mov dx,ax

    lodsb
    cmp al,'0'
      jb .nie_jest_cyfra
    cmp al,'9'
      ja .nie_jest_cyfra

    and al,0x0F
    mov ch,al

    shl dx,0x01
    mov ax,dx
    shl ax,0x01
    shl ax,0x01
    add ax,dx
      ; szybkie mno�enie przez dziesi��

    add al,ch
    adc ah,0x00

    or cl,(0x01 or 0x02)
    jmp .petla_znakow

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  .nie_jest_cyfra:

    test cl,0x02
      jnz .zakoncz_odczytywanie

    cmp al,' '
      je .back_to_petla_znakow
    cmp al,0x09
      je .back_to_petla_znakow

    cmp al,'+'
      je .od_znaku_specjalnego

    cmp al,'-'
      jne .zakoncz_odczytywanie
    or cl,0x04

  .od_znaku_specjalnego:

    or cl,0x02

  .back_to_petla_znakow:

    mov ax,dx
    jmp .petla_znakow

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  .zakoncz_odczytywanie:

    mov ax,dx

    test cl,0x04
      jz .powracanie

    neg ax

  .powracanie:

    mov dx,si
    dec dx

    pop si
    pop bx

    test cl,0x01
      jnz .bezblednie

    stc
    ret

  .bezblednie:

    clc
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

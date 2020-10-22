
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Common function for single-segment executables.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DZIA£ANIE:
;;  * Odczytanie 32-bitowej liczby sta³oprzecinkowej w formacie:
;;     [1 bit znaku, 24 bity liczby, 7 bitów u³amkowych].
;;   Funkcja przerywa odczytywanie, kiedy natrafi na inny znak ni¿
;;   bia³e znaki (tabulatory, spacje), inny ni¿ cyfry, lub na
;;   powtórzone znaki specjalne ('+', '-', '.').
;; INPUT:
;;  * reg DX => wkaŸnik na pocz¹tek ci¹gu znaków.
;; RETURNS:
;;  * reg DX:AX => odczytana liczba sta³oprzecinkowa.
;;  * CARRY BIT => ustawiony w przypadku wyst¹pienia b³êdu.
;; KONWENCJA:
;;  * niszczy: CX.
;;  * zachowuje: BX, SI.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ATOI_Q7:

    push bp
    mov bp,sp

    xor ax,ax
    push ax ; [bp - 0x02] = HIWORD wynik
    push ax ; [bp - 0x04] = LOWORD wynik
    push ax ; [bp - 0x06] = czêœæ u³amkowa
    inc ax
    push ax ; [bp - 0x08] = test do czeœci u³amkowej

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    push bx
    xor bx,bx

    push si
    mov si,dx

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    xor cl,cl
      ; CL = mini-stan sprawdzania
      ; (0x01 = wynik poprawny)
      ; (0x02 = sprawdzaj tylko cyfry, jeden plus lub jeden minus)
      ; (0x04 = cyfry po przecinku, jedna kropka)
      ; (0x08 = zaneguj wynik)

  .petla_znakow:

    lodsb
    cmp al,'0'
      jb .nie_jest_cyfra
    cmp al,'9'
      ja .nie_jest_cyfra

    and al,0x0F

    test cl,0x04
      jz .odczytywanie_glownej_liczby

    jmp .odczytywanie_liczby_po_kropce

  .nie_jest_cyfra:

    jmp .nie_jest_cyfra__out_of_range

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  .odczytywanie_liczby_po_kropce:

    mov bx,word[bp - 0x08]
    cmp bx,0x03E8 ; "10^3"
      ja .petla_znakow

    xor bx,bx
    mov bl,al

    mov ax,word[bp - 0x06]

    shl ax,0x01
    mov dx,ax
    shl ax,0x01
    shl ax,0x01
    add ax,dx
      ; szybkie mno¿enie przez dziesiêæ

    add ax,bx
    mov word[bp - 0x06],ax

    mov ax,word[bp - 0x08]

    shl ax,0x01
    mov dx,ax
    shl ax,0x01
    shl ax,0x01
    add ax,dx
      ; szybkie mno¿enie przez dziesiêæ

    mov word[bp - 0x08],ax

    or cl,0x01
      ; przypadek, gdyby wpisano liczbê ".*" (bez cyfr przed kropk¹)
    jmp .petla_znakow

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  .odczytywanie_glownej_liczby:

    mov bh,al
    mov bl,cl

    mov dx,word[bp - 0x02]
    mov ax,word[bp - 0x04]
    shl ax,0x01
    rcl dx,0x01
      ; rozpoczêcie szybckiego mno¿enia przez dziesiêæ (najpierw x2)

    test dx,(0x8000 shr 7)
      jz .brak_przepelnienia

    mov cl,bl

  .nastapilo_przepelnienie:

    and cl,(not 0x01)
    jmp .powracanie

  .brak_przepelnienia:

    mov word[bp - 0x04],ax
    mov word[bp - 0x02],dx
    shl ax,0x01
    rcl dx,0x01
    shl ax,0x01
    rcl dx,0x01
    add ax,word[bp - 0x04]
    adc dx,word[bp - 0x02]
    mov word[bp - 0x02],dx
    mov word[bp - 0x04],ax
      ; dokoñczenie szybkiego mno¿enia przez dziesiêæ

    mov cl,bl
    add al,bh
    adc ah,0x00
    adc dx,0x0000

    test dx,(0x8000 shr 7)
      jnz .nastapilo_przepelnienie

    mov word[bp - 0x02],dx
    mov word[bp - 0x04],ax

    or cl,(0x01 or 0x02)
    jmp .petla_znakow

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  .nie_jest_cyfra__out_of_range:

    test cl,0x04
      jnz .zakoncz_odczytywanie

    cmp al,'.'
      je .znaleziono_kropke

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

    or cl,0x08

  .od_znaku_specjalnego:

    or cl,0x02
    jmp .petla_znakow

  .znaleziono_kropke:

    or cl,0x04

  .back_to_petla_znakow:

    jmp .petla_znakow

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  .zakoncz_odczytywanie:

    xor bx,bx
    mov ch,(0x01 shl (7 - 1))

    mov dx,word[bp - 0x08]
    mov ax,word[bp - 0x06]

  .petla_konwertowania_ulamkowego:

    test ax,ax
      jz .przeniesienie_ostatecznego_wyniku

    shl ax,0x01
    cmp ax,dx
      jb .koniec_petli_konwertowania_ulamkowego

    or bl,ch
    sub ax,dx

  .koniec_petli_konwertowania_ulamkowego:

    shr ch,0x01
      jnz .petla_konwertowania_ulamkowego

  .przeniesienie_ostatecznego_wyniku:

    mov dx,word[bp - 0x02]
    mov ax,word[bp - 0x04]
    mov ch,0x07

  .petla_przesuwania_glownej_liczby:

    shl ax,0x01
    rcl dx,0x01

    dec ch
      jnz .petla_przesuwania_glownej_liczby

    or al,bl
      ; dodanie bitów u³akowych do ostatecznej formy

    test cl,0x08
      jz .powracanie

    or dx,0x8000

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  .powracanie:

    pop si
    pop bx

    test cl,0x01
      jnz .bezblednie

    stc
    jmp .koniec

  .bezblednie:

    clc

  .koniec:

    mov sp,bp
    pop bp
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

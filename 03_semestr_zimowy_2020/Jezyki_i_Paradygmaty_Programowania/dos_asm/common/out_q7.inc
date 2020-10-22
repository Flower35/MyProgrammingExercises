
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Functions required for this module
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include 'out_u32.inc'


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Common function for single-segment executables.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DZIA£ANIE:
;;  * Wyœwietlenie 32-bitowej liczby sta³oprzecinkowej z formatu:
;;     [1 bit znaku, 24 bity liczby, 7 bitów u³amkowych]
;;   na format dziesiêtny: "*.??".
;; INPUT:
;;  * regs DX:AX => liczba do wyœwietlenia na ekranie.
;; KONWENCJA:
;;  * niszczy: AX, CX, DX.
;;  * zachowuje: BX.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

OUTPUT_Q7:

    push bp
    mov bp,sp

    xor cx,cx
    push cx ; [bp - 0x02] = HIWORD wartoœæ po przecinku
    push cx ; [bp - 0x04] = LOWORD wartoœæ po przecinku
    push cx ; [bp - 0x06] = HIWORD test
    inc cx
    push cx ; [bp - 0x08] = LOWORD test
      ; test po ka¿dej iteracji: [(x * 10 / 2) = (x * 5)]
      ; na pocz¹tku: wynik = 0, test = 1.

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    push bx
    mov bl,al

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    test dx,0x8000 ; "1000 0000 0000 0000 b"
      jz .glowna_liczba_nie_jest_ujemna

    not dx
    not ax
      ; negating HIWORD ("DX") takes care of the pierwszy bit

    add ax,0x80 ; "1000 0000 b"
    adc dx,0x0000

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  .glowna_liczba_nie_jest_ujemna:

    mov cl,0x07

  .petla_wydobycia_glownej_liczby:

    shr dx,0x01
    rcr ax,0x01

    dec cl
      jnz .petla_wydobycia_glownej_liczby

    call OUTPUT_UINT32

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    mov dl,'.'
    mov ah,0x02
    int 0x21

    mov bh,(0x01 shl (7 - 1))

  .petla_konwertowania_ulamkowego:

    mov dx,word[bp - 0x06]
    mov ax,word[bp - 0x08]
    shl ax,0x01
    rcl dx,0x01
    shl ax,0x01
    rcl dx,0x01
    add ax,word[bp - 0x08]
    adc dx,word[bp - 0x06]
    mov word[bp - 0x06],dx
    mov word[bp - 0x08],ax
      ; (test = test x 5)

    mov dx,word[bp - 0x02]
    mov ax,word[bp - 0x04]
    shl ax,0x01
    rcl dx,0x01
    mov word[bp - 0x02],dx
    mov word[bp - 0x04],ax
    shl ax,0x01
    rcl dx,0x01
    shl ax,0x01
    rcl dx,0x01
    add ax,word[bp - 0x04]
    adc dx,word[bp - 0x02]
      ; (wynik = wynik x 10)

    test bl,bh
      jz .koniec_petli_konwertowania_ulamkowego

    add ax,word[bp - 0x08]
    adc dx,word[bp - 0x06]
      ; je¿eli bit na i-tej pozycji [0...7] jest ustawiony,
      ; to (wynik += test)

  .koniec_petli_konwertowania_ulamkowego:

    mov word[bp - 0x02],dx
    mov word[bp - 0x04],ax

    shr bh,0x01
      jnz .petla_konwertowania_ulamkowego

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    call OUTPUT_UINT32

    pop bx

    mov sp,bp
    pop bp
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

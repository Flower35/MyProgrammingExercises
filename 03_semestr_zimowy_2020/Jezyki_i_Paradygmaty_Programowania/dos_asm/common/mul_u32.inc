
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Common function for single-segment executables.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DZIA£ANIE:
;;  * Mno¿enie dwóch liczb 32-bitowych ("BEZ ZNAKU").
;; INPUT:
;;  * regs DX:AX => pierwszy sk³adnik 32-bitowa.
;;  * Arg1 => LOWORD drugiego sk³adnika 32-bitowego.
;;  * Arg2 => HIWORD drugiego sk³adnika 32-bitowego.
;; RETURNS:
;;  * regs DX:AX => wynik mno¿enia w super-rejestrze.
;; KONWENCJA:
;;  * niszczy: CX.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

MUL_UINT32:

    push bp
    mov bp,sp

    xor cx,cx
    push cx
    push cx
      ; [bp - 0x02] = HIWORD wyniku
      ; [bp - 0x04] = LOWORD wyniku

  .petla_dodawania:

    mov cx,word[bp + 0x06] ; HIWORD drugiego sk³adnika
    test cx,cx
      jnz .dodaj_kolejny_raz

    mov cx,word[bp + 0x04] ; LOWORD drugiego sk³adnika
    test cx,cx
      jz .koniec_dodawania

  .dodaj_kolejny_raz:

    add word[bp - 0x04],ax
    adc word[bp - 0x02],dx

    sub word[bp + 0x04],0x01
    sbb word[bp + 0x06],0x0000

    jmp .petla_dodawania

  .koniec_dodawania:

    mov ax,word[bp - 0x04]
    mov dx,word[bp - 0x02]

    mov sp,bp
    pop bp
    ret 0x04


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

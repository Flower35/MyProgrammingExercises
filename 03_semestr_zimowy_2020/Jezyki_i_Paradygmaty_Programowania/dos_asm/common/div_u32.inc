
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Common function for single-segment executables.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DZIA£ANIE:
;;  * Dzielenie dwóch liczb 32-bitowych ("BEZ ZNAKU").
;; INPUT:
;;  * reg DX:AX => dzielna 32-bitowa.
;;  * Arg1 => LOWORD dzielnika 32-bitowego.
;;  * Arg2 => HIWORD dzielnika 32-bitowego.
;; RETURNS:
;;  * reg DX:AX => wynik dzielenia w super-rejestrze.
;;  * reg CX => reszta z dzielenia.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

DIV_UINT32:

    push bp
    mov bp,sp

    mov cx,word[bp + 0x06] ; HIWORD dzielnika
    test cx,cx
      jnz .nie_dziele_przez_zero

    mov cx,word[bp + 0x04] ; LOWORD dzielnika
    test cx,cx
      jnz .nie_dziele_przez_zero

    xor cx,cx
    xor dx,dx
    xor ax,ax
    ret

  .nie_dziele_przez_zero:

    xor cx,cx
    push cx
    push cx
      ; [bp - 0x02] = HIWORD wyniku
      ; [bp - 0x04] = LOWORD wyniku

  .petla_odejmowania:

    sub ax,word[bp + 0x04]
    sbb dx,word[bp + 0x06]
      jc .koniec_odejmowania

    add word[bp - 0x04],0x01
    adc word[bp - 0x02],0x0000

    test dx,dx
      jnz .petla_odejmowania
    test ax,ax
      jnz .petla_odejmowania
    jmp .brak_reszty

  .koniec_odejmowania:

    mov cx,ax
    add cx,word[bp + 0x04]

  .brak_reszty:

    mov ax,word[bp - 0x04]
    mov dx,word[bp - 0x02]

    mov sp,bp
    pop bp
    ret 0x04


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

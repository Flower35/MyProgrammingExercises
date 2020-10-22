
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Common function for single-segment executables.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DZIA£ANIE:
;;  * Wyœwietlenie 16-bitowej liczby ca³kowitej w formacie dziesiêtnym.
;; INPUT:
;;  * reg AX => UINT16 (liczba "bez znaku").
;; KONWENCJA:
;;  * niszczy: AX, CX, DX.
;;  * zachowuje: BX, DI.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

OUTPUT_UINT16:

    push bp
    mov bp,sp

    sub sp,0x06
      ; (5 znaków + symbol zakoñczenia tekstu)

    push bx
    push di

    std ; cyfry zrzucane od prawej do lewej

    mov bx,ax

    mov di,bp
    mov al,'$'
    dec di
    stosb

    test bx,bx
      jnz .rozpocznij_dzielenie

    xor al,0x14
    stosb
    jmp .wypisz_i_wroc

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  .rozpocznij_dzielenie:

    mov ax,bx
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

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  .wypisz_i_wroc:

    cld

    mov dx,di
    inc dx
    mov ah,0x09
    int 0x21

    pop di
    pop bx

    mov sp,bp
    pop bp
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

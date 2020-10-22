
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Common function for single-segment executables.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DZIA£ANIE:
;;  * wyœwietlenie 32-bitowej liczby ca³kowitej w formacie dziesiêtnym.
;; INPUT:
;;  * regs DX:AX => UINT16 (liczba "bez znaku").
;; KONWENCJA:
;;  * niszczy: AX, CX, DX.
;;  * zachowuje: BX, DI.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

OUTPUT_UINT32:

    push bp
    mov bp,sp

    sub sp,0x10
      ; [bp - 0x02] = "new HIWORD"
      ; [bp - 0x04] = "old LOWORD"
      ; [bp - 0x10] = forma tekstowa (10 znaków + symbol zakoñczenia)
      ;   (zakr¹glamy do 12 ¿eby wyrównaæ 16-bitowy stos)

    push bx
    push di

    std ; cyfry zrzucane od prawej do lewej

    mov bx,ax

    mov di,bp
    mov al,'$'
    sub di,0x05 ; przeskoczenie dwóch lokalnych zmiennych
    stosb

    test bx,bx
      jnz .rozpocznij_dzielenie
    test dx,dx
      jnz .rozpocznij_dzielenie

    mov al,'0'
    stosb
    jmp .wypisz_i_wroc

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  .rozpocznij_dzielenie:

    mov ax,bx
    mov bx,0x0A
    mov word[bp - 0x04],ax

  .petla_dzielenia:

    mov ax,dx
    xor dx,dx
    div bx
      ; [0 x 16 + "old HIWORD"] div 10

    mov word[bp - 0x02],ax
      ; skopiuj "new HIWORD"

    mov ax,word[bp - 0x04]
    div bx
      ; [reszta x 16 + "old LOWORD"] div 10

    mov word[bp - 0x04],ax
      ; skopiuj "new LOWORD"

    mov al,dl
    or al,'0'
    stosb

    mov ax,word[bp - 0x04]
    mov dx,word[bp - 0x02]

    test ax,ax
      jnz .petla_dzielenia
    test dx,dx
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

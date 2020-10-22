
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Common function for single-segment executables.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DZIA£ANIE:
;;  * wyliczenie pierwiastka kwadratowego liczby naturalnej.
;; INPUT:
;;  * reg AX => UINT16 (liczba "bez znaku").
;; RETURNS:
;;  * reg AX => oszacowany pierwiastek (obciête miejsca po przecinku)
;; KONWENCJA:
;;  * zachowuje: CX, DX.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SQRT_UINT16:

    push cx
    push dx

    xor cx,cx
    or dx,(-1)

    test ax,ax
      jle .nieprawidlowy_argument

  .petla_odejmowania:

    add dx,0x02
    inc cx

    sub ax,dx
      jg .petla_odejmowania
      jz .przenies_wynik

    dec cx
      ; przepe³nienie po ostatnim odejmowaniu

  .przenies_wynik:

    mov ax,cx
    jmp .powrot

  .nieprawidlowy_argument:

    xor ax,ax

  .powrot:

    pop dx
    pop cx

    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

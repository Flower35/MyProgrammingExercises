
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Common function for single-segment executables.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DZIA£ANIE:
;;  * Wyœwietlenie 8-bitowej liczby w formacie heksadecymalnym.
;; INPUT:
;;  * reg AL => liczba do wyœwietlenia na ekranie konsoli.
;; KONWENCJA:
;;  * niszczy: AX, CX, DX.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

OUTPUT_HEX8:

    mov cl,al
    shr al,0x04

    mov ch,0x02

  .petla_nibble:

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * "Allison's Algorithm 2"

    cmp al,0x0A
    sbb al,0x69
    das

    mov dl,al
    mov ah,0x02
    int 0x21

    dec ch
      jz .powrot

    mov al,cl
    and al,0x0F
    jmp .petla_nibble

  .powrot:

    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Functions required for this module
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include 'out_u16.inc'


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Common function for single-segment executables.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DZIA£ANIE:
;;  * Wyœwietlenie 16-bitowej liczby ca³kowitej w formacie dziesiêtnym.
;; INPUT:
;;  * reg AX => INT16 (liczba "ze znakiem").
;; KONWENCJA:
;;  * patrz: "OUTPUT_UINT16"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

OUTPUT_INT16:

    test ax,ax
      jns .nieujemna

    mov cx,ax

    mov dl,'-'
    mov ah,0x02
    int 0x21

    mov ax,cx

  .nieujemna:

    call OUTPUT_UINT16
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

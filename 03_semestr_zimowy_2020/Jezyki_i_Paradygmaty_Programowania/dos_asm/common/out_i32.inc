
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Functions required for this module
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include 'out_u32.inc'


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Common function for single-segment executables.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DZIA£ANIE:
;;  * Wyœwietlenie 32-bitowej liczby ca³kowitej w formacie dziesiêtnym.
;; INPUT:
;;  * regs DX:AX => INT32 (liczba "ze znakiem").
;; KONWENCJA:
;;  * patrz: "OUTPUT_UINT32"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

OUTPUT_INT32:

    test dx,dx
      jns .nieujemna

    push bx
    mov bx,dx
    mov cx,ax

    mov dl,'-'
    mov ah,0x02
    int 0x21

    mov ax,cx
    mov dx,bx
    pop bx

    not dx
    not ax

    add ax,0x0001
    adc dx,0x0000

  .nieujemna:

    call OUTPUT_UINT32
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

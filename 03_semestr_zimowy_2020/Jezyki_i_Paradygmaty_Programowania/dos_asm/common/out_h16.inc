
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Functions required for this module
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include 'out_h8.inc'


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Common function for single-segment executables.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DZIAŁANIE:
;;  * Wyświetlenie 16-bitowej liczby w formacie heksadecymalnym.
;; INPUT:
;;  * reg AX => liczba do wyświetlenia na ekranie konsoli.
;; KONWENCJA:
;;  * patrz: OUTPUT_HEX8.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

OUTPUT_HEX16:

    push ax

    mov al,ah
    call OUTPUT_HEX8

    pop ax
    call OUTPUT_HEX8

    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

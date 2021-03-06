
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Common function for single-segment executables.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DZIA�ANIE:
;;  * Por�wnywanie baj�w jednego bloku danych z drugim blokiem.
;; INPUT:
;;  * Arg1 => pocz�tek pierwszego bloku.
;;  * Arg2 => pocz�tek drugiego bloku.
;;  * Arg3 => rozmiar blok�w (liczba bajt�w do por�wnania).
;; RETURNS:
;;  * CARRY BIT => ustawiony w przypadku znalezienia r�nicy.
;;   (je�li `Arg3 <= 0`, to funkcja funkcja czy�ci CARRY BIT)
;; KONWENCJA:
;;  * niszczy: CX.
;;  * zachowuje: SI, DI.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

MEMCMP:

    push bp
    mov bp,sp

    push si
    mov si,word[bp + 0x06]
    push di
    mov di,word[bp + 0x04]

    mov cx,word[bp + 0x08]
    and cx,(not 0x0001)
      jz .ostatni_bajt
      js .nie_znaleziono_roznicy

    shr cx,0x01
    repe cmpsw
      jne .bloki_roznia_sie

  .ostatni_bajt:

    mov cx,word[bp + 0x08]
    shr cx,0x01
      jnc .nie_znaleziono_roznicy

    cmpsb
      je .nie_znaleziono_roznicy

  .bloki_roznia_sie:

    stc
    jmp .powrot

  .nie_znaleziono_roznicy:

    clc

  .powrot:

    mov sp,bp
    pop bp
    ret 0x06


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

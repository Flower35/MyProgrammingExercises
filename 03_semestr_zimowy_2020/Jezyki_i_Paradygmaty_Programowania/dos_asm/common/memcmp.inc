
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Common function for single-segment executables.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DZIA£ANIE:
;;  * Porównywanie bajów jednego bloku danych z drugim blokiem.
;; INPUT:
;;  * Arg1 => pocz¹tek pierwszego bloku.
;;  * Arg2 => pocz¹tek drugiego bloku.
;;  * Arg3 => rozmiar bloków (liczba bajtów do porównania).
;; RETURNS:
;;  * CARRY BIT => ustawiony w przypadku znalezienia ró¿nicy.
;;   (jeœli `Arg3 <= 0`, to funkcja funkcja czyœci CARRY BIT)
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

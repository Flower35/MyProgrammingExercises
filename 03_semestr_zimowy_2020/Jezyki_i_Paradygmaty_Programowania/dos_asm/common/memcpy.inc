
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Common function for single-segment executables.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DZIA£ANIE:
;;  * Kopiowanie bajtów z jednego bloku danych do drugiego bloku.
;; INPUT:
;;  * Arg1 => blok docelowy.
;;  * Arg2 => blok Ÿród³owy.
;;  * Arg3 => rozmiar bloków (liczba bajtów do skopiowania).
;; KONWENCJA:
;;  * niszczy: CX.
;;  * zachowuje: SI, DI.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

MEMCPY:

    push bp
    mov bp,sp

    push si
    mov si,word[bp + 0x06]
    push di
    mov di,word[bp + 0x04]

    mov cx,word[bp + 0x08]
    and cx,(not 0x0001)
      jz .ostatni_bajt
      js .powrot

    shr cx,0x01
    rep movsw

  .ostatni_bajt:

    mov cx,word[bp + 0x08]
    shr cx,0x01
      jnc .powrot

    movsb

  .powrot:

    mov sp,bp
    pop bp
    ret 0x06


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

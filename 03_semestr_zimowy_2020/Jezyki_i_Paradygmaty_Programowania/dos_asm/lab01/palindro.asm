;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; "palindro.asm"
;; kompilator: "flat assembler g"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; [LAB 01, ZADANIE 02]
;;   Napisz program, kt�ry okre�li czy wczytany ci�g
;;   (argument programu) jest palindromem tekstowym.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include 'cpu/8086.inc'

format binary as 'com'

org 0x0100

    jmp ROZPOCZNIJ


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DEFINICJE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ROZMIAR_CALY_TESKT EQU 127
ROZMIAR_POLOWA_TEKSTU EQU 63
    ; Na argumenty polecenia jest przeznaczone 127 bajt�w,
    ; z czego zwykle pierwszy bajt zajmuje Spacja (0x02),
    ; a ostatni to zawsze Carriage Return (0x0D)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; POCZ�TEK PROGRAMU
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ROZPOCZNIJ:

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; [1.1] Przygotowanie wska�nik�w na pocz�tek i na koniec listy argument�w

    xor ax,ax

    mov al,byte[0x80]
    mov si,0x81

    mov bx,si
    add bx,ax

    mov di,tablica_A

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; [1.2] Kopiowanie argumentu z pomini�ciem znak�w specjalnych

  .petla_kopiowanie_argumentu:

    cmp si,bx
      jae .kopiuj_polowe_tekstu

    lodsb

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; [1.2.1] Sprawd� cyfry (0x30 do 0x39)

    cmp al,'9'
      ja .petla_kopiowania_grupa_02

    cmp al,'0'
      jae .petla_kopiowania_litera_lub_cyfra

    jmp .petla_kopiowanie_argumentu

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; [1.2.2] Sprawd� du�e litery (0x41 do 0x5A)

  .petla_kopiowania_grupa_02:

    cmp al,'Z'
      ja .petla_kopiowania_grupa_03

    cmp al,'A'
      jae .petla_kopiowania_litera_lub_cyfra

    jmp .petla_kopiowanie_argumentu

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; [1.2.3] Sprawd� ma�e litery (0x41 do 0x5A)

  .petla_kopiowania_grupa_03:

    cmp al,'z'
      ja .petla_kopiowania_grupa_04

    cmp al,'a'
      jae .petla_kopiowania_mala_litera

    jmp .petla_kopiowanie_argumentu

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; [1.2.4] Pomi� pozosta�e znaki ASCII

  .petla_kopiowania_grupa_04:

    cmp al,0x80
      jbe .petla_kopiowanie_argumentu

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; [1.2.5] Inny pasuj�cy znak (np. diakrytyczne)

  .petla_kopiowania_litera_lub_cyfra:

    stosb

    jmp .petla_kopiowanie_argumentu

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; [1.3] Zamiana ma�ych liter alfabetu �aci�skiego na litery du�e

  .petla_kopiowania_mala_litera:

    sub al,('a' - 'A')
    jmp .petla_kopiowania_litera_lub_cyfra

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; [2.1] Dzielenie tekstu na po�ow� i skopiowanie prawej po�owy
    ;; z odwr�ceniem kolejno�ci znak�w

  .kopiuj_polowe_tekstu:

    mov cx,di
    sub cx,tablica_A
      jz .wynik_falszywy

    shr cx,0x01
    mov dx,cx
      ; zapami�taj rozmiar do nast�puj�cego por�wnywania palindromu

    mov si,di
    dec si
    mov di,tablica_B

  .petla_kopiowania_polowy:

    std
    lodsb

    cld
    stosb

    dec cx
      jnz .petla_kopiowania_polowy

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; [2.2] G��wne sprawdzenie palindromu

    mov cx,dx
    mov si,tablica_A
    mov di,tablica_B

    repe cmpsb
        jne .wynik_falszywy

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; [3] Og�oszenie wyniku

    mov dx,napis_YES
    jmp .ogloszenie_wyniku

  .wynik_falszywy:

    mov dx,napis_NO

  .ogloszenie_wyniku:

    mov ah,0x09
    int 0x21

  .koniec_programu:

    mov ax,0x4C00
    int 0x21


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DANE PROGRAMU
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

napis_YES: db "Yes.",0x0D,0x0A,'$'
napis_NO: db "No.",0x0D,0x0A,'$'

tablica_B: rb ROZMIAR_POLOWA_TEKSTU
tablica_A: rb ROZMIAR_CALY_TESKT


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

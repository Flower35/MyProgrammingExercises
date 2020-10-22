;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; "bmicalc.asm"
;; kompilator: "flat assembler g"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; [LAB 01, ZADANIE 03]
;;   Napisz kalkulator wyliczajacy wskaznik BMI (Body Mass Index).
;;
;; Ten program po prosi u¿ytkownika o podanie dwóch liczb.
;; Liczby w programie bêd¹ traktowane jako liczby sta³oprzecinkowe
;; z dwoma miejscami po przecinku (7 bitów w formacie binarnym).
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include 'cpu/8086.inc'

format binary as 'com'

org 0x0100

    jmp ROZPOCZNIJ


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; FUNKCJE POMOCNICZE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include '../common/mul_u32.inc'
include '../common/div_u32.inc'

include '../common/atoi_q7.inc'
include '../common/out_q7.inc'


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; POZOSTA£E FUNKCJE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

POBIERZ_TEKST_ORAZ_ODCZYTAJ_LICZBE:

    mov byte[bufor_header + 0x01],0x00

    mov dx,bufor_header
    mov ah,0x0A
    int 0x21

    call WYSWIETL_CRLF

    mov dx,bufor
    call ATOI_Q7
    ret


WYPISZ_TEKST_Z_DX:

    mov ah,0x09
    int 0x21
    ret


WYSWIETL_CRLF:

    mov ah,0x02
    mov dl,0x0D
    int 0x21
    mov dl,0x0A
    int 0x21

    ret


WYSWIETL_PAUZE:

    mov dl,'-'
    mov ah,0x02
    mov cl,0x40

  .petla:

    int 0x21

    dec cl
      jnz .petla

    call WYSWIETL_CRLF

    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; POCZ¥TEK PROGRAMU
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ROZPOCZNIJ:

    mov bp,sp
    sub sp,0x08
      ; [bp - 0x02] = HIWORD dane u¿ytkownika: "wzrost"
      ; [bp - 0x04] = LOWORD
      ; [bp - 0x06] = HIWORD dane u¿ytkownika: "masa"
      ; [bp - 0x08] = LOWORD

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Powitanie u¿ytkownika

    call WYSWIETL_PAUZE

    mov dx,napis_powitanie
    call WYPISZ_TEKST_Z_DX

    call WYSWIETL_CRLF

    call WYSWIETL_PAUZE

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Proœba o wprowadzenie danych

    call WYSWIETL_CRLF

    mov dx,napis_wprowadz_wzrost
    call WYPISZ_TEKST_Z_DX

    call POBIERZ_TEKST_ORAZ_ODCZYTAJ_LICZBE
      jnc .sprawdzanie_poprawnosci_wzrostu

  .nieprawidlowy_wzrost:

    jmp .niepowodzenie__out_of_range

  .sprawdzanie_poprawnosci_wzrostu:

      ; WZROST: od [+0.1953125] do [+3.00]

    test dx,dx
      jnz .nieprawidlowy_wzrost
    cmp ax,0x0019
      jb .nieprawidlowy_wzrost
    cmp ax,0x0180
      ja .nieprawidlowy_wzrost

    mov word[bp - 0x02],dx
    mov word[bp - 0x04],ax

    call WYSWIETL_CRLF

    mov dx,napis_wprowadz_wage
    call WYPISZ_TEKST_Z_DX

    call POBIERZ_TEKST_ORAZ_ODCZYTAJ_LICZBE
     jnc .sprawdzanie_poprawnosci_masy

  .nieprawidlowa_masa:

    jmp .niepowodzenie__out_of_range

  .sprawdzanie_poprawnosci_masy:

      ; MASA: od [+2.00] do [+250.00]

    test dx,dx
      jnz .nieprawidlowa_masa
    cmp ax,0x0100
      jb .nieprawidlowa_masa
    cmp ax,0x7D00
      ja .nieprawidlowa_masa

    mov word[bp - 0x06],dx
    mov word[bp - 0x08],ax

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Wyœwietlenie wprowadzonych danych

    call WYSWIETL_CRLF

    call WYSWIETL_PAUZE

    mov dx,napis_potwierdzenie_A
    call WYPISZ_TEKST_Z_DX

    mov dx,word[bp - 0x02]
    mov ax,word[bp - 0x04]
    call OUTPUT_Q7

    mov dx,napis_potwierdzenie_B
    call WYPISZ_TEKST_Z_DX

    mov dx,word[bp - 0x06]
    mov ax,word[bp - 0x08]
    call OUTPUT_Q7

    mov dx,napis_potwierdzenie_C
    call WYPISZ_TEKST_Z_DX

    call WYSWIETL_CRLF

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Mno¿enie i dzielenie
    ;; * BMI = [(masa) / (wzrost x wzrost)]

    mov dx,word[bp - 0x02]
    mov ax,word[bp - 0x04]
    push dx
    push ax
    call MUL_UINT32
      ; [dane_B x 2^(7)] -> [dane_B x 2^(14)]

    mov word[bp - 0x02],dx
    mov word[bp - 0x04],ax

    mov ax,word[bp - 0x08]
    mov dx,word[bp - 0x06]
    mov cl,0x0E
      ; czternaœcie przesuniêæ

  .zwiekszenie_precyzji_masy:

    shl ax,0x01
    rcl dx,0x01

    dec cl
      jnz .zwiekszenie_precyzji_masy

    mov cx,word[bp - 0x02]
    push cx
    mov cx,word[bp - 0x04]
    push cx
    call DIV_UINT32
      ; wynik = [dane_A x 2^(21)] / [dane_B x 2^(14)]

    mov word[bp - 0x04],ax
    mov word[bp - 0x02],dx

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Wypisanie odpowiedzi

    call WYSWIETL_CRLF

    call WYSWIETL_PAUZE

    mov dx,napis_wynik_A
    call WYPISZ_TEKST_Z_DX

    mov ax,word[bp - 0x04]
    mov dx,word[bp - 0x02]
    call OUTPUT_Q7

    call WYSWIETL_CRLF

    mov dx,napis_wynik_B
    call WYPISZ_TEKST_Z_DX

      ; C: (<...> do 16.00) -> poni¿ej 0x0800
      ; D: [16.00 do 17.00) -> poni¿ej 0x0880
      ; E: [17.00 do 18.50) -> poni¿ej 0x0940
      ; F: [18.50 do 25.00) -> poni¿ej 0x0C80
      ; G: [25.00 do 30.00) -> poni¿ej 0x0F00
      ; H: [30.00 do 35.00) -> poni¿ej 0x1180
      ; I: [35.00 do 40.00) -> poni¿ej 0x1400
      ; J: [40.00 do <...>)

    mov ax,word[bp - 0x02]
    test ax,ax
      jnz .rezultat_J

    mov ax,word[bp - 0x04]
    cmp ax,0x0800
      jb .rezultat_C
    cmp ax,0x0880
      jb .rezultat_D
    cmp ax,0x0940
      jb .rezultat_E
    cmp ax,0x0C80
      jb .rezultat_F
    cmp ax,0x0F00
      jb .rezultat_G
    cmp ax,0x1180
      jb .rezultat_H
    cmp ax,0x1400
      jb .rezultat_I
  .rezultat_J:
    mov dx,napis_wynik_J
    jmp .ostatni_tekst

  .rezultat_I:
    mov dx,napis_wynik_I
    jmp .ostatni_tekst

  .rezultat_H:
    mov dx,napis_wynik_H
    jmp .ostatni_tekst

  .rezultat_G:
    mov dx,napis_wynik_G
    jmp .ostatni_tekst

  .rezultat_F:
    mov dx,napis_wynik_F
    jmp .ostatni_tekst

  .rezultat_E:
    mov dx,napis_wynik_E
    jmp .ostatni_tekst

  .rezultat_D:
    mov dx,napis_wynik_D
    jmp .ostatni_tekst

  .rezultat_C:
    mov dx,napis_wynik_C

  .ostatni_tekst:
    call WYPISZ_TEKST_Z_DX

    call WYSWIETL_CRLF

    jmp .koniec

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Zakoñczenie programu

  .niepowodzenie__out_of_range:

    call WYSWIETL_CRLF

    call WYSWIETL_PAUZE

    mov dx,napis_niepowodzenie
    call WYPISZ_TEKST_Z_DX

    call WYSWIETL_CRLF

  .koniec:

    mov ax,0x4C00
    int 0x21


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DANE PROGRAMU
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

napis_powitanie:
  db "Simple BMI (Body Mass Index) Calculator",'$'

napis_wprowadz_wzrost:
  db "Enter your height, in meters (between 0.20 and 3.00)",0x0D,0x0A,">> ",'$'

napis_wprowadz_wage:
  db "Enter your weight, in kilograms (between 2.00 and 250.00)",0x0D,0x0A,">> ",'$'

napis_niepowodzenie:
  db "You provided invalid data! Goodbye.",'$'

napis_potwierdzenie_A:
  db "Here is the data I collected from you:",0x0D,0x0A," -> your height: ",'$'

napis_potwierdzenie_B:
  db " [m]",0x0D,0x0A," -> your weight: ",'$'

napis_potwierdzenie_C:
  db " [kg]",'$'

napis_wynik_A:
  db "Twoje BMI wynosi: ",'$'

napis_wynik_B:
  db " -> werdykt: ",'$'

napis_wynik_C:
  db "WYGLODZENIE",'$'

napis_wynik_D:
  db "WYCHUDZENIE",'$'

napis_wynik_E:
  db "NIEDOWAGA",'$'

napis_wynik_F:
  db "WARTOSC PRAWIDLOWA",'$'

napis_wynik_G:
  db "NADWAGA",'$'

napis_wynik_H:
  db "PIERWSZY STOPIEN OTYLOSCI",'$'

napis_wynik_I:
  db "DRUGI STOPIEN OTYLOSCI",'$'

napis_wynik_J:
  db "OTYLOSC SKRAJNA",'$'

bufor_header:
  db 0x10
  rb 0x01
bufor:
  rb 0x10


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


    DIODY DATA P2
    PRZYCISK BIT P3.0

    ; 1 000 000 us * (0.5 sekundy) = 500 000 us
    ; 500 000 / (256 * 256) = 500 000 / 65 536 = 7 r. 41248

    LICZBA_PELNYCH_PRZEPELNIEN EQU 7

CSEG at 0x0000

    JMP start

CSEG at 0x0100

  start:

    ; Ustaw "Timer 0" na tryb 16-bitowy.

    MOV TMOD,#0x01

    ; Wyczysc bit przycisku.
    ; (w trakcie debugowania wszystkie bity sa ustawione).

    CLR PRZYCISK

    MOV DIODY,#0x55

    ; Czekaj na wcisniecie przycisku.
    ; (pierwsze uruchomienie).

    JNB PRZYCISK,$

    MOV DIODY,#0xAA

  czekaj_na_puszczenie_przycisku:

    ; Czekaj na puszczenie przycisku.
    ; Program mierzy czas do ponownego wcisniecia.

    JB PRZYCISK,$

    ; Wyczysc akumulator. Wylacz diody.

    CLR A
    MOV DIODY,A

    ; Odliczaj pelne (65536-cyklowe) przepelnienia Timera.

    MOV R0,#LICZBA_PELNYCH_PRZEPELNIEN

  odliczanie_pelnych_przepelnien:

    MOV TL0,A
    MOV TH0,A

    CLR TF0
    SETB TR0

  pierwsze_sprawdzanie_timera:

    JB PRZYCISK,przycisk_wcisniety
    JNB TF0,pierwsze_sprawdzanie_timera

    CLR TR0

      DJNZ R0,odliczanie_pelnych_przepelnien

    ; Uzytkownik nie zdazyl wcisnac przycisku w zadanym czasie.
    ; Zatrzymaj program, dopoki uzytkownik nie wcisnie przycisku.

    JNB PRZYCISK,$

  przycisk_wcisniety:

    CLR TR0

    ; Zapalaj diody.
    ; (1: najlepiej, 8: najgorzej).

    MOV A,#0x08
    CLR C
    SUBB A,R0
    MOV R0,A
    CLR A

  petla_ustawiania_diod:

    SETB C
    RLC A

      DJNZ R0,petla_ustawiania_diod

    MOV DIODY,A

    SJMP czekaj_na_puszczenie_przycisku

END

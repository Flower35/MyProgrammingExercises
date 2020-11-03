
    DIODY DATA P2
    PRZYCISK BIT P3.0

    ; 1 000 000 us * (0.05 sekundy) = 50 000 us  
    CZAS_DO_PRZEPELNIENIA EQU ((1 << 16) - 50000)

CSEG at 0x0000

    JMP start

CSEG at 0x0100

  start:

    ; Ustaw "Timer 0" na tryb 16-bitowy.
    
    MOV TMOD,#0x01

    ; Wyczysc bit przycisku.
    ; (w trakcie debugowania wszystkie bity sa ustawione).
    
    CLR PRZYCISK
    
  czekaj_na_przycisk:

    ; Wylacz diody. Ustaw akumulator na 0x01.
    
    CLR A
    MOV DIODY,A
    INC A

    ; Czekaj na wcisniecie klawisza.
    
    JNB PRZYCISK,$

  odliczanie_timera:

    MOV TL0,#LOW(CZAS_DO_PRZEPELNIENIA)
    MOV TH0,#HIGH(CZAS_DO_PRZEPELNIENIA)
    
    ; Wlacz Timer. Poczekaj na przepelnienie. Wylacz Timer.
    
    SETB TR0
    
    JNB TF0,$
    CLR TF0
    
    CLR TR0
    
    ; Zapalanie kolejnych diod.
    ; Rotacja bitu akumulatora w lewo:
    ; (0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80).
    
    ORL DIODY,A
    RL A
    
    ; Kontynuuj odliczanie, jesli przycisk jest nadal wcisniety.
    
    JB PRZYCISK,odliczanie_timera
    
    SJMP czekaj_na_przycisk
    
END

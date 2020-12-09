CSEG AT 0x00
    AJMP reset

CSEG AT 0x0B
    AJMP TIMER0_przerwanie
    
DIODY DATA P2
CYKLE EQU 30
    
CSEG AT 0x30
  reset:
    MOV DIODY,#0
    
    SETB ET0
    SETB EA
    
    MOV TMOD,#0x01 ; T0 16-bitowy
   
    MOV A,#CYKLE ; pelne 65536 ticks
    MOV R0,#0 ; liczba sekund
    ACALL przygotuj_do_odliczenia
  petla:
    CLR TF0
    SJMP petla

  przygotuj_do_odliczenia:
    MOV TL0,#0
    MOV TH0,#0
    SETB TR0
    RET
    
  TIMER0_przerwanie:
    CLR TR0
    DEC A
      JNZ koniec_przerwania_T0
    INC R0
    MOV DIODY,R0
    MOV A,#CYKLE
  koniec_przerwania_T0:
    ACALL przygotuj_do_odliczenia
    RETI
    
END

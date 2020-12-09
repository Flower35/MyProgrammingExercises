
CSEG at 0

    AJMP reset
    
CSEG at 0x30

  reset:
    
    MOV SCON,#0x50 ; UART w trybie 1 (8-bit), REN = 1.
    MOV TMOD,#0x20 ; Licznik 1 w trybie 2.
    MOV TH1,#0xFD  ; 9600 Bds at 11.0592 MHz
    SETB TR1       ; uruchomienie timera
    CLR TI         ; wyzerowanie flagi wysylania
    
  loop:
  
    JNB RI,$       ; sprawdzenie flagi odbioru
    MOV A,SBUF     ; czytanie z UART
    CLR RI         ; wyzerowanie flagi odbioru
    
    CLR P2.0
    
    INC A          ; zakodowanie informacji
    MOV SBUF,A     ; zapis do UART
    JNB TI,$       ; czekanie na oproznienie bufora nadajnika
    CLR TI         ; wyzerowanie flagi wyslania
    
    SETB P2.0

    AJMP loop
    
END

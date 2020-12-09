
PRZYCISKI         DATA  P3
PRZYCISK_KODER    BIT   P3.7
PRZYCISK_DEKODER  BIT   P3.6

DIODY          DATA  P2
DIODA_KODER    BIT   P2.7
DIODA_DEKODER  BIT   P2.6

BIT_KODERA    EQU  10000000b
BIT_DEKODERA  EQU  01000000b

POPRZEDNI_STAN  EQU  R0
TESTOWY_BAJT    EQU  R1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

CSEG at 0x00

    AJMP reset
    
CSEG at 0x23

    AJMP port_szeregowy
    
CSEG at 0x30

  reset:

    MOV PRZYCISKI,#0
    MOV DIODY,#BIT_KODERA
    
    ; rejestr R0 = zapamietany stan przyciskow
    MOV POPRZEDNI_STAN,#BIT_KODERA  
    
    MOV SCON,#0x50 ; UART w trybie 1 (8-bit), REN = 1.
    MOV TMOD,#0x20 ; Licznik 1 w trybie 2.
    MOV TH1,#0xFD  ; 9600 Bds at 11.0592 MHz
    
    SETB TR1       ; uruchomienie timera
    CLR TI         ; wyzerowanie flagi wysylania
    
    SETB ES        ; zezwolenie na przerwania z portu szeregowego
    SETB EA        ; globalne odblokowanie przerwan
    
  loop:
  
    MOV A,POPRZEDNI_STAN
    CPL A
    ANL A,PRZYCISKI
    ANL A,#(BIT_KODERA|BIT_DEKODERA)
    
    JZ loop
    
  loop_odwrocenie_bitow:
  
    ; rejestr bedzie zawsze wynosil albo 0x80 albo 0x40
    MOV A,POPRZEDNI_STAN
    CPL A
    ANL A,#(BIT_KODERA|BIT_DEKODERA)
    MOV POPRZEDNI_STAN,A
    MOV DIODY,A

    JMP loop
  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  port_szeregowy:
  
    PUSH PSW
    PUSH ACC
  
    ; sprawdzenie powodu przerwania
    JBC TI,port_szeregowy_koniec
    
  port_szeregowy_odbieranie:
    
    CLR RI
    MOV TESTOWY_BAJT,SBUF
    
    MOV A,POPRZEDNI_STAN
    ANL A,#BIT_DEKODERA
    JNZ port_szeregowy_dekodowanie
    
    ; kodowanie znaku
    INC TESTOWY_BAJT
    JMP port_szeregowy_odbieranie_koniec
    
  port_szeregowy_dekodowanie:
  
    ; dekodowanie znaku
    DEC TESTOWY_BAJT
    
  port_szeregowy_odbieranie_koniec:
  
    MOV SBUF,TESTOWY_BAJT
    
  port_szeregowy_koniec:
  
    POP ACC
    POP PSW
    RETI
  
 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 
 END
 
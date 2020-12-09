
WSKAZNIK_BUFORA  EQU  R0
POBRANY_ZNAK     EQU  R1

ROZMIAR_BUFORA  EQU  16

BUFOR  DATA  0x30
DIODY  DATA  P2

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

CSEG at 0x00

    AJMP reset
    
CSEG at 0x23

    AJMP port_szeregowy
    
CSEG at 0x30

  reset:

    MOV WSKAZNIK_BUFORA,#BUFOR
    MOV DIODY,#0

    MOV SCON,#0x50 ; UART w trybie 1 (8-bit), REN = 1.
    MOV TMOD,#0x20 ; Licznik 1 w trybie 2.
    MOV TH1,#0xFD  ; 9600 Bds at 11.0592 MHz
    
    SETB TR1       ; uruchomienie timera
    CLR TI         ; wyzerowanie flagi wysylania
    
    SETB ES        ; zezwolenie na przerwania z portu szeregowego
    SETB EA        ; globalne odblokowanie przerwan
  
    JMP $

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  port_szeregowy:
  
    PUSH PSW
    PUSH ACC
  
    ; sprawdzenie powodu przerwania
    JBC TI,port_szeregowy_koniec
    
  port_szeregowy_odbieranie:
    
    CLR RI
    MOV POBRANY_ZNAK,SBUF
    
    ; testowanie cyklicznego wskaznika wzgledem poczatku
    MOV A,WSKAZNIK_BUFORA
    CLR C
    SUBB A,#BUFOR
    JC port_szeregowy_zresetuj_wskaznik
    
    ; testowanie cyklicznego wskaznika wzgledem konca
    MOV A,WSKAZNIK_BUFORA
    CLR C
    SUBB A,#(BUFOR + ROZMIAR_BUFORA)
    JNC port_szeregowy_zresetuj_wskaznik

    ; wskaznik miesci sie w dopuszczalnych granicach
    SJMP port_szeregowy_odbieranie_koniec

  port_szeregowy_zresetuj_wskaznik:
  
    MOV WSKAZNIK_BUFORA,#BUFOR
  
  port_szeregowy_odbieranie_koniec:
  
    MOV A,POBRANY_ZNAK
    MOV @WSKAZNIK_BUFORA,A
    INC WSKAZNIK_BUFORA
  
    MOV A,WSKAZNIK_BUFORA
    ANL A,#(ROZMIAR_BUFORA - 1)
    MOV DIODY,A
  
    MOV SBUF,POBRANY_ZNAK
    
  port_szeregowy_koniec:
  
    POP ACC
    POP PSW
    RETI
  
 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 
 END
 
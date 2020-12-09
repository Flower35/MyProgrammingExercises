CSEG AT 0
    AJMP reset
    
CSEG AT 0x0B
    AJMP TIMER0_przerwanie
    
CSEG AT 0x30
  reset:
    SETB ET0 ; wlaczenie przerwania dla przepelnienia timera T0
    SETB EA ; odblokowanie globalnych przerwan
    MOV TMOD,#00000010b
    MOV TH0,#-25
    SETB TR0 ; wlaczenie timera T0
    CLR TF0
  loop:
    ACALL delay
    CPL P2.7
    SJMP loop
    
  TIMER0_przerwanie:
    CPL P2.0
    CLR TF0
    RETI
    
  delay:
    MOV R0,#02
  del2:
    DJNZ R0,del2
    RET
    
END

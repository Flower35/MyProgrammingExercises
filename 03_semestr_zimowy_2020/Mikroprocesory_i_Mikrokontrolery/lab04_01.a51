CSEG AT 0
    AJMP reset

CSEG AT 0x03
    AJMP INT0_service

CSEG AT 0x13
    AJMP INT1_service

CSEG AT 0x30
  reset:
    SETB EX0 ; wlaczenie przerwania INT0
    SETB EX1 ; wlaczenie przerwania INT1
    SETB EA ; odblokowanie wszystkich przerwan
    CLR PX0 ; priorytet przerwania INT0
    SETB PX1 ; priorytet przerwania INT1
    ; SETB IT0 ; przerwanie INT0 wyzwalane zboczem
    ; SETB IT1 ; przerwanie INT1 wyzwalane zboczem
    MOV P2,#0 ; wyczysc PORT 2 (diody)
    MOV P3,#0 ; przyciski (bit 2 steruje INT0, bit 3 steruje INT1)
  loop:
    SJMP loop

  INT0_service:
    ACALL delay
    CPL P2.0
    RETI
  
  INT1_service:
    ACALL delay
    CPL P2.7
    RETI
    
  delay:
    MOV R0,#100
  del2:
    MOV R1,#255
  del1:
    NOP
    NOP
    DJNZ R1,del1
    DJNZ R0,del2
    RET
    
END

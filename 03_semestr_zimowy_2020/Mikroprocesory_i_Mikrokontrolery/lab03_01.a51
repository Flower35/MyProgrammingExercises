
DIODA BIT P2.0

CSEG AT 0
jmp start

CSEG at 0x0100
  start:
    MOV TMOD,#01 ; tryb pracy: timer 16-bitowy

  petla:
    MOV TH0,#0
    MOV TL0,#0
    SETB TR0 ; wlaczenie Timera 0

    JNB TF0,$ ; program zatrzymuje sie, dopóki nie dojdzie do przepelnienia
    CLR TF0 ; flaga przepelnienia zresetowana
    CLR TR0 ; wylaczenie Timera 0

    MOV C,DIODA
    CPL C
    MOV DIODA,C

    SJMP petla

END

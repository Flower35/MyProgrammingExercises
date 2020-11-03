PROGRAM SEGMENT CODE

KLAWISZE DATA P3
DIODY DATA P2

CSEG AT 0x0000
JMP start

RSEG PROGRAM
  start:

    ; A => poprzedni stan
    ; R0 => nastepny stan
    ; R1 => obecny stan

    MOV A,DIODY

  petla:

    MOV R0,KLAWISZE
    MOV R1,DIODY

    ; --------------------------------
    ; [poprzedni, obecny] -> flaga XOR
    ; --------------------------------
    ; [0, 0] -> 0
    ; [0, 1] -> 1
    ; [1, 0] -> 0
    ; [1, 1] -> 0

    CPL A
    ANL A,R0
    XRL A,R1

    MOV DIODY,A
    MOV A,R0

    SJMP petla
END

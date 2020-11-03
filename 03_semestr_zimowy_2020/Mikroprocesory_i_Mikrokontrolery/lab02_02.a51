PROG SEGMENT CODE

DIODA BIT P2.1
KLAWISZ BIT P3.0

CSEG AT 0
JMP start

RSEG PROG
  start:
    MOV C,KLAWISZ ; odczytanie stanu klawisza
    JC zmien_stan_diody ; sprawdzamy czy wcisnieto klawisz
    SJMP start ; jesli nie wcisnieto, wroc na start
  zmien_stan_diody:
    mov C,dioda
    CPL C ; przelacza bit na zmiane [0 -> 1, 1 -> 0]
    mov dioda,C
    SJMP start
END

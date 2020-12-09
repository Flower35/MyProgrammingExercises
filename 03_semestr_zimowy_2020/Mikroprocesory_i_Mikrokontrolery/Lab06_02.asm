
.nolist
.include "m32def.inc" ; potrzebne dla "DDRB" i "PORTB"

.list
.listmac

.def KierunekPodrozy   = R16
.def WedrujacaJedynka  = R17

.cseg
.org 0

    CLI

    ; Przygotowanie portu B na OUTPUT
    LDI R16,0xFF
    OUT DDRB,R16

    ; Przygotowanie "wedruj¹cego bitu"
    LDI KierunekPodrozy,0
    LDI WedrujacaJedynka,0x01

Petla:

    ; COM WedrujacaJedynka
    OUT PORTB,WedrujacaJedynka
    NOP 
    ; COM WedrujacaJedynka

    CLC
    TST KierunekPodrozy
    BREQ PrzesunWLewo

PrzesunWPrawo:

    ROR WedrujacaJedynka
    BRCC Petla

    COM KierunekPodrozy       ; 0xFF -> 0x00
    LDI WedrujacaJedynka,0x02 ; "0000 0010"
    RJMP Petla

PrzesunWLewo:

    ROL WedrujacaJedynka
    BRCC Petla

    COM KierunekPodrozy       ; 0x00 -> 0xFF
    LDI WedrujacaJedynka,0x40 ; "0100 0000"
    RJMP Petla

.exit

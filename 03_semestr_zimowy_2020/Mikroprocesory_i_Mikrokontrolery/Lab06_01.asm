
.nolist
.include "m32def.inc"

.list
.listmac

.cseg
.org 0
    RJMP ResetProcessor

ResetProcessor:
    CLI
    LDI R16,0xFF
    OUT DDRB,R16
    OUT PORTB,R16

Koniec:
    RJMP Koniec

.exit

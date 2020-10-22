;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; "sort.asm"
;; kompilator: "flat assembler g"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; [LAB 01, ZADANIE 01]
;;   Napisz program sortuj¹cy rosn¹co ci¹g liczb
;;   podanych jako argument programu.
;;
;;  Program mo¿e posortowaæ maksymalnie 63 liczby,
;;  pod warunkiem, ¿e wszystkie s¹ jednocyfrowe
;;  -- tylko tyle zmieœci siê w wierszu poleceñ.
;;
;;  Przyjmowane s¹ liczby 16-bitowe (w zakresie od -32768 do +32767).
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include 'cpu/8086.inc'

format binary as 'com'

org 0x0100

    jmp ROZPOCZNIJ


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DEFINICJE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

TABLICA_MAX EQU 63
ARGUMENTY_MAX EQU 126
    ; na argumenty polecenia jest przeznaczone 126 bajtów,
    ; co daje nam maksymalnie 63 jednocyfrowe liczby.


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; FUNKCJE POMOCNICZE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include '../common/out_i16.inc'
include '../common/atoi_i16.inc'


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; POCZ¥TEK PROGRAMU
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ROZPOCZNIJ:

    xor ax,ax

    mov bp,sp
    push ax
    push ax
      ; [bp - 0x02] = args counter
      ; [bp - 0x04] = args guard

    mov al,byte[0x80]
    mov dx,0x81

    mov bx,dx
    add bx,ax

    mov di,tablica

    xor ax,ax

  .petla_zczytywania:

    cmp dx,bx
      jae .wypisz_posortowane

    cmp ax,TABLICA_MAX
      jae .wypisz_posortowane

    call ATOI_INT16
      jc .blad_niepoprawny_argument

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Wyszukiwanie pozycji w posortowanej liœcie.

    mov word[bp - 0x04],bx
    mov bx,ax

    mov ax,word[bp - 0x02]
    dec ax
    shl ax,0x01

    mov si,tablica
    add si,ax

  .petla_wybieranie_pozycji:

    cmp si,tablica
      jb .wybrano_pozycje

    lodsw
    sub si,0x02

    cmp ax,bx
      jle .wybrano_pozycje

    sub si,0x02
    jmp .petla_wybieranie_pozycji

  .wybrano_pozycje:

    add si,0x02
      ; SI = pocz¹tek pozycji dwubajtowej,
      ; do której wkleimy obecnie sprawdzany argument

    mov cx,word[bp - 0x02]
    shl cx,0x01
    add cx,tablica
    mov di,cx
      ; DI = pocz¹tek pozycji dwubajtowej, pierwszy bajt
      ; za tablic¹ obecnie zapisanych argumentów

    sub cx,si
      ; liczba bajtów do przekopiowania

    inc di
      ; Kopiowany bêdzie ci¹g dwubajtowych liczb o jedn¹ pozycjê w prawo.
      ; WskaŸnik celu musi maksymalnie wyprzedzaæ wskaŸnik Ÿród³a,
      ; wobec tego przechodzimy na skrajnie prawy bajt
      ; (w praktyce zwiêkszamy obydwa wskaŸniki o jeden!)
    mov si,di
    sub si,0x02

    std
    rep movsb
    cld

    mov ax,bx
    dec di
      ; (w tym momencie "INC SI" = "DEC DI")
      ; Przenoszenie argumentów o jeden w prawo sprawi ¿e SI bêdzie wskazywa³
      ; na jeden bajt przed miejscem docelowym obecnego argumentu, poniewa¿
      ; ostatni skopiowany bajt w "REP MOVBS" by³ pierwszym (lewym) bajtem
      ; jednego z poprzednich argumentów, który jest teraz skopiowany
      ; o jedno miejsce w prawo (wraz z reszt¹ argumentów z prawej strony)
    stosw

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Koniec g³ównej pêli.

    mov ax,word[bp - 0x02]
    inc ax
    mov word[bp - 0x02],ax

    mov bx,word[bp - 0x04]

    jmp .petla_zczytywania

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Komunikaty o b³êdach.

  .blad_brak_argumentow:

    mov dx,napis_brak_arg
    mov ah,0x09
    int 0x21

    jmp .koniec_programu

  .blad_niepoprawny_argument:

    mov dx,napis_bledny_arg
    mov ah,0x09
    int 0x21

    mov ax,word[bp - 0x02]
    inc ax
    call OUTPUT_INT16

    mov dx,napis_crlf_excl
    mov ah,0x09
    int 0x21

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; * Wypisanie tabeli w obecnym stanie,
    ;; * bezpoœrednio po komunikacie o b³êdnym argumencie
    ;; * lub po prostu po wyczerpaniu wszystkich argumentów.

  .wypisz_posortowane:

    mov bx,word[bp - 0x02]

    test bx,bx
      jle .blad_brak_argumentow

    mov si,tablica

    jmp .wypisz_element
      ; wypisanie pierwszego elementu bez poprzedzaj¹cej go spacji

  .petla_wypisywania:

    mov dl,' '
    mov ah,0x02
    int 0x21

  .wypisz_element:

    lodsw
    call OUTPUT_INT16

    dec bx
    jnz .petla_wypisywania

    mov dx,napis_crlf
    mov ah,0x09
    int 0x21

  .koniec_programu:

    mov ax,0x4C00
    int 0x21


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DANE PROGRAMU
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

napis_bledny_arg: db "Invalid argument #",'$'

napis_brak_arg: db "Please provide some numbers as arguments"
napis_crlf_excl: db '!'
napis_crlf: db 0x0D,0x0A,'$'

tablica: rb (2 * TABLICA_MAX)
    ; Teoretyczna, niejawna rezerwacja 126 bajtów (dla 63 16-bitowych liczb)
    ; na samym koñcu pliku Ÿród³owego.
    ; Poniewa¿ system DOS przydziela zawsze 64kB pamiêci na kod, dane i stos,
    ; wobec tego nie ma potrzeby wpisywania pustej tablicy
    ; w œrodku wynikowego pliku "SORT.COM".


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

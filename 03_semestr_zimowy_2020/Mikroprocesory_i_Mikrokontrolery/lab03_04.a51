;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Diody D0-D3 zpalaja sie w losowej kolejnosci na krotki odstep czasu.
;; Czas wyswietlana i przerwe mozna zmniejszac/zwiekszac klawiszami S7/S6.
;; Jesli w czasie wyswietlania na danej diodzie uzytkownik wcisnie
;; odpowiedni klawisz, wyswietlany jest na diodach sygnal zakonczenia
;; (dowolna sekwencja kolejno zapalonych diod) i program konczy dzialanie.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    DIODY DATA P2
    PRZYCISKI DATA P3
    
    ; Stale i Dane do naszego zadania
    
    LOSOWE_J EQU 7
    LOSOWE_K EQU 10
    
    PREDKOSC_POZIOMY EQU 8
    PREDKOSC_SRODKOWA EQU (PREDKOSC_POZIOMY / 2 - 1)
    
    PREDKOSC \
      DATA 0x30
      
    LOSOWE_POZYCJA \
      DATA (PREDKOSC + 0x01)
    
    DANE_DO_WKLEJENIA_POCZATEK \
      DATA (LOSOWE_POZYCJA + 0x01)
 
    PREDKOSC_DANE_TIMERA \
      DATA DANE_DO_WKLEJENIA_POCZATEK
 
    LOSOWE_STARTOWE \
      DATA (PREDKOSC_DANE_TIMERA + PREDKOSC_POZIOMY * 0x03)
    
    LOSOWE_POZOSTALE \
      DATA (LOSOWE_STARTOWE + LOSOWE_K)
    
    DANE_DO_WKLEJENIA_ROZMIAR \
      EQU (LOSOWE_POZOSTALE - PREDKOSC_DANE_TIMERA)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

CSEG at 0x0000

    JMP start

CSEG at 0x0100

  start:

    CALL dane_inicjalizacja
    
    ; Ustaw "Timer 0" na tryb 16-bitowy.
    
    MOV TMOD,#0x01
    
    ; Ustaw domyslna predkosc
    MOV PREDKOSC,#PREDKOSC_SRODKOWA
    
  glowna_funkcja_petla:
  
    ; Zresetuj diody
    
    MOV DIODY,#0x00

    ; Pierwsza faza - czekaj na wcisniecie "S6" lub "S7".

    MOV R1,#0xC0
    MOV A,#PREDKOSC_SRODKOWA 
    CALL odczekaj
      JNZ glowna_funkcja_zmiana_predkosci
    
    ; Druga faza - testowanie refleksu.
    
    CALL liczby_losowe_wez_kolejna
    
    MOV DIODY,R1
    
    MOV A,PREDKOSC
    CALL odczekaj
      JNZ glowna_funkcja_sukces

    SJMP glowna_funkcja_petla
    
  glowna_funkcja_zmiana_predkosci:
  
    ; Przeskocz, jesli "S7" jest nieustawione.
  
    RLC A
      JNC glowna_funkcja_zmiana_predkosci_2
      
    INC PREDKOSC
    
    JMP glowna_funkcja_zmiana_predkosci_3
      
  glowna_funkcja_zmiana_predkosci_2:

    DEC PREDKOSC

  glowna_funkcja_zmiana_predkosci_3:
  
    MOV A,PREDKOSC
    ANL A,#(PREDKOSC_POZIOMY - 1)
    INC A
    MOV R0,A
    CLR A

  glowna_funkcja_zmiana_predkosci_4:

    SETB C
    RRC A
      DJNZ R0,glowna_funkcja_zmiana_predkosci_4    
  
    SJMP glowna_funkcja_komunikat
  
  glowna_funkcja_sukces:
  
    MOV A,#0xAA
    
  glowna_funkcja_komunikat:
  
    MOV DIODY,A

    MOV R1,#0x00
    MOV A,#(PREDKOSC_POZIOMY - 1)
    CALL odczekaj

    SJMP glowna_funkcja_petla
  

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Odczekaj wybrany interwal czasu.
;; Do funkcji przekazujemy w Akumulatorze poziom predkosci.
;; W rejestrze "R1" jest flaga diody do sprawdzania.
;; Funkcja w Akumulatorze zwroci ktore przyciski wcisnieto.
;; Ignorowane sa przyciski trzymane przed wlaczeniem funkcji!

  odczekaj:

    ; Szybkie mnozenie przez trzy (2 * A + A).
    ; "R3" <- liczba pelnych cykli.
    ; "R4" <- HIGH BYTE reszty mikrosekund.
    ; "R5" <- LOW BYTE reszty mikrosekund.
  
    ANL A,#(PREDKOSC_POZIOMY - 1)
    
    MOV R0,A
    RL A
    ADD A,R0
    ADD A,#PREDKOSC_DANE_TIMERA
    MOV R0,A
    
    MOV A,@R0
    MOV R3,A
    
    INC R0
    MOV A,@R0
    MOV R4,A
    
    INC R0
    MOV A,@R0
    MOV R5,A
    
    ; Odczytaj stan przyciskow przed odczekiwaniem.
    ; Odwroc stan. Dokonaj logicznego "AND" z szukanymi bitami. 
    
    MOV A,PRZYCISKI
    CPL A
    ANL A,R1
    MOV R0,A
    
  ; Wieksza petla.
    
  odczekaj_pelne_cykle:
    
    MOV TL0,#0x00
    MOV TH0,#0x00
    
    CLR TF0
    SETB TR0
    
  odczekaj_pelne_cykle_podpetla:
  
    MOV A,PRZYCISKI
    ANL A,R0
      JNZ odczekaj_koniec
    
      JNB TF0,odczekaj_pelne_cykle_podpetla
    
    CLR TR0
    
      DJNZ R3,odczekaj_pelne_cykle
    
    ; Mniejsza petla.
    
    MOV TL0,R5
    MOV TH0,R4
    
    CLR TF0
    SETB TR0
    
  odczekaj_koncowka_podpetla:
  
    MOV A,PRZYCISKI
    ANL A,R0
      JNZ odczekaj_koniec
    
      JNB TF0,odczekaj_koncowka_podpetla

  odczekaj_koniec:
      
    CLR TR0
    
    RET


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Kolejna wartosc zwracana jest w rejestrze "R1"

  liczby_losowe_wez_kolejna:

    MOV R0,LOSOWE_POZYCJA
    MOV A,@R0
    MOV R1,A
    
    JMP liczby_losowe_zwiekszenie_pozycji


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Funkcja ustawia "CARRY FLAG" po przygotowaniu ostatniej losowej flagi.

  liczby_losowe_losuj_nastepna:
  
    ; Pierwszy skladnik.
    
    MOV A,LOSOWE_POZYCJA
    ADD A,#(0 - LOSOWE_J)

    MOV R0,A
    MOV A,@R0
    MOV R1,A
    
    ; Drugi skladnik.
    
    MOV A,LOSOWE_POZYCJA
    ADD A,#(0 - LOSOWE_K)
    
    MOV R0,A
    MOV A,@R0
    ADD A,R1
    
    ; Modulo cztery wyniku dodawania. Ustawienie flag dla diod.
    ; (0x00 -> 0x01, 0x01 -> 0x02, 0x02 -> 0x04, 0x03 -> 0x08).
    
    ANL A,#((1 << 2) - 1)
    INC A
    MOV R0,A
    MOV A,#0x80
    
  liczby_losowe_losuj_nastepna_ustaw_flagi:
  
    RL A
    
      DJNZ R0,liczby_losowe_losuj_nastepna_ustaw_flagi
    
    MOV R0,LOSOWE_POZYCJA
    MOV @R0,A
    
  liczby_losowe_zwiekszenie_pozycji:

    MOV A,LOSOWE_POZYCJA
    INC A
    MOV R0,A
    
    ; Jesli doszlismy do 0x80, to resetuj licznik pozycji
    ; oraz zwroc "CARRY FLAG = 1". W przeciwnym wypadku "CF = 0".
    
    RLC A
      JNC liczby_losowe_losuj_nastepna_zmien_pozycje
    
    MOV R0,#LOSOWE_POZOSTALE
    
  liczby_losowe_losuj_nastepna_zmien_pozycje:
  
    MOV LOSOWE_POZYCJA,R0
    RET


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Kopiuje liczby z sekcji KODU oraz wypelnia cala przestrzen
;; danych losowymi flagami dla diod (D0, D1, D2 lub D3).

  dane_inicjalizacja:
  
    MOV R0,#DANE_DO_WKLEJENIA_POCZATEK
    MOV R1,#DANE_DO_WKLEJENIA_ROZMIAR
    MOV DPTR,#DANE_DO_SKOPIOWANIA_POCZATEK
    
  dane_inicjalizacja_kopiowanie:
  
    CLR A
    MOVC A,@A+DPTR
    MOV @R0,A
    
    INC DPTR
    INC R0
    
      DJNZ R1,dane_inicjalizacja_kopiowanie
    
    MOV LOSOWE_POZYCJA,#LOSOWE_POZOSTALE
    
  dane_inicjalizacja_druga_petla:
  
    CALL liczby_losowe_losuj_nastepna
      JNC dane_inicjalizacja_druga_petla
    
    RET


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Dane, ktore program skopiuje po uruchomieniu sie.
;; Zwroc uwage, ze "dw" zapisuje liczbe w formacie "Big Endian"!
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; (czas w mikrosekundach: liczba 16-bitowych cykli, reszta)
;;   250 000:   3,  53 392.
;;   500 000:   7,  41 248.
;;   750 000:  11,  29 104.
;; 1 000 000:  15,  16 960.
;; 1 250 000:  19,   4 816.
;; 1 500 000:  22,  58 208.
;; 1 750 000:  26,  46 064.
;; 2 000 000:  30,  33 920.

  DANE_DO_SKOPIOWANIA_POCZATEK EQU $

  PREDKOSC_DANE_TIMERA_W_KODZIE:
  
    db 3
      dw ((1 << 16) - 53392)
    db 7
      dw ((1 << 16) - 41248)
    db 11
      dw ((1 << 16) - 29104)
    db 15
      dw ((1 << 16) - 16960)
    db 19
      dw ((1 << 16) -  4816)
    db 22
      dw ((1 << 16) - 58208)
    db 26
      dw ((1 << 16) - 46064)
    db 30
      dw ((1 << 16) - 33920)

  LOSOWE_STARTOWE_W_KODZIE:

    db 4, 5, 3, 3, 3, 4, 6, 2, 6, 5 

  DANE_DO_SKOPIOWANIA_KONIEC EQU $


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    IF ((DANE_DO_SKOPIOWANIA_KONIEC - DANE_DO_SKOPIOWANIA_POCZATEK) <> DANE_DO_WKLEJENIA_ROZMIAR) 
      __ERROR__ "Rozmiar danych do skopiowania rozny od danych do wklejenia!"
    ENDIF


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

END

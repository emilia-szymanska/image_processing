#include<stdio.h>
#include<string.h>
#define W_OK 0                   /* wartosc oznaczajaca brak bledow */
#define B_NIEPOPRAWNAOPCJA -1    /* kody bledow rozpoznawania opcji */
#define B_BRAKNAZWY   -2
#define B_BRAKWARTOSCI  -3
#define B_BRAKPLIKU   -4

typedef struct {
  FILE *plik_we, *plik_wy;
  char *nazwa_we, *nazwa_wy;
  int negatyw,progowanie,konturowanie,wyswietlenie, rozmycie, maska, zmianapoziomow, help;
  int w_prog, biel, czern;
} w_opcje;

void wyzeruj_opcje(w_opcje *);
int przetwarzaj_opcje(int , char **, w_opcje *);
void komunikat();

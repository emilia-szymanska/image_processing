/********************************************************
 * based on a code given at programming course led by 
 * Ph.D. Robert Muszynski and MSc Arkadiusz Mielczarek
 * at Wroclaw University of Science and Technology
 * https://kcir.pwr.edu.pl/~mucha/PProg/
 * COPYRIGHT (c) 2007-2013 ZPCiR
 * MIT Licence
 ********************************************************/

#include<stdio.h>
#include<string.h>
#define W_OK 0                   /* wartosc oznaczajaca brak bledow */
#define B_NIEPOPRAWNAOPCJA -1    /* kody bledow rozpoznawania opcji */
#define B_BRAKNAZWY   -2
#define B_BRAKWARTOSCI  -3
#define B_BRAKPLIKU   -4

/* strukura do zapamietywania opcji podanych w wywolaniu programu */
typedef struct {
  FILE *plik_we, *plik_wy;        /* uchwyty do pliku wej. i */
  char *nazwa_we, *nazwa_wy;
  int negatyw,progowanie,konturowanie,wyswietlenie, rozmycie, maska, zmianapoziomow, help;      /* opcje */
  int w_prog, biel, czern;              /* wartosc progu dla opcji progowanie oraz biel i czern dla zmiany poziomow*/ 
} w_opcje;

/*******************************************************/
/* Funkcja inicjuje strukture wskazywana przez wybor   */
/* PRE:                                                */
/*      poprawnie zainicjowany argument wybor (!=NULL) */
/* POST:                                               */
/*      "wyzerowana" struktura wybor wybranych opcji   */
/*******************************************************/

void wyzeruj_opcje(w_opcje * wybor) {
  wybor->plik_we=NULL;
  wybor->plik_wy=NULL;
  wybor->negatyw=0;
  wybor->konturowanie=0;
  wybor->progowanie=0;
  wybor->wyswietlenie=0;
  wybor->rozmycie=0;
  wybor->maska=0;
  wybor->zmianapoziomow=0;
  wybor->help=0;
}

/************************************************************************/
/* Funkcja rozpoznaje opcje wywolania programu zapisane w tablicy argv  */
/* i zapisuje je w strukturze wybor                                     */
/* Skladnia opcji wywolania programu                                    */
/*         program {[-i nazwa] [-o nazwa] [-p liczba] [-n] [-k] [-d]    */
/*		   [-r] [-m] [-z liczba liczba] [-h] }                  */
/* Argumenty funkcji:                                                   */
/*         argc  -  liczba argumentow wywolania wraz z nazwa programu   */
/*         argv  -  tablica argumentow wywolania                        */
/*         wybor -  struktura z informacjami o wywolanych opcjach       */
/* PRE:                                                                 */
/*      brak                                                            */
/* POST:                                                                */
/*      funkcja otwiera odpowiednie pliki, zwraca uchwyty do nich       */
/*      w strukturze wybór, do tego ustawia na 1 pola, ktore            */
/*	poprawnie wystapily w linii wywolania programu,                 */
/*	pola opcji nie wystepujacych w wywolaniu ustawione sa na 0;     */
/*	zwraca wartosc W_OK, gdy wywolanie bylo poprawne                */
/*	lub kod bledu zdefiniowany stalymi B_* (<0)                     */
/* UWAGA:                                                               */
/*      funkcja nie sprawdza istnienia i praw dostepu do plikow         */
/*      w takich przypadkach zwracane uchwyty maja wartosc NULL         */
/************************************************************************/

int przetwarzaj_opcje(int argc, char **argv, w_opcje *wybor) {
  int i, prog, biel, czern;

  wyzeruj_opcje(wybor);
  wybor->plik_wy=stdout;        /* na wypadek gdy nie podano opcji "-o" */

  for (i=1; i<argc; i++) {
    if (argv[i][0] != '-')  	/* blad: to nie jest opcja - brak znaku "-" */
      return B_NIEPOPRAWNAOPCJA; 
    switch (argv[i][1]) {
    case 'i': {                	/* opcja z nazwa pliku wejsciowego */
      wybor->plik_wy=NULL;
      if (++i<argc) {   	/* wczytujemy kolejny argument jako nazwe pliku */
	wybor->nazwa_we=argv[i];
	if (strcmp(wybor->nazwa_we,"-")==0) 	/* gdy nazwa jest "-"        */
	  wybor->plik_we=stdin;            	/* ustwiamy wejscie na stdin */
	else                               	/* otwieramy wskazany plik   */
	  wybor->plik_we=fopen(wybor->nazwa_we,"r");
      } else 
	return B_BRAKNAZWY;                   	/* blad: brak nazwy pliku */
      break;
    }
    case 'o': {                 		/* opcja z nazwa pliku wyjsciowego */
      if (++i<argc) {   			/* wczytujemy kolejny argument jako nazwe pliku */
	wybor->nazwa_wy=argv[i];
	if (strcmp(wybor->nazwa_wy,"-")==0)	/* gdy nazwa jest "-"         */
	  wybor->plik_wy=stdout;          	/* ustwiamy wyjscie na stdout */
	else                             	 /* otwieramy wskazany plik    */
	  wybor->plik_wy=fopen(wybor->nazwa_wy,"w");
      } else 
	return B_BRAKNAZWY;                   	/* blad: brak nazwy pliku */
      break;
    }
    case 'p': {
      if (++i<argc) { 				/* wczytujemy kolejny argument jako wartosc progu */
	if (sscanf(argv[i],"%d",&prog)==1) {
	  wybor->progowanie=1;
	  wybor->w_prog=prog;
	} else
	  return B_BRAKWARTOSCI;     		/* blad: niepoprawna wartosc progu */
      } else 
	return B_BRAKWARTOSCI;             	/* blad: brak wartosci progu */
      break;
    }
    case 'n': {                 		/* mamy wykonac negatyw */
      wybor->negatyw=1;
      break;
    }
    case 'k': {                 		/* mamy wykonac konturowanie */
      wybor->konturowanie=1;
      break;
    }
    case 'd': {                 		/* mamy wyswietlic obraz */
      wybor->wyswietlenie=1;
      break;
    }
    case 'h': {					/* mamy wyswietlic komunikat jak wywolac program */
	wybor->help=1;	    
   	printf("Poprawne wywolanie programu: program {[-i nazwa] [-o nazwa] [-p liczba] [-n] [-k] [-d] [-r] [-m] [-z liczba liczba] [-h] }\n" );
	      }
    case 'r':{
	wybor->rozmycie=1;			/* mamy wykonac rozmycie */
        break;
	     }
    case 'm':{					/* mamy nalozyc maske */
	wybor->maska=1;
	break;
	     }	
    case 'z': {					/* mamy wykonac zmiane poziomow */
      if (++i<argc) { 				/* wczytujemy kolejne argumenty jako wartosci bieli i czerni */
	if (sscanf(argv[i],"%d",&czern)==1 && sscanf(argv[++i], "%d", &biel)==1) {
	  wybor->zmianapoziomow=1;
	  wybor->czern=czern;
	  wybor->biel=biel;
	} else
	  return B_BRAKWARTOSCI;     		/* blad: brak wartosci  */
      } else 
	return B_BRAKWARTOSCI;             	/* blad: brak wartosci */
      break;
    }
    default:                    /* nierozpoznana opcja */
      return B_NIEPOPRAWNAOPCJA;
    } /*koniec switch */
  } /* koniec for */

  if (wybor->plik_we!=NULL)     /* ok: wej. strumien danych zainicjowany */
    return W_OK;
  else 
    return B_BRAKPLIKU;         /* blad:  nie otwarto pliku wejsciowego  */
}

/* Funkcja wyswietla komunikat o poprawnym wywolywaniu programu  */
void komunikat(){
   	printf("Poprawne wywolanie programu: program {[-i nazwa] [-o nazwa] [-p liczba] [-n] [-k] [-d] [-r] [-m] [-z liczba liczba] [-h] }\n" );
}

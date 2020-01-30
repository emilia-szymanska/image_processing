#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX 512            /* Maksymalny rozmiar wczytywanego obrazu */
#define DL_LINII 1024      /* Dlugosc buforow pomocniczych */

/********************************************************
 * based on a code given at programming course led by 
 * Ph.D. Robert Muszynski and MSc Arkadiusz Mielczarek
 * at Wroclaw University of Science and Technology
 * https://kcir.pwr.edu.pl/~mucha/PProg/
 * COPYRIGHT (c) 2007-2013 ZPCiR
 * MIT Licence
 ********************************************************/


/************************************************************************************
 * Funkcja wczytuje obraz PGM i PPM z pliku do tablicy       	       	       	    *
 *										    *
 * \param[in] plik_we uchwyt do pliku z obrazem w formacie PGM			    *
 * \param[out] obraz_pgm tablica, do ktorej zostanie zapisany obraz		    *
 * \param[out] wymx szerokosc obrazka						    *
 * \param[out] wymy wysokosc obrazka						    *
 * \param[out] szarosci liczba odcieni szarosci					    *
 * \return liczba wczytanych pikseli						    *
 ************************************************************************************/

/*struktura przechowujaca parametry obrazu */
typedef struct {
  int wymx, wymy, szarosci; 
  void *obraz_pgm;
  int P3; 
} xobraz; 

int bezwzgl(int a) /*funkcja obliczajaca wartosc bezwgledna */
{
  if(a>0) return a;
  else return -a;
}

/* PRE: plik jest w formacie PGM lub PPM, prawidlowy uchwyt, wymiary i skala szarosci */
/* POST: plik zostanie wczytany lub w przypadku niespe¿nionych warunkow PRE zostanie wypisany komunikat */

int czytaj(FILE *plik_we,xobraz *obraz) {
  char buf[DL_LINII];      /* bufor pomocniczy do czytania naglowka i komentarzy */
  int znak;                /* zmienna pomocnicza do czytania komentarzy */
  int koniec=0;            /* czy napotkano koniec danych w pliku */
  int i,j, mnoznik;
  fflush(stdout);
  /*Sprawdzenie czy podano prawidlowy uchwyt pliku */
  if (plik_we==NULL) {
    fprintf(stderr,"Blad: Nie podano uchwytu do pliku\n");
    return(0);
  }

  /* Sprawdzenie "numeru magicznego" - powinien byc P2 lub P3 */
  if (fgets(buf,DL_LINII,plik_we)==NULL)   		/* Wczytanie pierwszej linii pliku do bufora */
    koniec=1;                              		/* Nie udalo sie? Koniec danych! */
  if (buf[0]=='P' && buf[1]=='3') obraz->P3=1;
  else obraz->P3=0;				        /*zapamietujemy, czy jest to P2 lub P3 */
  if ( (buf[0]!='P') || (buf[1]!='2') || koniec) {  	/* Czy jest magiczne "P2" ? */
	  if(obraz->P3!=1){ 			    	/*Czy jest to "P3"? */
    		fprintf(stderr,"Blad: To nie jest plik PGM ani PPM\n");
		return(0);
	  }
  }
  /* Pominiecie komentarzy */
  do {
    if ((znak=fgetc(plik_we))=='#') {         /* Czy linia rozpoczyna sie od znaku '#'? */
      if (fgets(buf,DL_LINII,plik_we)==NULL)  /* Przeczytaj ja do bufora                */
	koniec=1;                   	      /* Zapamietaj ewentualny koniec danych */
    }  else {
      ungetc(znak,plik_we);                   /* Gdy przeczytany znak z poczatku linii */
    }                                         /* nie jest '#' zwroc go                 */
  } while (znak=='#' && !koniec);   	      /* Powtarzaj dopoki sa linie komentarza */
                                    	      /* i nie nastapil koniec danych         */
  
  /* Pobranie wymiarow obrazu i liczby odcieni szarosci */
  if (fscanf(plik_we,"%d %d %d",&(obraz->wymx),&(obraz->wymy),&(obraz->szarosci))!=3) {
    fprintf(stderr,"Blad: Brak wymiarow obrazu lub liczby stopni szarosci\n");
    return(0);
  }

 /*Dynamiczne zaalokowowanie odpowiedniej tablicy */
  mnoznik=obraz->P3*2+1;	/*mnoznik dla P3 bedzie rowny 3, a dla P2 1 */
  obraz->obraz_pgm= malloc(obraz->wymx*obraz->wymy*mnoznik*sizeof(int));
  int(*obraz_pgm)[obraz->wymx*mnoznik];
  obraz_pgm=(int(*)[obraz->wymx*mnoznik]) obraz->obraz_pgm;

  /* Pobranie obrazu i zapisanie w tablicy obraz_pgm*/
  for (i=0;i<obraz->wymy;i++) {
    for (j=0;j<(obraz->wymx)*mnoznik;j++) {
      if (fscanf(plik_we,"%d",&(obraz_pgm[i][j]))!=1) {
	fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
	return(0);
      }
    }
  }

  return obraz->wymx*obraz->wymy;   	/* Czytanie zakonczone sukcesem    */
}                       		/* Zwroc liczbe wczytanych pikseli */



/* Wyswietlenie obrazu o zadanej nazwie za pomoca programu "display"   */
/*PRE: podana nazwa pliku*/
/*POST: funkcja wyswietla plik*/

void wyswietl(char *n_pliku) {
  char polecenie[DL_LINII];      /* bufor pomocniczy do zestawienia polecenia */

  strcpy(polecenie,"display ");  /* konstrukcja polecenia postaci */
  strcat(polecenie,n_pliku);     /* display "nazwa_pliku" &       */
  strcat(polecenie," &");
  printf("%s\n",polecenie);      /* wydruk kontrolny polecenia */
  system(polecenie);             /* wykonanie polecenia        */
}

/* PRE: podanie struktury bedacej obrazem w P3, na ktorym bedziemy operowac */
/* POST: funkcja zmieni obraz z P3 na P2 w celu pracy na pikselach przy nastepnych funkcjach*/

int zamiana(xobraz *obraz){
  int i,j,k=0, nowaszarosci=0;
  /* deklaracja tablic pomocniczych */
  int(*obraz_pgm)[obraz->wymx*3];
  obraz_pgm=(int(*)[obraz->wymx*3]) obraz->obraz_pgm;
  int(*tab)[obraz->wymx]; 
  tab=(int(*)[obraz->wymx])malloc(obraz->wymx*obraz->wymy*sizeof(int));
  for (i=0;i<obraz->wymy;i++) { /*przypisywanie kolejnym elementom tablicy pomocniczej sredniej arytmetycznej wartosci elementow RGB tablicy obraz_pgm */
    for (j=0;j<obraz->wymx*3;j+=3) {
      tab[i][k]=(obraz_pgm[i][j]+obraz_pgm[i][j+1]+obraz_pgm[i][j+2])/3; 
      if(tab[i][k]>nowaszarosci) nowaszarosci=tab[i][k]; /*znalezienie nowej szarosci */
      k++; 						 /* k sluzace do przemieszczania sie po tab, j do obraz_pgm */
    }
    k=0;
  }
  obraz->szarosci=nowaszarosci;
  /*zapis do tablicy obraz_pgm wartosci elementow z tablicy pomocniczej */
  for (i=0;i<obraz->wymy;i++) {
    for (j=0;j<obraz->wymx;j++) {
      obraz_pgm[i][j]=tab[i][j];
    }
  }
  free(tab);
  return obraz->wymx*obraz->wymy;   /* Zamiana zakonczona sukcesem    */
}

/* PRE: podany prawidlowy uchwyt na plik, struktura obraz i wartosc zmiennej czykolor */
/* POST: funkcja zapisuje obraz w formacie P2 lub, w zaleznosci od wartosci zmiennej czykolor, w formacie P3 */

int zapisz(FILE *plik_we,xobraz *obraz, int czykolor) { 
  int i,j, mnoznik;
  /*dynamiczne alokowanie tablic pomocniczych */
  int(*obraz_pgm)[obraz->wymx];
  obraz_pgm=(int(*)[obraz->wymx]) obraz->obraz_pgm;
  int(*obraz_ppm)[obraz->wymx*3];
  obraz_ppm=(int(*)[obraz->wymx*3]) obraz->obraz_pgm;
  
  mnoznik=obraz->P3*2+1; /*obliczenie wartosci mnoznika: 1 dla P2 i 3 dla P3 */
  if(obraz->P3==1){
  	if (czykolor==1) fprintf(plik_we,"P3\n"); /*zapisanie liczby magicznej P3 dla czykolor rownej 1 */
	else 	{
		mnoznik=1;			  /*w innym przypadku ustawiamy mnoznik na 1 i zapisujemy jako P2 */
		fprintf(plik_we, "P2\n");
		}
  	fprintf(plik_we, "%d %d %d\n", obraz->wymx, obraz->wymy, obraz->szarosci); /*zapisanie wartosci wymiarow i skali szarosci */ 
  	for (i=0;i<obraz->wymy;i++) { 						   /* zapis kolejnych elementow tablicy w pliku */
    		for (j=0;j<obraz->wymx*mnoznik;j++) {
      			fprintf(plik_we, "%d ", obraz_ppm[i][j]);
    			}
      		fprintf(plik_we, "\n");
  	}
 
  } 
  else {			/*w innym przypadku postepujemy jak dla P2 */
  	fprintf(plik_we, "P2\n");	
  	fprintf(plik_we, "%d %d %d\n", obraz->wymx, obraz->wymy, obraz->szarosci); /*zapisanie wartosci wymiarow i skali szarosci */ 
  	for (i=0;i<obraz->wymy;i++) { 						   /* zapis kolejnych elementow tablicy w pliku */
    		for (j=0;j<obraz->wymx;j++) {
      			fprintf(plik_we, "%d ", obraz_pgm[i][j]);
    			}
      		fprintf(plik_we, "\n");
  	}
  }
  return (obraz->wymx)*(obraz->wymy);   /* zapis zakonczony sukcesem    */
}

/*********************************************************************/
/* W przypadku kazdej nastepnej funkcji postepujemy podobnie:        */
/* - obliczamy mnoznik zalezny od wartosci obraz->P3                 */
/* - dynamicznie alokujemy tablice pomocnicze                        */
/* - przypisujemy kolejnym elementom tablicy pomocniczej odpowiednio */
/*   zmodyfikowane wartosci elementow tablicy obraz_pgm              */
/* - zapisujemy do tablicy obraz_pgm wartosci elementow z tab        */
/* - zwracamy ilosc pikseli                                          */
/*********************************************************************/

/* PRE: poprawna struktura bedaca obrazem, na ktorego pikselach bedziemy operowac */
/* POST: otrzymujemy nalozona funkcje negatyw na piksele */

int negatyw(xobraz *obraz)
{
  int i,j, mnoznik;
  mnoznik=2*obraz->P3+1;
  int(*obraz_pgm)[obraz->wymx*mnoznik];
  obraz_pgm=(int(*)[obraz->wymx*mnoznik]) obraz->obraz_pgm;
  int(*tab)[obraz->wymx];
  tab= (int(*)[obraz->wymx])malloc(obraz->wymx*obraz->wymy*sizeof(int)); /*tablica pomocnicza */
  for (i=0;i<obraz->wymy;i++) { /*przypisywanie kolejnym elementom tablicy pomocniczej odpowiednio zmodyfikowanych wartosci elementow tablicy obraz_pgm */
    for (j=0;j<obraz->wymx;j++) {
      tab[i][j]=obraz->szarosci-obraz_pgm[i][j];
    }
  }
  /*zapis do tablicy obraz_pgm wartosci elementow z tablicy pomocniczej */
  for (i=0;i<obraz->wymy;i++) {
    for (j=0;j<obraz->wymx;j++) {
      obraz_pgm[i][j]=tab[i][j];
    }
  }
  free(tab);
  return obraz->wymx*obraz->wymy;   /* Negatyw zakonczony sukcesem    */
}

/* PRE: poprawna struktura bedaca obrazem, na ktorego pikselach bedziemy operowac */
/* POST: otrzymujemy nalozona funkcje konturowania na piksele */

int kontur(xobraz *obraz)
{
  int i,j, mnoznik;
  mnoznik=2*obraz->P3+1;
  int(*obraz_pgm)[obraz->wymx*mnoznik];
  obraz_pgm=(int(*)[obraz->wymx*mnoznik]) obraz->obraz_pgm;
  int(*tab)[obraz->wymx];
  tab= (int(*)[obraz->wymx]) malloc(obraz->wymx*obraz->wymy*sizeof(int)); /*tablica pomocnicza */
    for (i=0;i<obraz->wymy;i++) {  /*przypisywanie kolejnym elementom tablicy pomocniczej odpowiednio zmodyfikowanych wartosci elementow tablicy obraz_pgm */
    for (j=0;j<obraz->wymx;j++) { 
      if(i+1== obraz->wymy || j+1== obraz->wymx) tab[i][j]=obraz_pgm[i][j]; /*sprawdzenie, czy nastepne elementy tablicy beda wychodzily poza zakres, jesli tak, to nie zmianiamy ich wartosci */
      else tab[i][j]=bezwzgl(obraz_pgm[i+1][j]-obraz_pgm[i][j]) + bezwzgl(obraz_pgm[i][j+1]-obraz_pgm[i][j]); /*jesli nie wychodza poza zakres, to wedlug definicji danej funkcji przypisujemy im konkretna wartosc */
    }
  }
  /*przypisanie elementom tablicy obraz_pgm wartosci elementow tablicy pomocniczej */
    for (i=0;i<obraz->wymy;i++) { 
    for (j=0;j<obraz->wymx;j++) {
      obraz_pgm[i][j]=tab[i][j];
    }
  }
    free(tab);
  return obraz->wymx*obraz->wymy;   /* Konturowanie zakonczone sukcesem    */
}

/* PRE: poprawna struktura bedaca obrazem, na ktorego pikselach bedziemy operowac, poprawna wartosc progu od 0 do 100 w % */
/* POST: otrzymujemy nalozona funkcje progowania na piksele */

int prog(xobraz *obraz, int progwczyt)
{
  int i,j, mnoznik;
  mnoznik=2*obraz->P3+1;
  float prog; 
  int(*obraz_pgm)[obraz->wymx*mnoznik];
  obraz_pgm=(int(*)[obraz->wymx*mnoznik]) obraz->obraz_pgm;
  int(*tab)[obraz->wymx];
  tab= (int(*)[obraz->wymx])malloc(obraz->wymx*obraz->wymy*sizeof(int)); /*tablica pomocnicza */
  if(progwczyt > 100 || progwczyt < 0){			/* sprawdzenie poprawnosci wartosci progu */
    fprintf(stderr,"Blad: Nieprawidlowa wartosc progu\n");
    return 0;
  }
  prog=obraz->szarosci*progwczyt/100.0; /*oblicza prog na podstawie wartosci progu podanej przez uzytkownika w procentach */ 
  /* przypisanie kolejnym elementom tablicy pomocniczej odpowiednich wartosci wynikajacych z wartosci elementow tablicy obraz_pgm na podstawie wzoru na element obrazu sprogowanego*/
  for (i=0;i<obraz->wymy;i++) { 
    for (j=0;j<obraz->wymx;j++) {
      if (obraz_pgm[i][j]<=prog) tab[i][j]=0;
      else tab[i][j]=obraz->szarosci;
    }
  }
  /*przypisanie kolejnych eleementow tablicy pomocniczej elementom tablicy obraz_pgm*/
  for (i=0;i<obraz->wymy;i++) {
    for (j=0;j<obraz->wymx;j++) {
      obraz_pgm[i][j]=tab[i][j];
    }
  }
  free(tab);
  return obraz->wymx*obraz->wymy;   /* progowanie zakonczone sukcesem    */
}

/* PRE: poprawna struktura bedaca obrazem, na ktorego pikselach bedziemy operowac */
/* POST: otrzymujemy nalozona funkcje rozmycia na piksele */

int rozmycie(xobraz *obraz)
{
  int i,j, mnoznik;
  mnoznik=2*obraz->P3+1;
  int(*obraz_pgm)[obraz->wymx*mnoznik];
  obraz_pgm=(int(*)[obraz->wymx*mnoznik]) obraz->obraz_pgm;
  int(*tab)[obraz->wymx];
  tab= (int(*)[obraz->wymx])malloc(obraz->wymx*obraz->wymy*sizeof(int)); /*tablica pomocnicza */
   for (i=0;i<obraz->wymy;i++) { /*skorzystanie ze wzoru na kolejne elementy obrazu rozmytego */
    for (j=0;j<obraz->wymx;j++) {
      if (i==0 || (i+1)==obraz->wymy) tab[i][j]=obraz_pgm[i][j]; /* dla przypadku, gdybysmy mieli wyjsc poza wymiary tablicy */
      else tab[i][j]=((obraz_pgm[i-1][j]+obraz_pgm[i][j]+ obraz_pgm[i+1][j])/3);
    }
  }
  /* przypisanie elementow tablicy pomocniczej elementom tablicy obraz_pgm */ 
  for (i=0;i<obraz->wymy;i++) {
    for (j=0;j<obraz->wymx;j++) {
      obraz_pgm[i][j]=tab[i][j];
    }
  }
  free(tab);
  return obraz->wymx*obraz->wymy;   /* rozmycie zakonczone sukcesem    */
}

/* PRE: poprawna struktura bedaca obrazem, na ktorego pikselach bedziemy operowac */
/* POST: otrzymujemy nalozona maske na piksele */

int maska(xobraz *obraz)
{
  int i,j, nowaszarosci=0, mnoznik; /*deklaruje nowaszarosci, dzieki ktorej zbadam skale szarosci nowopowstajacego obrazu */
  mnoznik=2*obraz->P3+1;
  int(*obraz_pgm)[obraz->wymx*mnoznik];
  obraz_pgm=(int(*)[obraz->wymx*mnoznik]) obraz->obraz_pgm;
  int(*tab)[obraz->wymx];
  tab= (int(*)[obraz->wymx])malloc(obraz->wymx*obraz->wymy*sizeof(int)); /*tablica pomocnicza */
  for (i=0;i<obraz->wymy;i++) {
    for (j=0;j<obraz->wymx;j++) {
      if (i==0 || i+1==obraz->wymy || j==0 || j+1==obraz->wymx) tab[i][j]=obraz_pgm[i][j]; /*dla przypadkow, gdzie wyjdziemy poza zakres tablicy*/ 
      else tab[i][j]=(obraz_pgm[i-1][j-1]+obraz_pgm[i-1][j]+obraz_pgm[i-1][j+1]+obraz_pgm[i][j-1]+obraz_pgm[i][j]+obraz_pgm[i][j+1]+obraz_pgm[i+1][j-1]+obraz_pgm[i+1][j]+obraz_pgm[i+1][j+1]); /*obliczamy kolejne elementy tablicy wedlug maski, ktora ma wszedzie wartosci 1 */
      tab[i][j]=tab[i][j]/9; /*normalizacja dla sumy elementow maski, ktorej kazdy element ma warosc 1 */
      if(tab[i][j]>nowaszarosci) nowaszarosci=tab[i][j]; /*zapamietanie nowej wartosci szarosci (najwiekszej znalezionej wartosci) */
    }
  }
  obraz->szarosci=nowaszarosci;/*przypisanie szarosci nowej wartosci */
  /*przerzucenie wartosci z tablicy pomocniczej do obraz_pgm */
  for (i=0;i<obraz->wymy;i++) {
    for (j=0;j<obraz->wymx;j++) {
      obraz_pgm[i][j]=tab[i][j];
    }
  }
  free(tab);
  return obraz->wymx*obraz->wymy;   /* nakladanie maski  zakonczone sukcesem    */
}


/* PRE: poprawna struktura bedaca obrazem, na ktorego pikselach bedziemy operowac, poprawna wartosc bielwczyt i czernwczyt od 0 do wartosci zmiennej szarosci */
/*      oraz bielwczyt>czernwczyt */
/* POST: otrzymujemy nalozona funkcje zmiany poziomow na piksele */

int zmianapoziomow(xobraz *obraz, int czernwczyt, int bielwczyt)
{
  if(czernwczyt<0 || czernwczyt> bielwczyt || bielwczyt>obraz->szarosci){ /*sprawdzenie poprawnosci parametrow */
    fprintf(stderr,"Blad: Nieprawidlowa wartosc bieli lub czerni\n");
    return 0;
  }
  int i,j, mnoznik;
  mnoznik=2*obraz->P3+1;
  int(*obraz_pgm)[obraz->wymx*mnoznik];
  obraz_pgm=(int(*)[obraz->wymx*mnoznik]) obraz->obraz_pgm;
  int(*tab)[obraz->wymx];
  tab= (int(*)[obraz->wymx])malloc(obraz->wymx*obraz->wymy*sizeof(int)); 
  for (i=0;i<obraz->wymy;i++) { /*przypisywanie kolejnym elementom tablicy pomocniczej odpowiednio zmodyfikowanych wartosci elementow tablicy obraz_pgm */
    for (j=0;j<obraz->wymx;j++) {
      if(obraz_pgm[i][j]<=czernwczyt) tab[i][j]=0;
      else if(czernwczyt<obraz_pgm[i][j] && obraz_pgm[i][j]<bielwczyt) (obraz_pgm[i][j] - czernwczyt*obraz->szarosci)/(bielwczyt-czernwczyt);
      else tab[i][j]=obraz->szarosci;
    }
  }
  /*zapis do tablicy obraz_pgm wartosci elementow z tablicy pomocniczej */
  for (i=0;i<obraz->wymy;i++) {
    for (j=0;j<obraz->wymx;j++) {
      obraz_pgm[i][j]=tab[i][j];
    }
  }
  free(tab);
  return obraz->wymx*obraz->wymy;   /* Zmiana poziomow  zakonczona sukcesem    */
}

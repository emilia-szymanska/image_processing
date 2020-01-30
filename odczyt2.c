#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "modul.h"
#include "opcje.h"

int main(int argc, char ** argv) {
  w_opcje opcje; 
  xobraz obraz; 	/*deklaruje struktury, ktorych bede uzywala przy wywolywaniu odpowiednich funkcji */
  int czykolor=0; 	/*zmienna pomocnicza, ktora okresla, czy mamy zmienic P3 na P2, jesli nie bedzie wywolanych funkcji operujacych na pikselach */
    switch(przetwarzaj_opcje(argc, argv, &opcje)) /*switch dzialajacy w zaleznosci od przelacznikow podanych przy wywolywaniu programu */
	{
		/*case 0 dla poprawnego wywolania programu */ 
	case 0: czytaj(opcje.plik_we, &obraz); /*wczytanie pliku */
		if (obraz.P3==1 && opcje.negatyw==0 && opcje.konturowanie==0 && opcje.progowanie==0 && opcje.wyswietlenie==0 && opcje.rozmycie==0 && opcje.maska==0 && opcje.zmianapoziomow==0) czykolor=1;		/*jesli nie zostaly wywolane zadne funkcje operujace na pikselach, to P3 ma byc niezamieniane na P2 */
		else if (obraz.P3==1) zamiana(&obraz); 		/*w przeciwnym razie zostanie zamienione na P2 */
		/*wywolywanie odpowiednich funkcji w zaleznosci od wywolania */
		if(opcje.negatyw ==1) negatyw(&obraz);
		if(opcje.konturowanie==1) kontur(&obraz);
		if(opcje.progowanie==1) prog(&obraz, opcje.w_prog);
		if(opcje.wyswietlenie==1) wyswietl(opcje.nazwa_we);
		if(opcje.rozmycie==1) rozmycie(&obraz);
		if(opcje.maska==1) maska(&obraz);
		if(opcje.zmianapoziomow==1) zmianapoziomow(&obraz, opcje.czern, opcje.biel);
		if(opcje.plik_wy != NULL) zapisz(opcje.plik_wy, &obraz, czykolor);	/*jesli podany zostal plik wyjsciowy, to ma zapisac w nim zmiany */
		fclose(opcje.plik_wy);							/*zamkniecie wpliku wyjsciowego */
	  	break;
	case -1:printf("Niepoprawna opcja\n");	/*   W przypadku innych wartosci zwracanych przez funkcje przetwarzaj_opcje    */
		if (opcje.help==1) break;	/* i jesli juz nie zostalo wykonane wyswietlenie komunikatu przy przelaczniku, */
		komunikat();			/*     zostanie wyswietlony komunikat o problemie (np. niepoprawna opcja)      */
		break;				/*      oraz komunikat opisujacy, jakie jest poprawne wywolanie programu       */
	case -2:printf("Brak nazwy pliku\n");
		if (opcje.help==1) break;
		komunikat();
		break;
	case -3:printf("Brak wartosci\n");
		if (opcje.help==1) break;
		komunikat();
		break;
	case -4:printf("Brak pliku\n");
		if (opcje.help==1) break;
		komunikat();
		break;
    }
  return 0;
}
/*
 Emilia Szymańska przetwarzanie_obrazow2 03.01.2019r.
 Program sluzy do przetwarzania obrazow, wczytywania, wyswietlania, zapisu i nakladania na nich odpowiednich zmian (np. negatywu, konturowania itp.). Po skompilowaniu i konsolidacji (za pomoca komendy make i pliku makefile) trzeba odpowiednio wywolac program a.out.
 Skladnia wywolania programu:
	program {[-i nazwa] [-o nazwa] [-p liczba] [-n] [-k] [-d] [-r] [-m] [-z liczba liczba] [-h]}
-i nazwa: podajemy nazwe pliku, ktory chcemy wczytac
-o nazwa: podajemy nazwe pliku, do ktorego chcemy zapisac przetworzony obraz 
-p liczba: podajemy wartosc progu i wykonujemy funkcje progowania
-n: wykonujemy funkcje negatywu
-k: wykonujemy funkcje konturowania
-d: wyswietlamy obraz (ktory wczytalismy na poczatku, aby wystwielic przetworzony obraz musimy go zapisac i wtedy wyswietlic funkcja display lub ponownie uruchomic nasz program z przelacznikiem -d z zapisanym plikiem jako plik wejsciowy )
-r: wykonujemy funkcje rozmycia
-m: nakladamy maske
-z liczba liczba: wykonujemy funkcje zmiany poziomow
-h: wyswietlamy, jaka jest poprawna skladnia wywolania programu
W przypadku, gdy podamy zly przelacznik, wyswietli sie komunikat mowiacy, ze wybralismy zla opcje oraz wyswtietli poprawna skladnie wywolania.
Odpowiednio do bledow (brak nazwy pliku, brak podania jakiejs wartosci) wyswietla sie komunikat informujacy uzytkowika, czego brakuje i jaka jest skladnia wywolania. Przykladowo dla ./a.out -i pokaze sie komunikat: 
Brak nazwy pliku
Poprawne wywolanie programu: program {[-i nazwa] [-o nazwa] [-p liczba] [-n] [-k] [-d] [-r] [-m] [-z liczba liczba] [-h]}
Analogiczne komunikaty beda w przypadku braku pliku, braku wartosci i niepoprawnej opcji.
Przy podaniu niepoprawnych wartosci progu, bieli i czerni przy funkcjach progowania i zmiany poziomow rowniez otrzymamy powiadomienie o bledzie (Niepoprawna wartosc progu lub Niepoprawna wartosc bieli lub czerni np dla wywolania ./a.out -i kubus.pgm -o kubus -z 160 20 lub ./a.out -i kubus.ppm -o kubus -p 130).
W przypadku plikow o rozszerzeniu .PPM program wyswietla plik w kolorze i jesli nie zostaja nalozone na niego zadne funkcje, to zapisuje w tym samym formacie. W przypadku jednak nakladania negatywu, konturowania itp. plik zamieniany jest z P3 na P2 i wowczas pracujemy i zapisujemy w skali szarosci.
*/
   	

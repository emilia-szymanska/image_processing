obrazy: odczyt2.o modul.o modul.h opcje.o opcje.h
	cc -Xc odczyt2.o modul.o opcje.o
modul.o: modul.c modul.h opcje.h
	cc -Xc -c modul.c
opcje.o: opcje.c opcje.h modul.h
	cc -Xc -c opcje.c
odczyt2.o: odczyt2.c modul.h opcje.h
	cc -Xc -c odczyt2.c

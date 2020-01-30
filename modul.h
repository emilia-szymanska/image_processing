#define MAX 512
#define DL_LINII 1024

typedef struct {
  int wymx, wymy, szarosci;
  void *obraz_pgm;
  int P3;
} xobraz;


int bezwzgl(int);
int czytaj(FILE *, xobraz *);
void wyswietl(char *);
int zapisz(FILE *, xobraz *, int );
int zamiana( xobraz *);
int negatyw( xobraz *);
int kontur( xobraz *);
int prog(xobraz *,  int);
int rozmycie(xobraz *);
int maska(xobraz *);
int zmianapoziomow(xobraz *, int, int);

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include<stdio.h>
#include<string.h>
#include"conio2.h"
#include <stdlib.h>


#define PLANSZA_MIN '1'
#define PLANSZA_MEDIUM '2'
#define PLANSZA_MAX '3'
#define PLANSZA_GRACZA '4'
#define WCZYTANA_PLANSZA '5'
#define STRZALKA_DO_GORY 0x48
#define STRZALKA_DO_DOLU 0x50
#define STRZALKA_W_LEWO 0x4b
#define STRZALKA_W_PRAWO 0x4d
#define WARTOSC_MIN_Y 1
#define WARTOSC_MIN_X 48
#define WARTOSC_MENU_X 4
#define WARTOSC_MENU_Y 1
#define MAX_WARTOSC_PLANSZY 19
#define MEDIUM_WARTOSC_PLANSZY 13
#define MIN_WARTOSC_PLANSZY 9
#define KLOCEK "\376"
#define ZNAK_GWIAZDKA "\305"
#define ZNAK_LEWY_GORNY_ROG "\332"
#define ZNAK_PRAWY_DOLNY_ROG "\331"
#define ZNAK_PRAWY_GORNY_ROG "\277"
#define ZNAK_LEWY_DOLNY_ROG "\300"
#define ZNAK_GORNY_POLA "\302"
#define ZNAK_DOLNY_POLA "\301"
#define ZNAK_LEWY_POLA "\303"
#define ZNAK_PRAWY_POLA "\264"
#define SCIANA "\263"
#define ZNAK_MINUS "\304"
#define ZNAK_SPACJA "\40"
#define KLAWISZ_Q 'q'
#define KLAWISZ_S 's'
#define KLAWISZ_L 'l'
#define KLAWISZ_N 'n'
#define KLAWISZ_I 'i'
#define KLAWISZ_F 'f'
#define KLAWISZ_W 'w'
#define KLAWISZ_U 'u'
#define KLAWISZ_C 'c'
#define KLAWISZ_B 'b'
#define ESCAPE (char)27
#define ENTER 0x0d
#define IDLEN 3

typedef enum MyEnum
{
	WolnePole = 'W',
	ZajetePrzezCzarnego = 'c',
	ZajetePrzezBialego = 'b',
	OkupowanePrzezCzarnego = 'C',
	OkupowanePrzezBialego = 'B',
	KoBialego = 'K',
	KoCzarnego = 'k'
};
//wywolywanie klas 
int RysujPlansze(int poleGry);
void WybierzTrybGry();
void WczytajUstawieniaGry(int coordX, int coordY);
char NarysujZmianyNaPlanszy(int rozmiarPlanszy);
void RysujKursor(int coordX, int coordY, int ktoryGracz);
char Gra();
int CzytajPlansze(const char* plik);
int ZapiszPlansze(const char* plik);
void AlokujPlansze(int rozmiar);
void DealokujPlansze();
int SprawdzZbicie(MyEnum wolne, MyEnum zajetePrzezGracza, MyEnum zajetePrzezPrzeciwnika, MyEnum okupowanePole);
void DodajKursor(int coordX, int coordY);
void DodajDoPlanszy(int coordX, int coordY);
void RysujMenuHandicapu();
int SprawdzKo(int y, int x, MyEnum KoGracza, MyEnum zajetePoleGracza, MyEnum okupowanePole, MyEnum zajetePolePrzeciwnika);

typedef struct {
	MyEnum type;
} Pole;

static Pole** tablicaPol = NULL;
static int rozmiarPol = 0;
static double punktyGraczaCzarnego = 0;
static double punktyGraczaBialego = 0;
static int ktoryGraczGra = 1;
static const char FILEID[IDLEN + 1] = "GRA";

int main() {
	char zn;
	do {
		zn = Gra();
	} while (zn != KLAWISZ_Q);

	textcolor(WHITE);
	return 0;
}
	
int RysujPlansze(int poleGry) { 
	gotoxy(WARTOSC_MIN_X, WARTOSC_MIN_Y);
	for (int i = 1;i < 2*poleGry;i++) {
		for (int j = 1;j < 2*poleGry;j++) {
			if (i == 1 && j == 1) {
				cputs(ZNAK_LEWY_GORNY_ROG);
			}
			else if (i == 2*poleGry - 1 && j == 2*poleGry - 1) {
				cputs(ZNAK_PRAWY_DOLNY_ROG);
			}
			else if (i == 2*poleGry - 1 && j == 1) {
				cputs(ZNAK_LEWY_DOLNY_ROG);
			}
			else if (i == 1 && j == 2*poleGry - 1) {
				cputs(ZNAK_PRAWY_GORNY_ROG);
			}
			else if (i == 1 && j % 2 == 1) {
				cputs(ZNAK_GORNY_POLA);
			}
			else if (i == 2*poleGry-1 && j%2==1) {
				cputs(ZNAK_DOLNY_POLA);
			}
			else if ((i == 1 && j % 2 == 0) || (i == 2*poleGry-1 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 0)) {
				cputs(ZNAK_MINUS);
			}
			else if ((j == 1 && i % 2 == 0) || (j == 2 * poleGry - 1 && i % 2 == 0) || (i % 2 == 0 && j % 2 == 1)) {
				cputs(SCIANA);
			}
			else if (j == 1 && i % 2 == 1) {
				cputs(ZNAK_LEWY_POLA);
			}
			else if (j == 2 * poleGry - 1 && i % 2 == 1) {
				cputs(ZNAK_PRAWY_POLA);
			}
			else if (i % 2 == 1 && j % 2 == 1) {
				cputs(ZNAK_GWIAZDKA);
			}
			else if (i % 2 == 0 && j % 2 == 0) {
				cputs(ZNAK_SPACJA);
			}
			gotoxy(WARTOSC_MIN_X + j, i);
		}
		gotoxy(WARTOSC_MIN_X, WARTOSC_MIN_Y + i);
	}
	return 0;
}
void WybierzTrybGry() { //graficzne wyswietlanie wyboru planszy
	textcolor(WHITE);
	gotoxy(WARTOSC_MENU_X-3, WARTOSC_MENU_Y);
	cputs("Choose game option:");

	gotoxy(WARTOSC_MENU_X-3, WARTOSC_MENU_Y+1);
	cputs("1  = Plansza 9x9");

	gotoxy(WARTOSC_MENU_X-3, WARTOSC_MENU_Y+2);
	cputs("2  = Plansza 13x13");

	gotoxy(WARTOSC_MENU_X-3, WARTOSC_MENU_Y+3);
	cputs("3  = Plansza 19x19");
	
	gotoxy(WARTOSC_MENU_X - 3, WARTOSC_MENU_Y + 4);
	cputs("4  = Your gameboard size:");
	
	gotoxy(WARTOSC_MENU_X - 3, WARTOSC_MENU_Y + 7);
	cputs("After clicking option number '4' adjust your own size of a board by up and down arrows. Number shown on the screen");
	gotoxy(WARTOSC_MENU_X - 3, WARTOSC_MENU_Y + 8);
	cputs("corresponds to the vertical and horizontal size of the board. Confirm your choice by pressing 'enter'.");
	_setcursortype(_NOCURSOR);
}
void WczytajUstawieniaGry(int coordX, int coordY) { //wyswietla menu gry w oknie z plansza
	
	_setcursortype(_NOCURSOR);
	char punktyC[40]; char punktyB[40]; char coordXY[50];
	textbackground(BLACK);
	textcolor(WHITE);
	
	gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y);
	cputs("Mateusz, Michalski, 193355");
	gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y+1);
	cputs("arrows  = moving");
	gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y+2);
	cputs("q       = quit the program");
	gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y+3);
	cputs("n       = start a new game");
	gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y+4);
	cputs("enter   = confirm your turn");
	gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y+5);
	cputs("esc     = cancel current action");
	gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y+6);
	cputs("i       = place a stone on the board");
	gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y+7);
	cputs("s       = save game");
	gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y+8);
	cputs("l       = load the game state");
	gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y+9);
	cputs("f       = finish the game");
	gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y + 11);
	sprintf(punktyC, "Black score %.1f", punktyGraczaCzarnego);
	puts(punktyC);
	gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y + 12);
	sprintf(punktyB, "White score %.1f", punktyGraczaBialego);
	puts(punktyB);
	gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y + 13);
	sprintf(coordXY, "Coordinates (x,y): ( %d , %d )", coordX+1, coordY+1);
	puts(coordXY);
	gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y + 24);
	cputs("Points:a,b,c,d,e,f,g,h,i,k,l");

}
char NarysujZmianyNaPlanszy(int rozmiarPlanszy) {//rysowanie planszy po zbiciu
	
	for (int j = 0;j < rozmiarPlanszy;j++) {
		for (int i = 0;i < rozmiarPlanszy;i++) {
			gotoxy(WARTOSC_MIN_X+2*i, WARTOSC_MIN_Y+2*j);
			if (tablicaPol[j][i].type == ZajetePrzezBialego) {
				textcolor(WHITE);
				cputs(KLOCEK);
			}
			else if (tablicaPol[j][i].type == ZajetePrzezCzarnego) {
				textcolor(BLACK);
				cputs(KLOCEK);
			}
			else if (tablicaPol[j][i].type == OkupowanePrzezCzarnego || tablicaPol[j][i].type == OkupowanePrzezBialego) {
				textcolor(LIGHTBLUE);
				
				if (i == 0 && j == 0) {
					cputs(ZNAK_LEWY_GORNY_ROG);
				}
				else if (j == rozmiarPlanszy - 1 && i == rozmiarPlanszy - 1) {
					cputs(ZNAK_PRAWY_DOLNY_ROG);
				}
				else if (j == rozmiarPlanszy - 1 && i == 0) {
					cputs(ZNAK_LEWY_DOLNY_ROG);
				}
				else if (j == 0 && i == rozmiarPlanszy - 1) {
					cputs(ZNAK_PRAWY_GORNY_ROG);
				}
				else if (j == 0 && i % 2 == 0) {
					cputs(ZNAK_GORNY_POLA);
				}
				else if (j == rozmiarPlanszy - 1 && i % 2 == 0) {
					cputs(ZNAK_DOLNY_POLA);
				}
				else if (i == 0 && j % 2 == 0) {
					cputs(ZNAK_LEWY_POLA);
				}
				else if (i == rozmiarPlanszy-1 && j % 2 == 0) {
					cputs(ZNAK_PRAWY_POLA);
				}
				else if ((j!=0 && i%2==0) && (j!=rozmiarPlanszy-1 && i%2==0) && (i!=0 && j%2==0) && (i!=rozmiarPlanszy-1 && j%2==0)) {
					cputs(ZNAK_GWIAZDKA);
				}
			}
		}
	}
	return 0;
}
void RysujKursor(int coordX, int coordY, int ktoryGracz) {//przemieszczanie sie kursora
	if (ktoryGracz % 2 == 0) {
		gotoxy(2*coordX+WARTOSC_MIN_X, 2*coordY+WARTOSC_MIN_Y);
		textbackground(WHITE);
		putch(' ');
		gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y + 10);
		textbackground(BLACK);
		textcolor(RED);
		cputs("White's player turn");
	}
	else if (ktoryGracz % 2 == 1) {
		gotoxy(2*coordX+WARTOSC_MIN_X, 2*coordY+WARTOSC_MIN_Y);
		textbackground(DARKGRAY);
		putch(' ');
		gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y+10);
		textbackground(BLACK);
		textcolor(RED);
		cputs("Black's player turn");
	}
	wherex();
	wherey();
}
char Gra() { //cala rozgrywka
	int zn = 0, x = 0, y = 0; char planszaWybranaPrzezGracza[100]; char wynikkoncowy[50];
	punktyGraczaBialego = 0;
	punktyGraczaCzarnego = 0;
#ifndef __cplusplus
	Conio2_Init();
#endif
	settitle("Go!");
	WybierzTrybGry();
	//czesc o wybieraniu planszy
	zn = getch();
	if (zn == PLANSZA_MIN) {
		AlokujPlansze(MIN_WARTOSC_PLANSZY);
	}
	else if (zn == PLANSZA_MEDIUM) {
		AlokujPlansze(MEDIUM_WARTOSC_PLANSZY);
	}
	else if (zn == PLANSZA_MAX) {
		AlokujPlansze(MAX_WARTOSC_PLANSZY);
	}
	else if (zn == PLANSZA_GRACZA) {
		DealokujPlansze();
		while (zn != ENTER) {
			zn = getch();
			if (zn == STRZALKA_DO_GORY) {
				rozmiarPol++;
			}
			else if (zn == STRZALKA_DO_DOLU) {
				rozmiarPol--;
				if (rozmiarPol < 0) {
					rozmiarPol = 0;
				}
			}
			else if (zn == KLAWISZ_Q) {
				return KLAWISZ_Q;
			}
			gotoxy(WARTOSC_MENU_X + 25, WARTOSC_MENU_Y + 4);
			sprintf(planszaWybranaPrzezGracza, "Your board : %d x %d", rozmiarPol, rozmiarPol);
			puts(planszaWybranaPrzezGracza);
		}
		
		if (rozmiarPol == 0 || rozmiarPol == 1) {
			return KLAWISZ_Q;
		}
		else {
			AlokujPlansze(rozmiarPol);
		}
	}
	clrscr();
	while (zn != ESCAPE) {//czesc o customowym trybie gry
		textbackground(LIGHTGRAY);
		textcolor(LIGHTBLUE);
		RysujPlansze(rozmiarPol);
		NarysujZmianyNaPlanszy(rozmiarPol);
		RysujMenuHandicapu();
		DodajKursor(x, y);
		textbackground(BLACK);
		zn = getch();
		if (zn == 0) {
			zn = getch();
			if (zn == STRZALKA_DO_GORY) y -= 1;
			if (y < 0) y = 0;
			else if (zn == STRZALKA_DO_DOLU) y += 1;
			if (y >= rozmiarPol) y = rozmiarPol - 1;
			else if (zn == STRZALKA_W_LEWO) x -= 1;
			if (x < 0) x = 0;
			else if (zn == STRZALKA_W_PRAWO) x += 1;
			if (x >= rozmiarPol) x = rozmiarPol - 1;
		}
		else if (zn == KLAWISZ_B) {
			tablicaPol[y][x].type = ZajetePrzezCzarnego;
		}
		else if (zn == KLAWISZ_W) {
			punktyGraczaBialego += 0.5;
		}
		else if (zn == KLAWISZ_S) {
			punktyGraczaBialego -= 0.5;
			if (punktyGraczaBialego < 0) {
				punktyGraczaBialego = 0;
			}
		}
		else if (zn == KLAWISZ_U) {
			if (tablicaPol[y][x].type == ZajetePrzezBialego || tablicaPol[y][x].type == ZajetePrzezCzarnego) {
				tablicaPol[y][x].type = WolnePole;
			}
		}
		else if (zn == KLAWISZ_C) {
			tablicaPol[y][x].type = ZajetePrzezBialego;
		}
		textbackground(BLACK);
		ktoryGraczGra = 1;
	}
	if (SprawdzZbicie(WolnePole, ZajetePrzezBialego, ZajetePrzezCzarnego, OkupowanePrzezBialego) == 1) {
		punktyGraczaBialego++;
	}
	if (SprawdzZbicie(WolnePole, ZajetePrzezCzarnego, ZajetePrzezBialego, OkupowanePrzezCzarnego) == 1) {
		punktyGraczaCzarnego++;
	}
	y = 0;
	x = 0;
	clrscr();
	_setcursortype(_NOCURSOR);
	textbackground(BLACK);
	do { //ca³a rozgrywka
		textcolor(LIGHTBLUE);
		textbackground(LIGHTGRAY);
		RysujPlansze(rozmiarPol);
		NarysujZmianyNaPlanszy(rozmiarPol);
		WczytajUstawieniaGry(x,y);
		RysujKursor(x, y, ktoryGraczGra);
		textbackground(BLACK);
		zn = getch();
		if (zn == 0) { //przesuwanie sie kursora
			zn = getch();
			if (zn == STRZALKA_DO_GORY) y -= 1;
			if (y < 0) y = 0;
			else if (zn == STRZALKA_DO_DOLU) y += 1;
			if (y >= rozmiarPol) y = rozmiarPol-1;
			else if (zn == STRZALKA_W_LEWO) x -= 1;
			if (x < 0) x = 0;
			else if (zn == STRZALKA_W_PRAWO) x += 1;
			if (x >= rozmiarPol) x = rozmiarPol-1;

		}
		else if (zn == KLAWISZ_I) { //dodawanie klocka i sprawdzanie zasad
			zn = getch();
			if (zn == ENTER) {
				DodajDoPlanszy(x, y);
				if (SprawdzZbicie(WolnePole, ZajetePrzezBialego, ZajetePrzezCzarnego, OkupowanePrzezBialego) == 1) {
					punktyGraczaBialego++;
				}
				if (SprawdzZbicie(WolnePole, ZajetePrzezCzarnego, ZajetePrzezBialego, OkupowanePrzezCzarnego) == 1) {
					punktyGraczaCzarnego++;
				}
				if (SprawdzKo(y, x, KoCzarnego, ZajetePrzezCzarnego, OkupowanePrzezBialego, ZajetePrzezBialego) == 1) {
					punktyGraczaCzarnego++;
				};
				if (SprawdzKo(y, x, KoBialego, ZajetePrzezBialego, OkupowanePrzezCzarnego, ZajetePrzezCzarnego) == 1) {
					punktyGraczaBialego++;
				} 
			}
			else if (zn == ESCAPE) {
				ungetch(zn);
			}
		}
		else if (zn == KLAWISZ_N) { //tworzenie nowej gry
			
			zn = getch();
			if (zn == ENTER) {
				clrscr();
				return zn;
			}
			else if (zn == ESCAPE) {
				ungetch(zn);
			}
		}
		else if (zn == KLAWISZ_F) { //konczenie gry w danym momencie//podsumowanie wyniku oraz wyswietlenie zwyciezcy
			zn = getch();
			if (zn == ENTER) {
				textcolor(LIGHTRED);
				gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y + 17);
				punktyGraczaBialego = 0;
				punktyGraczaCzarnego = 0;
				for (int i = 0;i < rozmiarPol;i++){
					for (int j = 0;j < rozmiarPol;j++) {
						if (tablicaPol[j][i].type == OkupowanePrzezBialego || tablicaPol[j][i].type == KoBialego) {
							punktyGraczaBialego++;
						}
						else if (tablicaPol[j][i].type == OkupowanePrzezCzarnego || tablicaPol[j][i].type == KoCzarnego) {
							punktyGraczaCzarnego++;
						}
					}
				}
				if (punktyGraczaBialego > punktyGraczaCzarnego) {
					sprintf(wynikkoncowy, "White player won and scored: %.1f", punktyGraczaBialego);
					puts(wynikkoncowy);
				}
				else if (punktyGraczaBialego < punktyGraczaCzarnego) {
					sprintf(wynikkoncowy, "Black player won and scored: %.1f", punktyGraczaCzarnego);
					puts(wynikkoncowy);
				}
				else {
					cputs("Tie");
				}
				gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y + 15);
				cputs("Press 'n' and confirm to start a new game");
			}
			else if (zn == ESCAPE) {
				ungetch(zn);
			}
		}
		else if (zn == KLAWISZ_Q) {//wyjscie z programu
			return KLAWISZ_Q;
		}
		else if (zn == KLAWISZ_S) {	//zapis gry
			char tablica[50] = {};
			int i = 0;
			gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y + 15);
			cputs("File path: ");
			gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y + 16);

			while (i < 49) {
				zn = getch();
				if (zn == ENTER) {
					break;
				}
				tablica[i] = zn;
				putch(zn);
				gotoxy(WARTOSC_MENU_X + 1 + i, WARTOSC_MENU_Y + 16);
				i++;
			}
			tablica[i] = 0x0;
			ZapiszPlansze(tablica);
			gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y + 15);
			cputs("            ");
			gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y + 16);
			cputs("                                  ");
		}
		else if (zn == KLAWISZ_L) {		//wczytywanie gry
			char tablica[50] = {};
			int i = 0;
			gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y + 15);
			cputs("                              ");
			gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y + 15);
			cputs("File path: ");
			gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y + 16);
			cputs("                              ");
			gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y + 16);
			while (i<49) {
				zn = getch();
				if (zn == ENTER) {
					break;
				}
				
				tablica[i] = zn;
				putch(zn);
				gotoxy(WARTOSC_MENU_X+1+i, WARTOSC_MENU_Y + 16);
				i++;
			}
			tablica[i] = 0x0;
			clrscr();
			if (CzytajPlansze(tablica) == 0) { //jezeli 0 to plik nie istnieje wiec komunikat
				gotoxy(WARTOSC_MENU_X,WARTOSC_MENU_Y+16);
				cputs("Unable to load the file");
			}
			else if (CzytajPlansze(tablica) == 1) { //czytanie z pliku
				continue;
			}
		}		
	}
	while (true);
}
int SprawdzZbicie(MyEnum wolne, MyEnum zajetePrzezGracza, MyEnum zajetePrzezPrzeciwnika, MyEnum okupowanePole) { //sprawdzanie zbijania 1 pola
	for (int y = 0;y < rozmiarPol; y++) {
		for (int x = 0;x < rozmiarPol; x++) {
			if (y == 0 && x == 0) {
				if ((tablicaPol[y][x].type == zajetePrzezPrzeciwnika) && tablicaPol[y + 1][x].type == zajetePrzezGracza && tablicaPol[y][x + 1].type == zajetePrzezGracza) {
					tablicaPol[y][x].type = okupowanePole;
					return 1;
				}
				else if (tablicaPol[y][x].type == wolne && tablicaPol[y + 1][x].type == zajetePrzezGracza && tablicaPol[y][x + 1].type == zajetePrzezGracza){
					tablicaPol[y][x].type = okupowanePole;
				}
			}
			else if (y == rozmiarPol-1 && x == 0){
				if (tablicaPol[y][x].type == zajetePrzezPrzeciwnika && tablicaPol[y - 1][x].type == zajetePrzezGracza && tablicaPol[y][x + 1].type == zajetePrzezGracza) {
					tablicaPol[y][x].type = okupowanePole;
					return 1;
				}
				else if (tablicaPol[y][x].type == wolne && tablicaPol[y - 1][x].type == zajetePrzezGracza && tablicaPol[y][x + 1].type == zajetePrzezGracza) {
					tablicaPol[y][x].type = okupowanePole;
				}
			}
			else if (y == rozmiarPol - 1 && x == rozmiarPol - 1) {
				if (tablicaPol[y][x].type == zajetePrzezPrzeciwnika && tablicaPol[y - 1][x].type == zajetePrzezGracza && tablicaPol[y][x - 1].type == zajetePrzezGracza) {
					tablicaPol[y][x].type = okupowanePole;
					return 1;
				}
				else if (tablicaPol[y][x].type == wolne && tablicaPol[y - 1][x].type == zajetePrzezGracza && tablicaPol[y][x - 1].type == zajetePrzezGracza) {
					tablicaPol[y][x].type = okupowanePole;
				}
			}
			else if (y == 0 && x == rozmiarPol - 1) {
				if (tablicaPol[y][x].type == zajetePrzezPrzeciwnika && tablicaPol[y + 1][x].type == zajetePrzezGracza && tablicaPol[y][x - 1].type == zajetePrzezGracza) {
					tablicaPol[y][x].type = okupowanePole;
					return 1;
				}
				else if (tablicaPol[y][x].type == wolne && tablicaPol[y + 1][x].type == zajetePrzezGracza && tablicaPol[y][x - 1].type == zajetePrzezGracza) {
					tablicaPol[y][x].type = okupowanePole;
				}
			}
			else if (y == 0) {
				if (tablicaPol[y][x].type == zajetePrzezPrzeciwnika && tablicaPol[y][x - 1].type == zajetePrzezGracza && tablicaPol[y][x + 1].type == zajetePrzezGracza && tablicaPol[y + 1][x].type == zajetePrzezGracza) {					
					tablicaPol[y][x].type = okupowanePole;
					return 1;
				}
				else if (tablicaPol[y][x].type == wolne && tablicaPol[y][x - 1].type == zajetePrzezGracza && tablicaPol[y][x + 1].type == zajetePrzezGracza && tablicaPol[y + 1][x].type == zajetePrzezGracza) {
					tablicaPol[y][x].type = okupowanePole;
				}
			}
			else if (y == rozmiarPol - 1) {
				if (tablicaPol[y][x].type == zajetePrzezPrzeciwnika && tablicaPol[y][x - 1].type == zajetePrzezGracza && tablicaPol[y][x + 1].type == zajetePrzezGracza && tablicaPol[y - 1][x].type == zajetePrzezGracza) {
					tablicaPol[y][x].type = okupowanePole;
					return 1;
				}
				else if (tablicaPol[y][x].type == wolne && tablicaPol[y][x - 1].type == zajetePrzezGracza && tablicaPol[y][x + 1].type == zajetePrzezGracza && tablicaPol[y - 1][x].type == zajetePrzezGracza) {
					tablicaPol[y][x].type = okupowanePole;
				}
			}
			else if (x == 0) {
				if (tablicaPol[y][x].type == zajetePrzezPrzeciwnika && tablicaPol[y - 1][x].type == zajetePrzezGracza && tablicaPol[y][x + 1].type == zajetePrzezGracza && tablicaPol[y + 1][x].type == zajetePrzezGracza) {
					tablicaPol[y][x].type = okupowanePole;
					return 1;
				}
				else if (tablicaPol[y][x].type == wolne && tablicaPol[y - 1][x].type == zajetePrzezGracza && tablicaPol[y][x + 1].type == zajetePrzezGracza && tablicaPol[y + 1][x].type == zajetePrzezGracza) {
					tablicaPol[y][x].type = okupowanePole;
				}
			}
			else if (x == rozmiarPol - 1) {
				if (tablicaPol[y][x].type == zajetePrzezPrzeciwnika && tablicaPol[y][x - 1].type == zajetePrzezGracza && tablicaPol[y + 1][x].type == zajetePrzezGracza && tablicaPol[y - 1][x].type == zajetePrzezGracza) {
					tablicaPol[y][x].type = okupowanePole;
					return 1;
				}
				else if (tablicaPol[y][x].type == wolne && tablicaPol[y][x - 1].type == zajetePrzezGracza && tablicaPol[y + 1][x].type == zajetePrzezGracza && tablicaPol[y - 1][x].type == zajetePrzezGracza) {
					tablicaPol[y][x].type = okupowanePole;
				}
			}
			else {	//zbijanie z klockami z 4 stron
				if (tablicaPol[y][x].type == zajetePrzezPrzeciwnika && tablicaPol[y][x - 1].type == zajetePrzezGracza && tablicaPol[y][x + 1].type == zajetePrzezGracza && tablicaPol[y - 1][x].type == zajetePrzezGracza && tablicaPol[y + 1][x].type == zajetePrzezGracza) {
				tablicaPol[y][x].type = okupowanePole;
				return 1;
				}
				else if (tablicaPol[y][x].type == wolne && tablicaPol[y][x - 1].type == zajetePrzezGracza && tablicaPol[y][x + 1].type == zajetePrzezGracza && tablicaPol[y - 1][x].type == zajetePrzezGracza && tablicaPol[y + 1][x].type == zajetePrzezGracza) {
				tablicaPol[y][x].type = okupowanePole;
				}
			}
		}
	}
	return 0;
}
void DealokujPlansze() {//funkcja zwalnia pamiec dla tablicaPol, cala operacja zachodzi odwrotnie co do alokacji pamieci do tablicaPol
	if (tablicaPol && rozmiarPol) { //sprawdzanie czy nie jest pelne
		for (int y = 0; y < rozmiarPol; y++) {
			free(tablicaPol[y]); //zwalnianie pamieci
		}
		free(tablicaPol); //zwalnianie pamieci
		tablicaPol = NULL;
		rozmiarPol = 0;
	}
}
void AlokujPlansze(int rozmiar) {		//alokuje pamiec dla tablicy tablicaPol
	if (tablicaPol) {
		DealokujPlansze();				//zwalnianie pamieci jezeli potrzeba
	}
	rozmiarPol = rozmiar;
	tablicaPol = (Pole**)malloc(rozmiar * sizeof(Pole*));
	for (int y = 0; y < rozmiar; y++) {
		tablicaPol[y] = (Pole*)malloc(rozmiar * sizeof(Pole));		//wskaznik na wskaznik
		for (int x = 0; x < rozmiar; x++) {
			tablicaPol[y][x].type = WolnePole;						//przypisanie wartosci poczatkowej pola 
		}
	}
}
int ZapiszPlansze(const char* plik) {
	if (tablicaPol && rozmiarPol) {
		FILE* f = fopen(plik, "w+"); //nadpisywanie // jezeli plik istnieje to nadpisuje, jezeli nie istnieje to jest tworzony

		if (f) {
			fwrite(FILEID, strlen(FILEID), 1, f);		//zapisywanie identyfikatora pliku i innych elementow
			fwrite(&rozmiarPol, sizeof(rozmiarPol), 1, f);		
			fwrite(&punktyGraczaBialego, sizeof(double), 1, f);		
			fwrite(&punktyGraczaCzarnego, sizeof(double), 1, f);
			fwrite(&ktoryGraczGra, sizeof(int), 1, f);

			for (int y = 0; y < rozmiarPol; y++) {
				fwrite(tablicaPol[y], sizeof(Pole), rozmiarPol, f);	//zapis kazdej komorki z tablicy
			}
			fclose(f);
			return 1;
		}
	}
	return 0;														//wartosc 0 jak sie nie uda , wartosc 1 jak sie uda
}
int CzytajPlansze(const char* plik) {
	if (tablicaPol && rozmiarPol){
	FILE* f = fopen(plik, "r");
		if (f) {
			int rozmiar = 0;
			char fileid[IDLEN+1];

			fread(fileid, strlen(FILEID), 1, f);				//analogicznie jak ZapiszPlansze() tylko tutaj odczyt
																//memcmp sprawdza zgodnosc bajtow pamieci i zwraca 0 jezeli jest zgodna
			if (memcmp(FILEID, fileid, strlen(FILEID)) == 0) {	//sprawdzenie czy identyfikator sie zgadza
			
				fread(&rozmiar, sizeof(rozmiar), 1, f);				

				if (rozmiar > 0) {
					fread(&punktyGraczaBialego, sizeof(double), 1, f);
					fread(&punktyGraczaCzarnego, sizeof(double), 1, f);
					fread(&ktoryGraczGra, sizeof(int), 1, f);

					AlokujPlansze(rozmiar);

					for (int y = 0; y < rozmiarPol; y++) {
						fread(tablicaPol[y], sizeof(Pole), rozmiarPol, f);
					}
					fclose(f);
					return 1;
				}
			}
			fclose(f);
		}
	}
	return 0;
}
void DodajKursor(int coordX, int coordY) { //dodawanie kursora do trybu handicap
	_setcursortype(_NOCURSOR);
	gotoxy(2 * coordX + WARTOSC_MIN_X, 2 * coordY + WARTOSC_MIN_Y);
	textbackground(GREEN);
	putch(' ');
	wherex();
	wherey();
}
void DodajDoPlanszy(int coordX, int coordY) { //dodawanie klockow
	if (tablicaPol[coordY][coordX].type == WolnePole) {
		if (ktoryGraczGra % 2 == 0) {
			if (tablicaPol[coordY][coordX].type != OkupowanePrzezCzarnego) {
				tablicaPol[coordY][coordX].type = ZajetePrzezBialego;
				ktoryGraczGra++;
			}
		}
		else if (ktoryGraczGra % 2 == 1) {
			if (tablicaPol[coordY][coordX].type != OkupowanePrzezBialego) {
				tablicaPol[coordY][coordX].type = ZajetePrzezCzarnego;
				ktoryGraczGra++;
			}
		}
	}
	else if (tablicaPol[coordY][coordX].type == OkupowanePrzezBialego) {
		if (ktoryGraczGra % 2 == 0) {
			tablicaPol[coordY][coordX].type = ZajetePrzezBialego;
			ktoryGraczGra++;
		}	
	}
	else if (tablicaPol[coordY][coordX].type == OkupowanePrzezCzarnego) {
		
		if (ktoryGraczGra % 2 == 1) {
			tablicaPol[coordY][coordX].type = ZajetePrzezCzarnego;
			ktoryGraczGra++;
		}
	}
	else if (tablicaPol[coordY][coordX].type == KoBialego) {
		if (ktoryGraczGra % 2 == 0) {
			tablicaPol[coordY][coordX].type = ZajetePrzezBialego;
			ktoryGraczGra++;
		}
	}
	else if (tablicaPol[coordY][coordX].type == KoCzarnego) {
		if (ktoryGraczGra % 2 == 1) {
			tablicaPol[coordY][coordX].type = ZajetePrzezCzarnego;
			ktoryGraczGra++;
		}
	}
}
void RysujMenuHandicapu() {		//wyrysowywanie menu handicap
	char komi[50];
	textbackground(BLACK);
	textcolor(LIGHTRED);
	gotoxy(WARTOSC_MENU_X+5, WARTOSC_MENU_Y);
	cputs("WELCOME IN THE HANDICAP MODE");
	textcolor(LIGHTBLUE);
	gotoxy(WARTOSC_MENU_X-3, WARTOSC_MENU_Y+1);
	cputs("This window is designed for inserting stones");
	gotoxy(WARTOSC_MENU_X-3, WARTOSC_MENU_Y+2);
	cputs("for black player to have an adventage over ");
	gotoxy(WARTOSC_MENU_X-3, WARTOSC_MENU_Y+3);
	cputs("a white player.");
	gotoxy(WARTOSC_MENU_X-3, WARTOSC_MENU_Y+5);
	cputs("Start inserting stones and  adjust the 'komi'");
	gotoxy(WARTOSC_MENU_X-3, WARTOSC_MENU_Y+6);
	cputs("for a white player if you intend to do so.");
	gotoxy(WARTOSC_MENU_X+15, WARTOSC_MENU_Y + 8);
	textcolor(LIGHTRED);
	cputs("ACTIONS");
	textcolor(LIGHTBLUE);
	gotoxy(WARTOSC_MENU_X - 3, WARTOSC_MENU_Y + 10);
	cputs("b      = insert black stone");
	gotoxy(WARTOSC_MENU_X - 3, WARTOSC_MENU_Y + 11);
	cputs("c      = insert white stone");
	gotoxy(WARTOSC_MENU_X - 3, WARTOSC_MENU_Y + 12);
	cputs("escape = go to the play mode");
	gotoxy(WARTOSC_MENU_X - 3, WARTOSC_MENU_Y + 13);
	cputs("To undo the misplaced stone go to location of");
	gotoxy(WARTOSC_MENU_X - 3, WARTOSC_MENU_Y + 14);
	cputs("the stone and press 'u'");
	gotoxy(WARTOSC_MENU_X - 3, WARTOSC_MENU_Y + 15);
	cputs("w      = add 'komi'");
	gotoxy(WARTOSC_MENU_X - 3, WARTOSC_MENU_Y + 16);
	cputs("s      = substract 'komi'");
	gotoxy(WARTOSC_MENU_X, WARTOSC_MENU_Y + 18);
	sprintf(komi, "Additional points for white player: %.1f", punktyGraczaBialego);
	puts(komi);

}
int SprawdzKo(int y, int x, MyEnum KoGracza, MyEnum zajetePoleGracza, MyEnum okupowanePole, MyEnum zajetePolePrzeciwnika) {	//sprawdzenie zasady Ko w zaleznosci od polozenia na planszy
	if (y == 0) {
		if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y][x + 1].type == zajetePolePrzeciwnika && tablicaPol[y][x + 2].type == zajetePoleGracza && tablicaPol[y + 1][x + 1].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y][x + 1].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
		else if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y][x - 1].type == zajetePolePrzeciwnika && tablicaPol[y][x - 2].type == zajetePoleGracza && tablicaPol[y + 1][x - 1].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y][x - 1].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
		else if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y + 1][x].type == zajetePolePrzeciwnika && tablicaPol[y + 2][x].type == zajetePoleGracza && tablicaPol[y + 1][x + 1].type == zajetePoleGracza && tablicaPol[y + 1][x - 1].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y + 1][x].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}

	}
	else if (y == rozmiarPol - 1) {
		if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y][x + 1].type == zajetePolePrzeciwnika && tablicaPol[y][x + 2].type == zajetePoleGracza && tablicaPol[y - 1][x + 1].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y][x + 1].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
		else if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y][x - 1].type == zajetePolePrzeciwnika && tablicaPol[y][x - 2].type == zajetePoleGracza && tablicaPol[y - 1][x - 1].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y][x - 1].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
		else if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y - 1][x].type == zajetePolePrzeciwnika && tablicaPol[y - 2][x].type == zajetePoleGracza && tablicaPol[y - 1][x + 1].type == zajetePoleGracza && tablicaPol[y - 1][x - 1].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y - 1][x].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
	}
	else if (x == 0) {
		if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y][x + 1].type == zajetePolePrzeciwnika && tablicaPol[y][x + 2].type == zajetePoleGracza && tablicaPol[y + 1][x + 1].type == zajetePoleGracza && tablicaPol[y - 1][x + 1].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y][x + 1].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
		else if (y < rozmiarPol - 2 && tablicaPol[y][x].type == okupowanePole && tablicaPol[y + 1][x].type == zajetePolePrzeciwnika && tablicaPol[y + 2][x].type == zajetePoleGracza && tablicaPol[y + 1][x + 1].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y + 1][x].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
		else if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y - 1][x].type == zajetePolePrzeciwnika && tablicaPol[y - 2][x].type == zajetePoleGracza && tablicaPol[y - 1][x + 1].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y - 1][x].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
	}
	else if (x == rozmiarPol - 1) {
		if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y][x - 1].type == zajetePolePrzeciwnika && tablicaPol[y][x - 2].type == zajetePoleGracza && tablicaPol[y + 1][x - 1].type == zajetePoleGracza && tablicaPol[y - 1][x - 1].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y][x - 1].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
		else if (y > 2 && tablicaPol[y][x].type == okupowanePole && tablicaPol[y - 1][x].type == zajetePolePrzeciwnika && tablicaPol[y - 2][x].type == zajetePoleGracza && tablicaPol[y - 1][x - 1].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y - 1][x].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
		else if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y + 1][x].type == zajetePolePrzeciwnika && tablicaPol[y + 2][x].type == zajetePoleGracza && tablicaPol[y + 1][x - 1].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y + 1][x].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
	}
	else if (y == 1) {
		if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y - 1][x].type == zajetePolePrzeciwnika && tablicaPol[y - 1][x + 1].type == zajetePoleGracza && tablicaPol[y - 1][x - 1].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y - 1][x].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
		else if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y + 1][x].type == zajetePolePrzeciwnika && tablicaPol[y + 1][x + 1].type == zajetePoleGracza && tablicaPol[y + 1][x - 1].type == zajetePoleGracza && tablicaPol[y + 2][x].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y + 1][x].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
	}
	else if (y == rozmiarPol-2) {
		if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y + 1][x].type == zajetePolePrzeciwnika && tablicaPol[y + 1][x + 1].type == zajetePoleGracza && tablicaPol[y + 1][x - 1].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y + 1][x].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
		else if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y - 1][x].type == zajetePolePrzeciwnika && tablicaPol[y - 1][x + 1].type == zajetePoleGracza && tablicaPol[y - 1][x - 1].type == zajetePoleGracza && tablicaPol[y - 2][x].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y - 1][x].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
	}
	else if (x==1) {
		if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y][x-1].type == zajetePolePrzeciwnika && tablicaPol[y - 1][x - 1].type == zajetePoleGracza && tablicaPol[y + 1][x - 1].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y][x-1].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
		if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y][x + 1].type == zajetePolePrzeciwnika && tablicaPol[y + 1][x + 1].type == zajetePoleGracza && tablicaPol[y - 1][x + 1].type == zajetePoleGracza && tablicaPol[y][x + 2].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y][x + 1].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
	}
	else if (x == rozmiarPol-2) {
		if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y - 1][x].type == zajetePolePrzeciwnika && tablicaPol[y - 1][x + 1].type == zajetePoleGracza && tablicaPol[y + 1][x + 1].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y][x+1].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
		else if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y][x - 1].type == zajetePolePrzeciwnika && tablicaPol[y + 1][x - 1].type == zajetePoleGracza && tablicaPol[y - 1][x - 1].type == zajetePoleGracza && tablicaPol[y][x - 2].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y][x - 1].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
	}
	else {
		if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y][x + 1].type == zajetePolePrzeciwnika && tablicaPol[y + 1][x + 1].type == zajetePoleGracza && tablicaPol[y - 1][x + 1].type == zajetePoleGracza && tablicaPol[y][x + 2].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y][x + 1].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
		else if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y - 1][x].type == zajetePolePrzeciwnika && tablicaPol[y - 1][x + 1].type == zajetePoleGracza && tablicaPol[y - 1][x - 1].type == zajetePoleGracza && tablicaPol[y - 2][x].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y - 1][x].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
		else if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y + 1][x].type == zajetePolePrzeciwnika && tablicaPol[y + 1][x + 1].type == zajetePoleGracza && tablicaPol[y + 1][x - 1].type == zajetePoleGracza && tablicaPol[y + 2][x].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y + 1][x].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
		else if (tablicaPol[y][x].type == okupowanePole && tablicaPol[y][x - 1].type == zajetePolePrzeciwnika && tablicaPol[y + 1][x - 1].type == zajetePoleGracza && tablicaPol[y - 1][x - 1].type == zajetePoleGracza && tablicaPol[y][x - 2].type == zajetePoleGracza) {
			tablicaPol[y][x].type = zajetePoleGracza;
			tablicaPol[y][x - 1].type = KoGracza;
			ktoryGraczGra++;
			return 1;
		}
	}
}






#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "ConsoleTools.h"
#include <assert.h>
#include <locale.h>
#include "demo.h"

//
//	Auteur : Pascal RICQ
//	version projet :  7.0
//  date 09-03-2022
//	Note : pedagogical purpose


COORD org = { 0, 0};
char allKey[] = "()wW0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMmNoOqQrRsStTuUwWyxXYzZ+-*/=<.\n\r";
COORD consoleSize;

int main(int argc, char* argv[]) {
	
	COORD pos = { 0,0 };
	char key;

	// stores some functions return code
	int ret;

	// pour la d�mo, fixer la taille de la console � 120 colonnes, 60 lignes
	// Console initialisation
	openConsole();
	
	// limite de la zone d'�criture
	consoleSize = getConsoleSize();
	char buff[256] = { 0 };
	sprintf_s(buff, sizeof(buff),"Console Size : %d x %d ",  consoleSize.X, consoleSize.Y);


	COORD centre = { 60,30};
	// changement de la couleur d'�criture
	setWriteColor(BRIGHTBLUE);
	circle(centre, 25);
	setWriteColor(BRIGHTRED);
	elipse(centre, 45, 15);
	setWriteColor(BRIGHTGREEN);
	lissajous(centre, 48 , 28, 3, 2);
	// place le curseur en bas � gauche de l'�cran
	moveCursor(0, 59);
	// clignotement d'une partie de l'�cran
	pos.X = centre.X - 5;
	pos.Y = centre.Y - 5;
	blink(pos, 10,10, 1000);
	

	clearScreen();
	
	int tab1[] = { 9,8,7,6,5,4,3,2,1};
	int tab2[] = { 1,2,3,4,5,6,7,8,9 };
	COORD p1 = { 10,10 };
	COORD p2 = { 33,20 };


	
	ret = drawArray(tab1, _countof(tab1), p1, p2, true, false, true, BLACK);
	assert(ret == 0);


	p1.X += 30;
	p2.X += 30;

	ret = drawArray(tab1, _countof(tab1), p1, p2, false, true, true , BLACK);

	assert(ret == 0);
	printStatusLine(buff, consoleSize);

	Sleep(3000);

	
	// Enter the alternate buffer
	enterAlternateBuffer();
	// Clear screen, tab stops, set, stop at columns 16, 32
	printf(CSI "1;1H");
	printf(CSI "2J"); // Clear screen

	int topMargin = 10;
	int bottomMargin = 10;
	int iNumLines = consoleSize.Y - topMargin - bottomMargin - 2;
	int iNumTabStops = 4; // (0, 20, 40, width)
	printf(CSI "3g"); // clear all tab stops
	printf(CSI "1;20H"); // Move to column 20
	printf(ESC "H"); // set a tab stop

	printf(CSI "1;40H"); // Move to column 40
	printf(ESC "H"); // set a tab stop

	// Set scrolling margins to 3, h-2
	// printf(CSI "3;%dr", consoleSize.Y - 2);


	printf(CSI "1;1H");
	printf(CSI "102;30m");
	printf("Windows 10 Anniversary Update - VT Example");
	printf(CSI "0m");

	int posTab[4] = { 1, 20, 40, 120 };
	bool dispCol[4] = { true, true, true, true };
	drawColumnedFrame(iNumTabStops, posTab, dispCol, topMargin, bottomMargin);



	printStatusLine("Press any key to see text printed between tab stops.", consoleSize);
	wchar_t wch;
	wch = _getwch();

	// Fill columns with output
	moveCursor(0, topMargin + 2);

	int line;
	for (line = 0; line < iNumLines; line++)
	{
		int tab = 0;
		for (tab = 0; tab < iNumTabStops - 1; tab++)
		{
			printVerticalBorder();
			printf("line=%d", line);
			printf("\t"); // advance to next tab stop
		}
		printVerticalBorder();// print border at right side
		if (line + 1 != iNumLines)
			printf("\t"); // advance to next tab stop, (on the next line)
	}

	printStatusLine("Press any key to demonstrate scroll margins", consoleSize);
	wch = _getwch();

	moveCursor(0, topMargin + 2);
	for (line = 0; line < iNumLines * 2; line++)
	{
		printf(CSI "K"); // clear the line
		int tab = 0;
		for (tab = 0; tab < iNumTabStops - 1; tab++)
		{
			printVerticalBorder();
			printf("line=%d", line);
			printf("\t"); // advance to next tab stop
		}
		printVerticalBorder(); // print border at right side
		if (line + 1 != iNumLines * 2)
		{
			printf("\n"); //Advance to next line. If we're at the bottom of the margins, the text will scroll.
			printf("\r"); //return to first col in buffer
		}
	}

	

	// Exit the alternate buffer
	printStatusLine("Press any key to exit alternate buffer", consoleSize);
	wch = _getwch();
	exitAlternateBuffer();

	// attente d'une touche pour continuer
	printStatusLine("Press any key to exit", consoleSize);
	wch = _getwch();
	// effacement de l'�cran

	clearScreen();
	// key=readChar(allKey);
	// fermeture de la console
	closeConsole();
  
}





int plot(COORD coord) {
	//coord.Y = (short) ((double)coord.Y * 5.0 / 12.0 );
	int wasclipped = 0;
	if (!clipping) {
		moveCursor(coord.X, coord.Y);
		plotChar('*');
		wasclipped = 0;
	}
	else {
		if ((coord.X >= 0) && (coord.X < consoleSize.X) && (coord.Y >= 0) && (coord.Y < consoleSize.Y)) {
			moveCursor(coord.X, coord.Y);
			plotChar('*');
			wasclipped = 0;
		}
		else {
			wasclipped = 1;
		}
	}
	return(wasclipped);
}


int elipse(COORD centre, double rayonX, double rayonY) {
	double deuxPi = 2 * Pi;
	double pasRad = deuxPi / 360;
	double anglerad = 0;
	COORD pt;
	for (anglerad = 0; anglerad <= deuxPi; anglerad += pasRad) {
		pt.X = centre.X + (int)(rayonX * cos(anglerad));
		pt.Y = centre.Y + (int)(rayonY * sin(anglerad));
		plot(pt);
	}

	return(EXIT_SUCCESS);

}

int circle(COORD centre, double rayon) {
	elipse(centre, rayon, rayon);
	return(EXIT_SUCCESS);
}

int lissajous(COORD centre, double rayonX, double rayonY, double p, double q) {
	double deuxPi = 2 * Pi;
	double pasRad = deuxPi / 3600;
	double anglerad = 0;
	double phi = 0;
	COORD pt;
	for (anglerad = 0; anglerad <= deuxPi; anglerad += pasRad) {
		pt.X = centre.X + (int)(rayonX * sin(p * anglerad + phi));
		pt.Y = centre.Y + (int)(rayonY * sin(q * anglerad + phi));
		plot(pt);
	}

	return(EXIT_SUCCESS);

}

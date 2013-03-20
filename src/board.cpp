#include "board.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX(a,b) (a<b?b:a)

bool canPlaceWord(char *b, Word *word, int x, int y, int direction, bool requireIntersect) {
	char *w = word->word;
	int dx, dy;
	if (direction==ACROSS) {
		dx=1; dy=0;
		// Would continue already placed word?
		if (x>0 && LETTER(b,x-1,y) != 0) return false;
		if (x+word->length<WIDTH-1 && LETTER(b,x+word->length+1,y) != 0) return false;
	} else { // direction==DOWN
		dx=0; dy=1;
		// Would continue already placed word?
		if (y>0 && LETTER(b,x,y-1) != 0) return false;
		if (y+word->length<HEIGHT-1 && LETTER(b,x,y+word->length+1) != 0) return false;
	}
	bool intersected = false;

	do {
		char l = LETTER(b,x,y);
		if (l == '_') return false; // Clash with ambiguous letter
		if (l == 0) {
			if (direction==ACROSS) {
				if (y!=0 && LETTER(b,x,y-1)!=0) return false; // Too close above
				if (y!=HEIGHT-1 && LETTER(b,x,y+1)!=0) return false; // Too close below
			} else {
				if (x!=0 && LETTER(b,x-1,y)!=0) return false; // Too close left
				if (x!=WIDTH-1 && LETTER(b,x+1,y)!=0) return false; // Too close right
			}
		} else if (LETTER(b,x,y) != *w) {
			return false; // Clash with specific letter
		} else {
			intersected = true; // Compatible letter found
		}
		x+=dx; y+=dy;
		w++;
	} while (*w != 0);
	return intersected || !requireIntersect;
}

int placeWord(char *b, Word *word, int x, int y, int direction) {
	int quality = 0;
	int crossingValue = 4;
	char *w = word->word;

	word->x = x;
	word->y = y;
	word->direction = direction;
	word->used = true;

	int dx, dy;
	if (direction==ACROSS) {
		dx=1; dy=0;
	} else { // direction==DOWN
		dx=0; dy=1;
	}

	do {
		if (LETTER(b,x,y) != 0) {
			// Value crossings highly, especially multiples
			quality+=(crossingValue *= crossingValue);
		}
		LETTER(b,x,y) = *w;
		x+=dx; y+=dy;
		w++;
		quality++; // Value word lengths slightly
	} while (*w != 0);

	return quality;
}

void dumpBoard(char *b) {
	for (int y=0; y<HEIGHT; y++) {
		for (int x=0; x<WIDTH; x++) {
			char l = LETTER(b,x,y);
			if (l!=0) fputc(l, stdout);
			else fputc(' ', stdout);
		}
		fputc('\n', stdout);
	}
}

void drawBoard(char *b, Wordlist *w) {
	int maxx=0, maxy=0;
	for (int pass=0; pass<=3; pass++) { // 0: get extents; 1: board; 2: across clues; 3: down clues
		int cluenum = 1;
		switch (pass) {
			case 2: fputs("\nAcross:\n", stdout); break;
			case 3: fputs("\nDown:\n", stdout); break;
		}
		for (int y=0; y<HEIGHT; y++) {
			for (int x=0; x<WIDTH; x++) {
				bool foundSomething = false;
				for (int i=0; i<w->c; i++) {
					if (w->words[i].x == x && w->words[i].y == y && w->words[i].used) {
						switch (pass) {
							case 2:
								if (w->words[i].direction == ACROSS) printf("%02i %s\n", cluenum, w->words[i].clue);
								break;
							case 3:
								if (w->words[i].direction == DOWN) printf("%02i %s\n", cluenum, w->words[i].clue);
								break;
						}
						foundSomething = true;
					}
				}
				switch (pass) {
					case 0:
						if (LETTER(b, x, y) != 0) {
							maxx=MAX(maxx,x);
							maxy=MAX(maxy,y);
						}
						break;
					case 1:
						if (foundSomething) {
							printf("%02i ", cluenum);
							cluenum++;
						} else if (LETTER(b, x, y)==0) {
							if (x<=maxx && y<=maxy) fputs(".. ", stdout);
						} else {
							fputs("__ ", stdout);
						}
						break;
					case 2:
					case 3:
						if (foundSomething) {
							cluenum++;
						}
				}
			}
			switch (pass) {
				case 1: if (y<=maxy) fputc('\n', stdout); break;
			}
		}
	}
}

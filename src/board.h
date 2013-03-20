#ifndef BOARD_H
#define BOARD_H

#include "wordlist.h"

#define WIDTH 20
#define HEIGHT 20

#define LETTER(b,x,y) b[((y)*HEIGHT)+(x)]

#define ACROSS 0
#define DOWN 1

bool canPlaceWord(char *b, Word *word, int x, int y, int direction, bool requireIntersect = true);
int placeWord(char *b, Word *w, int x, int y, int direction);

void dumpBoard(char *b);
void drawBoard(char *b, Wordlist *w);

#endif

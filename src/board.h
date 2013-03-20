/**
 * Crossword "board" implementation header
 * Copyright (c) 2013 by Alec Smecher
 * Released under the GPL v2 licence. See COPYING for details.
 */

#ifndef BOARD_H
#define BOARD_H

#include "wordlist.h"

#define WIDTH 20
#define HEIGHT 20

#define LETTER(b,x,y) b[((y)*HEIGHT)+(x)]

#define ACROSS 0
#define DOWN 1

bool canPlaceWord(char *b, Word *word, int x, int y, int direction, bool requireIntersect = true);
int placeWord(char *b, Word *w, int x, int y, int direction, bool solve = false);

void dumpBoard(char *b);
void drawBoard(char *b, Wordlist *w, bool html, bool solve);

#endif

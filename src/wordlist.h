/**
 * Crassword word list header
 * Copyright (c) 2013 by Alec Smecher
 * Released under the GPL v2 licence. See COPYING for details.
 */

#ifndef WORDLIST_H
#define WORDLIST_H

typedef struct {
	char *clue;
	char *word;
	char *clean;
	int length;
	bool used;
	int x, y, direction;
} Word;

typedef struct {
	int c;
	Word *words;
} Wordlist;

Wordlist *loadWords(const char *filename);
void freeWords(Wordlist *w);
void shuffleWords(Wordlist *l);

#endif

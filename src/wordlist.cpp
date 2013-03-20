/**
 * Crassword word list implementation
 * Copyright (c) 2013 by Alec Smecher
 * Released under the GPL v2 licence. See COPYING for details.
 */

#include "wordlist.h"

#define BUFSIZE 120

#include <malloc.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

Wordlist *loadWords(const char *filename) {
	FILE *f = fopen(filename, "r");
	if (!f) return NULL;
	char buf[BUFSIZE];

	Wordlist *l = (Wordlist *) malloc(sizeof(Wordlist));
	l->c = 0;

	while (fgets(buf, BUFSIZE, f)) {
		// Allocate space for the new word
		l->words = (Word *) realloc(l->words, sizeof(Word) * (l->c+1));

		// Read the tokens
		l->words[l->c].word = strdup(strtok(buf, ","));
		l->words[l->c].clean = strdup(strtok(NULL, ","));
		l->words[l->c].clue = strdup(strtok(NULL, "\n"));

		// Initialize the metadata
		l->words[l->c].used = false;
		l->words[l->c].length = strlen(l->words[l->c].word);
		l->words[l->c].x = l->words[l->c].y = -1;

		// Increment the counter
		l->c++;
	}
	fclose(f);
	return l;
}

void swapWords(Word *a, Word *b) {
	char *buf;
	buf = a->word; a->word = b->word; b->word = buf;
	buf = a->clean; a->clean = b->clean; b->clean = buf;
	buf = a->clue; a->clue = b->clue; b->clue = buf;

	bool bb = a->used;
	a->used = b->used; b->used = bb;

	int i = a->length;
	a->length = b->length; b->length = i;

	i = a->x; a->x=b->x; b->x=i;
	i = a->y; a->y=b->y; b->y=i;
	i = a->direction; a->direction=b->direction; b->direction=i;
}

void shuffleWords(Wordlist *l) {
	for (int i=l->c-1; i>0; i--) {
		int newIndex = rand() % (i+1);
		swapWords(&(l->words[i]), &(l->words[newIndex]));
	}
}

void freeWord(Word *w) {
	free(w->clue);
	free(w->word);
	free(w->clean);
}

void freeWords(Wordlist *w) {
	for (int i=0; i<w->c; i++) {
		freeWord(&(w->words[i]));
	}
	free(w->words);
	free(w);
}

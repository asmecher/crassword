#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <time.h>

#include "wordlist.h"
#include "board.h"

#define MIN_QUALITY 325

int main(int argc, char **argv) {
	Wordlist *w = loadWords("words.txt");
	srand(time(NULL));

	char *board = (char *) malloc(WIDTH * HEIGHT);

	int quality;
	do { // Repeat until we generate a board of sufficient quality
		memset(board, 0, WIDTH * HEIGHT);
		shuffleWords(w);
		quality = 0;
		bool needIntersect = false;
		bool changed;
		for (int i=0; i<w->c; i++) w->words[i].used = false;

		do { // Repeat until we can't build the board any further
			changed = false;
			for (int i=0; i<w->c; i++) { // Loop through all words
				if (w->words[i].used) continue;

				for (int y=0; y<HEIGHT; y++) {
					for (int x=0; x<WIDTH; x++) {
						for (int direction = ACROSS; direction <= DOWN; direction++) {
							if (
								!w->words[i].used &&
								(direction == DOWN || x+w->words[i].length<WIDTH) &&
								(direction == ACROSS || y+w->words[i].length<HEIGHT) &&
								canPlaceWord(board, &(w->words[i]), x, y, direction, needIntersect)
							) {
								quality += placeWord(board, &(w->words[i]), x, y, direction);
								needIntersect = true;
								changed = true;
							}
						} // Placement directions
					} // X
				} // Y
			} // Words
		} while (changed);
	} while (quality < MIN_QUALITY);

	drawBoard(board, w);

	freeWords(w);
	free(board);
	return 0;
}

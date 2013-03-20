/**
 * Crassword main entry point
 * Copyright (c) 2013 by Alec Smecher
 * Released under the GPL v2 licence. See COPYING for details.
 */

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <time.h>

#include "wordlist.h"
#include "board.h"

int main(int argc, char **argv) {
	// Option defaults
	bool htmlMode = false, solveMode = false;
	int seed = time(NULL);
	int minQuality = 325;

	// Get options
	for (int i=1; i<argc; i++) {
		if (strcmp(argv[i], "--html")==0) htmlMode = true;
		else if (strcmp(argv[i], "--solve")==0) solveMode = true;
		else if (strcmp(argv[i], "--help")==0) {
			fprintf(stderr, "Usage: %s [--options]\n...where [--options] can be any of...\n  --help: Display usage information\n  --html: Generate HTML output\n  --seed [seed]: Use a specific random number seed\n  --minquality [quality] Require the specified minimum quality\n  --solve: Show solution\n", argv[0]);
			return -1;
		} else if (strcmp(argv[i], "--seed")==0) {
			if (i+1 == argc) {
				fprintf(stderr, "Random number seed not specified.\n");
				return -1;
			}
			char *endptr;
			i++;
			seed = strtol(argv[i], &endptr, 10);
			if (*endptr != 0 || endptr == argv[i]) {
				fprintf(stderr, "The specified seed was not a valid number.\n");
				return -1;
			}
		} else if (strcmp(argv[i], "--minquality")==0) {
			if (i+1 == argc) {
				fprintf(stderr, "Minimum quality not specified.\n");
				return -1;
			}
			char *endptr;
			i++;
			minQuality = strtol(argv[i], &endptr, 10);
			if (*endptr != 0 || endptr == argv[i]) {
				fprintf(stderr, "The specified minimum quality was not a valid number.\n");
				return -1;
			}
		} else {
			fprintf(stderr, "Unknown argument \"%s\". Run \"%s --help\" for usage.\n", argv[i], argv[0]);
			return -1;
		}
	}

	// Initialization
	Wordlist *w = loadWords("words.txt");
	srand(seed);
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
	} while (quality < minQuality);

	drawBoard(board, w, htmlMode, solveMode);

	freeWords(w);
	free(board);
	return 0;
}

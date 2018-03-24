#include <3ds.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "beatmap.h"
#include "pp2d/pp2d.h"

#define bufSize 8000

/*
 * Beatmap Section IDs:
 * 0 = NONE
 * 1 = GENERAL
 * 2 = EDITOR
 * 3 = METADATA
 * 4 = DIFFICULTY
 * 5 = EVENTS
 * 6 = TIMINGPOINTS
 * 7 = COLOURS
 * 8 = HITOBJECTS
 */
int beatmapSection;

char *obj_x[1000];
char *obj_y[1000];
char *obj_songtime[1000];

int elementObj = 0;

bool parseBeatmap(char* fileName) {
	FILE *beatmapFile;
	char buf[bufSize];

	beatmapFile = fopen(fileName, "r");
	if (!beatmapFile) {
    	fprintf(stderr, "Failed to open file: (%i) %s\n", errno, strerror(errno));
    	return 1;
	}

	while (fgets(buf, sizeof(buf), beatmapFile) != NULL) {
		char* bufend = buf + strlen(buf);

		if (bufend != buf) {
		    if (*(--bufend) == '\n') *bufend = '\0';
		    if (*(--bufend) == '\r') *bufend = '\0';
		}

		if (!*buf) continue;

		if (!strcmp(buf, "[General]")) {
			beatmapSection = 1;
			continue;
		} else if (!strcmp(buf, "[Editor]")) {
			beatmapSection = 2;
			continue;
		} else if (!strcmp(buf, "[Metadata]")) {
			beatmapSection = 3;
			continue;
		} else if (!strcmp(buf, "[Difficulty]")) {
			beatmapSection = 4;
			continue;
		} else if (!strcmp(buf, "[Events]")) {
			beatmapSection = 5;
			continue;
		} else if (!strcmp(buf, "[TimingPoints]")) {
			beatmapSection = 6;
			continue;
		} else if (!strcmp(buf, "[Colours]")) {
			beatmapSection = 7;
			continue;
		} else if (!strcmp(buf, "[HitObjects]")) {
			beatmapSection = 8;
			continue;
		}

		if (beatmapSection == 8) {
        	char* val1 = strtok(buf, ","); // X
        	char* val2 = strtok(NULL, ","); // Y
        	char* val3 = strtok(NULL, ","); // SongTime in MS
			obj_x[elementObj] = val1;
			obj_y[elementObj] = val2;
			obj_songtime[elementObj] = val3;
			elementObj++;
		}
	}

	beatmapSection = 0;
	fclose(beatmapFile);
	return true;
}
// 3DS
#include <3ds.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

// Graphics
#include "pp2d/pp2d.h"

// osu! objects
#include "cursor.h"
#include "circle.h"
//#include "beatmap.h"

// osu! game states
#include "states/menu.h" // Main Menu
#include "states/song.h" // Song Select

// osu! skin
#define HITCIRCLE_TEXTURE 2
#define HITOVERLAY_TEXTURE 3
#define HITAPPROACH_TEXTURE 4
#define HIT_300 5
#define HIT_100 6
#define HIT_50 7
#define HIT_0 8

void loadSkin() {
	pp2d_load_texture_png(HITCIRCLE_TEXTURE, "romfs:/default/hitcircle.png");
	pp2d_load_texture_png(HITOVERLAY_TEXTURE, "romfs:/default/hitcircleoverlay.png");
	pp2d_load_texture_png(HITAPPROACH_TEXTURE, "romfs:/default/approachcircle.png");
	pp2d_load_texture_png(HIT_300, "romfs:/default/hit300.png");
	pp2d_load_texture_png(HIT_100, "romfs:/default/hit100.png");
	pp2d_load_texture_png(HIT_50, "romfs:/default/hit50.png");
	pp2d_load_texture_png(HIT_0, "romfs:/default/hit0.png");
	setCircleTexture(HITCIRCLE_TEXTURE, HITOVERLAY_TEXTURE, HITAPPROACH_TEXTURE);
}

//
#define bufSize 8000

long long startTime = -1;
long currentTime;

int gameState = 0; // Our game state

int beatmapSection;

int obj_x[1000];
int obj_y[1000];
long obj_songtime[1000];

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
        	char *ptr;
        	long ret;
        	ret = strtol(val3, &ptr, 10);
			obj_x[elementObj] = atoi(val1);
			obj_y[elementObj] = atoi(val2);
			obj_songtime[elementObj] = ret;
			elementObj++;
		}
	}

	beatmapSection = 0;
	fclose(beatmapFile);
	return true;
}

int lastX = 0;

int indexOf(long *array, int array_size, long number) {
    for (int i = 0; i < array_size; ++i) {
        if (array[i] == number) {
            return i;
        }
    }
    return -1;
}

int oofer = 0;

int main() {
	// Set start time
	if (startTime == -1) {
		startTime = osGetTime();
	}

	// Init
	int a;
	romfsInit();
	pp2d_init();
	consoleInit(GFX_TOP, NULL);
	loadSkin();
	initCursor();
	//initMenu();
	//initSong();
	parseBeatmap("osutest.osu");
	pp2d_set_screen_color(GFX_BOTTOM, ABGR8(255, 51, 51, 51));

	while (aptMainLoop() && !(hidKeysDown() & KEY_START)) {
		hidScanInput();

		// Begin frame
		pp2d_begin_draw(GFX_BOTTOM);
			pp2d_draw_text(0, 0, 0.5f, 0.5f, RGBA8(255, 255, 255, 255), "osu!3DS");
			/*switch (gameState) {
				// Main Menu
				case 0:
				{
					int menuState = drawMenu();
					if (menuState) {
						gameState = menuState;
					}
					break;
				}

				// Song Selection
				case 1:
				{
					//int songState = drawSong();
					//if (songState) {
					//	gameState = songState;
					//}
					break;
				}

				// Playfield
				case 2:
				{
					// Not implemented
					break;
				}
			}*/
			//for (a = 0; a < sizeof(obj_songtime); a++) {
			oofer = indexOf(obj_songtime, sizeof(obj_songtime), currentTime);
			//if ()
				//if (obj_songtime[a] == currentTime) {
					if (obj_x[oofer] != 0) {
						lastX = obj_x[oofer];
					}
					char str[12];
					sprintf(str, "%d", lastX);
					pp2d_draw_text(50, 50, 0.5f, 0.5f, RGBA8(255, 255, 255, 255), str);
					if (obj_songtime[oofer] != 0) {
						if (obj_x[oofer] != 0) {
							if (obj_y[oofer] != 0) {
								drawCircleHitandApproach(obj_x[oofer] / 10, obj_y[oofer] / 10, 1, currentTime + 2000, currentTime, 8);
							}
						}
					}
				//}
			//}
			drawCursor(); // Always draw cursor ontop of stuff
		pp2d_end_draw();

		currentTime = osGetTime() - startTime; // Set our current time

		// Debug info
		printf("\x1b[1;1HTIME:     %6ld  %6lld", currentTime, startTime);
		printf("\x1b[3;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		printf("\x1b[4;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		printf("\x1b[5;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);
	}

	// Kill graphics enviorment
	pp2d_exit();
	return 0;
}
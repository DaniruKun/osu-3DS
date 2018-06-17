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
#include "hitcircle.h"

// osu! game states
#include "states/menu.h" // Main Menu
#include "states/song.h" // Song Select

int       gameState = 0; // Our game state
long long startTime = -1;
long      currentTime;

int main() {
	// Set start time
	if (startTime == -1) {
		startTime = osGetTime();
	}

	// Init
	romfsInit();
	pp2d_init();
	consoleInit(GFX_TOP, NULL);
	cursor_Initialize();
	hitcircle_Initialize();
	pp2d_set_screen_color(GFX_BOTTOM, ABGR8(255, 51, 51, 51));

	while (aptMainLoop() && !(hidKeysDown() & KEY_START)) {
		hidScanInput();

		// Begin frame
		pp2d_begin_draw(GFX_BOTTOM);
			pp2d_draw_text(0, 0, 0.5f, 0.5f, RGBA8(255, 255, 255, 255), "osu!3DS");

			hitcircle_DrawHitCircleAndApproach(160, 120, 1, 1000, currentTime, 8);
			hitcircle_DrawHitCircleAndApproach(130, 100, 1, 1500, currentTime, 8);
			hitcircle_DrawHitCircleAndApproach(190, 60, 1, 2000, currentTime, 8);

			if (currentTime >= 2500) {
				startTime = osGetTime();
			}

			cursor_Draw(); // Always draw cursor on top of stuff
		pp2d_end_draw();

		currentTime = osGetTime() - startTime; // Set our current time

		// Debug info
		printf("\x1b[1;1HTIME:     %6ld  %6lld", currentTime, startTime);
		printf("\x1b[3;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime() * 6.0f);
		printf("\x1b[4;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime() * 6.0f);
		printf("\x1b[5;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage() * 100.0f);
	}

	// Kill graphics enviorment
	pp2d_exit();
	return 0;
}
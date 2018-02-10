// 3DS
#include <3ds.h>
#include <3ds/os.h>

// Graphics
#include "pp2d/pp2d.h"

// osu! objects
#include "cursor.h"
#include "circle.h"

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
long long startTime = -1;
long currentTime;

int gameState = 0; // Our game state

int main() {
	// Set start time
	if (startTime == -1) {
		startTime = osGetTime();
	}

	// Init
	romfsInit();
	pp2d_init();
	consoleInit(GFX_TOP, NULL);
	loadSkin();
	initCursor();
	initMenu();
	initSong();
	pp2d_set_screen_color(GFX_BOTTOM, ABGR8(255, 51, 51, 51));

	while (aptMainLoop() && !(hidKeysDown() & KEY_START)) {
		hidScanInput();

		// Begin frame
		pp2d_begin_draw(GFX_BOTTOM);
			switch (gameState) {
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
					int songState = drawSong();
					if (songState) {
						gameState = songState;
					}
					break;
				}

				// Playfield
				case 2:
				{
					// Not implemented
					break;
				}
			}
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

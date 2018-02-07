#include <3ds.h>
#include <3ds/os.h>
#include "pp2d/pp2d.h"
#include "cursor.h"
#include "circle.h"

#define CURSOR_TEXTURE 1
#define HITCIRCLE_TEXTURE 2
#define HITOVERLAY_TEXTURE 3
#define HITAPPROACH_TEXTURE 4
#define HIT_300 5
#define HIT_100 6
#define HIT_50 7
#define HIT_0 8

long long startTime = -1;
long curTime;

int gameState = 0;

int main() {
	if (startTime == -1) {
		startTime = osGetTime();
	}

	// Since our skin is in the romfs we need to init romfs
	romfsInit();

	// Init pp2d
	pp2d_init();
	// For debugging purposes the top screen gives us more info
	consoleInit(GFX_TOP, NULL);

	// Load our osu! skin
	pp2d_load_texture_png(CURSOR_TEXTURE, "romfs:/default/cursor.png");
	pp2d_load_texture_png(HITCIRCLE_TEXTURE, "romfs:/default/hitcircle.png");
	pp2d_load_texture_png(HITOVERLAY_TEXTURE, "romfs:/default/hitcircleoverlay.png");
	pp2d_load_texture_png(HITAPPROACH_TEXTURE, "romfs:/default/approachcircle.png");
	pp2d_load_texture_png(HIT_300, "romfs:/default/hit300.png");
	pp2d_load_texture_png(HIT_100, "romfs:/default/hit100.png");
	pp2d_load_texture_png(HIT_50, "romfs:/default/hit50.png");
	pp2d_load_texture_png(HIT_0, "romfs:/default/hit0.png");
	setCursorTexture(CURSOR_TEXTURE);
	setCircleTexture(HITCIRCLE_TEXTURE, HITOVERLAY_TEXTURE, HITAPPROACH_TEXTURE);

	// Set our screen color
	pp2d_set_screen_color(GFX_BOTTOM, ABGR8(255,52,52,52));

	while (aptMainLoop() && !(hidKeysDown() & KEY_START)) {
		hidScanInput();

		pp2d_begin_draw(GFX_BOTTOM);
			switch (gameState) {
				// Main Menu
				case 0:
					drawMenu();
				break;

				// Song Selection
				case 1:
					// Not implemented
				break;

				// Playfield
				case 2:
					// Not implemented
				break;
			}
		pp2d_end_draw();

		curTime = osGetTime() - startTime;

		printf("\x1b[1;1HTIME:     %6ld  %6lld", curTime, startTime);
		printf("\x1b[3;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		printf("\x1b[4;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		printf("\x1b[5;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);
	}

	// free pp2d environment
	pp2d_exit();
	return 0;
}

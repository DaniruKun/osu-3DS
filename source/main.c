#include <3ds.h>
#include <3ds/os.h>
#include "pp2d/pp2d.h"
#include "cursor.h"
#include "circle.h"

// assign ids for each texture
#define CURSOR_TEXTURE 1
#define HITCIRCLE_TEXTURE 2
#define HITOVERLAY_TEXTURE 3
#define HITSELECT_TEXTURE 4
#define HITAPPROACH_TEXTURE 5
#define HIT_300 6
#define HIT_100 7
#define HIT_50 8
#define HIT_0 9
long long startTime = -1;
long curTime;
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
	pp2d_load_texture_png(HITSELECT_TEXTURE, "romfs:/default/hitcircleselect.png");
	pp2d_load_texture_png(HITAPPROACH_TEXTURE, "romfs:/default/approachcircle.png");
	pp2d_load_texture_png(HIT_300, "romfs:/default/hit300.png");
	pp2d_load_texture_png(HIT_100, "romfs:/default/hit100.png");
	pp2d_load_texture_png(HIT_50, "romfs:/default/hit50.png");
	pp2d_load_texture_png(HIT_0, "romfs:/default/hit0.png");
	setCursorTexture(CURSOR_TEXTURE);
	setCircleTexture(HITCIRCLE_TEXTURE, HITOVERLAY_TEXTURE, HITSELECT_TEXTURE, HITAPPROACH_TEXTURE);

	// Set our screen color
	pp2d_set_screen_color(GFX_BOTTOM, ABGR8(255,52,52,52));

	while (aptMainLoop() && !(hidKeysDown() & KEY_START)) {
		hidScanInput();

		pp2d_begin_draw(GFX_BOTTOM);

			//pp2d_draw_text(0, 0, 0.5f, 0.5f, RGBA8(255,255,255,255), "osu!3DS");

			drawCursor();
			/*drawHit(70, 70, HIT_300);
			drawHit(90, 70, HIT_100);
			drawHit(90, 90, HIT_50);
			drawHit(70, 90, HIT_0);*/
			if (curTime >= 1000 && curTime <= 1750) {
				drawCircleHitandApproach(30, 30, HIT_300);
			}
			if (curTime >= 1751 && curTime <= 1760) {
				resetForNext();
			}
			if (curTime >= 2000 && curTime <= 2750) {
				drawCircleHitandApproach(100, 100, HIT_50);
			}
			if (curTime >= 2751 && curTime <= 2760) {
				resetForNext();
			}
		pp2d_end_draw();
		curTime = osGetTime() - startTime;
		printf("\x1b[1;1HTIME:     %6ld  %6lld", curTime, startTime);
		printf("\x1b[2;1HTIME:     %6lld  %6lld", osGetTime(), (osGetTime() - startTime));
		printf("\x1b[3;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		printf("\x1b[4;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		printf("\x1b[5;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);
	}

	// free pp2d environment
	pp2d_exit();
	return 0;
}

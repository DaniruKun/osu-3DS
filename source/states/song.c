#include <3ds.h>
#include "../pp2d/pp2d.h"
#include "song.h"

void initSong() {
	pp2d_load_texture_png(BUTTON, "romfs:/default/menu-button-background.png");
}

int drawSong() {
	touchPosition touch;
	hidTouchRead(&touch);
	// Hacked in touch cords
	if ((hidKeysHeld() & KEY_TOUCH)
		&& touch.px > 11
		&& touch.px < 11 + 152
		&& touch.py > 191
		&& touch.py < 191 + 36) {
		return 2;
	}
	pp2d_draw_texture_scale(BUTTON, 20, 185, 0.4f, 0.4f);
	return 1;
}
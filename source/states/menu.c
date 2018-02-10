#include <3ds.h>
#include "../pp2d/pp2d.h"
#include "menu.h"

void initMenu() {
	pp2d_load_texture_png(MENU_OSU, "romfs:/default/menu-osu.png");
}

int drawMenu() {
	touchPosition touch;
	hidTouchRead(&touch);
	// Hacked in touch cords
	if ((hidKeysHeld() & KEY_TOUCH)
		&& touch.px > 57
		&& touch.px < 57 + 203
		&& touch.py > 12
		&& touch.py < 12 + 203) {
		return 1;
	}
	pp2d_draw_texture_scale(MENU_OSU, 50, 5, 0.4f, 0.4f);
	pp2d_draw_text(45, 220, 0.5f, 0.5f, RGBA8(255, 255, 255, 255), "Tap the circle to enter song selection!");
	return 0;
}
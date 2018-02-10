#include <3ds.h>
#include "pp2d/pp2d.h"
#include "cursor.h"

float cursorSize = 0.5f;
int cursorX = 0;
int cursorY = 0;
float cursorRotation = 0.0f;
int extendCursor = 0;
bool isExtended = false;

void initCursor() {
	pp2d_load_texture_png(CURSOR_TEXTURE, "romfs:/default/cursor.png");
}

void drawCursor() {
	touchPosition touch;
	hidTouchRead(&touch);

	if (touch.px != 0 && touch.py != 0) {
		isExtended = false;
		cursorX = touch.px - (int)(40 * cursorSize);
		cursorY = touch.py - (int)(40 * cursorSize);
		pp2d_draw_texture_scale_rotate(CURSOR_TEXTURE, cursorX, cursorY, cursorSize, cursorSize, cursorRotation);
	} else if (extendCursor > 0) {
		pp2d_draw_texture_scale_rotate(CURSOR_TEXTURE, cursorX, cursorY, cursorSize, cursorSize, cursorRotation);
		extendCursor -= 1;
	} else {
		if (!isExtended) {
			isExtended = true;
			extendCursor = 20;
		}
		return;
	}

	cursorRotation += 0.8f;
}
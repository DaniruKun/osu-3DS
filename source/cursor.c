#include <3ds.h>
#include "pp2d/pp2d.h"
#include "cursor.h"

float cursor_Size     = 0.5f;
int   cursor_X        = 0;
int   cursor_Y        = 0;
float cursor_Rotation = 0.0f;
int   cursor_Extend   = 0;
bool  cursor_Extended = false;

void cursor_Initialize() {
	pp2d_load_texture_png(CURSOR_TEXTURE, "romfs:/default/cursor.png");
}

void cursor_Draw() {
	touchPosition touch;
	hidTouchRead(&touch);

	if (touch.px != 0 && touch.py != 0) {
		cursor_Extended = false;
		cursor_X        = touch.px - (int)(40 * cursor_Size);
		cursor_Y        = touch.py - (int)(40 * cursor_Size);
		pp2d_draw_texture_scale_rotate(CURSOR_TEXTURE, cursor_X, cursor_Y, cursor_Size, cursor_Size, cursor_Rotation);
	} else if (cursor_Extend > 0) {
		pp2d_draw_texture_scale_rotate(CURSOR_TEXTURE, cursor_X, cursor_Y, cursor_Size, cursor_Size, cursor_Rotation);
		cursor_Extend -= 1;
	} else {
		if (!cursor_Extended) {
			cursor_Extended = true;
			cursor_Extend   = 20;
		}
		return;
	}

	cursor_Rotation += 0.8f;
}
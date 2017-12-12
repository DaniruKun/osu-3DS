#include <3ds.h>
#include "cursor.h"
#include "pp2d/pp2d.h"

// Defines
int cursorTextureId = 0;
float cursorSize = 0.5f;
int cursorX = 0;
int cursorY = 0;
float cursorRotation = 0.0f;

// Update/Draw our cursor
void drawCursor() {
  touchPosition touch;
  hidTouchRead(&touch);

  if (touch.px != 0 && touch.py != 0) {
    cursorX = touch.px - (int)(25 * cursorSize);
    cursorY = touch.py - (int)(25 * cursorSize);
    pp2d_draw_texture_scale_rotate(cursorTextureId, cursorX, cursorY, cursorSize, cursorSize, cursorRotation);
  } else {
    return;
  }

  cursorRotation += 0.8f;
}

// Set our texture ID for cursor
void setCursorTexture(int id) {
  cursorTextureId = id;
}

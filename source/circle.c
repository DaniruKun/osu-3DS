#include <3ds.h>
#include "circle.h"
#include "pp2d/pp2d.h"

int circleTextureID = 0;
int circleOverlayID = 0;
int circleHitID = 0;
int approachCircleID = 0;
int currentHitCircle = 0;
float circleSize = 0.5f;

float approachSize = 1.0f;
int extendMax = 10;
int extend = -10;
bool draw = true;

void drawCircle(int x, int y) {
	int centerX = x - (int)(64 * circleSize);
	int centerY = y - (int)(64 * circleSize);
	pp2d_draw_texture_scale(circleTextureID, centerX, centerY, circleSize, circleSize);
	pp2d_draw_texture_scale(circleOverlayID, centerX, centerY, circleSize, circleSize);
}

void drawApproach(int x, int y, float size) {
	int centerX = x - (int)(64 * size);
	int centerY = y - (int)(64 * size);
	pp2d_draw_texture_scale(approachCircleID, centerX, centerY, size, size);
}

void drawHit(int x, int y, int score) {
	int hitTexture = 8;
	if (score >= 300) {
		hitTexture = 5;
	}
	else if (score < 300 && score >= 100) {
		hitTexture = 6;
	}
	else if (score < 100 && score >= 50) {
		hitTexture = 7;
	}
	int centerX = x - (int)(25 * circleSize);
	int centerY = y - (int)(25 * circleSize);
	pp2d_draw_texture_scale(hitTexture, centerX, centerY, circleSize, circleSize);
}

void drawCircleHitandApproach(int x, int y, int curCircle) {
	if (currentHitCircle != curCircle) {
		currentHitCircle = curCircle;
		resetForNext();
	}

	if (draw) {
		drawCircle(x, y);
	}

	if (extend == (-1 * extendMax)) {
		drawApproach(x, y, approachSize);
	} else {
		approachSize = 1.0f;
	}

	if (approachSize <= 0.45f) {
		approachSize = 1.0f;
		extend = extendMax;
	}

	if (extend > (-1 * extendMax)) {
		if (extend <= (int)(extendMax * 0.7) && extend >= (int)(-1 * extendMax * 0.5)) {
			drawHit(x,  y, 300);
		}

		if (extend < 0) {
			draw = false;
		} else {
			// Not implemented, but this would normally zoom out the hit circle 
		}

		extend -= 1;
	}

	if (extend == (-1 * extendMax)) {
		draw = true;
	}

	approachSize -= 0.015f;
}

void resetForNext() {
	draw = true;
	approachSize = 1.0f;
	extend = -10;
}

void setCircleTexture(int circle, int overlay, int approach) {
	circleTextureID = circle;
	circleOverlayID = overlay;
	approachCircleID = approach;
}
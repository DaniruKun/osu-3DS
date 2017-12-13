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
double hit_range = 300;
double hit_50 = 150;
double hit_100 = 80;
double hit_300 = 30;

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

void drawCircleHitandApproach(int x, int y, int curCircle, long timing, long currentTiming, float approachRate) {
	float rate = 6 - (.4 * approachRate);
	if (currentTiming + (hit_range*rate) > timing && currentTiming - (hit_range*rate) < timing) {
		if (currentTiming + (hit_range*rate) > timing && currentTiming - (hit_300*rate) < timing) {
			drawCircle(x, y);
			float msRate = rate * hit_range;
			long difference = (currentTiming - timing);
			/* AR0 1800ms before - 1f, 0ms 0.5f
			AR1 1680ms before
			AR2 1560ms before
			AR3 1440ms before
			*/
			float currentApproachSize = 1.0f;
			printf("\x1b[2;1HAS:     %6f  %6f", (float)difference, currentApproachSize);
			drawApproach(x, y, currentApproachSize);
		}
		int score = 0;
		if (currentTiming + (hit_300*rate) > timing && currentTiming - (0*rate) < timing) {
			score = 300;
		}
		else if (currentTiming + (hit_100*rate) > timing && currentTiming - (hit_300*rate) < timing) {
			score = 100;
		}
		else if (currentTiming + (hit_50*rate) > timing && currentTiming - (hit_100*rate) < timing) {
			score = 50;
		}
		drawHit(x, y, score);
	}
	else {
		resetForNext();
	}
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
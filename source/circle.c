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
int currentMSTick = 0;
long cStartTime = 0;

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

void drawCircleHitandApproach(int x, int y, int curCircle, long timing, long currentTiming, double approachRate) {
	double pre = difficultyRange(approachRate, 1800, 1200, 450);
	double actualApproachRate = pre > 1200 ? (1800 - pre) / 120 : (1200 - pre) / 150 + 5;
	double approachTime = pre;
	printf("\x1b[6;1HDEBUG:     %6f %6f", (float)approachTime, (float) pre);
	if (currentTiming + (approachTime) > timing && currentTiming - (approachTime) < timing) {
		if (currentTiming + (approachTime) > timing && currentTiming - (approachTime /10) < timing) {
			if (cStartTime == 0) {
				cStartTime = currentTiming;
				currentMSTick = currentTiming - cStartTime;
			}
			drawCircle(x, y);
			long difference = (currentTiming - timing);
			float doub = 2 * approachTime;
			float currentApproachSize = (-1 * (difference / doub)) + 0.5;
			printf("\x1b[2;1HAS:     %6ld %6f %6d", currentTiming, currentApproachSize, currentMSTick);
			if (difference > (-1 * approachTime * hit_range) && difference < 0) {
				currentMSTick = currentTiming - cStartTime;
				drawApproach(x, y, currentApproachSize);
			}
		}
		int score = 0;
		if (currentTiming + (approachTime / 10) > timing && currentTiming < timing) {
			score = 300;
		}
		else if (currentTiming + (approachTime / 3.75) > timing && currentTiming - (approachTime /10) < timing) {
			score = 100;
		}
		else if (currentTiming + (approachTime / 6) > timing && currentTiming - (approachTime /3.75) < timing) {
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
	currentMSTick = 0;
	cStartTime = 0;
}

void setCircleTexture(int circle, int overlay, int approach) {
	circleTextureID = circle;
	circleOverlayID = overlay;
	approachCircleID = approach;
}

double difficultyRange(double difficulty, double min, double mid, double max) {
	if (difficulty > 5)
		return mid + (max - mid) * (difficulty - 5) / 5;
	if (difficulty < 5)
		return mid - (mid - min) * (5 - difficulty) / 5;
	return mid;
}
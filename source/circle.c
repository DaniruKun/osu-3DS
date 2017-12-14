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
bool canClick = true;
double hit_range = 300;
double hit_50 = 150;
double hit_100 = 80;
double hit_300 = 30;
int currentMSTick = 0;
long cStartTime = 0;
int circX = 0;
int circY = 0;

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
	printf("\x1b[6;1HDEBUG:     %3d %3d", circX, circY);
	double pre = difficultyRange(approachRate, 1800, 1200, 450);
	double actualApproachRate = pre > 1200 ? (1800 - pre) / 120 : (1200 - pre) / 150 + 5;
	double inverseApproach = 11 - actualApproachRate;
	double approachTime = difficultyRange(actualApproachRate, 1800, 1200, 450);
	//printf("\x1b[6;1HDEBUG:     %6f %6f", (float)approachTime, (float)actualApproachRate);
	if (currentTiming + (approachTime) + 100 > timing && currentTiming - (approachTime) < timing) {
		if (draw) {
			if (currentTiming + (approachTime)+100 > timing && currentTiming - (approachTime / 10) < timing) {
				if (cStartTime == 0) {
					cStartTime = currentTiming;
					currentMSTick = currentTiming - cStartTime;
				}
				circX = x;
				circY = y;
				drawCircle(x, y);
				long difference = (currentTiming - timing);
				float doub = 2 * approachTime;
				float currentApproachSize = (-1 * (difference / doub)) + 0.5;
				//printf("\x1b[2;1HAS:     %6ld %6f %6d", currentTiming, currentApproachSize, currentMSTick);
				if (difference > (-1 * approachTime * hit_range) && difference < 0) {
					currentMSTick = currentTiming - cStartTime;
					drawApproach(x, y, currentApproachSize);
				}
				else if (currentTiming + (approachTime) > timing && difference < 0) {
					drawApproach(x, y, 1.0f);
				}
			}
			int score = 0;
			if (currentTiming + (hit_300 * inverseApproach) > timing && currentTiming - (hit_300 * inverseApproach) < timing) {
				score = 300;
			}
			else if (currentTiming + (hit_100 * inverseApproach) > timing && currentTiming - (hit_100 * inverseApproach) < timing) {
				score = 100;
			}
			else if (currentTiming + (hit_50 * inverseApproach) > timing && currentTiming - (hit_50 * inverseApproach) < timing) {
				score = 50;
			}
			drawHit(x, y, score);
		}
	}
	else {
		resetForNext();
	}
}
void attemptClick(int x, int y, long currentTime) {
	int xP = circX + (int)(64 * circleSize);
	int xM = circX - (int)(64 * circleSize);
	int yP = circY + (int)(64 * circleSize);
	int yM = circY - (int)(64 * circleSize);
	printf("\x1b[2;1HRANGE:     %3d %3d %3d %3d", xM, xP, yM, yP);
	if (canClick) {
		if (x < xP && x > xM && y < yP && y > yM) {
			draw = false;
			canClick = false;
		}
	}
}
void resetForNext() {
	draw = true;
	canClick = true;
	approachSize = 1.0f;
	extend = -10;
	currentMSTick = 0;
	cStartTime = 0;
	circX = 0;
	circY = 0;
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
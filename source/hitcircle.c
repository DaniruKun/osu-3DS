#include <3ds.h>
#include "hitcircle.h"
#include "pp2d/pp2d.h"

float  hitcircle_Size         = 0.5f;
float  hitcircle_ApproachSize = 1.0f;
bool   hitcircle_Draw         = true;
double hitcircle_HitRange     = 300;
int    hitcircle_MSTick       = 0;
long   hitcircle_StartTime    = 0;

void hitcircle_Initialize() {
	pp2d_load_texture_png(HITCIRCLE_TEXTURE, "romfs:/default/hitcircle.png");
	pp2d_load_texture_png(HITOVERLAY_TEXTURE, "romfs:/default/hitcircleoverlay.png");
	pp2d_load_texture_png(HITAPPROACH_TEXTURE, "romfs:/default/approachcircle.png");
}

void hitcircle_DrawHitCircle(int x, int y) {
	int centerX = x - (int)(64 * hitcircle_Size);
	int centerY = y - (int)(64 * hitcircle_Size);
	pp2d_draw_texture_scale(HITCIRCLE_TEXTURE, centerX, centerY, hitcircle_Size, hitcircle_Size);
	pp2d_draw_texture_scale(HITOVERLAY_TEXTURE, centerX, centerY, hitcircle_Size, hitcircle_Size);
}

void hitcircle_DrawApproach(int x, int y, float size) {
	int centerX = x - (int)(64 * size);
	int centerY = y - (int)(64 * size);
	pp2d_draw_texture_scale(HITAPPROACH_TEXTURE, centerX, centerY, size, size);
}

void hitcircle_DrawHitCircleAndApproach(int x, int y, int circleId, long timing, long currentTiming, double approachRate) {
	double pre                = hitcircle_DifficultyRange(approachRate, 1800, 1200, 450);
	double actualApproachRate = pre > 1200 ? (1800 - pre) / 120 : (1200 - pre) / 150 + 5;
	double approachTime       = hitcircle_DifficultyRange(actualApproachRate, 1800, 1200, 450);

	if (currentTiming + (approachTime) + 100 > timing && currentTiming - (approachTime) < timing) {
		if (hitcircle_Draw) {
			if (currentTiming + (approachTime) + 100 > timing && currentTiming - (approachTime / 10) < timing) {
				if (hitcircle_StartTime == 0) {
					hitcircle_StartTime = currentTiming;
					hitcircle_MSTick    = currentTiming - hitcircle_StartTime;
				}

				hitcircle_DrawHitCircle(x, y);

				long  difference          = (currentTiming - timing);
				float doub                = 2 * approachTime;
				float currentApproachSize = (-1 * (difference / doub)) + 0.5;

				if (difference > (-1 * approachTime * hitcircle_HitRange) && difference < 0) {
					hitcircle_MSTick = currentTiming - hitcircle_StartTime;
					hitcircle_DrawApproach(x, y, currentApproachSize);
				} else if (currentTiming + (approachTime) > timing && difference < 0) {
					hitcircle_DrawApproach(x, y, 1.0f);
				}
			}
		}
	} else {
		hitcircle_Reset();
	}
}

void hitcircle_Reset() {
	hitcircle_Draw = true;
	hitcircle_ApproachSize = 1.0f;
	hitcircle_MSTick = 0;
	hitcircle_StartTime = 0;
}

double hitcircle_DifficultyRange(double difficulty, double min, double mid, double max) {
	if (difficulty > 5)
		return mid + (max - mid) * (difficulty - 5) / 5;
	if (difficulty < 5)
		return mid - (mid - min) * (5 - difficulty) / 5;
	return mid;
}
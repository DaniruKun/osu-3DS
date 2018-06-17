#ifndef __HITCIRCLE_H
#define __HITCIRCLE_H

#define HITCIRCLE_TEXTURE   00051
#define HITOVERLAY_TEXTURE  00052
#define HITAPPROACH_TEXTURE 00053
#define HIT_300             00054
#define HIT_100             00055
#define HIT_50              00056
#define HIT_0               00057

void   hitcircle_Initialize();
void   hitcircle_DrawHitCircle(int x, int y);
void   hitcircle_DrawApproach(int x, int y, float size);
void   hitcircle_DrawHitCircleAndApproach(int x, int y, int circleId, long timing, long currentTiming, double approachRate);
void   hitcircle_Reset();
double hitcircle_DifficultyRange(double difficulty, double min, double mid, double max);

#endif
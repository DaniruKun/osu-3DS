#ifndef __HITCIRCLE_H
#define __HITCIRCLE_H
void drawCircle(int x,int y);
void drawHitCircle(int x, int y);
void drawApproach(int x, int y, float size);
void drawHit(int x, int y, int score);
void drawCircleHitandApproach(int x, int y, int curCircle, long timing, long currentTiming, float approachRate);
void resetForNext();
void setCircleTexture(int circle, int overlay, int approach);
#endif
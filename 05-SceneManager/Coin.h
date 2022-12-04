#pragma once

#include "GameObject.h"
#include "Animations.h"

#define COIN_STATE_IDLE		0

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_TYPE_NORMAL	6
#define COIN_TYPE_QUESTION_BRICK	61

#define COIN_STATE_UP		100
#define COIN_STATE_DOWN		111

#define COIN_SPEED	0.2f
#define COIN_PUSHING_Y	100

class CCoin : public CGameObject {
	bool isAppear;

	DWORD timing_start = 0;
public:
	CCoin(float x, float y) : CGameObject(x, y) {}
	CCoin(int type = COIN_TYPE_NORMAL);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	void SetAppear(bool appear) { isAppear = appear; }
	void SetState(int state);
	bool IsAppear() { return isAppear; };
	void StartTiming() { timing_start = GetTickCount64();}
};
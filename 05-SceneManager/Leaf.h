#pragma once
#include "GameObject.h"

#define LEAF_BBOX_WIDTH  16
#define LEAF_BBOX_HEIGHT 16

#define LEAF_STATE_IDLE		0
#define LEAF_STATE_UP		100
#define LEAF_STATE_FALLING	111

#define LEAF_UP_HEIGHT	 48

#define LEAF_ANI_RIGHT		0
#define LEAF_ANI_LEFT		1

#define LEAF_FALLING_TIME	450

class CLeaf : public CGameObject
{
	ULONGLONG start_timing = -1;
public:

	bool isAppear = false;
	void SetAppear(bool isAppear)
	{
		this->isAppear = isAppear;
	}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual int IsCollidable() { return 0; };
	int IsBlocking() { return 0; }
	void StartTiming() { start_timing = GetTickCount64(); }
};
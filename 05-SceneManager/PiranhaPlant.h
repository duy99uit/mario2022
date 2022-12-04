#pragma once
#include "GameObject.h"

#define PIRANHAPLANT_BBOX_WIDTH					20
#define PIRANHAPLANT_BBOX_HEIGHT				24
#define PIRANHAPLANT_DARTING_SPEED				0.03f
#define PIRANHAPLANT_STATE_DARTING				0
#define PIRANHAPLANT_STATE_BITING				100
#define PIRANHAPLANT_ANI_IDLE	0
#define PIRANHAPLANT_STATE_INACTIVE				300
#define PIRANHAPLANT_ACTIVE_RANGE				20
#define PIRANHAPLANT_BITING_TIME				1000

// x, y old value is 358	348

class PiranhaPlant :
	public CGameObject
{
	float limitY = 0;
	ULONGLONG biting_start = 0;

public:

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	void StartBitting() { biting_start = GetTickCount64(); }
	void SetLimitY(float ly) { limitY = ly - PIRANHAPLANT_BBOX_HEIGHT; }

	PiranhaPlant();
};
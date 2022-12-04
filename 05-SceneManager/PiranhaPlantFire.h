#pragma once
#include "PiranhaPlant.h"

#define PIRANHAPLANTFIRE_BBOX_WIDTH		20
#define PIRANHAPLANTFIRE_BBOX_HEIGHT	24
#define PIRANHAPLANTFIRE_STATE_DARTING	0
#define PIRANHAPLANTFIRE_SPEED	0.02f



class PiranhaPlantFire :
	public PiranhaPlant
{
	float limitY = 0;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

public:

	PiranhaPlantFire(int tag);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; };
	void SetLimitY(float ly)
	{
		limitY = ly - PIRANHAPLANTFIRE_BBOX_HEIGHT;
	}
};
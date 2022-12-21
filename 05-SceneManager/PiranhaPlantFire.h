#pragma once
#include "PiranhaPlant.h"
#include "FireBullet.h"

#define PIRANHAPLANT_BBOX_WIDTH					20
#define PIRANHAPLANT_BOX_HEIGHT					24

#define PIRANHAPLANT_DARTING_SPEED				0.03f
#define PIRANHAPLANT_STATE_DARTING				0

#define PIRANHAPLANT_GREEN_BBOX_HEIGHT			24
#define PIRANHAPLANT_RED_BBOX_HEIGHT			32

#define BB_Height	0

#define PIRANHAPLANT_ANI_RIGHT_UP			0
#define PIRANHAPLANT_ANI_RIGHT_DOWN			1
#define PIRANHAPLANT_ANI_LEFT_UP			2
#define PIRANHAPLANT_ANI_LEFT_DOWN			3

#define PIRANHAPLANT_STATE_DEATH			1
#define PIRANHAPLANT_DIE_TIME			300
#define PIRANHAPLANT_DELAY_TIME			1500

#define PIRANHAPLANT_AIM_TIME			1000

#define PIRANHAPLANT_STATE_SHOOTING		100
#define BULLET_ANI_SET_ID				9

class PiranhaPlantFire :
	public PiranhaPlant
{
	float limitY = 0;
	int BBHeight = 0;

	ULONGLONG die_start = 0;
	ULONGLONG delay_start = 0;

	ULONGLONG shooting_start = 0;
	ULONGLONG aim_start = 0;

	FireBullet* bullet = NULL;

	bool Up = false;
	bool Right = false;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
public:

	PiranhaPlantFire(int tag);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	void StartDie() { die_start = GetTickCount64(); }
	void StartDelay() { delay_start = GetTickCount64(); }

	void SetLimitY(float ly)
	{
		if (tagType == 0) {
			BBHeight = PIRANHAPLANT_GREEN_BBOX_HEIGHT;

		}
		else { BBHeight = PIRANHAPLANT_RED_BBOX_HEIGHT; }
		limitY = ly - BBHeight;
	}

	void Shoot();
	void StartAim() { aim_start = GetTickCount64(); }
	void StartShooting() { shooting_start = GetTickCount64(); }

};
#pragma once
#include "GameObject.h"

#define MUSHROOM_GRAVITY	0.2f
#define MUSHROOM_SPEED	0.07f

#define MUSHROOM_BBOX_WIDTH	16
#define MUSHROOM_BBOX_HEIGHT 16

//state
#define MUSHROOM_STATE_IDLE	0
#define MUSHROOM_STATE_UP 6
#define MUSHROOM_STATE_RIGHT 1
#define MUSHROOM_STATE_LEFT	-1

class CMushroom :public CGameObject
{
	bool isAppear;
	int typeMushroom;
public:
	virtual void Render();

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x;
		t = y;
		r = x + MUSHROOM_BBOX_WIDTH;
		b = y + MUSHROOM_BBOX_HEIGHT;
	};

	void SetState(int state);
	void SetAppear(bool appear) {
		isAppear = appear;
	}
	bool GetAppear() { return isAppear; }


	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	CMushroom(int type = 0);
	~CMushroom();
};
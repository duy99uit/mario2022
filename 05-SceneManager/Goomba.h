#pragma once
#include "GameObject.h"

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_WALKING_SPEED 0.035f


#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define GOOMBA_DIE_TIMEOUT 500

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001

#define GOOMBA_DIE_DEFLECT_SPEED	0.35f
#define GOOMBA_JUMP_SPEED			0.125f
#define GOOMBA_HIGHJUMP_SPEED		0.25f

#define GOOMBA_NORMAL			0
#define GOOMBA_RED				1
#define GOOMBA_RED_NORMAL		2


#define GOOMBA_RED_JUMPING_STACKS	2

#define GOOMBA_NORMAL_BBOX_HEIGHT		16

#define GOOMBA_NORMAL_ANI_WALKING		0
#define GOOMBA_NORMAL_ANI_DIE			1
#define GOOMBA_RED_ANI_JUMPING			2
#define GOOMBA_RED_ANI_WINGSWALKING		3
#define GOOMBA_RED_ANI_WALKING			4
#define GOOMBA_RED_ANI_DIE				5

#define GOOMBA_STATE_DIE_BY_MARIO		300
#define GOOMBA_STATE_RED_JUMPING		400
#define GOOMBA_STATE_RED_HIGHJUMPING	450
#define GOOMBA_STATE_RED_WINGSWALKING	500

#define GOOMBA_RED_TIME_WALKING			500
#define GOOMBA_RED_BBOX_HEIGHT			17
#define GOOMBA_RED_BBOX_WINGS_HEIGHT	18


class CGoomba : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;
	ULONGLONG chasingTimer;
	ULONGLONG walkingTimer = 0;


	int jumpingStacks = 0;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CGoomba(int tagType);
	virtual void SetState(int state);
};
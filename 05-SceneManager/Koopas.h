#pragma once
#include "GameObject.h"

#define KOOPAS_BBOX_WIDTH				16
#define KOOPAS_BBOX_HEIGHT				20
#define KOOPAS_BBOX_SHELL_HEIGHT		14

#define KOOPAS_STATE_WALKING	100
#define KOOPAS_STATE_SHELL_UP	200
#define KOOPAS_STATE_TURNING	300
#define KOOPAS_STATE_IN_SHELL	400

#define KOOPAS_WALKING_SPEED			0.03f
#define KOOPAS_FALL_SPEED				0.002f
#define KOOPAS_SHELL_DEFLECT_SPEED		0.4f

#define KOOPAS_GREEN		0
#define KOOPAS_RED			1

#define KOOPAS_ANI_WALKING_RIGHT	0
#define KOOPAS_ANI_SPIN_RIGHT		1
#define KOOPAS_ANI_WALKING_LEFT		2
#define KOOPAS_ANI_SPIN_LEFT		3
#define KOOPAS_ANI_SHELL			4
#define KOOPAS_ANI_SHELL_UP			6
#define KOOPAS_ANI_PARA_RIGHT		7
#define KOOPAS_ANI_PARA_LEFT		8

#define KOOPAS_SPIN_DIFF	4
#define KOOPAS_GREEN_PARA	2

#define KOOPAS_REVIVE_TIME		1500
#define KOOPAS_SHELL_TIME		4000

#define KOOPAS_GRAVITY					0.002f
#define KOOPAS_PARA_GRAVITY				0.001f

class CKoopas : public CGameObject
{
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	float ax;
	float ay;

	// koopas return walking
	ULONGLONG reviving_start = 0;
	ULONGLONG shell_start = 0;
	ULONGLONG dt = 0;
public:

	bool isBeingHeld = false;

	int start_tag = 0;

	void SetCanBeHeld(bool m) { isBeingHeld = m; };
	void HandleCanBeHeld(LPGAMEOBJECT player);
	void StartShell() { shell_start = GetTickCount64(); reviving_start = 0; }
	void StartReviving() { reviving_start = GetTickCount64(); }

	CKoopas(int tag);

	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	void OnCollisionWithBlock(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithBreakableBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithPlan(LPCOLLISIONEVENT e);

	// check collision with block
	bool KoopasCollision(LPGAMEOBJECT object);

};
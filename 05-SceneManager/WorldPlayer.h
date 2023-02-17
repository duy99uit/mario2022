#pragma once
#include "GameObject.h"
#include "Game.h"
#include "FireBullet.h"
#include "Utils.h"
#include "Portal.h"


#define PLAYER_SPEED	0.1f

#define PLAYER_STATE_IDLE			0
#define PLAYER_STATE_RIGHT			100
#define PLAYER_STATE_LEFT			111
#define PLAYER_STATE_UP				200
#define PLAYER_STATE_DOWN			201

#define PLAYER_BBOX_WIDTH	8	

// mario ani
#define MARIO_ANI_SMALL			0
#define MARIO_ANI_BIG			1
#define MARIO_ANI_TAIL			2

#define MARIO	0
#define LUIGI	1


class CWorldPlayer : public CGameObject
{
	float start_x;
	float start_y;
	int level;
public:
	bool cgLeft, cgRight, cgUp, cgDown;
	int sceneId;
	CWorldPlayer(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int vState);
	void SetLevel(int l) { level = l; };
	void SetMove(bool cLeft, bool cUp, bool cRight, bool cDown)
	{
		cgLeft = cLeft;
		cgRight = cRight;
		cgUp = cUp;
		cgDown = cDown;
	};

	void GoToPlayScene()
	{
		// go to play scene
		int scene = 1;
		CGame::GetInstance()->InitiateSwitchScene(scene);
	}

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 1; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithPOT(LPCOLLISIONEVENT e);
};
#pragma once
#include "GameObject.h"

#define QUESTION_BRICK_NORMAL	0
#define QUESTION_BRICK_ANI_NORMAL	0

#define QUESTION_BRICK_HIT	1
#define QUESTION_BRICK_ANI_HIT	1

#define QUESTIONBRICK_SPEED		0.05f
#define QUESTIONBRICK_PUSH_MAX_HEIGHT 8

// coin
#define COIN_ITEM_QUESTION_BRICK_COIN	0
#define COIN_ITEM_QUESTION_BRICK_ANI_SET_ID 6

//mushroom
#define MUSHROOM_ITEM_QUESTION_BRICK	1
#define ITEM_MUSHROOM_ANI_SET_ID 37


class QuestionBrick :public CGameObject
{
	int tagType;

	bool isPushingUp = false;
	bool isFallingDown = false;

	int totalItems = 1;

public:
	CGameObject* obj = NULL;
	QuestionBrick(int tagType);

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 1; }

	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	CGameObject* HandleQRItem(int itemType); // handle item in QR, include coin/mushroom
	void HandleShowItem(int itemType = COIN_ITEM_QUESTION_BRICK_COIN);

	void SetState(int state);

	void startPushedUp();
	void stopPushedUp();


};
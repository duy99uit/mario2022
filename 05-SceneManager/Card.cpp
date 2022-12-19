#pragma once
#include "GameObject.h"

#define CARD_BBOX_WIDTH		16

class CardItem : public CGameObject
{
public:
	CardItem();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

};
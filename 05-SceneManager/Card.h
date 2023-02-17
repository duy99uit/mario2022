#pragma once
#include "GameObject.h"

#define CARD_BBOX_WIDTH		16

#define CARD_STATE_RANDOM		0
#define CARD_STATE_MUSHROOM		1
#define CARD_STATE_FIREFLOWER	2
#define CARD_STATE_STAR			3

#define CARD_ANI_RANDOM		0
#define CARD_ANI_MUSHROOM	1
#define CARD_ANI_FIREFLOWER	2
#define CARD_ANI_STAR		3

#define CARD_RANDOM_TIME 100


class CardItem : public CGameObject
{
private:
	ULONGLONG start = 0;
	bool isAppear = true;
public:
	int state = 0;
	CardItem();
	~CardItem();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	void SetAppear(bool ap) { this->isAppear = ap; }
	int getState() {
		return this->state;
	}
};

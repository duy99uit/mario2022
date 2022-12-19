#include "Card.h"

CardItem::CardItem() {
	vx = 0;
	vy = 0;
}

void CardItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + CARD_BBOX_WIDTH;
	b = t + CARD_BBOX_WIDTH;
}

void CardItem::Render() {

	int ani = 1;

	animation_set->at(ani)->Render(x, y);
}

void CardItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CGameObject::Update(dt);
}
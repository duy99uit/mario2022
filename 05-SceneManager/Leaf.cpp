#include "Leaf.h"

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + LEAF_BBOX_WIDTH;
	b = y + LEAF_BBOX_HEIGHT;
}

void CLeaf::Render()
{
	//animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CGameObject::Update(dt);

	x += vx * dt;
	y += vy * dt;
}

void CLeaf::SetState(int state) {

}
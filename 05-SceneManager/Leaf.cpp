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

	if (!isAppear || isDeleted)
		return;
	int ani = LEAF_ANI_LEFT;
	if (state == LEAF_STATE_FALLING)
	{
		if (vx >= 0)
			ani = LEAF_ANI_RIGHT;
		else
			ani = LEAF_ANI_LEFT;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CGameObject::Update(dt);

	x += vx * dt;
	y += vy * dt;

	if (state == LEAF_STATE_UP) {
		if (start_y - y >= LEAF_UP_HEIGHT) {
			SetState(LEAF_STATE_FALLING);
		}
	}
	if (state == LEAF_STATE_FALLING) {
		if (GetTickCount64() - start_timing >= LEAF_FALLING_TIME) {
			vx = -vx;
			StartTiming();
		}
	}
}

void CLeaf::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case LEAF_STATE_IDLE:
		vx = vy = 0;
		break;
	case LEAF_STATE_UP:
		vy = -0.05f;
		vx = 0;
		start_y = y;
		break;
	case LEAF_STATE_FALLING:
		vy = 0.025f;
		vx = 0.075f;
		StartTiming();
		break;
	}

}
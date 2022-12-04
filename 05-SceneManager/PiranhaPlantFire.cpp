#include "PiranhaPlantFire.h"
#include "PlayScene.h"

PiranhaPlantFire::PiranhaPlantFire(int tag) {
	this->tagType = tag;
	SetState(PIRANHAPLANTFIRE_STATE_DARTING);
}

void PiranhaPlantFire::Render()
{
	int ani = PIRANHAPLANTFIRE_STATE_DARTING;
	animation_set->at(ani)->Render(x, y);
}

void PiranhaPlantFire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CGameObject::Update(dt);
	y += vy * dt;
	/*y = limitY;*/
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario != NULL) {
		float mLeft, mTop, mRight, mBottom;
		float mWidth = mario->GetWidth();
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom)) {
			if (mario->GetLevel() != MARIO_LEVEL_SMALL)
			{
				mario->SetLevel(mario->GetLevel() - 1);
			}
			else
			{
				DebugOut(L" Mario DIE  \n");
				/*mario->SetState(MARIO_STATE_DIE);*/
			}
		}
	}
}


void PiranhaPlantFire::GetBoundingBox(float& left, float& top,
	float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PIRANHAPLANTFIRE_BBOX_WIDTH;
	bottom = y + PIRANHAPLANTFIRE_BBOX_HEIGHT;
}

void PiranhaPlantFire::SetState(int state) {
	CGameObject::Update(state);
	switch (state)
	{
	case PIRANHAPLANTFIRE_STATE_DARTING:
		vy = 0;
		break;
	}
}

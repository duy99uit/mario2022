#include "PiranhaPlant.h"
#include "Game.h"
#include "PlayScene.h"

void PiranhaPlant::GetBoundingBox(float& left, float& top,
	float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PIRANHAPLANT_BBOX_WIDTH;
	bottom = y + PIRANHAPLANT_BBOX_HEIGHT;
}

PiranhaPlant::PiranhaPlant()
{
	SetState(PIRANHAPLANT_ANI_IDLE);
}

void PiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CGameObject::Update(dt, coObjects);

	//x += vx * dt;
	y += vy * dt;

	// die by mario tail
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float mLeft, mTop, mRight, mBottom;
	if (mario->GetLevel() == MARIO_LEVEL_TAIL) {
		mario->tail->GetBoundingBox(mLeft, mTop, mRight, mBottom);

		if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom) && mario->isTuring) {
			DebugOut(L"PiranhaPlant die by mario tail \n");
			//SetState();
		}
	}
}

void PiranhaPlant::Render()
{
	int ani = PIRANHAPLANT_ANI_IDLE;

	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void PiranhaPlant::SetState(int _state)
{
	CGameObject::SetState(_state);
	switch (_state)
	{
	case PIRANHAPLANT_ANI_IDLE:
		vy = vx = 0;
		break;
	case PIRANHAPLANT_STATE_INACTIVE:
		vy = 0;
		StartBitting();
		break;
	}
	
}
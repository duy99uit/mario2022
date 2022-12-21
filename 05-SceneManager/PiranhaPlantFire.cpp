#include "PiranhaPlantFire.h"
#include "PlayScene.h"

PiranhaPlantFire::PiranhaPlantFire(int tag) {
	this->tagType = tag;
	SetState(PIRANHAPLANT_STATE_DARTING);
}

void PiranhaPlantFire::Render()
{
	int ani = PIRANHAPLANT_STATE_DARTING;
	if (state == PIRANHAPLANT_STATE_DARTING)
	{
		if (Up)
			if (Right)
				ani = PIRANHAPLANT_ANI_RIGHT_UP;
			else
				ani = PIRANHAPLANT_ANI_LEFT_UP;
		else
			if (Right)
				ani = PIRANHAPLANT_ANI_RIGHT_DOWN;
			else
				ani = PIRANHAPLANT_ANI_LEFT_DOWN;
	}
	animation_set->at(ani)->Render(x, y);
}

void PiranhaPlantFire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CGameObject::Update(dt);

	if (y <= limitY && vy < 0)
	{
		y = limitY;
		vy = 0;
		StartDelay();
	}
	if (y >= limitY + BBHeight && vy > 0)
	{
		y = limitY + BBHeight + 12;
		SetState(PIRANHAPLANT_STATE_INACTIVE);
	}
	if (GetTickCount64() - delay_start >= PIRANHAPLANT_DELAY_TIME && delay_start != 0)
	{
		if (y == limitY) {
			vy = PIRANHAPLANT_DARTING_SPEED;
		}

		delay_start = 0;
	}
	if (y > limitY && vy == 0)
	{
		// start darting when turn off success
		SetState(PIRANHAPLANT_STATE_DARTING);
	}

	y += vy * dt;


	// die
	if (GetTickCount64() - die_start >= PIRANHAPLANT_DIE_TIME && die_start != 0)
		isDeleted = true;

	if (state == PIRANHAPLANT_STATE_DEATH)
		return;

	float mLeft, mTop, mRight, mBottom;
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetLevel() == MARIO_LEVEL_TAIL) {
		mario->tail->GetBoundingBox(mLeft, mTop, mRight, mBottom);

		if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom) && mario->isTuring) {
			SetState(PIRANHAPLANT_STATE_DEATH);
		}
	}
}


void PiranhaPlantFire::GetBoundingBox(float& left, float& top,
	float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PIRANHAPLANT_BBOX_WIDTH;
	bottom = y + PIRANHAPLANT_BBOX_HEIGHT;
}

void PiranhaPlantFire::SetState(int state) {
	CGameObject::Update(state);
	switch (state)
	{
	case PIRANHAPLANT_STATE_DARTING:
		vy = -PIRANHAPLANT_DARTING_SPEED;
		break;
	case PIRANHAPLANT_STATE_DEATH:
		vy = 0;
		StartDie();
		break;
	case PIRANHAPLANT_STATE_INACTIVE:
		vy = 0;
		StartDelay();
		break;
	}
}
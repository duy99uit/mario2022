#include "PiranhaPlantFire.h"
#include "PlayScene.h"

PiranhaPlantFire::PiranhaPlantFire(int tag) {
	this->tagType = tag;
	SetState(PIRANHAPLANT_STATE_INACTIVE);
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

	GetDirect();

	// limit Y
	if (y <= limitY && vy < 0)
	{
		y = limitY;
		vy = 0;
		//DebugOut(L"start aim \n");
		//StartDelay();
		StartAim();
	}
	if (y >= limitY + BBHeight && vy > 0)
	{
		y = limitY + BBHeight + 12;
		SetState(PIRANHAPLANT_STATE_INACTIVE);
		//StartDelay();
	}

	if (GetTickCount64() - aim_start >= PIRANHAPLANT_AIM_TIME && aim_start != 0)
	{
		//DebugOut(L"start shoot \n");
		aim_start = 0;
		SetState(PIRANHAPLANT_STATE_SHOOTING);
		StartDelay();
	}

	if (GetTickCount64() - delay_start >= PIRANHAPLANT_DELAY_TIME && delay_start != 0)
	{
		if (y == limitY) {
			//DebugOut(L"start turn off \n");
			vy = PIRANHAPLANT_DARTING_SPEED;
		}
		delay_start = 0;
	}
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario != NULL) {
		if (floor(mario->x) <= x - 130 || floor(mario->x) > x + 130) {
			idle = false;
			//DebugOut(L"range idle\n");
		}
		else if (floor(mario->x) <= x - 30 || floor(mario->x) > x + 30) {
			idle = true;
			//DebugOut(L"range active\n");
			//DebugOut(L"x for piranha:: %f \n", this->x);
			//DebugOut(L"x for mario:: %f \n", mario->x);
		}
		else if (floor(mario->x) > x - 30 || floor(mario->x) < x + 30) {
			idle = false;
			//DebugOut(L"range idle\n");
		}

	}


	if (y > limitY && vy == 0 && aim_start == 0 && delay_start == 0 && idle)
	{
		//start darting when turn off success
		//DebugOut(L"start darting again \n");
		SetState(PIRANHAPLANT_STATE_DARTING);
	}

	y += vy * dt;

	// die
	if (GetTickCount64() - die_start >= PIRANHAPLANT_DIYING_TIME && die_start != 0)
		isDeleted = true;

	if (state == PIRANHAPLANT_STATE_DEATH)
		return;
	float mLeft, mTop, mRight, mBottom;

	if (mario->GetLevel() == MARIO_LEVEL_TAIL) {
		mario->tail->GetBoundingBox(mLeft, mTop, mRight, mBottom);

		if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom) && mario->isTuring) {
			DebugOut(L"PiranhaPlantFire die by mario tail \n");
			SetState(PIRANHAPLANT_STATE_DEATH);
			mario->InitScore(this->x, this->y, 100);
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
	case PIRANHAPLANT_STATE_SHOOTING:
		vy = 0;
		Shoot();
		break;
	case PIRANHAPLANT_STATE_INACTIVE:
		vy = 0;
		StartDelay();
		break;
	}
}

void PiranhaPlantFire::Shoot() {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	this->bullet = new FireBullet(x, y, Up, Right);

	//! setup for bullet object
	int ani_set_id = BULLET_ANI_SET_ID;
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	bullet->SetAnimationSet(ani_set);
	currentScene->AddObjectToScene(bullet);
}
void PiranhaPlantFire::GetDirect() {
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int mH;
	if (mario->GetLevel() == MARIO_LEVEL_SMALL)
		mH = MARIO_SMALL_BBOX_HEIGHT;
	else
		mH = MARIO_BIG_BBOX_HEIGHT;

	if (mario->y + mH < limitY + BBHeight)
		Up = true;
	else
		Up = false;
	if (mario->x <= x)
		Right = false;
	else
		Right = true;
};
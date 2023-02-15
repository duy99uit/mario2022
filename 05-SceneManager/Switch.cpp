#include "Switch.h"
#include "BreakableBrick.h"
#include "PlayScene.h"
#include "Coin.h"

void Switch::Render() {
	if (!isAppear || isDeleted)
		return;
	if (state == SWITCH_STATE_PRESSED)
		animation_set->at(SWITCH_ANI_PRESSED)->Render(x, y);
	else
		animation_set->at(SWITCH_ANI_IDLE)->Render(x, y);
	RenderBoundingBox();
}

void Switch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDeleted)
		return;
	CGameObject::Update(dt);
	if (state == SWITCH_STATE_UP)
	{
		y += vy * dt;
		if (start_y - y >= SWITCH_BBOX_HEIGHT)
		{
			vy = 0;
			y = start_y - SWITCH_BBOX_HEIGHT - 0.1f;
			SetState(SWITCH_STATE_IDLE);
		}
	}
}

void Switch::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{

	case SWITCH_STATE_UP:
		vy = -0.05f;
		start_y = y;
		break;

	case SWITCH_STATE_PRESSED:
		ChangeBreakBrickToCoin();
		break;
	}
}

void Switch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + SWITCH_BBOX_WIDTH;
	b = y + SWITCH_BBOX_HEIGHT;
}

void Switch::ChangeBreakBrickToCoin() {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	vector<LPGAMEOBJECT> objects = currentScene->GetObjects();
	CMario* mario = currentScene->GetPlayer();
	//DebugOut(L"Obj: %d \n", objects.size());
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(COIN_ANI_SET_ID);
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<BreakableBrick*>(objects.at(i)) && !objects.at(i)->isDeleted) {
			//DebugOut(L"BBrick not null \n");
			BreakableBrick* bBrick = dynamic_cast<BreakableBrick*>(objects.at(i));
			CCoin* coin = new CCoin();
			coin->SetPosition(bBrick->x, bBrick->y);
			mario->isExtraTop = true;
			coin->SetAppear(true);
			coin->SetAnimationSet(ani_set);
			currentScene->AddObjectToScene(coin);
			bBrick->isDeleted = true;
		}
	}
}
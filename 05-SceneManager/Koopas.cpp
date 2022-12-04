#include "Koopas.h"
#include "PlayScene.h"
#include "Block.h"

CKoopas::CKoopas(int tag)
{
	this->start_x = x;
	this->start_y = y;
	this->nx = -1;
	this->start_tag = tag;
	if (tag == KOOPAS_GREEN || tag == KOOPAS_GREEN_PARA) {
		this->nx = -1;
	}
	this->SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	/*vy += ay * dt;
	vx += ax * dt;*/
	// koopas shell up
	if (GetTickCount64() - shell_start >= KOOPAS_SHELL_TIME && shell_start != 0 && state != KOOPAS_STATE_TURNING) {
		shell_start = 0;
		DebugOut(L"koopas start shell \n");
		StartReviving();
	}

	if (GetTickCount64() - reviving_start >= KOOPAS_REVIVE_TIME && reviving_start != 0 && state != KOOPAS_STATE_TURNING && shell_start == 0)
	{
		reviving_start = 0;
		y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_SHELL_HEIGHT) + 1.0f;
		if (isBeingHeld)
		{
			isBeingHeld = false;
			mario->isHolding = false;
		}
		DebugOut(L"koopas return to walking \n");
		SetState(KOOPAS_STATE_WALKING);
	}

	// FOR HANDLE COLLISION WITH BLOCK
	for (int i = 0; i < coObjects->size(); i++) {
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CBlock*>(obj))
		{
			if (obj->getY() > this->y) {
				obj->SetIsBlocking(1);
			}
			else {
				obj->SetIsBlocking(0);
			}
		}
	}

	//vy += ay * dt;
	//vx += ax * dt;

	this->dt = dt;
	vy += KOOPAS_GRAVITY * dt;

	// set y when mario drop koopas
	if (!isBeingHeld)
	{
		if (tagType == KOOPAS_GREEN_PARA)
			vy += KOOPAS_PARA_GRAVITY * dt;
		if (tagType == KOOPAS_RED || tagType == KOOPAS_GREEN)
			vy += KOOPAS_GRAVITY * dt;
	}

	HandleCanBeHeld(mario);

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopas::HandleCanBeHeld(LPGAMEOBJECT player) {

	CMario* mario = dynamic_cast<CMario*>(player);

	if (mario->isHolding) {
		DebugOut(L"mario holding koopas \n");
		if (state == KOOPAS_STATE_IN_SHELL || state == KOOPAS_STATE_SHELL_UP) {
			if (mario->nx > 0) {
				// vi tri x sau khi mario cam len
				x = mario->x + MARIO_BIG_BBOX_WIDTH * mario->nx - 3.0f;
			}
			else x = mario->x + MARIO_BIG_BBOX_WIDTH * mario->nx;
			// mario hold in hand
			y = mario->y - 2.0f;

			// for koopas dung yen
			vy = 0;
		}
	}
	else if (isBeingHeld && !mario->isHolding) {
		// mario nem' rua` di
		if (state == KOOPAS_STATE_SHELL_UP || state == KOOPAS_STATE_IN_SHELL) {
			this->nx = mario->nx;
			isBeingHeld = false;
			DebugOut(L"koopas turning when mario drop \n");
			SetState(KOOPAS_STATE_TURNING);
		}
	}
}

void CKoopas::OnNoCollision(DWORD dt) {
	x += vx * dt;
	y += vy * dt;
}

void CKoopas::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0.002f;
		ay = vy;
	}
	else
	{
		// blocking x
		if (e->nx != 0 && e->obj->IsBlocking())
		{
			vx = -vx;
			nx = -nx;
		}
	}

	if (dynamic_cast<CBlock*>(e->obj))
	{
		//DebugOut(L"koopas on collision with block \n");
		OnCollisionWithBlock(e);
	}
	if (dynamic_cast<CGoomba*>(e->obj))
	{
		DebugOut(L"koopas on collision with goomba \n");
		OnCollisionWithGoomba(e);
	}
	if (dynamic_cast<CKoopas*>(e->obj)) {
		DebugOut(L"koopas on collision with koopas \n");
		OnCollisionWithKoopas(e);
	}

}

void CKoopas::OnCollisionWithBlock(LPCOLLISIONEVENT e) {

	if (e->ny < 0)
	{
		DebugOut(L"koopas on collision with block tag red and walking \n");
		vy = 0;
		if (state == KOOPAS_STATE_IN_SHELL)
			vx = 0;
		if (tagType == KOOPAS_RED && state == KOOPAS_STATE_WALKING)
		{
			DebugOut(L"koopas on collision with block tag red and walking \n");
			if (this->nx > 0 && x >= e->obj->x + KOOPAS_SPIN_DIFF)
			{
				DebugOut(L"collision right \n");
				if (KoopasCollision(e->obj))
				{
					//DebugOut(L"collision right \n");
					this->nx = -1;
					vx = this->nx * KOOPAS_WALKING_SPEED;
				}
			}
			if (this->nx < 0 && x <= e->obj->x - KOOPAS_SPIN_DIFF)
			{
				DebugOut(L"collision left \n");
				if (KoopasCollision(e->obj))
				{
					//DebugOut(L"collision left \n");
					this->nx = 1;
					vx = this->nx * KOOPAS_WALKING_SPEED;
				}
			}
		}
		if (tagType == KOOPAS_GREEN_PARA)
		{
			y = e->obj->y - KOOPAS_BBOX_HEIGHT;
			vy = -0.35f;
			vx = vx = this->nx * KOOPAS_WALKING_SPEED;
			this->nx = -1;
		}
	}
}

void CKoopas::OnCollisionWithGoomba(LPCOLLISIONEVENT e) {
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	if (this->GetState() == KOOPAS_STATE_TURNING)
	{
		goomba->SetState(GOOMBA_STATE_DIE);
		DebugOut(L"goomba die by koopas \n");
		vy = -MARIO_JUMP_DEFLECT_SPEED;
	}
	else {
		goomba->vx = -goomba->vx;
		goomba->nx = -goomba->nx;
		this->vx = -this->vx;
		this->nx = -this->nx;
	}
}

void CKoopas::OnCollisionWithKoopas(LPCOLLISIONEVENT e) {

	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);

	/*if (koopas->state == KOOPAS_STATE_WALKING)
	{
		this->vx = -this->vx;
		this->nx = -this->nx;
		koopas->vx = -koopas->vx;
		koopas->nx = -koopas->nx;
	}*/

	if (koopas->state == KOOPAS_STATE_TURNING)
	{
		if (koopas->tagType == KOOPAS_GREEN_PARA)
			koopas->tagType = KOOPAS_GREEN;
		DebugOut(L"Koopas die by koopas turning \n");
		//SetState();
	}
	else {
		if (koopas->state == KOOPAS_STATE_WALKING)
		{
			this->vx = -this->vx;
			this->nx = -this->nx;
			koopas->vx = -koopas->vx;
			koopas->nx = -koopas->nx;
		}
	}
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;
	if (state == KOOPAS_STATE_IN_SHELL || state == KOOPAS_STATE_TURNING) {
		bottom = y + KOOPAS_BBOX_SHELL_HEIGHT;
	}
	else {
		bottom = y + KOOPAS_BBOX_HEIGHT;
	}
}

void CKoopas::Render()
{
	int ani = -1;
	if (state == KOOPAS_STATE_SHELL_UP)
		ani = KOOPAS_ANI_SHELL_UP;
	else if (state == KOOPAS_STATE_IN_SHELL)
		ani = KOOPAS_ANI_SHELL;
	else if (state == KOOPAS_STATE_TURNING)
	{
		if (vx < 0)
			ani = KOOPAS_ANI_SPIN_LEFT;
		else
			ani = KOOPAS_ANI_SPIN_RIGHT;
	}
	else {
		if (this->nx < 0)
			ani = KOOPAS_ANI_WALKING_LEFT;
		else
			ani = KOOPAS_ANI_WALKING_RIGHT;
	}

	// koopas green
	if (tagType == KOOPAS_GREEN_PARA)
		if (this->nx < 0)
			ani = KOOPAS_ANI_PARA_LEFT;
		else
			ani = KOOPAS_ANI_PARA_RIGHT;

	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_WALKING:
		vx = this->nx * KOOPAS_WALKING_SPEED;
		vy = KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_TURNING:
		// turn
		vx = KOOPAS_WALKING_SPEED * 4;
		vy = KOOPAS_FALL_SPEED;
		ay = vy;
		break;
	case KOOPAS_STATE_IN_SHELL:
		// idle
		vx = 0;
		vy = 0;
		StartShell();
		break;
	case KOOPAS_STATE_SHELL_UP:
		// shell up and then walking
		vy = -KOOPAS_SHELL_DEFLECT_SPEED;
		vx = KOOPAS_WALKING_SPEED;
		nx = 1;
		StartShell();
		break;
	}
}

bool CKoopas::KoopasCollision(LPGAMEOBJECT object)
{
	// check block koopas standing == rest
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	vector<LPGAMEOBJECT> coObjects = currentScene->GetObjects();
	for (UINT i = 0; i < coObjects.size(); i++)
		if (dynamic_cast<CBlock*>(coObjects[i]))
			if (abs(coObjects[i]->y == object->y))
			{
				if (nx > 0)
					if (coObjects[i]->x > object->x && coObjects[i]->x - 16 < object->x + 16)
					{
						DebugOut(L"next is block, right \n");
						return false;
					}
				if (nx < 0)
					if (coObjects[i]->x + 16 > object->x - 16 && coObjects[i]->x < object->x)
					{
						DebugOut(L"next is block, left \n");
						return false;
					}
			}
	return true;
}
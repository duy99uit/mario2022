#include "MushRoom.h"
#include "debug.h"
#include "Block.h"
#include "PlayScene.h"
#include "Block.h"

CMushroom::CMushroom(int type) {
	SetAppear(false);
	typeMushroom = type;
}

void CMushroom::Render() {
	if (isAppear && !isDeleted) {
		if (typeMushroom == MUSHROOM_GREEN) {
			animation_set->at(1)->Render(x, y);
		}
		else
			animation_set->at(0)->Render(x, y);
	}
	RenderBoundingBox();
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (isDeleted) return;


	if (state == MUSHROOM_STATE_UP)
	{
		y += vy * dt;
		if (start_y - y >= MUSHROOM_BBOX_HEIGHT)
		{
			vy = 0;
			DebugOut(L"start_y! %d\n", start_y);
			y = start_y - MUSHROOM_BBOX_HEIGHT - 0.1f;

			//mushroom move right 
			SetState(MUSHROOM_STATE_RIGHT);
			DebugOut(L"Mushroom state right running!\n");

			////mushroom move left  
			//SetState(MUSHROOM_STATE_LEFT);
			//DebugOut(L"Mushroom state right running!\n");
		}
	}

	// for handle mushroom with block
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


	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushroom::SetState(int state) {
	CGameObject::SetState(state);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case MUSHROOM_STATE_IDLE:
		vy = vx = 0;
		break;
	case MUSHROOM_STATE_UP:
		vy = -0.05f;
		start_y = y;
		break;
	case MUSHROOM_STATE_RIGHT:
		vy = MUSHROOM_GRAVITY;
		vx = mario->nx * MUSHROOM_SPEED;
		DebugOut(L"Vao day %d\n", vx);
		break;
	case MUSHROOM_STATE_LEFT:
		vy = MUSHROOM_GRAVITY;
		vx = -MUSHROOM_SPEED;
		break;
	}
}

void CMushroom::OnNoCollision(DWORD dt) {
	if (state == MUSHROOM_STATE_RIGHT) {
		x += vx * dt;
		y += vy * dt;
		vy = MUSHROOM_GRAVITY;
	}
	if (state == MUSHROOM_STATE_LEFT) {
		x += -vx * dt;
		y += vy * dt;
		vy = MUSHROOM_GRAVITY;
	}
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (state == MUSHROOM_STATE_RIGHT) {
		if (e->ny != 0 && e->obj->IsBlocking())
			vy = 0;
	
		else
		{
			if (e->nx != 0 && e->obj->IsBlocking())
				vx = -vx;
			
		}
	}
}

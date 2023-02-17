#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "WorldPlayer.h"
#include "WorldScene.h"
#include "WMObject.h"
#include "debug.h"

CWorldPlayer::CWorldPlayer(float x, float y) : CGameObject()
{
	SetLevel(2);
	SetState(MARIO_STATE_IDLE);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	SetMove(false, false, true, false);
}

void CWorldPlayer::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 2;
	top = y + 2;
	right = left + PLAYER_BBOX_WIDTH - 4;
	bottom = top + PLAYER_BBOX_WIDTH - 4;
}

void CWorldPlayer::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CWorldPlayer::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking()) {
		vx = 0;
		vy = 0;
		DebugOut(L"is blocking \n");
	}
	SetState(PLAYER_STATE_IDLE);
	if (e->obj->tagType == OBJECT_TYPE_PORTAL || e->obj->tagType == OBJECT_TYPE_STOP)
	{
		OnCollisionWithPOT(e);
	}
}

void CWorldPlayer::OnCollisionWithPOT(LPCOLLISIONEVENT e)
{
	CWorldMapObject* tmp = dynamic_cast<CWorldMapObject*>(e->obj);
	bool cl, cr, cu, cd;
	tmp->GetMove(cl, cu, cr, cd);
	SetMove(cl, cu, cr, cd);
	if (e->obj->tagType == OBJECT_TYPE_PORTAL)
		sceneId = tmp->GetSceneId();
	else
		sceneId = -1;
}

void CWorldPlayer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	x += vx * dt;
	y += vy * dt;
	CCollision::GetInstance()->Process(this, dt, coObjects);

}
void CWorldPlayer::Render()
{
	animation_set->at(level - 1)->Render(x, y);
	RenderBoundingBox();
}
void CWorldPlayer::SetState(int state)
{
	switch (state)
	{
	case PLAYER_STATE_IDLE:
		vx = vy = 0;
		break;
	case PLAYER_STATE_RIGHT:
		vx = PLAYER_SPEED;
		vy = 0;
		break;
	case PLAYER_STATE_LEFT:
		vx = -PLAYER_SPEED;
		vy = 0;
		break;
	case PLAYER_STATE_UP:
		vx = 0;
		vy = -PLAYER_SPEED;
		break;
	case PLAYER_STATE_DOWN:
		vx = 0;
		vy = PLAYER_SPEED;
		break;
	}
	CGameObject::SetState(state);
}


#include "Portal.h"
#include "Game.h"
#include "Textures.h"
#include "Mario.h"
#include "PlayScene.h"

CPortal::CPortal(float x, float y, int scene_id)
{
	this->scene_id = scene_id;
	this->start_x = x;
	this->start_y = y;
}

void CPortal::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	CGame::GetInstance()->Draw(x - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CPortal::Render()
{
	RenderBoundingBox();
}

void CPortal::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PORTAL_BBOX_WIDTH;
	t = y - PORTAL_BBOX_HEIGHT;
	r = x + PORTAL_BBOX_WIDTH;
	b = y + PORTAL_BBOX_HEIGHT;
}

void CPortal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;

	if (mario != NULL)
	{
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		GetBoundingBox(oLeft, oTop, oRight, oBottom);

		if (isColliding(floor(mLeft), floor(mTop), ceil(mRight), ceil(mBottom))
			&& mLeft >= oLeft && mRight <= oRight)
		{
			mario->portal = this;
			mario->isSwitchMap = true;
			if (scene_id == 1) {
				// mario start up
				DebugOut(L"start go to up new scene \n");
				mario->StartPipeUp();
			}
			if (scene_id == 2 && mario->isSitting)
			{
				// mario start down
				DebugOut(L"start go to down new scene \n");
				mario->StartPipeDown();
			}
			return;
		}
	}
}
#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/

#define PORTAL_BBOX_HEIGHT	2
#define PORTAL_BBOX_WIDTH	26

class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 

	float width;
	float height;
public:
	float start_x = 0;
	float start_y = 0;
	bool pipeUp = false;
	CPortal(float x, float y, int scene_id);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void RenderBoundingBox(void);

	int GetSceneId() { return scene_id; }
	int IsBlocking() { return 0; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};
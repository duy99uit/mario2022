#pragma once
#include "GameObject.h"

#define OBJECT_TYPE_PLAYER				0
#define OBJECT_TYPE_BRICK				1
#define OBJECT_TYPE_STOP				2
#define OBJECT_TYPE_PORTAL				3
#define OBJECT_TYPE_BUSH				4
#define OBJECT_TYPE_HAMMER				5

#define HAMMER_LIMIT_X		176
#define OBJECT_BBOX_WIDTH	4
class CWorldMapObject :
	public CGameObject
{
	int sceneId;
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 1; }
	CWorldMapObject(int sceneId = -1);
	int GetSceneId() { return sceneId; };
};

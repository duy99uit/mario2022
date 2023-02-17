#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Map.h"
#include "Hud.h"
#include "Card.h"
//#include "Koopas.h"

#define GAMEDONE1_SPRITE_ID		50070
#define GAMEDONE1_SPRITE_ID_1	50071

#define CARD_MUSHROOM		50010
#define CARD_FIREFLOWER		50011
#define CARD_STAR			50012

class CPlayScene : public CScene
{
protected:
	// A play scene has to have player, right? 
	CMario* player;
	CMap* current_map = NULL;
	HUD* hud;

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEMAP_DATA(string line);

	void LoadObjects(LPCWSTR assetFile);
	// for end game text
	LPSPRITE gamedone = nullptr;

public:
	bool isTurnOnCamY = false;
	double  multiScene = 1.65;
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void SetCam(float cx, float cy, DWORD dt = 0);
	void SetTurnOnCamY(bool isOn) { isTurnOnCamY = isOn; }

	CMario* GetPlayer() { return player; }

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);

	void AddObjectToScene(LPGAMEOBJECT obj) { this->objects.push_back(obj); }

	// get all obj from scene
	vector<LPGAMEOBJECT> GetObjects() { return this->objects; }
	void SetPlayer(CMario* m) { player = m; }
};

typedef CPlayScene* LPPLAYSCENE;


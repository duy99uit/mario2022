#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"

#include "Block.h"

#include "SampleKeyEventHandler.h"

#include "QuestionBrick.h"
#include "PiranhaPlant.h"
#include "PiranhaPlantFire.h"
#include "Koopas.h"
#include "Card.h"
#include "BreakableBrick.h"
#include "Hud.h"
#include "Mario.h"
using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	hud = NULL;
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define	INTROSCENE	0
#define	WORLDSCENE	1
#define	PLAYSCENE	2

#define SCENE_SECTION_UNKNOWN		   -1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS			6
#define SCENE_SECTION_TILEMAP_DATA		7

#define MAX_SCENE_LINE					1024


void CPlayScene::_ParseSection_TILEMAP_DATA(string line)
{

	int ID, rowMap, columnMap, columnTile, rowTile, totalTiles;
	LPCWSTR path = ToLPCWSTR(line);
	ifstream f;
	f.open(path);
	f >> ID >> rowMap >> columnMap >> rowTile >> columnTile >> totalTiles;
	//Init Map Matrix
	int** TileMapData = new int* [rowMap];
	for (int i = 0; i < rowMap; i++)
	{
		TileMapData[i] = new int[columnMap];
		int j;
		for (j = 0; j < columnMap; j++) {
			f >> TileMapData[i][j];
			//DebugOut(L"[INFO] _ParseSection_TILEMAP %d \n", TileMapData[i][j]);
		}
	}
	f.close();

	current_map = new CMap(ID, rowMap, columnMap, rowTile, columnTile, totalTiles);
	current_map->ExtractTileFromTileSet();
	current_map->SetTileMapData(TileMapData);
	//mapWidth = current_map->GetMapWidth();
	DebugOut(L"[INFO] _ParseSection_TILEMAP_DATA done:: \n");

}
void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	if (ID == GAMEDONE1_SPRITE_ID) {
		DebugOut(L"game done true \n");
		gamedone = CSprites::GetInstance()->Get(ID);
	}


	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}
void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id
	int ani_set_id = atoi(tokens[0].c_str());
	LPANIMATION_SET s;
	if (CAnimationSets::GetInstance()->animation_sets[ani_set_id] != NULL)
		s = CAnimationSets::GetInstance()->animation_sets[ani_set_id];
	else
		s = new CAnimationSet();
	CAnimations* animations = CAnimations::GetInstance();

	for (unsigned int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}
	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}
/*
	Parse a line in section [OBJECTS]
*/

void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadObjects(path.c_str());

}

void CPlayScene::LoadObjects(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);
	if (!f)
		DebugOut(L"\nFailed to open object file!");

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);
		vector<string> tokens = split(line);

		if (line[0] == '#') continue;
		// skip invalid lines - an object set must have at least id, x, y
		if (tokens.size() < 3) return;

		int ani_set_id, tag = 0, option_tag_1 = 0, option_tag_2 = 0, option_tag_3 = 0;
		int object_type = atoi(tokens[0].c_str());
		float  x = 0, y = 0;
		if (object_type != 999)
		{
			x = (float)atof(tokens[1].c_str());
			y = (float)atof(tokens[2].c_str());

			ani_set_id = (int)atoi(tokens[3].c_str());
			if (tokens.size() >= 5)
				tag = (int)atof(tokens[4].c_str());
			if (tokens.size() >= 6)
				option_tag_1 = (int)atof(tokens[5].c_str());
			if (tokens.size() >= 7)
				option_tag_2 = (int)atof(tokens[6].c_str());
			if (tokens.size() >= 8)
				option_tag_3 = (int)atof(tokens[7].c_str());
		}

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();

		CGameObject* obj = NULL;

		switch (object_type)
		{
		case OBJECT_TYPE_MARIO:
			if (player != NULL)
			{
				DebugOut(L"[ERROR] MARIO object was created before!\n");
				return;
			}
			obj = new CMario(x, y);
			player = (CMario*)obj;
			DebugOut(L"[INFO] Mario object created!\n", obj);
			break;
		case OBJECT_TYPE_GOOMBA:
			obj = new CGoomba(tag);
			obj->SetTagType(tag);
			//DebugOut(L"[INFO] Goomba object created!\n", obj);
			break;
		case OBJECT_TYPE_KOOPAS:
			obj = new CKoopas(tag);
			obj->SetTagType(tag);
			break;
		case OBJECT_TYPE_PIRANHAPLANT:
			obj = new PiranhaPlant();
			((PiranhaPlant*)obj)->SetLimitY(y);
			obj->SetZIndex(-1);
			break;
		case OBJECT_TYPE_FIREPIRANHAPLANT:
			obj = new PiranhaPlantFire(tag);
			((PiranhaPlantFire*)obj)->SetLimitY(y);
			DebugOut(L"Running here\n");
			obj->SetZIndex(-1);
			break;
		case OBJECT_TYPE_BRICK:
			obj = new CBrick();
			break;
		case OBJECT_TYPE_QUESTIONBRICK:
			obj = new QuestionBrick(option_tag_1);
			break;
		case OBJECT_TYPE_BREAKABLEBRICK:
			obj = new BreakableBrick(x, y, option_tag_1, option_tag_2, option_tag_3);
			DebugOut(L"[INFO] Break brick is not null!\n");
			break;
		case OBJECT_TYPE_BLOCK:
			obj = new CBlock(x, y);
			break;
		case OBJECT_TYPE_COIN:
			obj = new CCoin(tag);
			break;
		case OBJECT_TYPE_CARD:
			obj = new CardItem();
			DebugOut(L"[INFO] Card was create \n");
			break;
		case OBJECT_TYPE_PORTAL:
		{
			int scene_id = atoi(tokens[4].c_str());
			int isToExtraScene = atoi(tokens[5].c_str());
			float start_x = 0, start_y = 0;
			start_x = (float)atoi(tokens[6].c_str());
			start_y = (float)atoi(tokens[7].c_str());

			obj = new CPortal(start_x, start_y, scene_id);
			DebugOut(L"[INFO] CPortal was create \n");
			int pipeUp = atoi(tokens[8].c_str());
			if (pipeUp == 1)
				((CPortal*)obj)->pipeUp = true;
			else
				((CPortal*)obj)->pipeUp = false;
			break;
		}
		default:
			DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
			return;
		}
		if (object_type != GRID)
		{
			// General object setup
			obj->SetPosition(x, y);
			objects.push_back(obj);
			LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
			obj->SetAnimationSet(ani_set);
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);
		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[TILEMAP DATA]") {
			section = SCENE_SECTION_TILEMAP_DATA; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_TILEMAP_DATA: _ParseSection_TILEMAP_DATA(line); break;
		}
	}

	f.close();
	hud = new HUD(PLAYSCENE_HUD); // new hud
	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	CGame* game = CGame::GetInstance();
	float cam_x, cam_y;
	game->GetCamPos(cam_x, cam_y);

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->isSW) {
		player->Update(dt * multiScene, &coObjects);
	}
	else {
		player->Update(dt, &coObjects);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	if (player == NULL) return;
	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);
	SetCam(cx, cy, dt);
	hud->Update(dt, &coObjects); // update for hud
	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	
	current_map->DrawMap();
	player->Render();

	// zIndex Render Front - Behind

	sort(this->objects.begin(), this->objects.end(), [](const CGameObject* lObj, const CGameObject* rObj) {
		return lObj->z < rObj->z;
		});

	for (int i = 0; i < objects.size(); i++) {
		objects[i]->Render();
	}
	hud->Render();

	// end game text
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->isFinish) {
		CSprites::GetInstance()->Get(GAMEDONE1_SPRITE_ID)->Draw(2688, 270);
		CSprites::GetInstance()->Get(GAMEDONE1_SPRITE_ID_1)->Draw(2688, 300);
		DebugOut(L"mario->cardItemSc: %d \n", mario->cardItemSc);
		if (mario->cardItemSc == 1) {
			CSprites::GetInstance()->Get(CARD_MUSHROOM)->Draw(2750, 300);
		}
		else if (mario->cardItemSc == 2) {
			CSprites::GetInstance()->Get(CARD_FIREFLOWER)->Draw(2750, 300);
		}
		else if (mario->cardItemSc == 3) {
			CSprites::GetInstance()->Get(CARD_STAR)->Draw(2750, 300);
		}
	}
}

void CPlayScene::SetCam(float cx, float cy, DWORD dt) {
	float sw, sh, mw, mh, mx, my;
	CGame* game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	sw = game->GetBackBufferWidth();
	sh = game->GetBackBufferHeight() - 32;
	mw = current_map->GetMapWidth();
	mh = current_map->GetMapHeight();
	cx -= sw / 2;
	// CamX
	if (cx <= 0)//Left Edge
		cx = 0;
	if (cx >= mw - sw)//Right Edge
		cx = mw - sw;


	// handle CamY when fly
	if (mario->isFlying && mario->GetY() < 300) {
		isTurnOnCamY = true;
		DebugOut(L"GetY: %d", mario->GetY());
	}
	else if (mario->GetLevel() == MARIO_LEVEL_TAIL && mario->isOnPlatform && mario->GetY() < 300) {
		isTurnOnCamY = true;
		DebugOut(L"GetY: %d", mario->GetY());
	}
	else if (mario->isOnPlatform) {
		isTurnOnCamY = false;
	}

	//CamY
	if (isTurnOnCamY)
	{
		DebugOut(L"cyyyyyyy: %d \n", cy);
		cy -= sh / 2;
	}
	else
	{
		DebugOut(L"cyyyyyyy: %d \n", cy);
		//cy -= sh / 2;
		cy = mh - sh;
	}


	if (cy <= 0)//Top Edge
		cy = 0;
	if (cy + sh >= mh)//Bottom Edge
		cy = mh - sh;

	game->SetCamPos(ceil(cx), ceil(cy));
	current_map->SetCamPos(cx, cy);
	hud->SetPosition(ceil(cx + 130), ceil(cy + sh + 20));
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}
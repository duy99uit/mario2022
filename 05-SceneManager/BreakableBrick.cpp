#include "BreakableBrick.h"
#include "GameObject.h"
#include "Coin.h"
#include "PlayScene.h"

void BreakableBrick::Render()
{
	if (isDeleted)
		return;
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void BreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

}


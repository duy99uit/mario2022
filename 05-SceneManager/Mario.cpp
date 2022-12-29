#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Block.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"

#include "Collision.h"

#include "QuestionBrick.h"
#include "Mushroom.h"
#include "PiranhaPlant.h"
#include "Koopas.h"
#include "PiranhaPlantFire.h"
#include "Leaf.h"
#include "Card.h"
#include "FireBullet.h"
#include "Switch.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (vy < -0.27f && level != MARIO_LEVEL_TAIL) {
		DebugOut(L"update vy");
		vy = -0.27f;
		pullDown();
	}
	if (vy < -0.27f && level == MARIO_LEVEL_TAIL && !isFlying) {
		DebugOut(L"update vy");
		vy = -0.27f;
		pullDown();
	}

	if (!isFlying)
	HandleMarioJump();
	HandleFlying();
	HandleFlapping();
	HandleTurning();
	HandleMarioKick();
	HandleFinishMap();

	for (int i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CBlock*>(obj))
		{
			if (obj->getY() - 16 < this->y) {
				obj->SetIsBlocking(0);
			}
			else {
				// collision
				obj->SetIsBlocking(1);
			}
		}
	}


	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	/*isOnPlatform = false;*/
	
	tail->Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	/*x = 16;
	y = 16;*/
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else {
		if (e->nx != 0 && e->obj->IsBlocking())
		{
			vx = 0;
		}
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);		
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<QuestionBrick*>(e->obj)) {
		OnCollisionWithQuestionBrick(e);
	}
	else if (dynamic_cast<CMushroom*>(e->obj))
	{
		OnCollisionWithMushroom(e);
	}
	else if (dynamic_cast<PiranhaPlant*>(e->obj))
	{
		DebugOut(L"Mario PiranhaPlant !\n");
		/*OnCollisionWithPiranhaPlant(e);*/
	}
	else if (dynamic_cast<PiranhaPlantFire*>(e->obj))
	{
		DebugOut(L"mario collision with piranhaPlantFire \n");
		OnCollisionWithPiranhaPlantFire();
	}
	else if (dynamic_cast<CKoopas*>(e->obj)) {
		DebugOut(L"mario collision with koopas \n");
		OnCollisionWithKoopas(e);
	}
	else if (dynamic_cast<CLeaf*>(e->obj)) {
		DebugOut(L"mario collision with leaf \n");
		OnCollisionWithLeaf(e);
	}
	else if (dynamic_cast<FireBullet*>(e->obj)) {
		
	}
	else if (dynamic_cast<Switch*>(e->obj)) {
		OnCollisionWithSwitch(e);
	}

		
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 

	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			if (goomba->tagType == GOOMBA_RED)
				goomba->SetTagType(GOOMBA_RED_NORMAL);
			else
				goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED_GB;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{

			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				
				HandleMarioDie();
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e)
{
	QuestionBrick* questionBrick = dynamic_cast<QuestionBrick*>(e->obj);

	// Hit from bottom
	if (e->ny > 0) {
		DebugOut(L"OnCollisionWithQuestionBrick SetState!\n");
		vy = 0;
		questionBrick->SetState(QUESTION_BRICK_HIT);
		
	}
}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CMushroom* mushRoom = dynamic_cast<CMushroom*>(e->obj);
	DebugOut(L"Mario OnCollisionWithMushRoom - change state - big!\n");
	e->obj->Delete();
	
	if (mushRoom->GetTypeMushRoom() != MUSHROOM_GREEN) {
		SetLevel(MARIO_LEVEL_BIG);
	}
}

void CMario::OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e)
{
	DebugOut(L"Mario OnCollisionWithPiranhaPlant !\n");
	if (level != MARIO_LEVEL_SMALL)
	{
		level -= 1;
		SetLevel(level);
	}
	else
	{
		/*SetState(MARIO_STATE_DIE);*/
	}
}
void CMario::OnCollisionWithPiranhaPlantFire()
{
	DebugOut(L"mario collision with PiranhaPlantFire \n");
	if (level != MARIO_LEVEL_SMALL)
	{
		level -= 1;
		SetLevel(level);
		//DebugOut(L">>> Mario tranfer small >>> \n");
	}
	else
	{
		DebugOut(L">>> Mario die >>> \n");
		//SetState(MARIO_STATE_DIE);
	}
}
void CMario::OnCollisionWithKoopas(LPCOLLISIONEVENT e) {

	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);

	DebugOut(L"in mario collision with koopas \n");

	if (e->nx != 0) {
		DebugOut(L"mario collision with koopas if 1");
		if (koopas->GetState() == KOOPAS_STATE_IN_SHELL || koopas->GetState() == KOOPAS_STATE_SHELL_UP) {
			if (isReadyToHold) {
				isHolding = true;
				koopas->SetCanBeHeld(true);
			}
			else {
				SetState(MARIO_STATE_KICK);
				koopas->SetState(KOOPAS_STATE_TURNING);
			}
		}
		else {
			HandleMarioDie();
		}
	}
	if (e->ny != 0) {
		DebugOut(L"mario collision with koopas if 2");
		vy = -MARIO_JUMP_DEFLECT_SPEED_GB;
		if (koopas->GetState() == KOOPAS_STATE_WALKING) {
			if (koopas->tagType == KOOPAS_GREEN_PARA) {
				koopas->SetTagType(KOOPAS_GREEN);
			}
			else koopas->SetState(KOOPAS_STATE_IN_SHELL);
		}
		else if (koopas->GetState() == KOOPAS_STATE_IN_SHELL) {
			koopas->SetState(KOOPAS_STATE_TURNING);
		}
		else if (koopas->GetState() == KOOPAS_STATE_TURNING) {
			koopas->SetState(KOOPAS_STATE_IN_SHELL);
		}
	}

}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	CLeaf* leaf = dynamic_cast<CLeaf*>(e->obj);
	if (e->ny != 0 || e->nx != 0) {
		DebugOut(L"Mario tranform to tail \n");
		SetLevel(MARIO_LEVEL_TAIL);
		leaf->SetAppear(false);
		e->obj->Delete();
	}
}
void CMario::OnCollisionWithCardItem(LPCOLLISIONEVENT e) {
	CardItem* card = dynamic_cast<CardItem*>(e->obj);
	if (e->ny != 0 || e->nx != 0) {
		card->SetAppear(false);
		card->isDeleted = true;
		isFinish = true;
	}
}
void CMario::OnCollisionWithFireBullet(LPCOLLISIONEVENT e) {
	FireBullet* firebullet = dynamic_cast<FireBullet*>(e->obj);

	if (e->ny != 0 || e->nx != 0) {
		DebugOut(L"Mario collision with fire bullet \n");
		if (level == MARIO_LEVEL_TAIL)
		{
			SetLevel(MARIO_LEVEL_BIG);
			StartUntouchable();
			DebugOut(L">>> tail to big >>>%d \n", level);
		}
		else if (level == MARIO_LEVEL_BIG) {
			SetLevel(MARIO_LEVEL_SMALL);
			StartUntouchable();
			DebugOut(L">>> big to small >>>%d \n", level);
		}
		else
		{
			DebugOut(L">>> Mario basic die >>> \n");
			//SetState(MARIO_STATE_DIE);
		}
		e->obj->Delete();
	}
}


void CMario::OnCollisionWithSwitch(LPCOLLISIONEVENT e) {
	Switch* sw = dynamic_cast<Switch*>(e->obj);
	if (e->ny < 0) {
		if (sw->GetState() != SWITCH_STATE_PRESSED) {
			sw->SetState(SWITCH_STATE_PRESSED);
			sw->isDeleted = true;
			vy = -MARIO_JUMP_DEFLECT_SPEED_GB;
		}
	}
}


//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = MARIO_ANI_SMALL_JUMPINGUP_RIGHT;
			else
				aniId = MARIO_ANI_SMALL_JUMPINGUP_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = MARIO_ANI_SMALL_JUMPINGUP_RIGHT;
			else
				aniId = MARIO_ANI_SMALL_JUMPINGUP_LEFT;
		}
	}
	if (state == MARIO_STATE_JUMP || state == MARIO_STATE_RELEASE_JUMP || isHolding || isKick) {
		if (nx > 0) {
			aniId = MARIO_ANI_SMALL_JUMPINGUP_RIGHT;
			if (isHolding) {
				aniId = MARIO_ANI_SMALL_HOLD_RUNNING_RIGHT;
			}
			else if (isKick)
			{
				aniId = MARIO_ANI_SMALL_KICKING_RIGHT;
			}
		}
		if (nx < 0) {
			aniId = MARIO_ANI_SMALL_JUMPINGUP_LEFT;
			if (isHolding) {
				aniId = MARIO_ANI_SMALL_HOLD_RUNNING_LEFT;
			}
			else if (isKick)
			{
				aniId = MARIO_ANI_SMALL_KICKING_LEFT;
			}
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = MARIO_ANI_SMALL_IDLE_RIGHT;
			else
				aniId = MARIO_ANI_SMALL_IDLE_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) {
					aniId = MARIO_ANI_SMALL_IDLE_RIGHT;
					if (isKick) {
						aniId = MARIO_ANI_SMALL_KICKING_RIGHT;
					}
				}
				else {
					aniId = MARIO_ANI_SMALL_IDLE_LEFT;
					if (isKick) {
						aniId = MARIO_ANI_SMALL_KICKING_LEFT;
					}
				}
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = MARIO_ANI_SMALL_BRAKING_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = MARIO_ANI_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X) {
					aniId = MARIO_ANI_SMALL_WALKING_RIGHT;
				}

				if (!isOnPlatform) {
					aniId = MARIO_ANI_SMALL_JUMPINGUP_RIGHT;
				}
				if (isKick) {
					aniId = MARIO_ANI_SMALL_KICKING_RIGHT;
				}

			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = MARIO_ANI_SMALL_BRAKING_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = MARIO_ANI_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = MARIO_ANI_SMALL_WALKING_LEFT;

				if (!isOnPlatform) {
					aniId = MARIO_ANI_SMALL_JUMPINGUP_LEFT;
				}
				if (isKick) {
					aniId = MARIO_ANI_SMALL_KICKING_LEFT;
				}
			}

	if (aniId == -1) aniId = MARIO_ANI_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = MARIO_ANI_BIG_RUNNING_RIGHT;
			else
				aniId = MARIO_ANI_BIG_RUNNING_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = MARIO_ANI_BIG_WALKING_FAST_RIGHT;
			else
				aniId = MARIO_ANI_BIG_WALKING_FAST_LEFT;
		}
	}
	if (state == MARIO_STATE_JUMP || state == MARIO_STATE_RELEASE_JUMP) {
		if (nx > 0) {
			aniId = MARIO_ANI_BIG_JUMPINGUP_RIGHT;
			if (isHolding) {
				aniId = MARIO_ANI_BIG_HOLD_JUMPINGUP_RIGHT;
			}
			else if (isKick)
			{
				aniId = MARIO_ANI_BIG_KICKING_RIGHT;

			}
		}
		if (nx < 0) {
			aniId = MARIO_ANI_BIG_JUMPINGUP_LEFT;
			if (isHolding) {
				aniId = MARIO_ANI_BIG_HOLD_JUMPINGUP_LEFT;
			}
			else if (isKick)
			{
				aniId = MARIO_ANI_BIG_KICKING_LEFT;
			}
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = MARIO_ANI_BIG_SITTING_RIGHT;
			else
				aniId = MARIO_ANI_BIG_SITTING_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) {
					aniId = MARIO_ANI_BIG_IDLE_RIGHT;
					if (isKick) {
						aniId = MARIO_ANI_BIG_KICKING_RIGHT;
					}
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_IDLE_RIGHT;
					}
				}
				else {
					aniId = MARIO_ANI_BIG_IDLE_LEFT;
					if (isKick) {
						aniId = MARIO_ANI_BIG_KICKING_LEFT;
					}
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_IDLE_LEFT;
					}
				}
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = MARIO_ANI_BIG_BRAKING_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
				{
					aniId = MARIO_ANI_BIG_RUNNING_RIGHT;
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_RUNNING_RIGHT;
					}
				}
				else if (ax == MARIO_ACCEL_WALK_X) {
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_WALKING_RIGHT;
					}
					aniId = MARIO_ANI_BIG_WALKING_RIGHT;
				}

				if (!isOnPlatform) {
					aniId = MARIO_ANI_BIG_JUMPINGUP_RIGHT;
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_JUMPINGUP_RIGHT;
					}
				}
				if (isKick) {
					aniId = MARIO_ANI_BIG_KICKING_RIGHT;
				}
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = MARIO_ANI_BIG_BRAKING_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X) {
					aniId = MARIO_ANI_BIG_RUNNING_LEFT;
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_RUNNING_LEFT;
					}
				}
				else if (ax == -MARIO_ACCEL_WALK_X)
				{
					aniId = MARIO_ANI_BIG_WALKING_LEFT;
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_WALKING_LEFT;
					}
				}

				if (!isOnPlatform) {
					aniId = MARIO_ANI_BIG_JUMPINGUP_LEFT;
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_JUMPINGUP_LEFT;
					}
				}

				if (isKick) {
					aniId = MARIO_ANI_BIG_KICKING_LEFT;
				}
			}

	if (aniId == -1) aniId = MARIO_ANI_BIG_IDLE_RIGHT;

	return aniId;
}

int CMario::GetAniIdTail() {
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = MARIO_ANI_TAIL_RUNNING_RIGHT;
			else
				aniId = MARIO_ANI_TAIL_RUNNING_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = MARIO_ANI_TAIL_WALKING_FAST_RIGHT;
			else
				aniId = MARIO_ANI_TAIL_WALKING_FAST_LEFT;
		}
	}
	if (state == MARIO_STATE_JUMP || state == MARIO_STATE_RELEASE_JUMP || isHolding || isKick) {
		if (nx > 0) {
			aniId = MARIO_ANI_TAIL_JUMPINGUP_RIGHT;
			if (isFlying) {
				aniId = MARIO_ANI_FLY_UP_RIGHT;
			}
			if (isFlapping)
				aniId = MARIO_ANI_FLAPPING_RIGHT;
			else if (isFlappingFlying)
				aniId = MARIO_ANI_FLY_FLAPPING_RIGHT;
			else if (isHolding) {
				aniId = MARIO_ANI_TAIL_HOLD_RUNNING_RIGHT;
			}
			else if (isKick)
			{
				aniId = MARIO_ANI_TAIL_KICKING_RIGHT;
			}
		}
		if (nx < 0) {
			aniId = MARIO_ANI_TAIL_JUMPINGUP_LEFT;
			if (isFlying) {
				aniId = MARIO_ANI_FLY_UP_LEFT;
			}
			if (isFlapping)
				aniId = MARIO_ANI_FLAPPING_LEFT;
			else if (isFlappingFlying)
				aniId = MARIO_ANI_FLY_FLAPPING_LEFT;
			else if (isHolding) {
				aniId = MARIO_ANI_TAIL_HOLD_RUNNING_LEFT;
			}
			else if (isKick)
			{
				aniId = MARIO_ANI_TAIL_KICKING_LEFT;
			}
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = MARIO_ANI_TAIL_SITTING_RIGHT;
			else
				aniId = MARIO_ANI_TAIL_SITTING_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = MARIO_ANI_TAIL_IDLE_RIGHT;
				else aniId = MARIO_ANI_TAIL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = MARIO_ANI_TAIL_BRAKING_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
				{
					aniId = MARIO_ANI_TAIL_RUNNING_RIGHT;
				}
				else if (ax == MARIO_ACCEL_WALK_X) {
					aniId = MARIO_ANI_TAIL_WALKING_RIGHT;
				}

				if (!isOnPlatform) {
					aniId = MARIO_ANI_TAIL_JUMPINGUP_RIGHT;
					if (isFlying) {
						aniId = MARIO_ANI_FLY_UP_RIGHT;
						if (isFlappingFlying)
							aniId = MARIO_ANI_FLY_FLAPPING_RIGHT;
					}
					if (isFlapping)
						aniId = MARIO_ANI_FLAPPING_RIGHT;
				}
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = MARIO_ANI_TAIL_BRAKING_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = MARIO_ANI_TAIL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = MARIO_ANI_TAIL_WALKING_LEFT;

				if (!isOnPlatform) {
					aniId = MARIO_ANI_TAIL_JUMPINGUP_LEFT;
					if (isFlying) {
						aniId = MARIO_ANI_FLY_UP_LEFT;
						if (isFlappingFlying)
							aniId = MARIO_ANI_FLY_FLAPPING_LEFT;
					}
					if (isFlapping)
						aniId = MARIO_ANI_FLAPPING_LEFT;
				}
			}

	if (aniId == -1) aniId = MARIO_ANI_TAIL_IDLE_RIGHT;

	return aniId;
}

void CMario::Render()
{
	int aniId = -1;
	int iY = 8;
	if (state == MARIO_STATE_DIE)
		aniId = MARIO_ANI_DIE;
	else if (level == MARIO_LEVEL_SMALL)
	{
		aniId = GetAniIdSmall();
	}
	else if (level == MARIO_LEVEL_BIG) {
		aniId = GetAniIdBig();
	}
	else if (level == MARIO_LEVEL_TAIL)
		aniId = GetAniIdTail();

	if (isSitting) {
		animation_set->at(aniId)->Render(x, y + 5);
	}
	else if (level == MARIO_LEVEL_TAIL) {
		if (state == MARIO_STATE_TAIL_ATTACK)
		{
			DebugOut(L"[Turning stack]:: %d\n", turningStack);
			if (isTuring && nx > 0) {
				if (turningStack == 1 || turningStack == 5) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_RIGHT_1_ID)->Draw(x - 8, y);
				if (turningStack == 2) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_RIGHT_2_ID)->Draw(x, y);
				if (turningStack == 3) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_RIGHT_3_ID)->Draw(x, y);
				if (turningStack == 4) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_RIGHT_4_ID)->Draw(x, y);
			}
			if (isTuring && nx < 0) {
				if (turningStack == 1 || turningStack == 5) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_LEFT_1_ID)->Draw(x, y);
				if (turningStack == 2) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_LEFT_2_ID)->Draw(x, y);
				if (turningStack == 3) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_LEFT_3_ID)->Draw(x - 8, y);
				if (turningStack == 4) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_LEFT_4_ID)->Draw(x, y);
			}
		}

		else animation_set->at(aniId)->Render(nx > 0 ? x - MARIO_DIFF : x + MARIO_DIFF, y);
	}
	else {
		animation_set->at(aniId)->Render(x, y);
	}

	DebugOutTitle(L"Coins: %d", coin);

	RenderBoundingBox();
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		// handle fly
		isReadyToRun = true;
		if (vx >= MARIO_SPEED_STACK && isReadyToRun) {
			isRunning = true;
			//DebugOut(L"isRunning true \n");
		}
		else {
			isRunning = false;
		}
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		// handle fly
		isReadyToRun = true;
		if (vx < MARIO_SPEED_STACK && isReadyToRun) {
			isRunning = true;
			//DebugOut(L"isRunning true \n");
		}
		else {
			isRunning = false;
		}
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		// handle fly
		isRunning = false;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		// handle fly
		isRunning = false;
		break;
	case MARIO_STATE_JUMP:
		isJumping = true;
		if (isSitting) break;
		if (isOnPlatform)
		{
	
			if (vy > -MARIO_JUMP_SPEED_MIN) {
				DebugOut(L" vy: %f \n", vy);
				vy = -MARIO_JUMP_SPEED_MIN;
				DebugOut(L" vy: %f \n", vy);
			}
			ay = -MARIO_ACCELERATION_JUMP;

		}
		if (isRunning) {
			if (level == MARIO_LEVEL_TAIL) {
				DebugOut(L"mario can fly \n");
				isFlying = true;
				StartFlying();
			}
			else {
				DebugOut(L"mario not tail \n");
			}
		}
		normalFallDown = false;
		isOnPlatform = false;
		break;

	case MARIO_STATE_RELEASE_JUMP:
		isJumping = false;
		pullDown();
		
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0;
			vy = 0;
			ay = 0;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -=MARIO_SIT_HEIGHT_ADJUST;
			ay = MARIO_GRAVITY;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		/*ay = MARIO_GRAVITY;*/
		isJumping = false;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	case MARIO_STATE_KICK:
		StartKicking();
		break;
	case MARIO_STATE_TAIL_ATTACK:
		if (level == MARIO_LEVEL_TAIL) {
			if (!isTuring) {
				turningStack = 0;
				StartTurning();
				//DebugOut(L"Start Attack by Tail \n");
			}
		}
		break;
	}
	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (level != MARIO_LEVEL_SMALL)
	{

		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
		if (state == MARIO_STATE_SIT) {
			bottom = top + MARIO_BBOX_SIT_HEIGHT;
		}
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
	}
	level = l;
}

void CMario::HandleMarioJump() {
	if (isJumping) {
		// Dung yen nhay
		if (vx == 0)
		{
			if (vy < -MARIO_JUMP_MAX) {
				pullDown();
			}
		}
		// Di chuyen nhay phai
		if (vx > 0) {
			// vx lon nhat
			if (vx >= MARIO_SPEED_MAX) {
				// super jump
				if (vy < -MARIO_SUPER_JUMP_MAX) {
					pullDown();
				}
			}
			else if (vx < MARIO_SPEED_MAX && vx > 0) {
				if (vy < -MARIO_JUMP_MAX) {
					DebugOut(L"vua chay vua jump \n");
					pullDown();
				}
			}
		}
		//Di chuyen nhay trai
		if (vx < 0) {
			// vx lon nhat
			if (abs(vx) >= MARIO_SPEED_MAX) {
				// super jump
				if (vy < -MARIO_SUPER_JUMP_MAX) {
					DebugOut(L"super jump \n");
					pullDown();
				}
			}
			else if (abs(vx) < MARIO_SPEED_MAX && vx < 0) {
				if (vy < -MARIO_JUMP_MAX) {
					DebugOut(L"vua chay vua jump \n");
					pullDown();
				}
			}
		}

	}
}
void CMario::HandleMarioKick() {
	if (isKick) {
		if (GetTickCount64() - start_kicking > MARIO_KICKING_TIME) {
			StopKick();
			SetState(MARIO_STATE_IDLE);
		}
	}
}
void CMario::HandleMarioDie() {

	if (level != MARIO_LEVEL_SMALL)
	{
		level -= 1;
		SetLevel(level);
		StartUntouchable();
		DebugOut(L">>> Mario tranfer to >>>%d \n", level);
	}
	else
	{
		DebugOut(L">>> Mario basic die >>> \n");
		/*SetState(MARIO_STATE_DIE);*/
	}
}
void CMario::HandleTurning() {

	if (GetTickCount64() - start_turning >= MARIO_TURNING_STATE_TIME && isTuring) {
		start_turning = GetTickCount64();
		turningStack++;
		DebugOut(L"Handle turning:: %d \n", turningStack);
	}
	if (GetTickCount64() - start_turning_state > MARIO_TURNING_TAIL_TIME && isTuring) {
		isTuring = false;
		start_turning_state = 0;
		start_turning = 0;
		turningStack = 0;
	}

}

void CMario::HandleFlying() {

	// mario fly normal, not out logic
	if (level == MARIO_LEVEL_TAIL) {
		if (isFlying)
		{
			if (vy <= -MARIO_NORMAL_FLY_MAX) {
				normalFallDown = true;
				DebugOut(L"Start fall down \n");
			}
		}
	}
	if (normalFallDown && isFlying) {
		ay = 0.0005f;
	}

	// handle fly
	if (GetTickCount64() - fly_start > MARIO_FLYING_TIME && fly_start != 0 && isFlying)
	{
		DebugOut(L"Start fly \n");
		fly_start = 0;
		isRunning = false;
		isFlying = false;
	}
}

void CMario::HandleFlapping() {
	if (level == MARIO_LEVEL_TAIL && isFlapping) {
		vy = MARIO_SLOW_FALLING_SPEED;
	}
}

void CMario::HandleFinishMap() {
	if (isFinish) {
		ax = MARIO_ACCELERATION;
		ay = MARIO_GRAVITY;
		nx = 1;
		vx = MARIO_WALKING_SPEED;
		DebugOut(L"HandleFinishMap Mario collision with Card and go to right - end 1 -1 \n");
		SetState(MARIO_STATE_WALKING_RIGHT);
	}
}

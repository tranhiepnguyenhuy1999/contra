#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Mushroom.h"
#include "Portal.h"
#include "QuestionBrick.h"
#include "DownBrick.h"
#include "KoopaTroopa.h"
#include "ParaKoopaTroopa.h"
#include "Flower.h"
#include "Fire.h"
#include "ParaGoomba.h"
#include "Leaf.h"
#include "Tail.h"

#include "AssetIDs.h"
#include "Collision.h"
#include "UserBoard.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	isOnPlatform = false;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else 
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CParaGoomba*>(e->obj))
		OnCollisionWithParaGoomba(e);
	else if (dynamic_cast<CKoopaTroopa*>(e->obj))
		OnCollisionWithKoopaTroopa(e);
	else if (dynamic_cast<CParaKoopaTroopa*>(e->obj))
		OnCollisionWithParaKoopaTroopa(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CQuestionBrick*>(e->obj))
		OnCollisionWithQuestionBrick(e);
	else if (dynamic_cast<CDownBrick*>(e->obj))
		OnCollisionWithDownBrick(e);
	else if (dynamic_cast<CFlower*>(e->obj))
		OnCollisionWithFlower(e);
	else if (dynamic_cast<CFire*>(e->obj))
		OnCollisionWithFire(e);
	else if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWithLeaf(e);
}
void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}
void CMario::OnCollisionWithParaGoomba(LPCOLLISIONEVENT e)
{
	CParaGoomba* goomba = dynamic_cast<CParaGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{

		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			if (goomba->GetState() != PARAGOOMBA_STATE_WALKING)
			{
				goomba->SetState(PARAGOOMBA_STATE_WALKING);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
			else
			{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}
void CMario::OnCollisionWithFlower(LPCOLLISIONEVENT e)
{
		if (untouchable == 0)
		{
			if (level > MARIO_LEVEL_SMALL)
			{
				level = MARIO_LEVEL_SMALL;
				StartUntouchable();
			}
			else
			{
				DebugOut(L">>> Mario DIE >>> \n");
				SetState(MARIO_STATE_DIE);
			}
		}
}
void CMario::OnCollisionWithFire(LPCOLLISIONEVENT e)
{
	if (untouchable == 0)
	{
		if (level > MARIO_LEVEL_SMALL)
		{
			level = level-1;
			StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
	}
}
void CMario::OnCollisionWithKoopaTroopa(LPCOLLISIONEVENT e)
{
	CKoopaTroopa* item = dynamic_cast<CKoopaTroopa*>(e->obj);
	// jump on top >> kill Goomba and deflect a bit 
	if (item->GetState() == KOOPATROOPA_STATE_DIE || item->GetState() == KOOPATROOPA_STATE_ALIVE) {
		
		if(e->nx > 0)
			item->SetState(KOOPATROOPA_STATE_KICKING_RIGHT);
		else
			item->SetState(KOOPATROOPA_STATE_KICKING_LEFT);
		return;
	}
	if (e->ny < 0)
	{
		if (item->GetState() != KOOPATROOPA_STATE_DIE)
		{
			item->SetState(KOOPATROOPA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (item->GetState() != KOOPATROOPA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}
void CMario::OnCollisionWithParaKoopaTroopa(LPCOLLISIONEVENT e)
{
	CParaKoopaTroopa* item = dynamic_cast<CParaKoopaTroopa*>(e->obj);
	// jump on top >> kill Goomba and deflect a bit 
	if (item->GetState() == KOOPATROOPA_STATE_DIE || item->GetState() == KOOPATROOPA_STATE_ALIVE) {

		if (e->nx > 0)
			item->SetState(KOOPATROOPA_STATE_KICKING_RIGHT);
		else
			item->SetState(KOOPATROOPA_STATE_KICKING_LEFT);
		return;
	}
	if (e->ny < 0)
	{
		// jumb state
		if (item->GetState() == PARAKOOPATROOPA_STATE_JUMP)
		{
			item->SetState(PARAKOOPATROOPA_STATE_WALKING);
		}
		else
		{
			// walking state
			if (item->GetState() == PARAKOOPATROOPA_STATE_WALKING)
			{
				item->SetState(KOOPATROOPA_STATE_DIE);
			}
		}
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		return;
	}
	else 
	{
		if (untouchable == 0)
		{
			if (item->GetState() != KOOPATROOPA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}
void CMario::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e)
{
	CQuestionBrick* questionBrick = dynamic_cast<CQuestionBrick*>(e->obj);

	if (e->ny > 0)
	{
		if (questionBrick->GetState() == QUESTIONBRICK_STATE_UNTOUCHED)
		{
			float qx, qy, qvx;
			questionBrick->GetPosition(qx, qy);
			
			if (questionBrick->getType() == 1)
			{
				CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_SMALLCOIN, qx, qy-16);
				coin++;
				point += 100;
			}
			else
			{
				// check direction
				if (this->x <= qx + 8) {
					qvx = -1;
				}
				else
				{
					qvx = 1;
				}
				if (level == MARIO_LEVEL_BIG)
				{
					CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_LEAF, qx, qy);
					point += 1000;
				}
				else
				{			
					CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_MUSHROOM, qx, qy, qvx);
					point += 1000;
				}
			}

			questionBrick->SetState(QUESTIONBRICK_STATE_TOUCHED_1);
		}
	}

}
void CMario::OnCollisionWithDownBrick(LPCOLLISIONEVENT e)
{

	CDownBrick* downBrick = dynamic_cast<CDownBrick*>(e->obj);
	if (e->ny < 0)
	{
		isOnPlatform = true;
		vy = 0;
	}
}
void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	if (level == MARIO_LEVEL_SMALL) {
	level = MARIO_LEVEL_BIG;
	}
	y = y - (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
}
void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	level = MARIO_LEVEL_RACOON;
	SetState(MARIO_STATE_RACOON_TRANSFORM);
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
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

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
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}
int CMario::GetAniIdRacoon()
{
	int aniId = -1;
	if (isFlyStak==5) {
		aniId = ID_ANI_RACOON_FLY_RIGHT;
	}
	else
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_RACOON_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_RACOON_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_RACOON_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_RACOON_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_RACOON_IDLE_RIGHT;
				else aniId = ID_ANI_RACOON_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_RACOON_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_RACOON_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_RACOON_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_RACOON_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_RACOON_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_RACOON_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}
void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;
	if (state == MARIO_STATE_RACOON_TRANSFORM) aniId = ID_ANI_RACOON_TRANSFORM;
	else if (state == MARIO_STATE_ATTACK)
	{
	 if(nx>0)
		aniId = ID_ANI_RACOON_ATTACK_RIGHT;
	 else
		 aniId = ID_ANI_RACOON_ATTACK_LEFT;
	}
	else if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_RACOON)
		aniId = GetAniIdRacoon();
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();

	animations->Get(aniId)->Render(x, y);

	RenderBoundingBox();
	
	DebugOutTitle(L"Coins: %d", coin);
}
void CMario::SetState(int state)
{
	if ((this->state == MARIO_STATE_RACOON_TRANSFORM) && (GetTickCount64() - count_start < 500)) return;
	if ((this->state == MARIO_STATE_ATTACK) && (GetTickCount64() - count_start < 500)) return;
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		if(readyFly_start==-1)
		{
			readyFly_start = GetTickCount64();
		}
		else
		{
			if (GetTickCount64() - readyFly_start > 250) {
				if (isFlyStak == 5) {
					fly_start = GetTickCount64();
					readyFly_start = -1;
				}
				else
				{
					isFlyStak++;
					readyFly_start = GetTickCount64();
				}
			}
		}
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		checkFlyStak();
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;

		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		else {
			if (level == 3) {
				vy = -0.15f;
			}
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y +=MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		checkFlyStak();
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_ATTACK:
		if (level != 3) return;
		count_start = GetTickCount64();
		createTailObject();
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	case MARIO_STATE_FLY:
		fly_start = GetTickCount64();
		vy = -MARIO_FLY_SPEED;
		break;
	case MARIO_STATE_RELEASE_FLY:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;
	case MARIO_STATE_RACOON_TRANSFORM:
		count_start = GetTickCount64();
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level!=MARIO_LEVEL_SMALL)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

void CMario::createTailObject() {

	if (nx < 0) {
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_TAIL, x - TAIL_BBOX_WIDTH / 2, y+ MARIO_BIG_BBOX_HEIGHT/2 - TAIL_BBOX_HEIGHT, -1);
	}
	else
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_TAIL, x + TAIL_BBOX_WIDTH / 2, y + MARIO_BIG_BBOX_HEIGHT / 2 - TAIL_BBOX_HEIGHT,1);

	}
}
void CMario::checkFlyStak() {
	if (GetTickCount64() - fly_start > 500) {
		if (readyFly_start == -1)	readyFly_start = GetTickCount64();
		else
		{
			if (GetTickCount64() - readyFly_start > 250) {
				if (isFlyStak == -1) {
					readyFly_start = -1;
				}
				else
				{
					isFlyStak--;
					readyFly_start = GetTickCount64();
				}
			}
		}
	}
}
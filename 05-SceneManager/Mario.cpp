#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Soldier.h"

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

	isOnPlatform = false;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if(state == MARIO_STATE_PRE_DIE ) DebugOut(L">>> Mario DIE >>> %f \n", vx);
	if (state==MARIO_STATE_PRE_DIE && GetTickCount64() - count_start > 500)
	{
		DebugOut(L">>> Mario DIE >>> %f \n", vx);
		count_start = -1;
		SetState(MARIO_STATE_DIE);
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{

	if (state == MARIO_STATE_DIE || state == MARIO_STATE_PRE_DIE) return;
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny > 0) isOnPlatform = true;
	}
	else 
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}
	

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CSoldier*>(e->obj))
		OnCollisionWithSoldier(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
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

					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
			}
		}
	}
}
void CMario::OnCollisionWithSoldier(LPCOLLISIONEVENT e)
{
	CSoldier* i = dynamic_cast<CSoldier*>(e->obj);

		if (untouchable == 0)
		{
			if (i->GetState() != SOLDIER_STATE_DIE)
			{
				DebugOut(L">>> Main touched soldier >>> \n");
				SetState(MARIO_STATE_PRE_DIE);
			}
		}
}
void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

//
// Get animdation ID for big Mario
//
int CMario::GetAniId()
{
	int aniId = -1;

	if (isPreDied)
	{
		if (nx > 0)
			aniId = ID_ANI_MARIO_PRE_DIE_LEFT;
		else
			aniId = ID_ANI_MARIO_PRE_DIE;

	}
	else if (isShooting)
	{
		if (nx >= 0)
			aniId = ID_ANI_MARIO_SHOOTING_RIGHT;
		else
			aniId = ID_ANI_MARIO_SHOOTING_LEFT;
	}
	else if (!isOnPlatform)
	{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_LEFT;
	}
	else if (vx == 0)
			{
				if (isSitting)
				{
					if (nx > 0)
						aniId = ID_ANI_MARIO_SIT_RIGHT;
					else
						aniId = ID_ANI_MARIO_SIT_LEFT;
				}
				else if (isLookingUp) {
					if (nx > 0) 
						aniId = ID_ANI_MARIO_STAND_UP_RIGHT;
					else 
						aniId = ID_ANI_MARIO_STAND_UP_LEFT;
				}
				else if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax == MARIO_ACCEL_RUN_X)
				{
					if (isSitting)
					{
						aniId = ID_ANI_MARIO_RUNNING_DOWN_RIGHT;
					}
					else if (isLookingUp) {
						aniId = ID_ANI_MARIO_RUNNING_UP_RIGHT;
					}
				}
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (isSitting)
				{
					aniId = ID_ANI_MARIO_RUNNING_DOWN_LEFT;
				}
				else if (isLookingUp) {
					aniId = ID_ANI_MARIO_RUNNING_UP_LEFT;
				}
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}
	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;
	return aniId;
}
void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;
	if (state == MARIO_STATE_DIE)
		if (nx > 0)
			aniId = ID_ANI_MARIO_DIE_LEFT;
		else
			aniId = ID_ANI_MARIO_DIE;
	else
		aniId = GetAniId();

	animations->Get(aniId)->Render(x, y);

	RenderBoundingBox();
	
	DebugOutTitle(L"Coins: %d", coin);
}
void CMario::SetState(int state)
{
	if (GetTickCount64() - shooting_start < 250) {
	}
	else
	{
		shooting_start = -1;
		isShooting = false;
	}
	
	if (this->state == MARIO_STATE_PRE_DIE && state!= MARIO_STATE_DIE) return;

	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;
	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		//if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		//if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_RUNNING_UP_LEFT:
	case MARIO_STATE_RUNNING_DOWN_LEFT:
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;

	case MARIO_STATE_RUNNING_UP_RIGHT:
	case MARIO_STATE_RUNNING_DOWN_RIGHT:
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		//if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		//if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		//if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy -= MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y -=MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_SHOOTING:
		shooting_start = GetTickCount64();
		isShooting = true;
		createTailObject();
		break;
	case MARIO_STATE_LOOKUP: 
		if (isOnPlatform)
		{
			state = MARIO_STATE_IDLE;
			isLookingUp = true;
			vx = 0; vy = 0.0f;
		}
		break;
	case MARIO_STATE_LOOKUP_RELEASE:
		if (isLookingUp)
		{
			isLookingUp = false;
			state = MARIO_STATE_IDLE;
		}
		break;
	case MARIO_STATE_DIE:
		vx = 0;
		ax = 0;
		break;
	case MARIO_STATE_PRE_DIE:
		isPreDied = true;
		count_start = GetTickCount64();
		vy = 2*MARIO_JUMP_DEFLECT_SPEED;
		if (nx) nx = 1;
		ax = -nx* MARIO_ACCEL_WALK_X;
		if(maxVx==0) maxVx = MARIO_PRE_DIE_SPEED;
		maxVx = -maxVx;
		break;
	}
	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	//if (level!=MARIO_LEVEL_SMALL)
	//{
		if (isSitting || isPreDied)
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
	//}
	//else
	//{
	//	left = x - MARIO_SMALL_BBOX_WIDTH/2;
	//	top = y - MARIO_SMALL_BBOX_HEIGHT/2;
	//	right = left + MARIO_SMALL_BBOX_WIDTH;
	//	bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	//}
}

void CMario::createTailObject() {

	if (nx < 0) {
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x - MARIO_BIG_BBOX_WIDTH / 2, y + MARIO_BIG_BBOX_HEIGHT/4, -1);
	}
	else
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + MARIO_BIG_BBOX_WIDTH / 2, y + MARIO_BIG_BBOX_HEIGHT / 4,1);

	}
}
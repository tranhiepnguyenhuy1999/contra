#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Soldier.h"
#include "GunSoldier.h"
#include "GunType.h"
#include "EnemyGun.h"

#include "Water.h"
#include "DownBrick.h"
#include "Portal.h"

#include "AssetIDs.h"
#include "Collision.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	isOnPlatform = false;
	isSwimming = false;

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
	if (dynamic_cast<CWater*>(e->obj)) {
		SetState(MARIO_STATE_SWIMMING);
	}
	else if (dynamic_cast<CSoldier*>(e->obj))
		OnCollisionWithSoldier(e);
	//else if (dynamic_cast<CGunSoldier*>(e->obj))
	//	OnCollisionWithGunSoldier(e);
	else if (dynamic_cast<CGunType*>(e->obj))
		OnCollisionWithGunType(e);
	else if (dynamic_cast<CEnemyGun*>(e->obj))
		OnCollisionWithEnemyGun(e);
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
void CMario::OnCollisionWithGunType(LPCOLLISIONEVENT e)
{
	DebugOut(L"Touched !!! \n");
	CGunType* i = dynamic_cast<CGunType*>(e->obj);
	gunType = i->type;
	i->SetState(GUNTYPE_STATE_DIE);
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
void CMario::OnCollisionWithGunSoldier(LPCOLLISIONEVENT e)
{
	CGunSoldier* i = dynamic_cast<CGunSoldier*>(e->obj);

	if (untouchable == 0)
	{
		if (i->GetState() != GUNSOLDIER_STATE_DIE)
		{
			DebugOut(L">>> Main touched gun soldier >>> \n");
			SetState(MARIO_STATE_PRE_DIE);
		}
	}
}
void CMario::OnCollisionWithEnemyGun(LPCOLLISIONEVENT e)
{
	CEnemyGun* i = dynamic_cast<CEnemyGun*>(e->obj);
	SetState(MARIO_STATE_PRE_DIE);
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
	else if (!isOnPlatform)
	{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_LEFT;
	}
	else if (isShooting)
	{
		if (isSwimming) {
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SWIMMING_SHOOTING_RIGHT;
			else
				aniId = ID_ANI_MARIO_SWIMMING_SHOOTING_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SHOOTING_RIGHT;
			else
				aniId = ID_ANI_MARIO_SHOOTING_LEFT;
		}
	}
	else if (isSwimming)
	{
		if (isSitting)
			aniId = ID_ANI_MARIO_DIVE;
		else if (nx > 0)
			aniId = ID_ANI_MARIO_SWIMMING_LEFT;
		else
			aniId = ID_ANI_MARIO_SWIMMING_RIGHT;

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
	//case MARIO_STATE_RUNNING_RIGHT:

	//	maxVx = MARIO_RUNNING_SPEED;
	//	ax = MARIO_ACCEL_RUN_X;
	//	nx = 1;
	//	break;
	//case MARIO_STATE_RUNNING_LEFT:
	//	if (isSitting) y += MARIO_SIT_HEIGHT_ADJUST;
	//	maxVx = -MARIO_RUNNING_SPEED;
	//	ax = -MARIO_ACCEL_RUN_X;
	//	nx = -1;
	//	break;
	case MARIO_STATE_RUNNING_UP_LEFT:
		if (isLookingUp && !isRunning) y -= MARIO_BIG_UP_BBOX_HEIGHT / 2 - MARIO_BIG_BBOX_HEIGHT / 2;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		isRunning = true;
		break;
	case MARIO_STATE_RUNNING_DOWN_LEFT:
		if (isSitting && !isRunning) y += MARIO_SIT_HEIGHT_ADJUST;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		isRunning = true;
		break;
	case MARIO_STATE_RUNNING_UP_RIGHT:
		if(isLookingUp && !isRunning) y -= MARIO_BIG_UP_BBOX_HEIGHT / 2 - MARIO_BIG_BBOX_HEIGHT / 2;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		isRunning = true;
		break;
	case MARIO_STATE_RUNNING_DOWN_RIGHT:
		// update y first time when cheng
		if (isSitting && !isRunning) y += MARIO_SIT_HEIGHT_ADJUST;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		isRunning = true;
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
		if (isSwimming) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = MARIO_JUMP_SPEED_Y;
		}
		break;
	case MARIO_STATE_SWIMMING:
		isSwimming = true;
		break;
	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy -= MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isSwimming) {
			//DebugOut(L">>> Main touched gun soldier >>> \n");

			isSitting = true;
			break;
		}
		if (isOnPlatform)
		{

			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			if(!isRunning) y -=MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			if(!isRunning) y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;
	case MARIO_STATE_MOVING_RELEASE:
		if (isLookingUp)
		{
			y += MARIO_BIG_UP_BBOX_HEIGHT / 2 - MARIO_BIG_BBOX_HEIGHT / 2;
		}
		if (isSitting && !isRunning)
		{
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;
	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		isRunning = false;
		break;

	case MARIO_STATE_SHOOTING:
		shooting_start = GetTickCount64();
		isShooting = true;
		createGun();
		break;
	case MARIO_STATE_LOOKUP: 
		if (isOnPlatform)
		{
			if(vx==0) y += MARIO_BIG_UP_BBOX_HEIGHT / 2 - MARIO_BIG_BBOX_HEIGHT / 2;
			state = MARIO_STATE_IDLE;
			isLookingUp = true;
			vx = 0; vy = 0.0f;
		}
		break;
	case MARIO_STATE_LOOKUP_RELEASE:
		if (isLookingUp)
		{
			// update y when not moving
			if(vx==0 && !isRunning) y -= MARIO_BIG_UP_BBOX_HEIGHT / 2 - MARIO_BIG_BBOX_HEIGHT / 2;
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
		if (isSwimming) {
			left = x - MARIO_BIG_SWIMMING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SWIMMING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SWIMMING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SWIMMING_BBOX_HEIGHT;
		}
		else if (isLookingUp) {
			if (vx != 0)
			{
				left = x - MARIO_BIG_BBOX_WIDTH / 2;
				top = y - MARIO_BIG_BBOX_HEIGHT / 2;
				right = left + MARIO_BIG_BBOX_WIDTH;
				bottom = top + MARIO_BIG_BBOX_HEIGHT;

			}
			else {
				left = x - MARIO_BIG_BBOX_WIDTH / 2;
				top = y - MARIO_BIG_UP_BBOX_HEIGHT / 2;
				right = left + MARIO_BIG_BBOX_WIDTH;
				bottom = top + MARIO_BIG_UP_BBOX_HEIGHT;
			}
		}
		else if ((isSitting || isPreDied) && vx==0)
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

void CMario::createGun() {

	switch (gunType)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	default:

		break;
	}

	if (nx < 0) {
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x - MARIO_BIG_BBOX_WIDTH / 2, y + MARIO_BIG_BBOX_HEIGHT/4, -1);
	}
	else
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + MARIO_BIG_BBOX_WIDTH / 2, y + MARIO_BIG_BBOX_HEIGHT / 4,1);

	}
}
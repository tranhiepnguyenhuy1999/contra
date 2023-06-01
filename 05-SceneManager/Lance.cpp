#include <algorithm>
#include "debug.h"

#include "Lance.h"
#include "Game.h"

#include "Goomba.h"
#include "Soldier.h"
#include "GunSoldier.h"
#include "GunType.h"
#include "EnemyGun.h"

#include "Water.h"
#include "Land.h"
#include "DownBrick.h"
#include "Portal.h"

#include "AssetIDs.h"
#include "Collision.h"
#include "PlayerData.h"

void CLance::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	isOnPlatform = false;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > LANCE_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (isClimb) {
		SetState(LANCE_STATE_CLIMB);
		return;
	}
	else if (state==LANCE_STATE_PRE_DIE && GetTickCount64() - count_start > 500)
	{
		DebugOut(L">>> Mario DIE >>> %f \n", vx);
		count_start = -1;
		SetState(LANCE_STATE_DIE);
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CLance::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CLance::OnCollisionWith(LPCOLLISIONEVENT e)
{

	if (state == LANCE_STATE_DIE || state == LANCE_STATE_PRE_DIE) return;

	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny > 0) isOnPlatform = true;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	// others
	if (dynamic_cast<CLand*>(e->obj) && isSwimming) {
		isSwimming = false;
		isClimb = true;
		ay = 0;
		vy = 0;
	}
	else if (dynamic_cast<CWater*>(e->obj) && !isSwimming) {
		isSwimming = true;
	}
	else if (dynamic_cast<CSoldier*>(e->obj))
		OnCollisionWithSoldier(e);
	//else if (dynamic_cast<CGunSoldier*>(e->obj))
	//	OnCollisionWithGunSoldier(e);
	else if (dynamic_cast<CGunType*>(e->obj))
		OnCollisionWithGunType(e);
	//else if (dynamic_cast<CEnemyGun*>(e->obj))
	//	OnCollisionWithEnemyGun(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
}
void CLance::OnCollisionWithGunType(LPCOLLISIONEVENT e)
{
	DebugOut(L"Touched !!! \n");
	CGunType* i = dynamic_cast<CGunType*>(e->obj);
	gunType = i->type;
	i->SetState(GUNTYPE_STATE_DIE);
}
void CLance::OnCollisionWithSoldier(LPCOLLISIONEVENT e)
{
	CSoldier* i = dynamic_cast<CSoldier*>(e->obj);

	if (untouchable == 0)
	{
		if (i->GetState() != SOLDIER_STATE_DIE)
		{
			DebugOut(L">>> Main touched soldier >>> \n");
			SetState(LANCE_STATE_PRE_DIE);
		}
	}
}
void CLance::OnCollisionWithGunSoldier(LPCOLLISIONEVENT e)
{
	CGunSoldier* i = dynamic_cast<CGunSoldier*>(e->obj);

	if (untouchable == 0)
	{
		if (i->GetState() != GUNSOLDIER_STATE_DIE)
		{
			DebugOut(L">>> Main touched gun soldier >>> \n");
			SetState(LANCE_STATE_PRE_DIE);
		}
	}
}
void CLance::OnCollisionWithEnemyGun(LPCOLLISIONEVENT e)
{
	CEnemyGun* i = dynamic_cast<CEnemyGun*>(e->obj);
	SetState(LANCE_STATE_PRE_DIE);
}
void CLance::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

//
// Get animdation ID for big Mario
//
int CLance::GetAniId()
{
	int aniId = -1;

	if (isPreDied)
	{
		if (nx > 0)
			aniId = ID_ANI_LANCE_PRE_DIE_LEFT;
		else
			aniId = ID_ANI_LANCE_PRE_DIE;

	}
	else if (!isOnPlatform)
	{
			if (nx >= 0)
				aniId = ID_ANI_LANCE_JUMP_RIGHT;
			else
				aniId = ID_ANI_LANCE_JUMP_LEFT;
	}
	else if (isSwimming && !isShooting)
	{
		if (isSitting)
			aniId = ID_ANI_LANCE_DIVE;
		else if (nx > 0)
			aniId = ID_ANI_LANCE_SWIMMING_LEFT;
		else
			aniId = ID_ANI_LANCE_SWIMMING_RIGHT;

	}
	else if (isShooting)
	{
		if (isSwimming) {
			if (isSitting) aniId = ID_ANI_LANCE_DIVE;
			else if (nx >= 0)
				aniId = ID_ANI_LANCE_SWIMMING_SHOOTING_RIGHT;
			else
				aniId = ID_ANI_LANCE_SWIMMING_SHOOTING_LEFT;
		}
		else if (vx == 0)
		{
			if (isSitting)
			{
				if (nx > 0)
					aniId = ID_ANI_LANCE_SIT_RIGHT;
				else
					aniId = ID_ANI_LANCE_SIT_LEFT;
			}
			else if (isLookingUp)
			{
				if (nx > 0)
					aniId = ID_ANI_LANCE_STAND_UP_RIGHT;
				else
					aniId = ID_ANI_LANCE_STAND_UP_LEFT;
			}
			else
			{
				if (nx >= 0)
					aniId = ID_ANI_LANCE_SHOOTING_RIGHT;
				else
					aniId = ID_ANI_LANCE_SHOOTING_LEFT;
			}
		}
		else {
			if (vx > 0)
			{
				if (isSitting)
						aniId = ID_ANI_LANCE_RUNNING_DOWN_RIGHT;
				else if (isLookingUp)
						aniId = ID_ANI_LANCE_RUNNING_UP_RIGHT;
				else	aniId = ID_ANI_LANCE_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (isSitting)
					aniId = ID_ANI_LANCE_RUNNING_DOWN_LEFT;
				else if (isLookingUp)
					aniId = ID_ANI_LANCE_RUNNING_UP_LEFT;
				else	aniId = ID_ANI_LANCE_WALKING_LEFT;
			}
		}
	}
	else if (vx == 0)
			{
				if (isSitting)
				{
					if (nx > 0)
						aniId = ID_ANI_LANCE_SIT_RIGHT;
					else
						aniId = ID_ANI_LANCE_SIT_LEFT;
				}
				else if (isLookingUp) {
					if (nx > 0) 
						aniId = ID_ANI_LANCE_STAND_UP_RIGHT;
					else 
						aniId = ID_ANI_LANCE_STAND_UP_LEFT;
				}
				else if (nx > 0) aniId = ID_ANI_LANCE_IDLE_RIGHT;
				else aniId = ID_ANI_LANCE_IDLE_LEFT;
			}
	else if (vx > 0)
	{
		if (isSitting)
			aniId = ID_ANI_LANCE_RUNNING_DOWN_RIGHT;
		else if (isLookingUp)
			aniId = ID_ANI_LANCE_RUNNING_UP_RIGHT;
		else
			aniId = ID_ANI_LANCE_WALKING_RIGHT;
	}
	else // vx < 0
	{
		if (isSitting)
			aniId = ID_ANI_LANCE_RUNNING_DOWN_LEFT;
		else if (isLookingUp)
			aniId = ID_ANI_LANCE_RUNNING_UP_LEFT;
		else aniId = ID_ANI_LANCE_WALKING_LEFT;
	}
	if (aniId == -1) aniId = ID_ANI_LANCE_IDLE_RIGHT;
	return aniId;
}
void CLance::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;
	if (state == LANCE_STATE_DIE)
		if (nx > 0)
			aniId = ID_ANI_LANCE_DIE_LEFT;
		else
			aniId = ID_ANI_LANCE_DIE;
	else
		aniId = GetAniId();

	animations->Get(aniId)->Render(x, y);

	RenderBoundingBox();

	//DebugOutTitle(L"point: %f", CPlayerData::GetInstance()->point);

}
void CLance::SetState(int state)
{
	if (GetTickCount64() - shooting_start < 250) {
	}
	else
	{
		shooting_start = -1;
		isShooting = false;
	}
	
	if (this->state == LANCE_STATE_PRE_DIE && state!= LANCE_STATE_DIE) return;

	// DIE is the end state, cannot be changed! 
	if (this->state == LANCE_STATE_DIE) return;
	switch (state)
	{
	case LANCE_STATE_RUNNING_UP_LEFT:
		if (isLookingUp && !isRunning && !isSwimming) y -= LANCE_BIG_LOOKUP_BBOX_HEIGHT / 2 - LANCE_BIG_BBOX_HEIGHT / 2;
		maxVx = -LANCE_WALKING_SPEED;
		ax = -LANCE_ACCEL_WALK_X;
		nx = -1;
		isRunning = true;
		break;
	case LANCE_STATE_RUNNING_DOWN_LEFT:
		if (isSwimming) {
			maxVx = 0;
			ax = 0;
			nx = -1;
		}
		else
		{
			if (isSitting && !isRunning) y += LANCE_SIT_HEIGHT_ADJUST;
			maxVx = -LANCE_WALKING_SPEED;
			ax = -LANCE_ACCEL_WALK_X;
			nx = -1;
			isRunning = true;
		}
		break;
	case LANCE_STATE_RUNNING_UP_RIGHT:
		if(isLookingUp && !isRunning && !isSwimming) y -= LANCE_BIG_LOOKUP_BBOX_HEIGHT / 2 - LANCE_BIG_BBOX_HEIGHT / 2;
		maxVx = LANCE_WALKING_SPEED;
		ax = LANCE_ACCEL_WALK_X;
		nx = 1;
		isRunning = true;
		break;
	case LANCE_STATE_RUNNING_DOWN_RIGHT:
		if (isSwimming) {
			maxVx = 0;
			ax = 0;
			nx = 1;
		}
		else
		{
			if (isSitting && !isRunning) y += LANCE_SIT_HEIGHT_ADJUST;
			maxVx = LANCE_WALKING_SPEED;
			ax = LANCE_ACCEL_WALK_X;
			nx = 1;
			isRunning = true;
		}
		break;
	case LANCE_STATE_MOVING_RELEASE:
		if (isSwimming) return;
		else if (isLookingUp)
		{
			y += LANCE_BIG_LOOKUP_BBOX_HEIGHT / 2 - LANCE_BIG_BBOX_HEIGHT / 2;
		}
		else if (isSitting && !isRunning)
		{
			y -= LANCE_SIT_HEIGHT_ADJUST;
		}
		break;
	case LANCE_STATE_WALKING_RIGHT:
		//if (isSitting) break;
		maxVx = LANCE_WALKING_SPEED;
		ax = LANCE_ACCEL_WALK_X;
		nx = 1;
		break;
	case LANCE_STATE_WALKING_LEFT:
		//if (isSitting) break;
		maxVx = -LANCE_WALKING_SPEED;
		ax = -LANCE_ACCEL_WALK_X;
		nx = -1;
		break;
	case LANCE_STATE_JUMP:
		if (isSwimming) break;
		if (isOnPlatform) vy = LANCE_JUMP_SPEED_Y;
		break;
	case LANCE_STATE_RELEASE_JUMP:
		if (vy < 0) vy -= LANCE_JUMP_SPEED_Y / 2;
		break;
	case LANCE_STATE_SIT:
		if (isSwimming) {
			//DebugOut(L">>> Main touched gun soldier >>> \n");
			isSitting = true;

			break;
		}
		else if (isOnPlatform)
		{
			state = LANCE_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			if(!isRunning) y -=LANCE_SIT_HEIGHT_ADJUST;
		}
		break;
	case LANCE_STATE_SIT_RELEASE:
		if (isSwimming) isSitting = false;
		else if (isSitting)
		{
			isSitting = false;
			state = LANCE_STATE_IDLE;
			if(!isRunning) y += LANCE_SIT_HEIGHT_ADJUST;
		}
		break;
	case LANCE_STATE_LOOKUP:
		if (isSwimming) isLookingUp = true;
		else if (isOnPlatform)
		{
			if(vx==0) y += LANCE_BIG_LOOKUP_BBOX_HEIGHT / 2 - LANCE_BIG_BBOX_HEIGHT / 2;
			state = LANCE_STATE_IDLE;
			isLookingUp = true;
			vx = 0; vy = 0.0f;
		}
		break;
	case LANCE_STATE_LOOKUP_RELEASE:
		isLookingUp = false;
		if (isSwimming) return;
		if (isLookingUp)
		{
			// update y when not moving
			if(vx==0 && !isRunning) y -= LANCE_BIG_LOOKUP_BBOX_HEIGHT / 2 - LANCE_BIG_BBOX_HEIGHT / 2;
			state = LANCE_STATE_IDLE;
		}
		break;
	case LANCE_STATE_SHOOTING:
		shooting_start = GetTickCount64();
		isShooting = true;
		handleShooting();
		break;
	case LANCE_STATE_CLIMB:
		isClimb = false;
		ay = - LANCE_GRAVITY;
		x += 16;
		y += 17 + LANCE_BIG_BBOX_HEIGHT/2-LANCE_BIG_SWIMMING_BBOX_HEIGHT/2;
		break;
	case LANCE_STATE_DIE:
		vx = 0;
		ax = 0;
		CPlayerData::GetInstance()->updateLife(1, -1);
		break;
	case LANCE_STATE_PRE_DIE:
		isPreDied = true;
		count_start = GetTickCount64();
		vy = 2*LANCE_JUMP_DEFLECT_SPEED;
		if (nx) nx = 1;
		ax = -nx* LANCE_ACCEL_WALK_X;
		if(maxVx==0) maxVx = LANCE_PRE_DIE_SPEED;
		maxVx = -maxVx;
		break;
	case LANCE_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		isRunning = false;
		break;
	}
	CGameObject::SetState(state);
}

void CLance::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
		if (isSwimming) {
			left = x - LANCE_BIG_SWIMMING_BBOX_WIDTH / 2;
			top = y + LANCE_BIG_SWIMMING_BBOX_HEIGHT / 2;
			right = left + LANCE_BIG_SWIMMING_BBOX_WIDTH;
			bottom = top - LANCE_BIG_SWIMMING_BBOX_HEIGHT;
		}
		else if (isLookingUp) {
			if (vx != 0)
			{
				left = x - LANCE_BIG_BBOX_WIDTH / 2;
				top = y + LANCE_BIG_BBOX_HEIGHT / 2;
				right = left + LANCE_BIG_BBOX_WIDTH;
				bottom = top - LANCE_BIG_BBOX_HEIGHT;

			}
			else {
				left = x - LANCE_BIG_BBOX_WIDTH / 2;
				top = y + LANCE_BIG_LOOKUP_BBOX_HEIGHT / 2;
				right = left + LANCE_BIG_BBOX_WIDTH;
				bottom = top - LANCE_BIG_LOOKUP_BBOX_HEIGHT;
			}
		}
		else if ((isSitting || isPreDied) && vx==0)
		{
			left = x - LANCE_BIG_SITTING_BBOX_WIDTH / 2;
			top = y + LANCE_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + LANCE_BIG_SITTING_BBOX_WIDTH;
			bottom = top - LANCE_BIG_SITTING_BBOX_HEIGHT;
		}
		//else if (!isOnPlatform)
		//{
		//	left = x - LANCE_BIG_JUMPING_BBOX_WIDTH / 2;
		//	top = y + LANCE_BIG_JUMPING_BBOX_HEIGHT / 2;
		//	right = left + LANCE_BIG_JUMPING_BBOX_WIDTH;
		//	bottom = top - LANCE_BIG_JUMPING_BBOX_HEIGHT;
		//}
		else
		{
			left = x - LANCE_BIG_BBOX_WIDTH/2;
			top = y + LANCE_BIG_BBOX_HEIGHT/2;
			right = left + LANCE_BIG_BBOX_WIDTH;
			bottom = top - LANCE_BIG_BBOX_HEIGHT;
		}
}

void CLance::handleShooting() {
	if (isSwimming && isSitting) return; // diving can't shoot
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
	if (vx == 0){
		if (isLookingUp)
			if (isSwimming) CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + nx * LANCE_BIG_SWIMMING_BBOX_WIDTH / 2, y + LANCE_BIG_SWIMMING_BBOX_HEIGHT / 2, 0, 1);
			else CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + nx * LANCE_BIG_BBOX_WIDTH / 2, y + LANCE_BIG_BBOX_HEIGHT / 2, 0, 1);
		else if (isSwimming)
			CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + nx * LANCE_BIG_SWIMMING_BBOX_WIDTH / 2, y - LANCE_BIG_SWIMMING_BBOX_HEIGHT/4, nx);
		else if (isSitting)
			CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + nx * LANCE_BIG_SITTING_BBOX_WIDTH / 2, y, nx);
		else CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + nx * LANCE_BIG_BBOX_WIDTH / 2, y + LANCE_BIG_BBOX_HEIGHT / 5, nx);
	}
	else{
		if (isLookingUp)
			if(isSwimming) CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + nx * LANCE_BIG_SWIMMING_BBOX_WIDTH / 2, y - LANCE_BIG_SWIMMING_BBOX_HEIGHT / 4, nx, 1);
			else CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + nx * LANCE_BIG_BBOX_WIDTH / 2, y + LANCE_BIG_BBOX_HEIGHT/2, nx, 1);
		else if (isSwimming)
			CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + nx * LANCE_BIG_SWIMMING_BBOX_WIDTH / 2, y - LANCE_BIG_SWIMMING_BBOX_HEIGHT / 4, nx);
		else if (isSitting) 
			CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + nx * LANCE_BIG_BBOX_WIDTH / 2, y + LANCE_BIG_BBOX_HEIGHT / 5, nx, -1);
		else CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + nx * LANCE_BIG_BBOX_WIDTH / 2, y + LANCE_BIG_BBOX_HEIGHT / 4, nx);
	}
	
}
#include <algorithm>
#include "debug.h"

#include "Lance.h"
#include "Game.h"

#include "Goomba.h"
#include "Soldier.h"
#include "GunSoldier.h"
#include "GunType.h"
#include "EnemyGun.h"
#include "Fire.h"
#include "MovingRock.h"
#include "Gun.h"

#include "Water.h"
#include "DeadLand.h"
#include "Land.h"
#include "DownBrick.h"
#include "EndWall.h"
#include "Portal.h"

#include "AssetIDs.h"
#include "Collision.h"
#include "PlayerData.h"

#include "Sound.h"

void CLance::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	
	isOnPlatform = false;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	//DebugOut(L">>> Mario DIE >>> %f \n", vx);

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
		count_start = -1;
		SetState(LANCE_STATE_DIE);
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);

	if (isJumping && isOnPlatform) {
		SetState(LANCE_STATE_RELEASE_JUMP);
	}
}

void CLance::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CLance::OnCollisionWith(LPCOLLISIONEVENT e)
{
	movingObjVx = 0.0f;

	if (state == LANCE_STATE_DIE || state == LANCE_STATE_PRE_DIE) return;

	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		isOnPlatform = true;
		if (e->ny > 0)
		{
			if (dynamic_cast<CDownBrick*>(e->obj)) {
				//DebugOut(L">>> Touched >>> \n");
				isOnDownBrick = true;
			}
		}
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		if (!dynamic_cast<CEndWall*>(e->obj)) vx = 0; 
	}

	// others
	if (dynamic_cast<CDeadLand*>(e->obj)) {
		SetState(LANCE_STATE_PRE_DIE);
	}
	else if (dynamic_cast<CLand*>(e->obj) && isSwimming) {
		isSwimming = false;
		isClimb = true;
		ay = 0;
		vy = 0;
	}
	else if (dynamic_cast<CWater*>(e->obj) && !isSwimming) {
		isSwimming = true;
		y -= LANCE_BIG_BBOX_HEIGHT / 2 - LANCE_BIG_SWIMMING_BBOX_HEIGHT/2;
	}
	else if (dynamic_cast<CMovingRock*>(e->obj)) {
		float mrvx, mrvy;
		e->obj->GetSpeed(mrvx, mrvy);
		movingObjVx = mrvx;

	}
	//else if (dynamic_cast<CSoldier*>(e->obj))
	//	OnCollisionWithSoldier(e);
	//else if (dynamic_cast<CFire*>(e->obj))
	//OnCollisionWithFire(e);
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
	CGunType* i = dynamic_cast<CGunType*>(e->obj);
	if (i->id < 5) {
		gunType = i->id;
	}
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
void CLance::OnCollisionWithFire(LPCOLLISIONEVENT e)
{
	CFire* i = dynamic_cast<CFire*>(e->obj);

	if (untouchable == 0)
	{
		SetState(LANCE_STATE_PRE_DIE);
	}
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
	else if (isSwimming && !isShooting)
	{
		if (isSitting)
			aniId = ID_ANI_LANCE_DIVE;
		else if (nx > 0)
			aniId = ID_ANI_LANCE_SWIMMING_LEFT;
		else
			aniId = ID_ANI_LANCE_SWIMMING_RIGHT;

	}
	else if (!isOnPlatform && isJumping)
	{
		if (nx >= 0)
			aniId = ID_ANI_LANCE_JUMP_RIGHT;
		else
			aniId = ID_ANI_LANCE_JUMP_LEFT;
	}
	else if (!isOnPlatform)
	{
		if (nx >= 0)
			aniId = ID_ANI_LANCE_FALLING_RIGHT;
		else
			aniId = ID_ANI_LANCE_FALLING_LEFT;
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
		else if(movingObjVx == 0) aniId = ID_ANI_LANCE_WALKING_RIGHT;
	}
	else // vx < 0
	{
		if (isSitting)
			aniId = ID_ANI_LANCE_RUNNING_DOWN_LEFT;
		else if (isLookingUp)
			aniId = ID_ANI_LANCE_RUNNING_UP_LEFT;
		else if (movingObjVx == 0) aniId = ID_ANI_LANCE_WALKING_LEFT;
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
	case LANCE_STATE_UNTOUCHABLE:
		untouchable = 1;
		untouchable_start = GetTickCount64();
		break;
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
		else if (isLookingUp && !isRunning)
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
		if (isSitting && isOnDownBrick) {
			isOnDownBrick = false; // can't change in state LANCE_STATE_GO_DOWN bcs keydown after keystate
			break;
		}
		else if (isOnPlatform)
		{
			isJumping = true;
			vy = LANCE_JUMP_SPEED_Y;
		}
		break;
	case LANCE_STATE_RELEASE_JUMP:
		isJumping = false;
		if(isRunning)
			y += LANCE_BIG_BBOX_HEIGHT / 2 - LANCE_BIG_JUMPING_BBOX_HEIGHT / 2;
		else if (isSitting)
			y += LANCE_BIG_SITTING_BBOX_HEIGHT / 2 - LANCE_BIG_JUMPING_BBOX_HEIGHT / 2;
		else if (isLookingUp)
			y += LANCE_BIG_LOOKUP_BBOX_HEIGHT / 2 - LANCE_BIG_JUMPING_BBOX_HEIGHT / 2;
		else y += LANCE_BIG_BBOX_HEIGHT / 2 - LANCE_BIG_JUMPING_BBOX_HEIGHT / 2;
		break;
	case LANCE_STATE_SIT:
		isSitting = true;
		if (isSwimming) break;
		else if (isOnPlatform)
		{
			state = LANCE_STATE_IDLE;
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
		isLookingUp = true;
		if (isSwimming) break;
		else if (isOnPlatform)
		{
			if(vx==0) y += LANCE_BIG_LOOKUP_BBOX_HEIGHT / 2 - LANCE_BIG_BBOX_HEIGHT / 2;
			state = LANCE_STATE_IDLE;
			vx = 0; vy = 0.0f;
		}
		break;
	case LANCE_STATE_LOOKUP_RELEASE:
			isLookingUp = false;
		if (isLookingUp && !isSwimming)
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
		x += 16.0f;
		y += 17.0f + LANCE_BIG_BBOX_HEIGHT/2-LANCE_BIG_SWIMMING_BBOX_HEIGHT/2;
		break;
	case LANCE_STATE_GO_DOWN:
		if (isOnDownBrick)
		{
			y -= 1.0f;
		}
		break;
	case LANCE_STATE_DIE:
		vx = 0.0f;
		ax = 0.0f;
		CPlayerData::GetInstance()->updateLife(1, -1);
		break;
	case LANCE_STATE_PRE_DIE:
		isPreDied = true;
		count_start = GetTickCount64();
		vy = LANCE_DIE_DEFLECT_SPEED;
		if (nx) nx = 1;
		ax = -nx* LANCE_ACCEL_WALK_X;
		if(maxVx==0) maxVx = LANCE_PRE_DIE_SPEED;
		maxVx = -maxVx;
		break;
	case LANCE_STATE_IDLE:
		ax = 0.0f;
		if (movingObjVx != 0) vx = movingObjVx; else vx = 0.0f;
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
		else if (isJumping)
		{
			left = x - LANCE_BIG_JUMPING_BBOX_WIDTH / 2;
			top = y + LANCE_BIG_JUMPING_BBOX_HEIGHT / 2;
			right = left + LANCE_BIG_JUMPING_BBOX_WIDTH;
			bottom = top - LANCE_BIG_JUMPING_BBOX_HEIGHT;
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
		else if (isPreDied)
		{
			left = x - LANCE_BIG_SITTING_BBOX_WIDTH / 2;
			top = y + LANCE_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + LANCE_BIG_SITTING_BBOX_WIDTH;
			bottom = top - LANCE_BIG_SITTING_BBOX_HEIGHT;
		}
		else if (isSitting && vx == 0)
		{
			left = x - LANCE_BIG_SITTING_BBOX_WIDTH / 2;
			top = y + LANCE_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + LANCE_BIG_SITTING_BBOX_WIDTH;
			bottom = top - LANCE_BIG_SITTING_BBOX_HEIGHT;
		}
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
	if (vx == 0){
		if (isLookingUp)
			if (isSwimming) CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + nx * LANCE_BIG_SWIMMING_BBOX_WIDTH / 2, y + LANCE_BIG_SWIMMING_BBOX_HEIGHT / 2, 0, 0, 0, GUN_MAX_SPEED, gunType);
			else CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + nx * LANCE_BIG_BBOX_WIDTH / 2, y + LANCE_BIG_BBOX_HEIGHT / 2, 0, 0, 0, GUN_MAX_SPEED, gunType);
		else if (isSwimming)
			CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + nx * LANCE_BIG_SWIMMING_BBOX_WIDTH / 2, y - LANCE_BIG_SWIMMING_BBOX_HEIGHT/4, 0, 0, nx * GUN_MAX_SPEED, 0, gunType);
		else if (isSitting)
			CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + nx * LANCE_BIG_SITTING_BBOX_WIDTH / 2, y, 0, 0, nx * GUN_MAX_SPEED, 0, gunType);
		else CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + nx * LANCE_BIG_BBOX_WIDTH / 2, y + LANCE_BIG_BBOX_HEIGHT / 5, 0, 0, nx * GUN_MAX_SPEED, 0, gunType);
	}
	else{
		if (isLookingUp)
			if(isSwimming) CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + nx * LANCE_BIG_SWIMMING_BBOX_WIDTH / 2, y - LANCE_BIG_SWIMMING_BBOX_HEIGHT / 4, 0, 0, nx * GUN_MAX_SPEED, GUN_MAX_SPEED, gunType);
			else CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + nx * LANCE_BIG_BBOX_WIDTH / 2, y + LANCE_BIG_BBOX_HEIGHT/2, 0, 0, nx * GUN_MAX_SPEED, GUN_MAX_SPEED, gunType);
		else if (isSwimming)
			CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + nx * LANCE_BIG_SWIMMING_BBOX_WIDTH / 2, y - LANCE_BIG_SWIMMING_BBOX_HEIGHT / 4, 0, 0, nx * GUN_MAX_SPEED, 0, gunType);
		else if (isSitting) 
			CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + nx * LANCE_BIG_BBOX_WIDTH / 2, y + LANCE_BIG_BBOX_HEIGHT / 5, 0, 0, nx * GUN_MAX_SPEED, -GUN_MAX_SPEED, gunType);
		else CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUN, x + nx * LANCE_BIG_BBOX_WIDTH / 2, y + LANCE_BIG_BBOX_HEIGHT / 4, 0, 0, nx * GUN_MAX_SPEED, 0, gunType);
	}
	CSound::GetInstance()->playSound_Pause();
}
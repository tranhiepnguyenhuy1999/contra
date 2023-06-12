#include "Soldier.h"

#include "AssetIDs.h"
#include "PlayerData.h"

#include "debug.h"

CSoldier::CSoldier(float x, float y, float isShooting, float isHaveFallObj) :CGameObject(x, y)
{
	this->ay = -SOLDIER_GRAVITY;
	this->isShooting = isShooting;
	this->isHaveFallObj = isHaveFallObj;
	die_start = -1;
	count_start = -1;
	isOnPlatform = false;
	fallObject = NULL;
	nx = -1;
	SetState(SOLDIER_STATE_WALKING);
}

void CSoldier::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
		left = x - SOLDIER_BBOX_WIDTH / 2;
		top = y + SOLDIER_BBOX_HEIGHT / 2;
		right = left + SOLDIER_BBOX_WIDTH;
		bottom = top - SOLDIER_BBOX_HEIGHT;
}

void CSoldier::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CSoldier::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) { isOnPlatform = true; }
	if (e->ny != 0)
	{
		vy = 0;
	}
}

void CSoldier::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);
	
	isOnPlatform = false;

	if (!isActive)
	{
		if (x < cx + CGame::GetInstance()->GetBackBufferWidth()) {
			isActive = true;
			if(isHaveFallObj !=0) CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_FALL_OBJECT, x - 50, y, -SOLDIER_SPEED,0,0,this);
		}
		return;
	}
	if (fallObject !=NULL)
	{
		if (fallObject->IsFallen()) {
			vx = -vx;
			removeFallObject();
			return;
		}
	}

	vy += ay * dt;

	if (isShooting != 0 && state == SOLDIER_STATE_WALKING && (GetTickCount64() - count_start) > SOLDIER_WAITING_SHOOTING_TIMEOUT) {
		SetState(SOLDIER_STATE_SHOOTING);
	}
	else if (state == SOLDIER_STATE_SHOOTING && (GetTickCount64() - count_start) > SOLDIER_SHOOTING_TIMEOUT) {
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x , y, nx);
		SetState(SOLDIER_STATE_WALKING);
	}
	else if (state == SOLDIER_STATE_DIE && vy > SOLDIER_DIE_DEFLECT) {
		ay = 0;
		vy = 0;
	}
	else if (state == SOLDIER_STATE_DIE && (GetTickCount64() - die_start > SOLDIER_DIE_TIMEOUT))
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 2);
		CPlayerData::GetInstance()->updatePoint(100);
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CSoldier::Render()
{
	int aniId = ID_ANI_SOLDIER_WALKING_LEFT;
	if (state == SOLDIER_STATE_DIE)
	{
		if (nx > 0)
			aniId = ID_ANI_SOLDIER_DIE_LEFT;
		else
			aniId = ID_ANI_SOLDIER_DIE_RIGHT;
	}
	else if (state == SOLDIER_STATE_SHOOTING) {
		if (nx > 0)
			aniId = ID_ANI_SOLDIER_SHOOTING_LEFT;
		else
			aniId = ID_ANI_SOLDIER_SHOOTING_RIGHT;
	}
	else if (!isOnPlatform) {
		if (nx > 0)
			aniId = ID_ANI_SOLDIER_DIE_LEFT;
		else
			aniId = ID_ANI_SOLDIER_DIE_RIGHT;
	}
	else if (vx > 0) aniId = ID_ANI_SOLDIER_WALKING_RIGHT;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CSoldier::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SOLDIER_STATE_DIE:
		die_start = GetTickCount64();
		vx = 0;
		ay = SOLDIER_GRAVITY;
		break;
	case SOLDIER_STATE_WALKING:
		vx = nx*SOLDIER_SPEED;
		count_start = GetTickCount64();
		break;
	case SOLDIER_STATE_SHOOTING:
		vx = 0;
		count_start = GetTickCount64();
		break;
	}
}

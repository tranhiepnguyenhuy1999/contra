#include "Soldier.h"
#include "Tail.h"
#include "debug.h"

CSoldier::CSoldier(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = SOLDIER_GRAVITY;
	die_start = -1;
	SetState(SOLDIER_STATE_WALKING);
}

void CSoldier::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == SOLDIER_STATE_DIE)
	{
		left = x - SOLDIER_BBOX_WIDTH / 2;
		top = y - SOLDIER_BBOX_HEIGHT_DIE / 2;
		right = left + SOLDIER_BBOX_WIDTH;
		bottom = top + SOLDIER_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - SOLDIER_BBOX_WIDTH / 2;
		top = y - SOLDIER_BBOX_HEIGHT / 2;
		right = left + SOLDIER_BBOX_WIDTH;
		bottom = top + SOLDIER_BBOX_HEIGHT;
	}
}

void CSoldier::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CSoldier::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CSoldier*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}


}

void CSoldier::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);
	if (!isActive)
	{
		if (x < cx + 1.5 * CGame::GetInstance()->GetBackBufferWidth()) {
			isActive = true;
		}
		return;
	}

	vy += ay * dt;
	vx += ax * dt;

	if ((state == SOLDIER_STATE_DIE) && (GetTickCount64() - die_start > SOLDIER_DIE_TIMEOUT))
	{
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
		aniId = ID_ANI_SOLDIER_DIE;
	}

	if (vx > 0) aniId = ID_ANI_SOLDIER_WALKING_RIGHT;

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
		y += (SOLDIER_BBOX_HEIGHT - SOLDIER_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case SOLDIER_STATE_WALKING:
		vx = -SOLDIER_WALKING_SPEED;
		break;
	}
}
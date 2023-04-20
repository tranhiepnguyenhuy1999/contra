#include "GunShip.h"
#include "QuestionBrick.h"
#include "AssetIDs.h"

CGunShip::CGunShip(float x, float y, float typeGun) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = PARAGOOMBA_GRAVITY;
	this->typeGun = typeGun;
	SetState(PARAGOOMBA_STATE_ACTIVE);
}

void CGunShip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == PARAGOOMBA_STATE_DIE)
	{
		left = x - PARAGOOMBA_BBOX_WIDTH / 2;
		top = y - (PARAGOOMBA_BBOX_HEIGHT_DIE / 2);
		right = left + PARAGOOMBA_BBOX_WIDTH;
		bottom = top + PARAGOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - PARAGOOMBA_BBOX_WIDTH / 2;
		top = y - (PARAGOOMBA_BBOX_HEIGHT / 2);
		right = left + PARAGOOMBA_BBOX_WIDTH;
		bottom = top + PARAGOOMBA_BBOX_HEIGHT;
	}
}

void CGunShip::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGunShip::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CGunShip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	vy += ay * dt;
	vx += ax * dt;
	//DebugOut(L"count %d", GetTickCount64() - count_start);
	if (state == PARAGOOMBA_STATE_DIE)
	{
		this->isDeleted = true;
		return;
	}
	if ((state == PARAGOOMBA_STATE_ACTIVE) && abs(vy)>PARAGOOMBA_GRAVITY_MAX)
	{
		if (ay > 0) vy = PARAGOOMBA_GRAVITY_MAX;
		else vy = -PARAGOOMBA_GRAVITY_MAX;
		ay = -ay;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGunShip::Render()
{
	int aniId = ID_ANI_PARAGOOMBA_STATIC;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CGunShip::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PARAGOOMBA_STATE_ACTIVE:
		vx = PARAGOOMBA_SPEED;
		break;
	case PARAGOOMBA_STATE_DIE:
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	}
}

#include "GunShip.h"
#include "AssetIDs.h"

CGunShip::CGunShip(float x, float y, float typeGun) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GUNSHIP_GRAVITY;
	this->typeGun = typeGun;
	SetState(GUNSHIP_STATE_ACTIVE);
}

void CGunShip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GUNSHIP_STATE_DIE)
	{
		left = x - GUNSHIP_BBOX_WIDTH / 2;
		top = y - (GUNSHIP_BBOX_HEIGHT_DIE / 2);
		right = left + GUNSHIP_BBOX_WIDTH;
		bottom = top + GUNSHIP_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - GUNSHIP_BBOX_WIDTH / 2;
		top = y - (GUNSHIP_BBOX_HEIGHT / 2);
		right = left + GUNSHIP_BBOX_WIDTH;
		bottom = top + GUNSHIP_BBOX_HEIGHT;
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
	if (state == GUNSHIP_STATE_DIE)
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 1);
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUNTYPE, x, y, 1, 1);
		this->isDeleted = true;
		return;
	}
	if ((state == GUNSHIP_STATE_ACTIVE) && abs(vy)>GUNSHIP_GRAVITY_MAX)
	{
		if (ay > 0) vy = GUNSHIP_GRAVITY_MAX;
		else vy = -GUNSHIP_GRAVITY_MAX;
		ay = -ay;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGunShip::Render()
{
	int aniId = ID_ANI_GUNSHIP_STATIC;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CGunShip::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GUNSHIP_STATE_ACTIVE:
		vx = GUNSHIP_SPEED;
		break;
	case GUNSHIP_STATE_DIE:
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	}
}

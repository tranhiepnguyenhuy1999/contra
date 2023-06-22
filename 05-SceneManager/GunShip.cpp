#include "GunShip.h"
#include "Camera.h"
#include "Explode.h"
#include "AssetIDs.h"

CGunShip::CGunShip(float x, float y, float type) :CGameObject(x, y)
{
	this->ay = -GUNSHIP_GRAVITY;
	gunType = type;
	isWorking = false;
	SetState(GUNSHIP_STATE_ACTIVE);
}

void CGunShip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	left = x - GUNSHIP_BBOX_WIDTH / 2;
	top = y + (GUNSHIP_BBOX_HEIGHT / 2);
	right = left + GUNSHIP_BBOX_WIDTH;
	bottom = top - GUNSHIP_BBOX_HEIGHT;
	
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
	float cl, ct, cr, cb;
	Camera::GetInstance()->getCamBoundingBox(cl, ct, cr, cb);
	
	if (!isWorking)
	{
		if (x < cl)
		{
			isWorking = true;
		}
		return;
	}

	vy += ay * dt;
	
	if (state == GUNSHIP_STATE_DIE)
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 0, 0, 0, 0, EXPLODE_TYPE_INFRASTRUCTURE);
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUNTYPE, x, y, 0, 0, 0, 0, gunType);
		this->isDeleted = true;
		return;
	}
	else if (state == GUNSHIP_STATE_ACTIVE && abs(vy)>GUNSHIP_GRAVITY_MAX)
	{
		vy = 0;
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

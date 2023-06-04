#include "FallObject.h"

#include "AssetIDs.h"
#include "PlayerData.h"

#include "debug.h"

CFallObject::CFallObject(float x, float y, float nx) :CGameObject(x, y)
{
	this->ay = -FALL_OBJECT_GRAVITY;
	vx = nx;
	isFallen = false;
	fallenRange = y - 50;
	SetState(FALL_OBJECT_STATE_ACTIVE);
}

void CFallObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - FALL_OBJECT_BBOX_WIDTH / 2;
	top = y + FALL_OBJECT_BBOX_HEIGHT / 2;
	right = left + FALL_OBJECT_BBOX_WIDTH;
	bottom = top - FALL_OBJECT_BBOX_HEIGHT;
}

void CFallObject::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CFallObject::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CFallObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	vy += ay * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

	if (y < fallenRange) isFallen = true;
}


void CFallObject::Render()
{
	RenderBoundingBox();
}

void CFallObject::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FALL_OBJECT_STATE_ACTIVE:
		break;
	}
}

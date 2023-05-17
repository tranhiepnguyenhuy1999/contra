#include "BreakBridge.h"
#include "debug.h"
#include "AssetIDs.h"
CBreakBridge::CBreakBridge(float x, float y) :CGameObject(x, y)
{
	die_start = -1;
	SetState(SOLDIER_STATE_UNACTIVE);
}

void CBreakBridge::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - SOLDIER_BBOX_WIDTH / 2;
	top = y - SOLDIER_BBOX_HEIGHT / 2;
	right = left + SOLDIER_BBOX_WIDTH;
	bottom = top + SOLDIER_BBOX_HEIGHT;
}

void CBreakBridge::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CBreakBridge::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CBreakBridge*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}


}

void CBreakBridge::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

	if ((state == SOLDIER_STATE_ACTIVE) && (GetTickCount64() - die_start > SOLDIER_DIE_TIMEOUT))
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 2);
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CBreakBridge::Render()
{
	int aniId = ID_ANI_SOLDIER_WALKING_LEFT;
	if (state == SOLDIER_STATE_UNACTIVE)
	{
		aniId = ID_ANI_SOLDIER_WALKING_LEFT;
	}

	if (vx > 0) aniId = ID_ANI_SOLDIER_WALKING_LEFT;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CBreakBridge::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SOLDIER_STATE_ACTIVE:
		die_start = GetTickCount64();
		break;
	case SOLDIER_STATE_UNACTIVE:
		break;
	}
}

#include "Explode.h"

#include "Goomba.h"
#include "Soldier.h"
#include "GunBox.h"

#include "debug.h"

CExplode::CExplode(float x, float y, float type=1) :CGameObject(x, y)
{
	vx = 0;
	vy = 0;
	exType = type;
	timeout = getTimeout();
	SetState(GUN_STATE_RELASE);
}
void CExplode::Render()
{
	int aniID = getAniId();
	CAnimations::GetInstance()->Get(aniID)->Render(x, y);
}
void CExplode::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CExplode::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CExplode::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if ((state == GUN_STATE_RELASE) && (GetTickCount64() - count_start > timeout))
	{
		count_start = -1;
		isDeleted = true;
		return;
	}
	CGameObject::Update(dt, coObjects);
}

void CExplode::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

int CExplode::getAniId() {
	switch (int(exType))
	{
	case 1:
		return ID_ANI_INFRASTRUCTURE_EXPLODE;
	case 2:
		return ID_ANI_HUMAN_EXPLODE;
	default:
		return ID_ANI_INFRASTRUCTURE_EXPLODE;
	}
}
int CExplode::getTimeout() {
	switch (int(exType))
	{
	case 1:
		return INFRASTRUCTURE_EXPLODE_TIMEOUT;
	case 2:
		return HUMAN_EXPLODE_TIMEOUT;
	default:
		return INFRASTRUCTURE_EXPLODE_TIMEOUT;
	}
}
void CExplode::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GUN_STATE_RELASE:
		count_start = GetTickCount64();
		break;
	}
}

#include "Explode.h"

#include "Goomba.h"
#include "Soldier.h"
#include "GunBox.h"

#include "debug.h"

CExplode::CExplode(float x, float y, int type) :CGameObject(x, y)
{
	vx = 0;
	vy = 0;
	exType = type;
	SetState(GUN_STATE_RELASE);
}
void CExplode::Render()
{
	int aniID = ID_ANI_INFRASTRUCTURE_EXPLODE;
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
	if ((state == GUN_STATE_RELASE) && (GetTickCount64() - count_start > 750))
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

#include "Gun.h"

#include "Goomba.h"
#include "Soldier.h"
#include "GunBox.h"

#include "debug.h"

CGun::CGun(float x, float y, float nx) :CGameObject(x, y)
{
	if (nx < 0)
		vx = -GUN_ATTACK_SPEED;
	else
		vx = GUN_ATTACK_SPEED;
	SetState(GUN_STATE_RELASE);
}
void CGun::Render()
{
	RenderBoundingBox();
}
void CGun::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGun::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CSoldier*>(e->obj))
		OnCollisionWithSoldier(e);
	else if (dynamic_cast<CGunBox*>(e->obj))
		OnCollisionWithGunBox(e);
}
void CGun::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	goomba->SetState(GOOMBA_STATE_DIE);
}
void CGun::OnCollisionWithSoldier(LPCOLLISIONEVENT e)
{
	CSoldier* i = dynamic_cast<CSoldier*>(e->obj);
	i->SetState(GOOMBA_STATE_DIE);
}
void CGun::OnCollisionWithGunBox(LPCOLLISIONEVENT e)
{
	CGunBox* i = dynamic_cast<CGunBox*>(e->obj);
	i->SetState(GUNBOX_STATE_DIE);
}
void CGun::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if ((state != GUN_STATE_DIE) && (GetTickCount64() - count_start > 5000))
	{
		count_start = -1;
		isDeleted = true;
		return;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CGun::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - GUN_BBOX_WIDTH / 2;
	t = y - GUN_BBOX_HEIGHT / 2;
	r = l + GUN_BBOX_WIDTH;
	b = t + GUN_BBOX_HEIGHT;
}
void CGun::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GUN_STATE_RELASE:
		count_start = GetTickCount64();
		break;
	}
}

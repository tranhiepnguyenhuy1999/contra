#include "Fire.h"

CFire::CFire(float x, float y, float nx, float ny ) :CGameObject(x, y)
{
	vx = nx*FIRE_MOVING_SPEED;
	vy = ny * FIRE_GRAVITY;
	SetState(FIRE_STATE_RELASE);
}
void CFire::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
		animations->Get(ID_ANI_FIRE)->Render(x, y);
	//RenderBoundingBox();
}
void CFire::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CFire::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CFire*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CFire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if ((state == FIRE_STATE_DIE))
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CFire::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - FIRE_BBOX_WIDTH / 2;
	t = y - FIRE_BBOX_HEIGHT / 2;
	r = l + FIRE_BBOX_WIDTH;
	b = t + FIRE_BBOX_HEIGHT;
}
void CFire::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	}
}

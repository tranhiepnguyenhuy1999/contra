#include "MovingRock.h"

CMovingRock::CMovingRock(float x, float y) :CGameObject(x, y)
{
	vx = MOVING_ROCK_SPEED;
}
void CMovingRock::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_MOVING_ROCK_DEFAULT)->Render(x, y);
	RenderBoundingBox();
}
void CMovingRock::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CMovingRock::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}
}
void CMovingRock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CMovingRock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MOVING_ROCK_BBOX_WIDTH / 2;
	t = y + MOVING_ROCK_BBOX_HEIGHT / 2;
	r = l + MOVING_ROCK_BBOX_WIDTH;
	b = t - MOVING_ROCK_BBOX_HEIGHT;
}

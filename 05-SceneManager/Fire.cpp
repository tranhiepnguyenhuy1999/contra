#include "Fire.h"

CFire::CFire(float x, float y, float nx, float movingRange) :CGameObject(x, y)
{
	this->movingRange = movingRange;
	center = x;
	vx = nx * FIRE_SPEED;
}
void CFire::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_FIRE_DEFAULT)->Render(x, y);
	RenderBoundingBox();
}
void CFire::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CFire::OnCollisionWith(LPCOLLISIONEVENT e)
{
}
void CFire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (abs(x - center) > movingRange) {
		if (vx > 0) x = center + movingRange;
		else x = center - movingRange;
		vx = -vx;
	}
	else x += vx * dt;

	CGameObject::Update(dt, coObjects);
}
void CFire::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - FIRE_BBOX_WIDTH / 2;
	t = y + FIRE_BBOX_HEIGHT / 2;
	r = l + FIRE_BBOX_WIDTH;
	b = t - FIRE_BBOX_HEIGHT;
}

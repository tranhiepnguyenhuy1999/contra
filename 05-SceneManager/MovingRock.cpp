#include "MovingRock.h"

CMovingRock::CMovingRock(float x, float y, float range) :CGameObject(x, y)
{
	this->range = range;
	center = x;
	vx = MOVING_ROCK_SPEED;
}
void CMovingRock::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_MOVING_ROCK_DEFAULT)->Render(x, y);
	RenderBoundingBox();
}
void CMovingRock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	y += vy * dt;

	if (abs(x - center) > range)
	{
		if (vx > 0) {
			x = center + range;
		}
		else
		{
			x = center - range;
		}
		vx = -vx;
	}
	CGameObject::Update(dt, coObjects);
}
void CMovingRock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MOVING_ROCK_BBOX_WIDTH / 2;
	t = y + MOVING_ROCK_BBOX_HEIGHT / 2;
	r = l + MOVING_ROCK_BBOX_WIDTH;
	b = t - MOVING_ROCK_BBOX_HEIGHT;
}

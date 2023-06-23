#include "FGun.h"
#include "debug.h"

CFGun::CFGun(float x, float y, float nx, float ny, float vx, float vy, float type) :CGun(x, y, nx, ny, vx, vy, type)
{	
	accel = 15;
	angle = 0;
	centerX = x;
	centerY = y;
	r = 15.0f;
}
void CFGun::OnNoCollision(DWORD dt)
{
	x += vx * 10;
	y += vy * 10;
};
void CFGun::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	angle += accel;

	if (angle > 360)
	{
		centerX += 2*r;
		angle = accel;
	}

	float percent = (float)angle / 180.0f;

	x = r * sin(percent * M_PI) + centerX;
	y = r * cos(percent * M_PI) + centerY;

	if (state == GUN_STATE_DIE)	
	{
		count_start = -1;
		isDeleted = true;
	}

	CGameObject::Update(dt, coObjects);

	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CFGun::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - FGUN_BBOX_WIDTH / 2;
	t = y + FGUN_BBOX_HEIGHT / 2;
	r = l + FGUN_BBOX_WIDTH;
	b = t - FGUN_BBOX_HEIGHT;
}
void CFGun::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GUN_STATE_RELEASE:
		count_start = GetTickCount64();
		break;
	}
}

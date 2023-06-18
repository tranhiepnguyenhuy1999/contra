#include "FGun.h"
#include "debug.h"

CFGun::CFGun(float x, float y, float nx, float ny, float vx, float vy, float type) :CGun(x, y, nx, ny, vx, vy, type)
{	
	vyMax = FGUN_MAX_SPEED_Y;
	vxMax = FGUN_MAX_SPEED_X;

	vy = vyMax;
	ay = -FGUN_ACCEL_Y;

	vx = 0;
	ax = 2.0f*FGUN_ACCEL_X;

	nextMovement = 1;
	this->nx = 1;
}
void CFGun::OnNoCollision(DWORD dt)
{
	x += vx * 10;
	y += vy * 10;
};
void CFGun::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * 10;
	vy += ay * 10;

	

	if (vy < 0 && nextMovement ==1) {
		vy = 0;
		ay = -FGUN_ACCEL_Y;
		
		vx = 2.0f*vxMax;
		ax = -2.0f*FGUN_ACCEL_X;
		nextMovement = 2;
	}
	else if (vy < -vyMax && nextMovement==2) {
		vy = -vyMax;
		ay = FGUN_ACCEL_Y;
		
		vx = 0;
		ax = -FGUN_ACCEL_X;
		
		nextMovement = 3;
	}
	else if (vy > 0 && nextMovement==3) {
		vy = 0;
		ay = FGUN_ACCEL_Y;

		vx = -vxMax;
		ax = FGUN_ACCEL_X;

		nextMovement = 4;
	}
	else if (vy > vyMax && nextMovement==4)
	{
		vy = vyMax;
		ay = -FGUN_ACCEL_Y;

		vx =0;
		ax = 2.0f*FGUN_ACCEL_X;
		
		nextMovement = 1;
	}

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

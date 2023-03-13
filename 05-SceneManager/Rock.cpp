#include "Rock.h"
#include "Goomba.h"
#include "ParaGoomba.h"
#include "KoopaTroopa.h"
#include "BreakBrick.h"
#include "debug.h"

CRock::CRock(float x, float y, float nx) :CGameObject(x, y)
{
	if (nx < 0)
		vx = -ROCK_MOVING_SPEED;
	else
		vx = ROCK_MOVING_SPEED;
	vy = -ROCK_FLYING_SPEED;
	SetState(TAIL_STATE_RELASE);
	ax = 0;
	ay = MARIO_GRAVITY;
}
void CRock::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_ROCK)->Render(x, y);
	RenderBoundingBox();
}
void CRock::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CRock::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//if (e->ny != 0)
	//{
	//	vy = 0;
	//}
	//else if (e->nx != 0)
	//{
	//	vx = -vx;
	//}
}
void CRock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	if ((state != TAIL_STATE_DIE) && (GetTickCount64() - count_start > 500))
	{
		count_start = -1;
		isDeleted = true;
		return;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CRock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - TAIL_BBOX_WIDTH / 2;
	t = y - TAIL_BBOX_HEIGHT / 2;
	r = l + TAIL_BBOX_WIDTH;
	b = t + TAIL_BBOX_HEIGHT;
}
void CRock::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case TAIL_STATE_RELASE:
		count_start = GetTickCount64();
		break;
	}
}

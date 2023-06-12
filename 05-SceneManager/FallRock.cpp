#include "FallRock.h"
#include "DownBrick.h"
#include "debug.h"

CFallRock::CFallRock(float x, float y) :CGameObject(x, y)
{
	count_start = -1;
	isBounce = true;
	SetState(FALL_ROCK_STATE_UNACTIVE);

}
void CFallRock::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_FALL_ROCK_DEFAULT)->Render(x, y);
	RenderBoundingBox();
}
void CFallRock::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CFallRock::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny > 0)
	{
		if (dynamic_cast<CDownBrick*>(e->obj)) {
			if (isBounce) {
				vy = 0;
				SetState(FALL_ROCK_STATE_BOUNCE);
				isBounce = false;
			}
			else isBounce = true;
		}

	}
}
void CFallRock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == FALL_ROCK_STATE_UNACTIVE && GetTickCount64() - count_start > FALL_ROCK_UNACTIVE_TIMEOUT)
	{
		SetState(FALL_ROCK_STATE_ACTIVE);
	}

	vy += ay * dt;

	if (abs(vy) > abs(FALL_ROCK_MAX_GRAVITY)) vy = -FALL_ROCK_MAX_GRAVITY;

	CGameObject::Update(dt, coObjects);

	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CFallRock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == FALL_ROCK_STATE_UNACTIVE) return;
	l = x - FALL_ROCK_BBOX_WIDTH / 2;
	t = y + FALL_ROCK_BBOX_HEIGHT / 2;
	r = l + FALL_ROCK_BBOX_WIDTH;
	b = t - FALL_ROCK_BBOX_HEIGHT;
}
void CFallRock::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FALL_ROCK_STATE_UNACTIVE:
		ay = 0;
		count_start = GetTickCount64();
		break;
	case FALL_ROCK_STATE_ACTIVE:
		ay = -FALL_ROCK_GRAVITY;
		count_start = -1;
		break;
	case FALL_ROCK_STATE_BOUNCE:
		DebugOut(L"touch \n");
		vy += FALL_ROCK_BOUNCE_DEFECT;
		break;
	}
}

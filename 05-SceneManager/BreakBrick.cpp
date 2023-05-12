#include "BreakBrick.h"
#include "AssetIDs.h"
void CBreakBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniID = ID_ANI_UNTOUCHED_BREAK_BRICK;
	animations->Get(aniID)->Render(x, y);

	//RenderBoundingBox();
}

void CBreakBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}
void CBreakBrick::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CBreakBrick::OnCollisionWith(LPCOLLISIONEVENT e)
{
}
void CBreakBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == BREAK_BRICK_STATE_STATIC)
	{
		createRockObject();
		isDeleted = true;
		return;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CBreakBrick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case  BREAK_BRICK_STATE_TOUCHED_1:
		count_start = GetTickCount64();
		y = yLimit;
		break;
	case  BREAK_BRICK_STATE_UNTOUCHED:
	case  BREAK_BRICK_STATE_STATIC:
		y = yLimit + 2;
		break;
	}
}
void CBreakBrick::createRockObject() {
}
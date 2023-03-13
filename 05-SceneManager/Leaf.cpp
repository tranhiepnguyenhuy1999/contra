#include "Leaf.h"
CLeaf::CLeaf(float x, float y) :CGameObject(x, y)
{
	count_start = -1;
	yLimit = y - 48;
	SetState(LEAF_STATE_RELASE);
}
void CLeaf::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniID = ID_ANI_UNTOUCHED_LEAF_LEFT;
	if (vx > 0) aniID = ID_ANI_UNTOUCHED_LEAF_RIGHT;
	animations->Get(aniID)->Render(x, y);
	RenderBoundingBox();
}
void CLeaf::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if ((state == LEAF_STATE_DIE))
	{
		isDeleted = true;
		return;
	}
	if ((state == LEAF_STATE_MOVING_TURN) && (GetTickCount64() - count_start > 700))
	{
		SetState(LEAF_STATE_MOVING_TURN);
		return;
	}
	if (y < yLimit && state== LEAF_STATE_RELASE) {
		SetState(LEAF_STATE_MOVING_TURN);
		vx = LEAF_WALKING_SPEED;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - LEAF_BBOX_WIDTH / 2;
	t = y - LEAF_BBOX_HEIGHT / 2;
	r = l + LEAF_BBOX_WIDTH;
	b = t + LEAF_BBOX_HEIGHT;
}
void CLeaf::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case LEAF_STATE_RELASE:
		vy = -LEAF_RELASE_SPEED;
		vx = 0;
		break;
	case LEAF_STATE_MOVING_TURN:
		count_start = GetTickCount64();
		vy = LEAF_GRAVITY;
		vx = -vx;
		break;
	}
}

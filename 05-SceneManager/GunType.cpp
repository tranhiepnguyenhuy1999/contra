#include "GunType.h"
#include "debug.h"

CGunType::CGunType(float x, float y, float nx, int type) :CGameObject(x, y)
{
	if (nx < 0)
		vx = -GUNTYPE_SPEED;
	else
		vx = GUNTYPE_SPEED;
	ay = GUNTYPE_GRAVITY;
	id = (int)type;
	SetState(GUNTYPE_STATE_ACTIVE);
}
void CGunType::Render()
{
	int aniID = getAniID();
	CAnimations::GetInstance()->Get(aniID)->Render(x, y);
	RenderBoundingBox();
}
void CGunType::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGunType::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0; ay = 0;
		vx = 0;
	}
}
int CGunType::getAniID()
{
	switch (this->id)
	{
	case GUNTYPE_M:
		return ID_ANI_GUNTYPE_M;
	case GUNTYPE_F:
		return ID_ANI_GUNTYPE_F;
	case GUNTYPE_L:
		return ID_ANI_GUNTYPE_L;
	case GUNTYPE_S:
		return ID_ANI_GUNTYPE_S;
	case GUNTYPE_R:
		return ID_ANI_GUNTYPE_R;
	case GUNTYPE_UNTOUCHABLE:
		return ID_ANI_GUNTYPE_UNDEAD;
	case GUNTYPE_B:
		return ID_ANI_GUNTYPE_B;
	default:
		return -1;
	}
}
void CGunType::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	if (state == GUNTYPE_STATE_DIE)
	{
		count_start = -1;
		isDeleted = true;
		return;
	}
	if ((state == GUNTYPE_STATE_ACTIVE) && vy > GUNTYPE_GRAVITY_MAX)
	{
		ay = -ay;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CGunType::GetBoundingBox(float& l, float& t, float& r, float& b)
{
		l = x - GUNTYPE_BBOX_WIDTH / 2;
		t = y + GUNTYPE_BBOX_HEIGHT / 2;
		r = l + GUNTYPE_BBOX_WIDTH;
		b = t - GUNTYPE_BBOX_HEIGHT;
}
void CGunType::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GUNTYPE_STATE_ACTIVE:
		break;
	case GUNTYPE_STATE_DIE:
		break;
	}
}

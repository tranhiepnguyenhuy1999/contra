#include "GunType.h"
#include "debug.h"

CGunType::CGunType(float x, float y, float nx, float gunType) :CGameObject(x, y)
{
	if (nx < 0)
		vx = -GUNTYPE_SPEED;
	else
		vx = GUNTYPE_SPEED;
	ay = GUNTYPE_GRAVITY;
	type = (int)gunType;
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
	switch (this->type)
	{
	case 1:
		return ID_ANI_GUNTYPE_M;
	case 2:
		return ID_ANI_GUNTYPE_B;
	case 3:
		return ID_ANI_GUNTYPE_F;
	case 4:
		return ID_ANI_GUNTYPE_S;
	case 5:
		return ID_ANI_GUNTYPE_L;
	case 6:
		return ID_ANI_GUNTYPE_R;
	case 7:
		return ID_ANI_GUNTYPE_UNDEAD;
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
	t = y - GUNTYPE_BBOX_HEIGHT / 2;
	r = l + GUNTYPE_BBOX_WIDTH;
	b = t + GUNTYPE_BBOX_HEIGHT;
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

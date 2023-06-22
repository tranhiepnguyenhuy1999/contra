#include "Explode.h"

CExplode::CExplode(float x, float y, float type) :CGameObject(x, y)
{
	vx = 0;
	vy = 0;
	id = type;
	timeout = getTimeout();
	count_start = GetTickCount64();
}
void CExplode::Render()
{
	int aniID = getAniId();
	CAnimations::GetInstance()->Get(aniID)->Render(x, y);
}

void CExplode::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - EXPLODE_BBOX_WIDTH / 2;
	top = y + (EXPLODE_BBOX_HEIGHT / 2);
	right = left + EXPLODE_BBOX_WIDTH;
	bottom = top - EXPLODE_BBOX_HEIGHT;
}
void CExplode::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount64() - count_start > timeout)
	{
		count_start = -1;
		isDeleted = true;
		return;
	}
	CGameObject::Update(dt, coObjects);
}

int CExplode::getAniId() {
	switch (int(id))
	{
	case EXPLODE_TYPE_INFRASTRUCTURE:
		return ID_ANI_INFRASTRUCTURE_EXPLODE;
	case EXPLODE_TYPE_HUMAN:
		return ID_ANI_HUMAN_EXPLODE;
	default:
		return ID_ANI_INFRASTRUCTURE_EXPLODE;
	}
}
int CExplode::getTimeout() {
	switch (int(id))
	{
	case EXPLODE_TYPE_INFRASTRUCTURE:
		return INFRASTRUCTURE_EXPLODE_TIMEOUT;
	case EXPLODE_TYPE_HUMAN:
		return HUMAN_EXPLODE_TIMEOUT;
	default:
		return 100;
	}
}
void CExplode::SetState(int state)
{
	CGameObject::SetState(state);
}

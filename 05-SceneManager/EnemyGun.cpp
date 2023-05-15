#include "EnemyGun.h"
#include "Mario.h"

#include "debug.h"


CEnemyGun::CEnemyGun(float x, float y, float nx, float ny, int type) :CGameObject(x, y)
{
	if (nx < 0)
		vx = -ENEMY_GUN_ATTACK_SPEED;
	else
		vx = ENEMY_GUN_ATTACK_SPEED;
	gunType = type;
	SetState(ENEMY_GUN_STATE_RELASE);

}
void CEnemyGun::Render()
{
	int aniId = -1;
	aniId = getAniId();
	if (aniId != -1) CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}
int CEnemyGun::getAniId() {
	if (state == ENEMY_GUN_STATE_DIE)
		return ID_ANI_ENEMY_GUN_EXPLODE;
	switch (gunType)
	{
	case 0: return ID_ANI_ENEMY_GUN_DEFAULT;
	case 1:
		return 1;
	case 2:
		return 1;
	case 3:
		return 1;
	case 4:
		return 1;
	case 5:
		return 1;
	case 6:
		return 1;
	case 7:
		return 1;
	default:
		break;
	}
}

void CEnemyGun::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CEnemyGun::OnCollisionWith(LPCOLLISIONEVENT e)
{
	DebugOut(L">>> Mario DIE >>> \n");

	if (e->ny != 0)
	{
		vy = 0;
	}
	if (dynamic_cast<CMario*>(e->obj))
		OnCollisionWithPlayer(e);
}
void CEnemyGun::OnCollisionWithPlayer(LPCOLLISIONEVENT e)
{
	DebugOut(L">>> Mario DIE >>> \n");
	CMario* player = dynamic_cast<CMario*>(e->obj);
	player->SetState(MARIO_STATE_PRE_DIE);
}
void CEnemyGun::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if ((state != ENEMY_GUN_STATE_DIE) && (GetTickCount64() - count_start > 3000))
	{
		count_start = -1;
		isDeleted = true;
		return;
	}
	if ((state == ENEMY_GUN_STATE_DIE) && (GetTickCount64() - count_start > 100))
	{
		count_start = -1;
		isDeleted = true;
		return;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CEnemyGun::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - ENEMY_GUN_BBOX_WIDTH / 2;
	t = y - ENEMY_GUN_BBOX_HEIGHT / 2;
	r = l + ENEMY_GUN_BBOX_WIDTH;
	b = t + ENEMY_GUN_BBOX_HEIGHT;
}
void CEnemyGun::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ENEMY_GUN_STATE_RELASE:
		count_start = GetTickCount64();
		break;
	}
}

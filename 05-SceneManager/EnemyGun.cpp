#include "EnemyGun.h"
#include "Explode.h"
#include "AssetIDs.h"
#include "debug.h"


CEnemyGun::CEnemyGun(float x, float y, float nx, float ny, float vx, float vy, int type) :CGameObject(x, y)
{
	this->vx = vx;
	this->vy = vy;

	id = type;
	count_start = GetTickCount64();
}
void CEnemyGun::Render()
{
	int aniId = getAniID();
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}
int CEnemyGun::getAniID() {
	switch (id)
	{
	case 1:
		return ID_ANI_ENEMY_GUN_HIDE_SOLIDER;
	case 2:
		return ID_ANI_ENEMY_GUN_BOSS_STAGE_1;
	default:
		return ID_ANI_ENEMY_GUN_DEFAULT;
	}
}
int CEnemyGun::getExplodeID() {
	switch (id)
	{
	case 1:
		return EXPLODE_TYPE_INFRASTRUCTURE;
	case 2:
		return EXPLODE_TYPE_INFRASTRUCTURE;
	default:
		return ID_ANI_ENEMY_GUN_DEFAULT;
	}
}
void CEnemyGun::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CEnemyGun::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (id == 0) return;
	if (e->obj->IsBlocking()) SetState(ENEMY_GUN_STATE_DIE);
	
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}
}
void CEnemyGun::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	y += vy * dt;

	if (state == ENEMY_GUN_STATE_DIE)
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 0, 0, 0, 0, getExplodeID());
		count_start = -1;
		isDeleted = true;
		return;
	}
	else if (GetTickCount64() - count_start > 15000) {
		count_start = -1;
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
}
void CEnemyGun::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - ENEMY_GUN_BBOX_WIDTH / 2;
	t = y + ENEMY_GUN_BBOX_HEIGHT / 2;
	r = l + ENEMY_GUN_BBOX_WIDTH;
	b = t - ENEMY_GUN_BBOX_HEIGHT;
}
void CEnemyGun::SetState(int state)
{
	CGameObject::SetState(state);
}

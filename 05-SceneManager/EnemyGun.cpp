#include "EnemyGun.h"
#include "Explode.h"
#include "AssetIDs.h"
#include "debug.h"


CEnemyGun::CEnemyGun(float x, float y, float nx, float ny, float vx, float vy, int type) :CGameObject(x, y)
{
	if (nx > 0) ax = ENEMY_GUN_ACCEL; else ax = -ENEMY_GUN_ACCEL;
	if (ny > 0) ay = ENEMY_GUN_ACCEL; else ay = -ENEMY_GUN_ACCEL;
	this->vx = vx;
	this->vy = vy;
	vxMax = nx*ENEMY_GUN_MAX_SPEED;
	vyMax = ny*ENEMY_GUN_MAX_SPEED;
	id = type;
	SetState(ENEMY_GUN_STATE_RELASE);

}
void CEnemyGun::Render()
{
	int aniId = -1;
	aniId = getAniId();
	if (aniId != -1) CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}
int CEnemyGun::getAniId() {
	if (state == ENEMY_GUN_STATE_DIE)
		return ID_ANI_ENEMY_GUN_EXPLODE;
	switch (id)
	{
	case 0: return ID_ANI_ENEMY_GUN_DEFAULT;
	case 1:
		return ID_ANI_ENEMY_GUN_HIDE_SOLIDER;
	case 2:
		return ID_ANI_ENEMY_GUN_BOSS_STAGE_1;
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
int CEnemyGun::getExplodeID() {
	switch (id)
	{
	case 0: return ID_ANI_ENEMY_GUN_DEFAULT;
	case 1:
		return EXPLODE_TYPE_INFRASTRUCTURE;
	case 2:
		return EXPLODE_TYPE_INFRASTRUCTURE;
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
	vx += ax * dt;
	vy += ay * dt;

	if (abs(vx) > abs(vxMax)) vx = vxMax;
	if (abs(vy) > abs(vyMax)) vy = vyMax;

	if (state == ENEMY_GUN_STATE_RELASE && (GetTickCount64() - count_start > 10000))
	{
		count_start = -1;
		isDeleted = true;
		if (id == 1) {
			CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y);
			CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, -0.25f, -1, 0.1f, 0.05f);
			CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, 0, -1, 0, 0.1f);
			CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, 0.25f, -1,-0.1f, 0.05f);
		}
		return;
	}
	else if (state == ENEMY_GUN_STATE_DIE)
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 0, 0, 0, 0, getExplodeID());
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
	t = y + ENEMY_GUN_BBOX_HEIGHT / 2;
	r = l + ENEMY_GUN_BBOX_WIDTH;
	b = t - ENEMY_GUN_BBOX_HEIGHT;
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

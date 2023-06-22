#include "EnemyGun_AxAy.h"
#include "Explode.h"
#include "AssetIDs.h"
#include "debug.h"


CEnemyGun_AxAy::CEnemyGun_AxAy(float x, float y, float nx, float ny, float vx, float vy, int type) : CEnemyGun(x, y, nx, ny, vx, vy, type)
{
	ax = nx * ENEMY_GUN_ACCEL;
	ay = ny * ENEMY_GUN_ACCEL;


	vxMax = ny * ENEMY_GUN_MAX_SPEED;
	vyMax = ny * ENEMY_GUN_MAX_SPEED;

	SetState(ENEMY_GUN_STATE_RELASE);
}
void  CEnemyGun_AxAy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;

	if (abs(vx) > abs(vxMax)) vx = vxMax;
	if (abs(vy) > abs(vyMax)) vy = vyMax;

	if (state == ENEMY_GUN_STATE_DIE)
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 0, 0, 0, 0, EXPLODE_TYPE_INFRASTRUCTURE);
		count_start = -1;
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void  CEnemyGun_AxAy::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - ENEMY_GUN_BBOX_WIDTH / 2;
	t = y + ENEMY_GUN_BBOX_HEIGHT / 2;
	r = l + ENEMY_GUN_BBOX_WIDTH;
	b = t - ENEMY_GUN_BBOX_HEIGHT;
}

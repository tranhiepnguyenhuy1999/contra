#include "EnemyGun_HideSoldier.h"
#include "Explode.h"
#include "AssetIDs.h"
#include "debug.h"


 CEnenyGun_HideSoldier:: CEnenyGun_HideSoldier(float x, float y, float nx, float ny, float vx, float vy, int type) : CEnemyGun(x, y, nx, ny, vx, vy, type)
{
	 if (ny > 0) ay = ENEMY_GUN_ACCEL; else ay = -ENEMY_GUN_ACCEL;
	 if (nx > 0) ax = ENEMY_GUN_ACCEL; else ax = -ENEMY_GUN_ACCEL;

	 vxMax = ny * ENEMY_GUN_MAX_SPEED;
	 vyMax = ny * ENEMY_GUN_MAX_SPEED;
}
void  CEnenyGun_HideSoldier::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;

	if (abs(vx) > abs(vxMax)) vx = vxMax;
	if (abs(vy) > abs(vyMax)) vy = vyMax;

	if (state == ENEMY_GUN_STATE_RELASE && (GetTickCount64() - count_start > 3000))
	{
		//explode
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 0, 0, 0, 0, EXPLODE_TYPE_INFRASTRUCTURE);
		//separate into 3 pieces
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, -0.25f, -1, 0.1f, 0.05f);
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, 0, -1, 0, 0.1f);
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, 0.25f, -1, -0.1f, 0.05f);

		isDeleted = true;
		count_start = -1;
		return;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void  CEnenyGun_HideSoldier::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - ENEMY_GUN_HIDE_SOLDIER_BBOX_WIDTH / 2;
	t = y + ENEMY_GUN_HIDE_SOLDIER_BBOX_HEIGHT / 2;
	r = l + ENEMY_GUN_HIDE_SOLDIER_BBOX_WIDTH;
	b = t - ENEMY_GUN_HIDE_SOLDIER_BBOX_HEIGHT;
}

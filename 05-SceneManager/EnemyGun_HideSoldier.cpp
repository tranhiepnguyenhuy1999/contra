#include "EnemyGun_HideSoldier.h"
#include "Explode.h"
#include "AssetIDs.h"
#include "debug.h"


 CEnenyGun_HideSoldier:: CEnenyGun_HideSoldier(float x, float y, float nx, float ny, float vx, float vy, int type) : CEnemyGun(x, y, nx, ny, vx, vy, type)
{
	 ay = ny*ENEMY_GUN_ACCEL;
	 ax = nx*ENEMY_GUN_ACCEL;

	 vxMax = ny * ENEMY_GUN_MAX_SPEED;
	 vyMax = ny * ENEMY_GUN_MAX_SPEED;

	 count_start = GetTickCount64();
}
void  CEnenyGun_HideSoldier::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;

	if (abs(vx) > abs(vxMax)) vx = vxMax;
	if (abs(vy) > abs(vyMax)) vy = vyMax;

	if ((GetTickCount64() - count_start > 1500))
	{
		//explode
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 0, 0, 0, 0, EXPLODE_TYPE_INFRASTRUCTURE);
		//separate into 3 pieces
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, -0.25f, -1, 0.1f, 0.075f, ENEMY_GUN_HIDESOLDIER_PIECE);
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, 0, -1, 0, 0.115f, ENEMY_GUN_HIDESOLDIER_PIECE);
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, 0.25f, -1, -0.1f, 0.075f, ENEMY_GUN_HIDESOLDIER_PIECE);

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

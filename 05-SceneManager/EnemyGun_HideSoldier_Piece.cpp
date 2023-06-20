#include "EnemyGun_HideSoldier_Piece.h"
#include "Explode.h"
#include "AssetIDs.h"
#include "debug.h"


CEnenyGun_HideSoldier_Piece::CEnenyGun_HideSoldier_Piece(float x, float y, float nx, float ny, float vx, float vy, int type) : CEnemyGun(x, y, nx, ny, vx, vy, type)
{
	if (ny > 0) ay = ENEMY_GUN_ACCEL; else ay = -ENEMY_GUN_ACCEL;
	if (nx > 0) ax = ENEMY_GUN_ACCEL; else ax = -ENEMY_GUN_ACCEL;

	vxMax = ny * ENEMY_GUN_MAX_SPEED;
	vyMax = ny * ENEMY_GUN_MAX_SPEED;
	SetState(ENEMY_GUN_STATE_RELASE);

}
void  CEnenyGun_HideSoldier_Piece::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_ENEMY_GUN_HIDE_SOLIDER)->Render(x, y);
	RenderBoundingBox();
}
void  CEnenyGun_HideSoldier_Piece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
void  CEnenyGun_HideSoldier_Piece::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - ENEMY_GUN_BBOX_WIDTH / 2;
	t = y + ENEMY_GUN_BBOX_HEIGHT / 2;
	r = l + ENEMY_GUN_BBOX_WIDTH;
	b = t - ENEMY_GUN_BBOX_HEIGHT;
}

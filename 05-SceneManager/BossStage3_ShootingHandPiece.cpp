#include "BossStage3_ShootingHandPiece.h"
#include "AssetIDs.h"
#include "EnemyGun.h"

CBossStage3_ShootingHandPiece::CBossStage3_ShootingHandPiece(float x, float y, float r, float accel, float initAngle, int li1, int li2, int li3, int li4) :CBossStage3_HandPiece(x, y, r, accel, initAngle, li1, li2, li3, li4)
{
}
void CBossStage3_ShootingHandPiece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == BOSS_STAGE_3_HANDPIECE_STATE_MOVE_2)
	{
		if (angle >= 360) angle = accel;
		else angle += accel;
	}
	else if (state == BOSS_STAGE_3_HANDPIECE_STATE_MOVE_1)
	{
		if (angle > limit1)
		{
			angle = limit1;
			accel = -accel;
		}
		else if (angle < limit2)
		{
			angle = limit2;
			accel = -accel;
		}
		else
			angle += accel;
	}

	float percent = (float)angle / 180.0f;

	x = r * sin(percent * M_PI) + centerX;
	y = r * cos(percent * M_PI) + centerY;

	if (state == BOSS_STAGE_3_HANDPIECE_STATE_DIE)
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 0, 0, 0, 0, 1);
		isDeleted = true;
		return;
	}
	// shooting
	else if (GetTickCount64() - loop_start > 3000)
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, 0, 0, ENEMY_GUN_MAX_SPEED, -ENEMY_GUN_MAX_SPEED, ENEMY_GUN_BOSS_STAGE_3);
		loop_start = GetTickCount64();
	}
	CGameObject::Update(dt, coObjects);
}
void CBossStage3_ShootingHandPiece::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_BOSS_STAGE_3_SHOOTING_HANDPIECE)->Render(x, y);
	RenderBoundingBox();
}
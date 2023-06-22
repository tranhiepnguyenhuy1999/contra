#include "BossStage3_Hand.h"
#include "AssetIDs.h"
#include "EnemyGun.h"

CBossStage3_Hand::CBossStage3_Hand(float x, float y):CGameObject(x, y)
{
	loop_start = -1;
}

void CBossStage3_Hand::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == BOSS_STAGE_3_HAND_STATE_DIE)
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 0, 0, 0, 0, 1);
		isDeleted = true;
		return;
	}
	//// shooting
	//else if (state == BOSS_STAGE_3_HAND_STATE_ACTIVE && (GetTickCount64() - loop_start > BOSS_STAGE_3_HAND_SHOOTING_LOOP_TIMEOUT))
	//{
	//	float r = (float)((double)rand() / (RAND_MAX));
	//	CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, -r, -1, 0, ENEMY_GUN_MAX_SPEED / 2, 2);
	//	loop_start = GetTickCount64();
	//}

	CGameObject::Update(dt, coObjects);
	//CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBossStage3_Hand::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

void CBossStage3_Hand::Render()
{
}

void CBossStage3_Hand::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	//case BOSS_STAGE_3_HAND_STATE_DMG:
	//	life--;
	//	break;
	case BOSS_STAGE_3_HAND_STATE_DIE:
		loop_start = -1;
		break;
	}
}

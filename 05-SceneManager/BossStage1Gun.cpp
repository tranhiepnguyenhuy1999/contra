#include "BossStage1Gun.h"
#include "AssetIDs.h"
#include "EnemyGun.h"

CBossStage1Gun::CBossStage1Gun(float x, float y) :CGameObject(x, y)
{
	loop_start = -1;
	life = 1;
	SetState(BOSS_STAGE_1_GUN_STATE_ACTIVE);
}

void CBossStage1Gun::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - BOSS_STAGE_1_GUN_BBOX_WIDTH / 2;
	top = y + (BOSS_STAGE_1_GUN_BBOX_HEIGHT / 2);
	right = left + BOSS_STAGE_1_GUN_BBOX_WIDTH;
	bottom = top - BOSS_STAGE_1_GUN_BBOX_HEIGHT;
}

void CBossStage1Gun::OnNoCollision(DWORD dt)
{
	y += vy * dt;
};

void CBossStage1Gun::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CBossStage1Gun::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == BOSS_STAGE_1_GUN_STATE_DIE)
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 2);
		isDeleted = true;
		return;
	}

	// shooting
	else if (state == BOSS_STAGE_1_GUN_STATE_ACTIVE && (GetTickCount64() - loop_start > BOSS_STAGE_1_GUN_SHOOTING_LOOP_TIMEOUT))
	{
		float r = ((double)rand() / (RAND_MAX));
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, -r, -1, 0, ENEMY_GUN_MAX_SPEED/2, 2);
		loop_start = GetTickCount64();
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CBossStage1Gun::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_BOSS_STAGE_1_GUN)->Render(x, y);
	RenderBoundingBox();
}
void CBossStage1Gun::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOSS_STAGE_1_GUN_STATE_DMG:
		life--;
		break;
	case BOSS_STAGE_1_GUN_STATE_ACTIVE:
		loop_start = GetTickCount64();
		break;
	case BOSS_STAGE_1_GUN_STATE_DIE:
		loop_start = -1;
		break;
	}
}

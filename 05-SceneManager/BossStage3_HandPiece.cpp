#include "BossStage3_HandPiece.h"
#include "AssetIDs.h"
#include "EnemyGun.h"

CBossStage3_HandPiece::CBossStage3_HandPiece(float x, float y, float r, float accel, float initAngle) :CGameObject(x, y)
{
	this->r = r;
	this->accel = accel;
	angle = initAngle;
	centerX = x;
	centerY = y;
	loop_start = -1;
}

void CBossStage3_HandPiece::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - BOSS_STAGE_3_HANDPIECE_BBOX_WIDTH / 2;
	top = y + (BOSS_STAGE_3_HANDPIECE_BBOX_HEIGHT / 2);
	right = left + BOSS_STAGE_3_HANDPIECE_BBOX_WIDTH;
	bottom = top - BOSS_STAGE_3_HANDPIECE_BBOX_HEIGHT;
}

void CBossStage3_HandPiece::OnNoCollision(DWORD dt)
{
};

void CBossStage3_HandPiece::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CBossStage3_HandPiece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (angle >= 360) angle = accel;
	else angle += accel;
	float percent = (float)angle / 180.0f;
	x = r*sin(percent * M_PI) + centerX;
	y = r*cos(percent * M_PI) + centerY;

	if (state == BOSS_STAGE_3_HANDPIECE_STATE_DIE)
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 0, 0, 0, 0, 1);
		isDeleted = true;
		return;
	}
	//// shooting
	//else if (state == BOSS_STAGE_3_HANDPIECE_STATE_ACTIVE && (GetTickCount64() - loop_start > BOSS_STAGE_3_HANDPIECE_SHOOTING_LOOP_TIMEOUT))
	//{
	//	float r = (float)((double)rand() / (RAND_MAX));
	//	CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, -r, -1, 0, ENEMY_GUN_MAX_SPEED / 2, 2);
	//	loop_start = GetTickCount64();
	//}

	CGameObject::Update(dt, coObjects);
	//CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CBossStage3_HandPiece::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_BOSS_STAGE_3_HANDPIECE)->Render(x, y);
	RenderBoundingBox();
}
void CBossStage3_HandPiece::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOSS_STAGE_3_HANDPIECE_STATE_DMG:
		
		break;
	case BOSS_STAGE_3_HANDPIECE_STATE_DIE:
		loop_start = -1;
		break;
	}
}

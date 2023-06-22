#include "BossStage3_HandPiece.h"
#include "AssetIDs.h"
#include "EnemyGun.h"

CBossStage3_HandPiece::CBossStage3_HandPiece(float x, float y, float r, float accel, float initAngle, int li1, int li2, int li3, int li4):CGameObject(x, y)
{
	this->r = r;
	this->accel = accel;
	limit1 = li1;
	limit2 = li2;
	limit3 = li3;
	limit4 = li4;
	isFinishState4 = 1;
	isFinishState1 = 5;
	angle = initAngle;
	centerX = x;
	centerY = y;
	loop_start = -1;
	SetState(BOSS_STAGE_3_HANDPIECE_STATE_MOVE_5);
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
	if (state == BOSS_STAGE_3_HANDPIECE_STATE_MOVE_1 )
	{
		if (angle > limit1)
		{
			angle = limit1;
			accel = -accel;
			isFinishState1--;
		}
		else if (angle < limit2)
		{
			angle = limit2;
			accel = -accel;
			isFinishState1--;
		}
		else
			angle += accel;
	}
	else if (state == BOSS_STAGE_3_HANDPIECE_STATE_MOVE_2)
	{
		if (angle > 360)
		{
			angle = accel;
			isFinishState2--;
		}
		else angle += accel;
	}
	else if (state == BOSS_STAGE_3_HANDPIECE_STATE_MOVE_3) {
		if (angle > limit3 ) {
			angle = limit3;
			isFinishState3 = true;
		}
		else if (angle > 360) angle = accel;
		else angle += accel;
	}
	else if (state ==BOSS_STAGE_3_HANDPIECE_STATE_MOVE_4) {
		if (angle < limit4 && isFinishState4 ==0) {
			angle = limit4;
			isFinishState4 = true;
		}
		else if (angle < 0) angle = 360;
		else angle += accel;
	}
	else if (state == BOSS_STAGE_3_HANDPIECE_STATE_MOVE_5)
	{
		getAngleBaseOnPlayer();
	}
	float percent = (float)angle / 180.0f;

	x = r*sin(percent * M_PI) + centerX;
	y = r*cos(percent * M_PI) + centerY;

	if (state == BOSS_STAGE_3_HANDPIECE_STATE_DIE)
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 0, 0, 0, 0, 1);
		isDeleted = true;
		return;
	}
	CGameObject::Update(dt, coObjects);
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
	case BOSS_STAGE_3_HANDPIECE_STATE_DIE:
		break;
	case BOSS_STAGE_3_HANDPIECE_STATE_MOVE_1:
		break;
	case BOSS_STAGE_3_HANDPIECE_STATE_MOVE_2:
		break;
	case BOSS_STAGE_3_HANDPIECE_STATE_MOVE_3:
		break;
	case BOSS_STAGE_3_HANDPIECE_STATE_MOVE_4:
		accel = -accel;
		break;

	}
}
float CBossStage3_HandPiece::translateToPercent(float data, boolean isXAxis) {
	float px, py;
	float result = 0;
	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);
	if (isXAxis)
		result = abs(px - data) / 200;
	else
		result = abs(py - data) / 200;

	//DebugOut(L">>> result: %f >>> \n", result);

	if (result >= 1) return 10.0f;
	else if (result >= 0.9f) return 9.0f;
	else if (result >= 0.8f) return 8.0f;
	else if (result >= 0.7f) return 7.0f;
	else if (result >= 0.6f) return 6.0f;
	else if (result >= 0.5f) return 5.0f;
	else if (result >= 0.4f) return 4.0f;
	else if (result >= 0.3f) return 3.0f;
	else if (result >= 0.2f) return 2.0f;
	else return 1.0f;
}
void CBossStage3_HandPiece::getAngleBaseOnPlayer()
{
	float px, py;
	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);

	float percentX = translateToPercent(centerX, true);
	float percentY = translateToPercent(centerY, false);

	float percent = percentX / percentY;

	float altAngel = acos(abs(percent) / (sqrt(1) * sqrt((percent * percent) + 1)))*180/M_PI;
	DebugOut(L" angle %f \n", altAngel);
	angle = (INT)(FLOAT)floor((90.0f - altAngel) + 180.0f);
	//angle = 0;
}


#include "GunMachine1.h"
#include "AssetIDs.h"
#include "Camera.h"
#include "Explode.h"
#include "EnemyGun.h"

CGunMachine1::CGunMachine1(float x, float y) :CGameObject(x, y)
{
	isWorking = false;
	gunLeft = 3;
	activeRange = 200;
	loop_start = -1;
	gun_loop_start = -1;
	life = 8;
}
void CGunMachine1::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - GUNMACHINE1_BBOX_WIDTH / 2;
	top = y + (GUNMACHINE1_BBOX_HEIGHT / 2);
	right = left + GUNMACHINE1_BBOX_WIDTH;
	bottom = top - GUNMACHINE1_BBOX_HEIGHT;
}
void CGunMachine1::OnNoCollision(DWORD dt)
{
};
void CGunMachine1::OnCollisionWith(LPCOLLISIONEVENT e)
{
}
void CGunMachine1::handleGetAttack(int dmg)
{
	life -= dmg;
	if (life <= 0) SetState(GUNMACHINE1_STATE_DIE);
}
void CGunMachine1::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == GUNMACHINE1_STATE_DIE)
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 0, 0, 0, 0, EXPLODE_TYPE_INFRASTRUCTURE);
		isDeleted = true;
		return;
	}
	float px, py;
	Camera::GetInstance()->getPlayerPosition(px, py);

	if (!isWorking)
	{
		if (px > x - activeRange)
		{
			isWorking = true;
			SetState(GUNMACHINE1_STATE_BOTTOM);
		}
		return;
	}
	if (state == GUNMACHINE1_STATE_BOTTOM && (GetTickCount64() - loop_start > GUNMACHINE1_LOOP_TIMEOUT))
	{
		return	SetState(GUNMACHINE1_STATE_MID);
	}
	else if (state == GUNMACHINE1_STATE_MID && (GetTickCount64() - loop_start > GUNMACHINE1_LOOP_TIMEOUT))
	{
		return	SetState(GUNMACHINE1_STATE_TOP);
	}
	// shooting
	else if (state == GUNMACHINE1_STATE_TOP && (GetTickCount64() - loop_start > GUNMACHINE1_SHOOTING_TIMEOUT))
	{
		if (gunLeft <= 0)
		{
			loop_start = GetTickCount64();
			gun_loop_start = -1;
			gunLeft = 3;
		}
		else if (GetTickCount64() - gun_loop_start > GUNMACHINE1_SHOOTING_LOOP_TIMEOUT) {
			handleShooting();
			gun_loop_start = GetTickCount64();
			gunLeft -= 1;
		}
		return;
	}
	CGameObject::Update(dt, coObjects);
}
int CGunMachine1::getAniId(int flag) {
	if (state == GUNMACHINE1_STATE_BOTTOM) return ID_ANI_GUNMACHINE1_BOTTOM;
	else if (state == GUNMACHINE1_STATE_MID) return ID_ANI_GUNMACHINE1_MID;
	else if (state == GUNMACHINE1_STATE_TOP) {
		switch (flag)
		{
		case 1:
		{
			if (getPercent() >= 1) return ID_ANI_GUNMACHINE1_LEFTTOP;
			else return ID_ANI_GUNMACHINE1_TOP;

		}
		case 2:
			return ID_ANI_GUNMACHINE1_LEFT;
		}
		return ID_ANI_GUNMACHINE1_LEFT;
	}
	else return -1;

};
int CGunMachine1::getPlayerPosition() {
	float px, py;
	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);
	if (px < x) {
		if (py > y + GUNMACHINE1_BBOX_HEIGHT / 2) return 1; //left-top
		else if (py <= y + GUNMACHINE1_BBOX_HEIGHT / 2 && py >= y - GUNMACHINE1_BBOX_HEIGHT / 2) return 2;// left
		else return -1; //left-bottom
	}
	else return -1; // gunmachine1 is not working
}
void CGunMachine1::handleShooting()
{
	int flag = getPlayerPosition();
	if (flag == -1) return;

	if (flag==2)
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, 0, 0, ENEMY_GUN_MAX_SPEED);
		return;
	}
	float altShootingSpeed = getPercent();
	CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, 0, 0, -altShootingSpeed*ENEMY_GUN_MAX_SPEED , ENEMY_GUN_MAX_SPEED);

	//DebugOut(L">>> percent: %f >>> \n", percent);
	//DebugOut(L">>> altShootingSpeed: %f >>> \n", altShootingSpeed);

}
float CGunMachine1::translateToPercent(float data, boolean isXAxis) {
	float px, py;
	float result = 0;
	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);
	if (isXAxis)
		result = abs(px - data) / activeRange;
	else
		result = abs(py - data) / activeRange;

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
float CGunMachine1::getPercent()
{
	float px, py;

	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);

	float percentX = translateToPercent(x, true);
	float percentY = translateToPercent(y, false);
	float percent = percentX / percentY;
	float altShootingSpeed = 0;

	if (percent >= 2)
	{
		altShootingSpeed = 2;
	}
	else if (percent >= 1)
	{
		altShootingSpeed = 1;
	}
	else if (percent >= 0.5f)
	{
		altShootingSpeed = 0.5f;
	}
	return altShootingSpeed;
}
void CGunMachine1::Render()
{
	int flag = getPlayerPosition();
	int aniId= getAniId(flag);
	
	if(aniId !=-1) CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}
void CGunMachine1::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GUNMACHINE1_STATE_DIE:
		break;
	case GUNMACHINE1_STATE_BOTTOM:
		loop_start = GetTickCount64();
		break;
	case GUNMACHINE1_STATE_MID:
		loop_start = GetTickCount64();
		break;
	case GUNMACHINE1_STATE_TOP:
		loop_start = GetTickCount64();
		break;
	}
}

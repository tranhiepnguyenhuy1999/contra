
#include "GunMachine1.h"
#include "AssetIDs.h"
#include "debug.h"

CGunMachine1::CGunMachine1(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	this->isShooting = true;
	this->gunLeft = 3;
	this->activeRange = 200;
	this->loop_start = -1;
	this->gun_loop_start = -1;
	this->life = 8;
	SetState(GUNMACHINE1_STATE_UNACTIVE);
}
void CGunMachine1::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GUNMACHINE1_STATE_UNACTIVE)
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
	else {

		left = x - GUNMACHINE1_BBOX_WIDTH / 2;
		top = y + (GUNMACHINE1_BBOX_HEIGHT / 2);
		right = left + GUNMACHINE1_BBOX_WIDTH;
		bottom = top - GUNMACHINE1_BBOX_HEIGHT;
	}
}
void CGunMachine1::OnNoCollision(DWORD dt)
{
	y += vy * dt;
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
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 1);
		isDeleted = true;
		return;
	}

	float px, py;
	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);

	if (state == GUNMACHINE1_STATE_UNACTIVE && px > x-activeRange)
		return	SetState(GUNMACHINE1_STATE_BOTTOM);

	if ((state == GUNMACHINE1_STATE_BOTTOM) && (GetTickCount64() - loop_start > GUNMACHINE1_LOOP_TIMEOUT))
	{
		return	SetState(GUNMACHINE1_STATE_MID);
	}

	if ((state == GUNMACHINE1_STATE_MID) && (GetTickCount64() - loop_start > GUNMACHINE1_LOOP_TIMEOUT))
	{
		return	SetState(GUNMACHINE1_STATE_TOP);
	}

	// shooting
	if ((state == GUNMACHINE1_STATE_TOP) && (GetTickCount64() - loop_start > GUNMACHINE1_SHOOTING_TIMEOUT))
	{
		isShooting = true;
		if (gunLeft <= 0)
		{
			loop_start = GetTickCount64();
			gun_loop_start = -1;
			gunLeft = 3;
			isShooting = false;
		}
		else if (GetTickCount64() - gun_loop_start > GUNMACHINE1_SHOOTING_LOOP_TIMEOUT) {
			handleShooting();
			gun_loop_start = GetTickCount64();
			gunLeft -= 1;
		}
		return;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
int CGunMachine1::getAniId(int flag) {
	if (this->state == GUNMACHINE1_STATE_BOTTOM) return ID_ANI_GUNMACHINE1_BOTTOM;
	else if (this->state == GUNMACHINE1_STATE_MID) return ID_ANI_GUNMACHINE1_MID;
	else if (this->state == GUNMACHINE1_STATE_TOP) {
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
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, -GUNMACHINE1_GUN_SPEED, 0, 0);
		return;
	}
	float altShootingSpeed = getPercent();
	CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, altShootingSpeed * -GUNMACHINE1_GUN_SPEED, GUNMACHINE1_GUN_SPEED, 0);

	//DebugOut(L">>> percent: %f >>> \n", percent);
	//DebugOut(L">>> altShootingSpeed: %f >>> \n", altShootingSpeed);

}
int CGunMachine1::translateToPercent(float data, boolean isXAxis) {
	float px, py;
	float result = 0;
	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);
	if (isXAxis)
		result = abs(px - data) / activeRange;
	else
		result = abs(py - data) / activeRange;

	//DebugOut(L">>> result: %f >>> \n", result);

	if (result >= 1) return 10;
	else if (result >= 0.9f) return 9;
	else if (result >= 0.8f) return 8;
	else if (result >= 0.7f) return 7;
	else if (result >= 0.6f) return 6;
	else if (result >= 0.5f) return 5;
	else if (result >= 0.4f) return 4;
	else if (result >= 0.3f) return 3;
	else if (result >= 0.2f) return 2;
	else return 1;
}
float CGunMachine1::getPercent()
{
	float px, py;

	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);

	float percentX = translateToPercent(x, true);
	float percentY = translateToPercent(y, false);
	//DebugOut(L">>> x: %f >>> \n", percentX);
	//DebugOut(L">>> y: %f >>> \n", percentY);
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
	case GUNMACHINE1_STATE_UNACTIVE:
		break;
	case GUNMACHINE1_STATE_DIE:
		break;
	case GUNMACHINE1_STATE_BOTTOM:
		//y -= GUNMACHINE1_BOTTOM_HEIGHT/2;
		loop_start = GetTickCount64();
		break;
	case GUNMACHINE1_STATE_MID:
		//y -= GUNMACHINE1_MID_HEIGHT - GUNMACHINE1_BOTTOM_HEIGHT/2;
		loop_start = GetTickCount64();
		break;
	case GUNMACHINE1_STATE_TOP:
		//y -= GUNMACHINE1_TOP_HEIGHT - GUNMACHINE1_MID_HEIGHT/2;
		loop_start = GetTickCount64();
		break;
	}
}

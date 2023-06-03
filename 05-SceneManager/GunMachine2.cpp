#include "GunMachine2.h"
#include "AssetIDs.h"
#include "debug.h"

CGunMachine2::CGunMachine2(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	this->isShooting = true;
	this->gunLeft = 3;
	this->activeRange = 200;
	this->loop_start = -1;
	this->gun_loop_start = -1;
	this->life = 8;
	SetState(GUNMACHINE2_STATE_UNACTIVE);
}
void CGunMachine2::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GUNMACHINE2_STATE_UNACTIVE)
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
	else {

		left = x - GUNMACHINE2_BBOX_WIDTH / 2;
		top = y + (GUNMACHINE2_BBOX_HEIGHT / 2);
		right = left + GUNMACHINE2_BBOX_WIDTH;
		bottom = top - GUNMACHINE2_BBOX_HEIGHT;
	}
}
void CGunMachine2::OnNoCollision(DWORD dt)
{
	y += vy * dt;
};
void CGunMachine2::OnCollisionWith(LPCOLLISIONEVENT e)
{
}
void CGunMachine2::handleGetAttack(int dmg)
{
	life -= dmg;
	if (life <= 0) SetState(GUNMACHINE2_STATE_DIE);
}
void CGunMachine2::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == GUNMACHINE2_STATE_DIE)
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 1);
		isDeleted = true;
		return;
	}

	float px, py;
	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);

	if (state == GUNMACHINE2_STATE_UNACTIVE && px > x - activeRange)
		return	SetState(GUNMACHINE2_STATE_PRE_ACTIVE);

	if ((state == GUNMACHINE2_STATE_PRE_ACTIVE) && (GetTickCount64() - loop_start > GUNMACHINE2_LOOP_TIMEOUT))
	{
		return	SetState(GUNMACHINE2_STATE_ACTIVE);
	}

	// shooting
	if ((state == GUNMACHINE2_STATE_ACTIVE) && (GetTickCount64() - loop_start > GUNMACHINE2_SHOOTING_TIMEOUT))
	{
		isShooting = true;
		if (gunLeft <= 0)
		{
			loop_start = GetTickCount64();
			gun_loop_start = -1;
			gunLeft = 3;
			isShooting = false;
		}
		else if (GetTickCount64() - gun_loop_start > GUNMACHINE2_SHOOTING_LOOP_TIMEOUT) {
			handleShooting();
			gun_loop_start = GetTickCount64();
			gunLeft -= 1;
		}
		return;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
int CGunMachine2::getAniId(int flag) {
	if (this->state == GUNMACHINE2_STATE_UNACTIVE) return ID_ANI_GUNMACHINE2_UNACTIVE;
	else if (this->state == GUNMACHINE2_STATE_PRE_ACTIVE) return ID_ANI_GUNMACHINE2_PRE_ACTIVE;
	else if (this->state == GUNMACHINE2_STATE_ACTIVE) {
		switch (flag)
		{
		case 1:
			if (getPercent() >= 1) return ID_ANI_GUNMACHINE2_LEFTTOP;
			else return ID_ANI_GUNMACHINE2_TOPLEFT;
		case 2:
			return ID_ANI_GUNMACHINE2_LEFT;
		case 3:
			if (getPercent() >= 1) return ID_ANI_GUNMACHINE2_LEFTDOWN;
			else return ID_ANI_GUNMACHINE2_DOWNLEFT;
		case 4:
			if (getPercent() >= 1) return ID_ANI_GUNMACHINE2_RIGHTTOP;
			else return ID_ANI_GUNMACHINE2_TOPRIGHT;
		case 5:
			return ID_ANI_GUNMACHINE2_RIGHT;
		case 6:
			if (getPercent() >= 1) return ID_ANI_GUNMACHINE2_RIGHTDOWN;
			else return ID_ANI_GUNMACHINE2_DOWNRIGHT;
		}
		return -1;
	}
	else return -1;

};
int CGunMachine2::getPlayerPosition() {
	float px, py;
	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);
	if (px < x) {
		if (py > y + GUNMACHINE2_BBOX_HEIGHT) return 1; //left-top
		else if (py <= y + GUNMACHINE2_BBOX_HEIGHT / 2 && py >= y - GUNMACHINE2_BBOX_HEIGHT / 2) return 2;// left
		else return 3; //left-bottom
	}
	else
	{
		if (py > y + GUNMACHINE2_BBOX_HEIGHT) return 4; //right-top
		else if (py <= y + GUNMACHINE2_BBOX_HEIGHT / 2 && py >= y - GUNMACHINE2_BBOX_HEIGHT / 2) return 5;// right
		else return 6; //right-bottom
	}
}
void CGunMachine2::handleShooting()
{
	int flag = getPlayerPosition();
	if (flag == -1) return;
	int nx=0, ny=0;
	switch (flag)
	{
	case 1:
		nx = -1; ny = 1;
		break;
	case 2:
		nx = -1; ny = 0;
		break;
	case 3:
		nx = -1; ny = -1;
		break;
	case 4:
		nx = 1; ny = 1;
		break;
	case 5:
		nx = 1; ny = 0;
		break;
	case 6:
		nx = 1; ny = -1;
		break;
	}
	float altShootingSpeed = getPercent();
	CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y,nx*altShootingSpeed * GUNMACHINE2_GUN_SPEED, ny* GUNMACHINE2_GUN_SPEED, 0);
}
int CGunMachine2::translateToPercent(float data, boolean isXAxis) {
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
float CGunMachine2::getPercent()
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
void CGunMachine2::Render()
{
	int flag = getPlayerPosition();
	int aniId = getAniId(flag);

	if (aniId != -1) CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}
void CGunMachine2::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GUNMACHINE2_STATE_UNACTIVE:
		break;
	case GUNMACHINE2_STATE_DIE:
		break;
	case GUNMACHINE2_STATE_PRE_ACTIVE:
		//y -= GUNMACHINE2_BOTTOM_HEIGHT/2;
		loop_start = GetTickCount64();
		break;
	case GUNMACHINE2_STATE_ACTIVE:
		//y -= GUNMACHINE2_MID_HEIGHT - GUNMACHINE2_BOTTOM_HEIGHT/2;
		loop_start = GetTickCount64();
		break;
	}
}

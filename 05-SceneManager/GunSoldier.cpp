#include "GunSoldier.h"
#include "AssetIDs.h"
#include "debug.h"

CGunSoldier::CGunSoldier(float x, float y) :CGameObject(x, y)
{
	this->ay = 0;
	activeRange = 200;
	loop_start = -1;
	die_start = -1;
	gun_loop_start = -1;
	gunLeft = 3;
	isShooting = false;
	SetState(GUNSOLDIER_STATE_UNACTIVE);
}

void CGunSoldier::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
		left = x - GUNSOLDIER_BBOX_WIDTH / 2;
		top = y - (GUNSOLDIER_BBOX_HEIGHT / 2);
		right = left + GUNSOLDIER_BBOX_WIDTH;
		bottom = top + GUNSOLDIER_BBOX_HEIGHT;
}

void CGunSoldier::OnNoCollision(DWORD dt)
{
	y += vy * dt;
};

void CGunSoldier::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CGunSoldier::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	vy += ay * dt;

	//DebugOut(L">>> Count time >>> %d \n", GetTickCount64() - loop_start);
	float px, py;
	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);

	if ( px < x-activeRange && state != GUNSOLDIER_STATE_DIE)
		SetState(GUNSOLDIER_STATE_UNACTIVE);
	else
		SetState(GUNSOLDIER_STATE_ACTIVE);

	// handle obj die
	if (vy > GUNSOLDIER_DIE_DEFLECT) {
		ay = 0;
		vy = 0;
	}

	if ((state == GUNSOLDIER_STATE_DIE) && (GetTickCount64() - die_start > 500))
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 2);
		isDeleted = true;
		return;
	}

	// shooting
	if ((state == GUNSOLDIER_STATE_ACTIVE) && (GetTickCount64() - loop_start > GUNSOLDIER_POW_TIMEOUT))
	{
		isShooting = true;
		if (gunLeft <= 0)
		{
			loop_start = GetTickCount64();
			gun_loop_start = -1;
			gunLeft = 3;
			isShooting = false;
		}
		else if(GetTickCount64() - gun_loop_start > GUNSOLDIER_POW_LOOP_TIMEOUT){
			handleShooting();
			gun_loop_start = GetTickCount64();
			gunLeft -= 1;
		}
		return;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
int CGunSoldier::getAniId(int flag) {
	if (state == GUNSOLDIER_STATE_UNACTIVE) {
		if (flag <= 3) return ID_ANI_GUNSOLDIER_LEFTBOTTOM;
		else return ID_ANI_GUNSOLDIER_RIGHTBOTTOM;
	}
	//DebugOut(L">>> Mario DIE >>> %d \n", flag);
	switch (flag)
	{
	case 1:
		if(isShooting)
			return ID_ANI_GUNSOLDIER_SHOOTING_LEFTTOP;
		else
		return ID_ANI_GUNSOLDIER_LEFTTOP;
	case 2:
		if (isShooting)
			return ID_ANI_GUNSOLDIER_SHOOTING_LEFT;
		else
		return ID_ANI_GUNSOLDIER_LEFT;
	case 3:
		return ID_ANI_GUNSOLDIER_LEFTBOTTOM;
	case 4:
		if (isShooting)
			return ID_ANI_GUNSOLDIER_SHOOTING_RIGHTTOP;
		else
		return ID_ANI_GUNSOLDIER_RIGHTTOP;
	case 5:
		if (isShooting)
			return ID_ANI_GUNSOLDIER_SHOOTING_RIGHT;
		else
		return ID_ANI_GUNSOLDIER_RIGHT;
	case 6:
		return ID_ANI_GUNSOLDIER_RIGHTBOTTOM;
	}
	return -1;
};

int CGunSoldier::getPlayerPosition() {
	float px, py;
	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);
	if (px < x) {
		if (py > y + GUNSOLDIER_HEIGHT) return 1; //left-top
		else if (py <= y + GUNSOLDIER_HEIGHT/2 && py >= y - GUNSOLDIER_HEIGHT/2) return 2;// left
		else return 3; //left-bottom
	}
	else
	{
		if (py > y + GUNSOLDIER_HEIGHT) return 4; //right-top
		else if (py <= y + GUNSOLDIER_HEIGHT/2 && py >= y - GUNSOLDIER_HEIGHT/2) return 5;// right
		else return 6; //right-bottom
	}
}
void CGunSoldier::Render()
{
	int flag = getPlayerPosition();
	int aniId = getAniId(flag);

	if (aniId != -1) CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}
void CGunSoldier::handleShooting()
{
	int flag = getPlayerPosition();
	float nx=0, ny=0;

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

	if (nx == 0 || ny == 0)
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, nx*GUNSOLDIER_GUN_SPEED, ny*GUNSOLDIER_GUN_SPEED, 0);
		return;
	}

	float px, py;
	
	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);
	
	float percentX = translateToPercent(x, true);
	float percentY = translateToPercent(y, false);
	//DebugOut(L">>> x: %f >>> \n", percentX);
	//DebugOut(L">>> y: %f >>> \n", percentY);
	float percent = percentX/percentY;
	float altShootingSpeed=0;

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

	CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, nx * altShootingSpeed* GUNSOLDIER_GUN_SPEED, ny * GUNSOLDIER_GUN_SPEED, 0);

	//DebugOut(L">>> percent: %f >>> \n", percent);
	//DebugOut(L">>> altShootingSpeed: %f >>> \n", altShootingSpeed);

}
int CGunSoldier::translateToPercent(float data, boolean isXAxis) {
	float px, py;
	float result = 0;
	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);
	if(isXAxis)
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
void CGunSoldier::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GUNSOLDIER_STATE_UNACTIVE:
		loop_start = -1;
		break;
	case GUNSOLDIER_STATE_ACTIVE:
		//loop_start = GetTickCount64();
		break;
	case GUNSOLDIER_STATE_BOTTOM:
		//y -= GUNSOLDIER_BOTTOM_HEIGHT/2;
		loop_start = GetTickCount64();
		break;
	case GUNSOLDIER_STATE_MID:
		//y -= GUNSOLDIER_MID_HEIGHT - GUNSOLDIER_BOTTOM_HEIGHT/2;
		loop_start = GetTickCount64();
		break;
	case GUNSOLDIER_STATE_TOP:
		//y -= GUNSOLDIER_TOP_HEIGHT - GUNSOLDIER_MID_HEIGHT/2;
		loop_start = GetTickCount64();
		break;
	case GUNSOLDIER_STATE_DIE:
		die_start = GetTickCount64();
		ay = GUNSOLDIER_GRAVITY;
		break;
	}
}

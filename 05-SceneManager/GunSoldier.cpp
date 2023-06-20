#include "GunSoldier.h"
#include "EnemyGun.h"
#include "Explode.h"
#include "AssetIDs.h"

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
	if (state == GUNSOLDIER_STATE_DIE) return;
	left = x - GUNSOLDIER_BBOX_WIDTH / 2;
	top = y + (GUNSOLDIER_BBOX_HEIGHT / 2);
	right = left + GUNSOLDIER_BBOX_WIDTH;
	bottom = top - GUNSOLDIER_BBOX_HEIGHT;
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

	if (vy > GUNSOLDIER_DIE_DEFLECT) {
		ay = 0;
		vy = 0;
	}

	if (state == GUNSOLDIER_STATE_DIE)
	{
		if (GetTickCount64() - die_start > 500)
		{
			CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 0, 0, 0, 0, EXPLODE_TYPE_HUMAN);
			CPlayerData::GetInstance()->updatePoint(GUNSOLDIER_POINT);
			isDeleted = true;
			return;
		}
	}
	// shooting
	else if (GetTickCount64() - loop_start > GUNSOLDIER_SHOOTING_LOOP_TIMEOUT)
	{
		isShooting = true;
		if (gunLeft <= 0)
		{
			loop_start = GetTickCount64();
			gun_loop_start = -1;
			gunLeft = 3;
			isShooting = false;
		}
		else if(GetTickCount64() - gun_loop_start > GUNSOLDIER_SHOOTING_TIMEOUT){
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
	if (!isActive) {
		if (flag <= 3) return ID_ANI_GUNSOLDIER_LEFTBOTTOM;
		else return ID_ANI_GUNSOLDIER_RIGHTBOTTOM;
	}
	else switch (flag)
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

	// check position base on y
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
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, 0, 0, nx * ENEMY_GUN_MAX_SPEED, ny * ENEMY_GUN_MAX_SPEED);
		return;
	}

	float px, py;
	
	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);
	
	float percentX = translateToPercent(x, true);
	float percentY = translateToPercent(y, false);
	//DebugOut(L">>> x: %f >>> \n", percentX);
	//DebugOut(L">>> y: %f >>> \n", percentY);
	float percent = percentX/percentY;
	float altShootingXSpeed = 1;
	float altShootingYSpeed = 1;

	if (percent >= 2)
	{
		altShootingYSpeed = 0.5f;
	}
	else if (percent >= 1)
	{
		
	}
	else if (percent >= 0.5f)
	{
		altShootingXSpeed = 0.5f;
	}
	else 
		altShootingXSpeed = 0;

	CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, 0, 0, nx * altShootingXSpeed * ENEMY_GUN_MAX_SPEED, altShootingYSpeed * ny * ENEMY_GUN_MAX_SPEED);

	//DebugOut(L">>> percent: %f >>> \n", percent);
	//DebugOut(L">>> altShootingSpeed: %f >>> \n", altShootingSpeed);

}
float CGunSoldier::translateToPercent(float data, boolean isXAxis) {
	float px, py;
	float result = 0;
	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);
	if(isXAxis)
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
void CGunSoldier::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GUNSOLDIER_STATE_BOTTOM:
		loop_start = GetTickCount64();
		break;
	case GUNSOLDIER_STATE_MID:
		loop_start = GetTickCount64();
		break;
	case GUNSOLDIER_STATE_TOP:
		loop_start = GetTickCount64();
		break;
	case GUNSOLDIER_STATE_DIE:
		die_start = GetTickCount64();
		ay = GUNSOLDIER_GRAVITY;
		break;
	}
}

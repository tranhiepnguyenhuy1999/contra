#include "GunMachine2.h"
#include "AssetIDs.h"
#include "debug.h"

CGunMachine2::CGunMachine2(float x, float y) :CGameObject(x, y)
{
	position = 2;
	this->activeRange = 200;
	this->loop_start = -1;
	shooting_loop_start = -1;
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
	if (isCameraOver) return;
	float px, py;
	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);
	int new_position;
	new_position = getPlayerPosition();
	if (state == GUNMACHINE2_STATE_DIE)
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 1);
		isDeleted = true;
		return;
	}
	else if (state == GUNMACHINE2_STATE_UNACTIVE && px > x - activeRange)
		return	SetState(GUNMACHINE2_STATE_PRE_ACTIVE);
	else if ((state == GUNMACHINE2_STATE_PRE_ACTIVE) && (GetTickCount64() - loop_start > GUNMACHINE2_LOOP_TIMEOUT))
	{
		return	SetState(GUNMACHINE2_STATE_ACTIVE);
	}
	else if (position != new_position) {
		if (GetTickCount64() - loop_start > GUNMACHINE2_CHANGE_ANGLE_TIMEOUT) {
			int extra=1;
			if (position > new_position)
				extra=-1;
			// checking if it's pass hafe circle
			if (abs(new_position - position >= 6))
				extra = -extra;

			position += extra;
			
			if (position <= 0) position = 12;
			else if (position >= 13) position = 1;

			loop_start = GetTickCount64();
		}
	}
	// shooting
	else if ((state == GUNMACHINE2_STATE_ACTIVE) && (GetTickCount64() - shooting_loop_start > GUNMACHINE2_SHOOTING_TIMEOUT))
	{
		handleShooting();
		shooting_loop_start = GetTickCount64();
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
int CGunMachine2::getAniId() {

	if (this->state == GUNMACHINE2_STATE_UNACTIVE || isCameraOver) return ID_ANI_GUNMACHINE2_UNACTIVE;
	else if (this->state == GUNMACHINE2_STATE_PRE_ACTIVE) return ID_ANI_GUNMACHINE2_PRE_ACTIVE;
	else if (this->state == GUNMACHINE2_STATE_ACTIVE) {
		switch (position)
		{
		case 1:
			return ID_ANI_GUNMACHINE2_TOP;
		case 2:
			return ID_ANI_GUNMACHINE2_TOPLEFT;
		case 3:
			return ID_ANI_GUNMACHINE2_LEFTTOP;
		case 4:
			return ID_ANI_GUNMACHINE2_LEFT;
		case 5:
			return ID_ANI_GUNMACHINE2_LEFTDOWN;
		case 6:
			return ID_ANI_GUNMACHINE2_DOWNLEFT;
		case 7:
			return ID_ANI_GUNMACHINE2_DOWN;
		case 8:
			return ID_ANI_GUNMACHINE2_DOWNRIGHT;
		case 9:
			return ID_ANI_GUNMACHINE2_RIGHTDOWN;
		case 10:
			return ID_ANI_GUNMACHINE2_RIGHT;
		case 11:
			return ID_ANI_GUNMACHINE2_RIGHTTOP;
		case 12:
			return ID_ANI_GUNMACHINE2_TOPRIGHT;

		}
		return -1;
	}
	else return -1;

};
int CGunMachine2::getPlayerPosition() {
	float px, py;
	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);
	float perAxisXSpeed, perAxisYSpeed;
	getPercentXYSpeed(perAxisXSpeed, perAxisYSpeed);

	if (px >= x - GUNMACHINE2_BBOX_WIDTH / 2 && px <= x + GUNMACHINE2_BBOX_WIDTH / 2)
	{
		if (py > y) return 1;//top
		else return 7;//down

	}
	else if (px < x) {
		if (py > y + GUNMACHINE2_BBOX_HEIGHT) {
			if (perAxisYSpeed > perAxisXSpeed) return 2; //top-left
			else return 3; // left-top
		} 
		else if (py <= y + GUNMACHINE2_BBOX_HEIGHT / 2 && py >= y - GUNMACHINE2_BBOX_HEIGHT / 2) return 4;// left
		else {
			if (perAxisYSpeed > perAxisXSpeed) return 6; //down-left
			else return 5; // left-down
		}; 
	}
	else
	{
		if (py > y + GUNMACHINE2_BBOX_HEIGHT) 
			if (perAxisYSpeed > perAxisXSpeed) return 12; //top-right
			else return 11; // right-top
		else if (py <= y + GUNMACHINE2_BBOX_HEIGHT / 2 && py >= y - GUNMACHINE2_BBOX_HEIGHT / 2) return 10;// right
		else 
			if (perAxisYSpeed > perAxisXSpeed) return 8; //down-right
			else return 9; // right-down
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
		nx = 0; ny = 1;
		break;
	case 2:
	case 3:
		nx = -1; ny = 1;
		break;
	case 4:
		nx = -1; ny = 0;
		break;
	case 5:
	case 6:
		nx = -1; ny = -1;
		break;
	case 7:
		nx = 0; ny = -1;
		break;
	case 8:
	case 9:
		nx = 1; ny = -1;
		break;
	case 10:
		nx = 1; ny = 0;
		break;
	case 11:
	case 12:
		nx = 1; ny = 1;
		break;
	}
	float perAxisXSpeed, perAxisYSpeed;
	getPercentXYSpeed(perAxisXSpeed, perAxisYSpeed);
	CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y,nx* perAxisXSpeed * GUNMACHINE2_GUN_SPEED, ny* perAxisYSpeed *GUNMACHINE2_GUN_SPEED, 0);
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
void CGunMachine2::getPercentXYSpeed(float & perAxisXSpeed, float & perAxisYSpeed)
{
	float px, py;

	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);

	float percentX = translateToPercent(x, true);
	float percentY = translateToPercent(y, false);
	float percent = percentX / percentY;
	perAxisXSpeed = 1;
	perAxisYSpeed = 1;

	if (percent >= 4)
	{
		perAxisYSpeed = 0.5f;
	}
	else if (percent >= 1)
	{
	}
	else if (percent >= 0.25f)
	{
		perAxisXSpeed = 0.5f;
	}
}
void CGunMachine2::Render()
{
	int flag = getPlayerPosition();
	int aniId = getAniId();

	if (aniId != -1) CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
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

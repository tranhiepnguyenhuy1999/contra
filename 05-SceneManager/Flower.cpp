#include "Flower.h"
#include "AssetIDs.h"
#include "Pile.h"
#include "debug.h"
CFlower::CFlower(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	SetState(FLOWER_STATE_ACTIVE);
	yLimit = y - FLOWER_BBOX_HEIGHT;
	loop_start = -1;
}

void CFlower::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == FLOWER_STATE_ONBOTTOM)
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
	else {
	
		left = x - FLOWER_BBOX_WIDTH / 2;
		top = y	- (FLOWER_BBOX_HEIGHT/2);
		right = left + FLOWER_BBOX_WIDTH;
		bottom = top + FLOWER_BBOX_HEIGHT;
	}
}

void CFlower::OnNoCollision(DWORD dt)
{
	y += vy * dt;
};

void CFlower::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//DebugOut(L">>> Count time >>> %d \n", GetTickCount64() - loop_start);
	float px, py;
	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);

	if ((state == FLOWER_STATE_ONTOP) && (GetTickCount64() - loop_start > FLOWER_LOOP_TIMEOUT))
	{
		SetState(FLOWER_STATE_POW);
		return;
	}
	if ((state == FLOWER_STATE_POW) && (GetTickCount64() - loop_start > FLOWER_LOOP_TIMEOUT))
	{

	 	CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_FIRE, x, y);
		SetState(FLOWER_STATE_ACTIVE);
		return;
	}
	if ((state == FLOWER_STATE_ONBOTTOM) && (GetTickCount64() - loop_start > FLOWER_BOTTOM_LOOP_TIMEOUT))
	{
		if (px >= x - 50 && px < x + 50) return;
		SetState(FLOWER_STATE_ACTIVE);
		return;
	}

	if (y < yLimit)
	{
		SetState(FLOWER_STATE_ONTOP);
	}else if (y > yLimit + FLOWER_BBOX_HEIGHT)
	{
		SetState(FLOWER_STATE_ONBOTTOM);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
int CFlower::getMovingFlowerAniId(int flag) {
	switch (flag)
	{
		case 1:
			return ID_ANI_FLOWER_UP_LEFT_MOVING;
		case 2:
			return ID_ANI_FLOWER_DOWN_LEFT_MOVING;
		case 3:
			return ID_ANI_FLOWER_UP_RIGHT_MOVING;
		case 4:
			return ID_ANI_FLOWER_DOWN_RIGHT_MOVING;
	}

};
int CFlower::getStaticFlowerAniId(int flag) {
	switch (flag)
	{
		case 1:
			return ID_ANI_FLOWER_UP_LEFT_IDLE;
		case 2:
			return ID_ANI_FLOWER_DOWN_LEFT_IDLE;
		case 3:
			return ID_ANI_FLOWER_UP_RIGHT_IDLE;
		case 4:
			return ID_ANI_FLOWER_DOWN_RIGHT_IDLE;
	}
};
int CFlower::getFlowerPosition() {
	float px, py;
	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);
	if (px < x) {
		if (py < y) return 1; //top-left
		else return 2; //bot-left
	}
	else
	{
		if (py < y) return 3; //top-right
		else return 4; // bot-right
	}
}
void CFlower::Render()
{
	int flag = getFlowerPosition();
	int aniId;
	if (this->state == FLOWER_STATE_ONTOP) {
		aniId = getStaticFlowerAniId(flag);
	}
	else {
		aniId = getMovingFlowerAniId(flag);
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}
void CFlower::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FLOWER_STATE_POW:
		break;
	case FLOWER_STATE_ONTOP:
		y = yLimit;
		loop_start = GetTickCount64();
		vy = 0;
		break;
	case FLOWER_STATE_ONBOTTOM:
		y = yLimit + FLOWER_BBOX_HEIGHT;
		loop_start = GetTickCount64();
		vy = 0;
		break;
	case FLOWER_STATE_ACTIVE:
		if (y <= yLimit)
		{
			vy = FLOWER_SPEED;
		}
		else if (y >= yLimit + FLOWER_BBOX_HEIGHT)
		{
			vy = -FLOWER_SPEED;
		}
		break;
	}
}

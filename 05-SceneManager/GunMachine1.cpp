
#include "GunMachine1.h"
#include "AssetIDs.h"
#include "debug.h"

CGunMachine1::CGunMachine1(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	SetState(GUNMACHINE1_STATE_UNACTIVE);
	yLimit = y - GUNMACHINE1_BBOX_HEIGHT;
	xActive = x - 100;
	loop_start = -1;
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
		top = y - (GUNMACHINE1_BBOX_HEIGHT / 2);
		right = left + GUNMACHINE1_BBOX_WIDTH;
		bottom = top + GUNMACHINE1_BBOX_HEIGHT;
	}
}

void CGunMachine1::OnNoCollision(DWORD dt)
{
	y += vy * dt;
};

void CGunMachine1::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CGunMachine1::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//DebugOut(L">>> Count time >>> %d \n", GetTickCount64() - loop_start);
	float px, py;
	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);

	if (state == GUNMACHINE1_STATE_UNACTIVE && px > xActive)
		return	SetState(GUNMACHINE1_STATE_BOTTOM);

	if ((state == GUNMACHINE1_STATE_BOTTOM) && (GetTickCount64() - loop_start > GUNMACHINE1_LOOP_TIMEOUT))
	{
		return	SetState(GUNMACHINE1_STATE_MID);
	}
	if ((state == GUNMACHINE1_STATE_MID) && (GetTickCount64() - loop_start > GUNMACHINE1_LOOP_TIMEOUT))
	{
		return	SetState(GUNMACHINE1_STATE_TOP);
	}
	if ((state == GUNMACHINE1_STATE_TOP) && (GetTickCount64() - loop_start > GUNMACHINE1_POW_LOOP_TIMEOUT))
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_FIRE, x, y);
		loop_start = GetTickCount64();
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
			return ID_ANI_GUNMACHINE1_LEFTTOP;
		case 2:
			return ID_ANI_GUNMACHINE1_LEFT;
		case 3:
			return ID_ANI_GUNMACHINE1_TOP;
		case 4:
			return ID_ANI_GUNMACHINE1_TOP;
		}
	}
	else return -1;

};

int CGunMachine1::getFlowerPosition() {
	float px, py;
	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);
	if (px < x) {
		if (py < y) return 1; //top-left
		else return 2; //left
	}
	else
	{
		if (py < y) return 3; //top
		else return 4;
	}
}
void CGunMachine1::Render()
{
	int flag = getFlowerPosition();
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

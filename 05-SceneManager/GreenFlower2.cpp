#include "GreenFlower2.h"
#include "AssetIDs.h"
#include "Pile.h"
#include "debug.h"
CGreenFlower2::CGreenFlower2(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	SetState(FLOWER_STATE_ACTIVE);
	yLimit = y - FLOWER_BBOX_HEIGHT;
	loop_start = -1;
}

void CGreenFlower2::GetBoundingBox(float& left, float& top, float& right, float& bottom)
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
		top = y - (FLOWER_BBOX_HEIGHT / 2);
		right = left + FLOWER_BBOX_WIDTH;
		bottom = top + FLOWER_BBOX_HEIGHT;
	}
}

void CGreenFlower2::OnNoCollision(DWORD dt)
{
	y += vy * dt;
};

void CGreenFlower2::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CGreenFlower2::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//DebugOut(L">>> Count time >>> %d \n", GetTickCount64() - loop_start);
	float px, py;
	CGame::GetInstance()->GetCurrentScene()->getPlayerPosition(px, py);

	if ((state == FLOWER_STATE_ONTOP) && (GetTickCount64() - loop_start > FLOWER_LOOP_TIMEOUT))
	{
		SetState(FLOWER_STATE_ACTIVE);
		return;
	}
	if ((state == FLOWER_STATE_ONBOTTOM) && (GetTickCount64() - loop_start > FLOWER_BOTTOM_LOOP_TIMEOUT))
	{
		if (px >= x - 50 && px < x + 50) return; // check if player in mute zone
		SetState(FLOWER_STATE_ACTIVE);
		return;
	}

	if (y < yLimit)
	{
		SetState(FLOWER_STATE_ONTOP);
	}
	else if (y > yLimit + FLOWER_BBOX_HEIGHT)
	{
		SetState(FLOWER_STATE_ONBOTTOM);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CGreenFlower2::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_FLOWER_MOVING)->Render(x, y);
	RenderBoundingBox();
}
void CGreenFlower2::SetState(int state)
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

#include "SoldierCreation.h"
#include "Camera.h"
#include "AssetIDs.h"

CSoldierCreation::CSoldierCreation(float x, float y) :CGameObject(x, y)
{
	isClosed = false;
	range = 200;
	count_start = GetTickCount64();
}

void CSoldierCreation::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - SOLDIER_BBOX_WIDTH / 2;
	top = y + SOLDIER_BBOX_HEIGHT / 2;
	right = left + SOLDIER_BBOX_WIDTH;
	bottom = top - SOLDIER_BBOX_HEIGHT;
}

void CSoldierCreation::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float px, py;
	Camera::GetInstance()->getPlayerPosition(px, py);
	if (!isClosed) {
		if (abs(x - px) < range)
		{
			isClosed = true;
		}
	}

	if (!isClosed && GetTickCount64() - count_start > SOLDIER_CREATION_TIMEOUT) {
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_SOLDIER, x, y);
		count_start = GetTickCount64();
	}
	CGameObject::Update(dt, coObjects);
}


void CSoldierCreation::Render()
{
	RenderBoundingBox();
}

void CSoldierCreation::SetState(int state)
{
	CGameObject::SetState(state);
}

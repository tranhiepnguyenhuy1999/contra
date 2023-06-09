#include "HideSoldier.h"
#include "AssetIDs.h"
#include "Camera.h"
#include "PlayerData.h"
#include "EnemyGun.h"
#include "Explode.h"
#include "debug.h"


CHideSoldier::CHideSoldier(float x, float y) :CGameObject(x, y)
{
	isWorking = false;
	count_start = -1;
	activeRange = 100;
	SetState(HIDE_SOLDIER_STATE_UNACTIVE);
}

void CHideSoldier::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - HIDE_SOLDIER_BBOX_WIDTH / 2;
	top = y + HIDE_SOLDIER_BBOX_HEIGHT / 2;
	right = left + HIDE_SOLDIER_BBOX_WIDTH;
	bottom = top - HIDE_SOLDIER_BBOX_HEIGHT;
}

void CHideSoldier::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float px, py;
	Camera::GetInstance()->getPlayerPosition(px, py);

	if (!isWorking)
	{
		if (py > y + activeRange)
		{
			isWorking = true;
			count_start = GetTickCount64();
		}
		return;
	}

	if (GetTickCount64() - count_start > HIDE_SOLDIER_WAITING_SHOOTING_TIMEOUT) {
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_ENEMY_GUN, x, y, 0, 1, 0, 0, ENEMY_GUN_HIDESOLDIER);
		count_start = GetTickCount64();
	}
	else if (state == HIDE_SOLDIER_STATE_DIE)
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 0, 0, 0, 0, EXPLODE_TYPE_HUMAN);
		CPlayerData::GetInstance()->updatePoint(500);
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
}


void CHideSoldier::Render()
{
	int aniId = -1;
	if (state == HIDE_SOLDIER_STATE_DIE) return;
	else if (!isWorking) {
		if (nx > 0)
			aniId = ID_ANI_HIDE_SOLDIER_UNACTIVE_LEFT;
		else
			aniId = ID_ANI_HIDE_SOLDIER_UNACTIVE_RIGHT;
	}
	else {
		if (nx > 0)
			aniId = ID_ANI_HIDE_SOLDIER_ACTIVE_LEFT;
		else
			aniId = ID_ANI_HIDE_SOLDIER_ACTIVE_RIGHT;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CHideSoldier::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case HIDE_SOLDIER_STATE_DIE:
		count_start = -1;
		break;
	}
}

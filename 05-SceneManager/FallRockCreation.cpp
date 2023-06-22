#include "FallRockCreation.h"
#include "AssetIDs.h"
#include "Camera.h"
#include "PlayerData.h"
#include "EnemyGun.h"
#include "Explode.h"
#include "debug.h"


CFallRockCreation::CFallRockCreation(float x, float y) :CGameObject(x, y)
{
	count_start = GetTickCount64();
}

void CFallRockCreation::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - FALL_ROCK_CREATION_BBOX_WIDTH / 2;
	top = y + FALL_ROCK_CREATION_BBOX_HEIGHT / 2;
	right = left + FALL_ROCK_CREATION_BBOX_WIDTH;
	bottom = top - FALL_ROCK_CREATION_BBOX_HEIGHT;
}

void CFallRockCreation::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount64() - count_start > FALL_ROCK_CREATION_CREATE_TIMEOUT) {
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_FALL_ROCK, x, y);
		count_start = GetTickCount64();
	}
	CGameObject::Update(dt, coObjects);
}


void CFallRockCreation::Render()
{
	RenderBoundingBox();
}

void CFallRockCreation::SetState(int state)
{
	CGameObject::SetState(state);
}

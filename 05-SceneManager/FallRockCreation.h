#pragma once
#include "GameObject.h"

#define FALL_ROCK_CREATION_BBOX_WIDTH 32
#define FALL_ROCK_CREATION_BBOX_HEIGHT 32

#define FALL_ROCK_CREATION_CREATE_TIMEOUT 4500



class CFallRockCreation : public CGameObject
{
protected:
	ULONGLONG count_start;
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	CFallRockCreation(float x, float y);
	virtual void SetState(int state);
};
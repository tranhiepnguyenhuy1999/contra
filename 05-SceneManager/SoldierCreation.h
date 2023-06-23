#pragma once
#include "GameObject.h"

#define SOLDIER_BBOX_WIDTH 16
#define SOLDIER_BBOX_HEIGHT 34

#define SOLDIER_CREATION_TIMEOUT 3000

class CSoldierCreation : public CGameObject
{
protected:
	ULONGLONG count_start;
	float range;
	bool isClosed;
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	CSoldierCreation(float x, float y);
	virtual void SetState(int state);
};
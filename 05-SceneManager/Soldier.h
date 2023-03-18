#pragma once
#include "GameObject.h"

#define SOLDIER_GRAVITY 0.002f
#define SOLDIER_WALKING_SPEED 0.03f

#define SOLDIER_BBOX_WIDTH 16
#define SOLDIER_BBOX_HEIGHT 34

#define SOLDIER_DIE_TIMEOUT 500

#define SOLDIER_STATE_WALKING 100
#define SOLDIER_STATE_DIE 200

#define ID_ANI_SOLDIER_WALKING_LEFT	2002
#define ID_ANI_SOLDIER_WALKING_RIGHT 2001
#define ID_ANI_SOLDIER_DIE_RIGHT	2003
#define ID_ANI_SOLDIER_DIE_LEFT	2004

class CSoldier : public CGameObject
{
protected:
	float ax;
	float ay;
	bool isActive = false;

	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CSoldier(float x, float y);
	virtual void SetState(int state);
};
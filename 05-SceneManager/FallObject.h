#pragma once
#include "GameObject.h"

#define FALLOBJECT_GRAVITY 0.002f
#define FALLOBJECT_WALKING_SPEED 0.005f

#define FALLOBJECT_DIE_TIMEOUT 1000

#define FALLOBJECT_BBOX_WIDTH 8
#define FALLOBJECT_BBOX_HEIGHT 8

#define FALLOBJECT_STATE_WALKING 100
#define FALLOBJECT_STATE_DIE 200

class CFallObject : public CGameObject
{
protected:
	float ax;
	float ay;
	float yLimit;
	ULONGLONG count_start;
	LPGAMEOBJECT obj;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CFallObject(float x, float y, LPGAMEOBJECT obj);
	virtual void SetState(int state);
};
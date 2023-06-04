#pragma once
#include "GameObject.h"

#define FALL_OBJECT_GRAVITY 0.002f

#define FALL_OBJECT_BBOX_WIDTH 8
#define FALL_OBJECT_BBOX_HEIGHT 8

#define FALL_OBJECT_STATE_ACTIVE 100
#define FALL_OBJECT_STATE_DIE 200

class CFallObject : public CGameObject
{
protected:
	float ay;
	float fallenRange;
	bool isFallen;


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CFallObject(float x, float y, float nx);
	virtual void SetState(int state);
	bool IsFallen() { return isFallen;}
};
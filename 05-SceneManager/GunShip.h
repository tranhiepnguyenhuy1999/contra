#pragma once
#include "GameObject.h"

#define GUNSHIP_GRAVITY 0.0002f
#define GUNSHIP_SPEED 0.085f
#define GUNSHIP_GRAVITY_MAX	0.125f 

#define GUNSHIP_BBOX_WIDTH 16
#define GUNSHIP_BBOX_HEIGHT 14

#define GUNSHIP_STATE_ACTIVE 100	
#define GUNSHIP_STATE_DIE 200

#define ID_ANI_GUNSHIP_STATIC	3104
class CGunShip : public CGameObject
{
protected:
	float ay;
	float gunType;
	bool isWorking;

	ULONGLONG count_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CGunShip(float x, float y, float type);
	virtual void SetState(int state);
};
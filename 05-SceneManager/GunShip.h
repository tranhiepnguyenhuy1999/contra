#pragma once
#include "GameObject.h"

#define PARAGOOMBA_GRAVITY 0.001f
#define PARAGOOMBA_SPEED 0.1f
#define PARAGOOMBA_GRAVITY_MAX	0.25f 

#define PARAGOOMBA_MAX_Y 50

#define PARAGOOMBA_BBOX_WIDTH 16
#define PARAGOOMBA_BBOX_HEIGHT 14
#define PARAGOOMBA_BBOX_HEIGHT_DIE 7

#define PARAGOOMBA_DIE_TIMEOUT 1000

#define PARAGOOMBA_STATE_ACTIVE 100	
#define PARAGOOMBA_STATE_DIE 200

#define ID_ANI_PARAGOOMBA_STATIC	3104
class CGunShip : public CGameObject
{
protected:
	float ax;
	float ay;
	float typeGun;
	bool isActive = false;

	ULONGLONG count_start;
	ULONGLONG ready_jump_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CGunShip(float x, float y, float typeGun=1);
	virtual void SetState(int state);
};
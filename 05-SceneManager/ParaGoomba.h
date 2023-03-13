#pragma once
#include "GameObject.h"

#define PARAGOOMBA_GRAVITY 0.002f
#define PARAGOOMBA_WALKING_SPEED 0.02f
#define PARAGOOMBA_KICKING_SPEED 0.2f
#define PARAGOOMBA_JUMP_SPEED 0.05f

#define PARAGOOMBA_MAX_Y 50

#define PARAGOOMBA_BBOX_WIDTH 16
#define PARAGOOMBA_BBOX_HEIGHT 14
#define PARAGOOMBA_BBOX_HEIGHT_DIE 7

#define PARAGOOMBA_DIE_TIMEOUT 3000
#define PARAGOOMBA_ALIVE_TIMEOUT 2000
#define PARAGOOMBA_JUMP_TIMEOUT 500

#define PARAGOOMBA_STATE_WALKING 100	
#define PARAGOOMBA_STATE_DIE 200
#define PARAGOOMBA_STATE_FLYING 300
#define PARAGOOMBA_STATE_JUMP 500
#define PARAGOOMBA_STATE_RELEASE_JUMP 600

#define ID_ANI_PARAGOOMBA_WALKING 6000
#define ID_ANI_PARAGOOMBA_DIE 6001
#define ID_ANI_PARAGOOMBA_WINNGS_RIGHT 6002
#define ID_ANI_PARAGOOMBA_WINNGS_LEFT 6003
class CParaGoomba : public CGameObject
{
protected:
	float ax;
	float ay;
	float type;
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
	CParaGoomba(float x, float y);
	virtual void SetState(int state);
};
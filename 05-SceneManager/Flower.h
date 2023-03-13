#pragma once
#include "GameObject.h"

#define FLOWER_SPEED 0.03
#define FLOWER_BBOX_WIDTH 16
#define FLOWER_BBOX_HEIGHT 32

#define FLOWER_LOOP_TIMEOUT 1500
#define FLOWER_BOTTOM_LOOP_TIMEOUT 3000

#define FLOWER_STATE_ACTIVE 100
#define FLOWER_STATE_POW 200
#define FLOWER_STATE_STATIC 300
#define FLOWER_STATE_ONTOP 400
#define FLOWER_STATE_ONBOTTOM 500

#define ID_ANI_FLOWER_UP_LEFT_IDLE 12001
#define ID_ANI_FLOWER_DOWN_LEFT_IDLE 12002

#define ID_ANI_FLOWER_UP_LEFT_MOVING 12003
#define ID_ANI_FLOWER_DOWN_LEFT_MOVING 12004

#define ID_ANI_FLOWER_UP_RIGHT_IDLE 12005
#define ID_ANI_FLOWER_DOWN_RIGHT_IDLE 12006

#define ID_ANI_FLOWER_UP_RIGHT_MOVING 12007
#define ID_ANI_FLOWER_DOWN_RIGHT_MOVING 12008

class CFlower : public CGameObject
{

protected:
	float yLimit;
	float ax;
	float ay;	
	float ani_pow_id;

	ULONGLONG loop_start;
	
	
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	int getFlowerPosition();
	int getMovingFlowerAniId(int flag);
	int getStaticFlowerAniId(int flag);

public:
	CFlower(float x, float y);
	virtual void SetState(int state);
};
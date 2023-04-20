#pragma once
#include "GameObject.h"

#define GUNMACHINE1_SPEED 0.03
#define GUNMACHINE1_BBOX_WIDTH 16
#define GUNMACHINE1_BBOX_HEIGHT 32

#define GUNMACHINE1_LOOP_TIMEOUT 300
#define GUNMACHINE1_POW_LOOP_TIMEOUT 3000

#define GUNMACHINE1_STATE_UNACTIVE 100
#define GUNMACHINE1_STATE_BOTTOM 200
#define GUNMACHINE1_STATE_MID 300
#define GUNMACHINE1_STATE_TOP 400

#define ID_ANI_GUNMACHINE1_BOTTOM	4001
#define ID_ANI_GUNMACHINE1_MID	4002
#define ID_ANI_GUNMACHINE1_LEFT	4003
#define ID_ANI_GUNMACHINE1_LEFTTOP	4004
#define ID_ANI_GUNMACHINE1_TOP	4005

#define GUNMACHINE1_BOTTOM_HEIGHT 16
#define GUNMACHINE1_MID_HEIGHT 24
#define GUNMACHINE1_TOP_HEIGHT 32

class CGunMachine1 : public CGameObject
{

protected:
	float yLimit;
	float xActive;
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
	int getAniId(int flag);

public:
	CGunMachine1(float x, float y);
	virtual void SetState(int state);
};


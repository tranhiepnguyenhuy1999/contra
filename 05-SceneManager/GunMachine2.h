#pragma once
#include "GameObject.h"

#define GUNMACHINE2_GUN_SPEED 0.1f
#define GUNMACHINE2_BBOX_WIDTH 32
#define GUNMACHINE2_BBOX_HEIGHT 32

#define GUNMACHINE2_LOOP_TIMEOUT 300
#define GUNMACHINE2_SHOOTING_TIMEOUT 3000
#define GUNMACHINE2_SHOOTING_LOOP_TIMEOUT 250

#define GUNMACHINE2_STATE_UNACTIVE 100
#define GUNMACHINE2_STATE_PRE_ACTIVE 200
#define GUNMACHINE2_STATE_ACTIVE 300
#define GUNMACHINE2_STATE_DIE 400


#define ID_ANI_GUNMACHINE2_TOPLEFT	4101
#define ID_ANI_GUNMACHINE2_TOP	4102
#define ID_ANI_GUNMACHINE2_TOPRIGHT	4103

#define ID_ANI_GUNMACHINE2_LEFTTOP	4104
#define ID_ANI_GUNMACHINE2_LEFT	4105
#define ID_ANI_GUNMACHINE2_LEFTDOWN 4106

#define ID_ANI_GUNMACHINE2_UNACTIVE	4107
#define ID_ANI_GUNMACHINE2_PRE_ACTIVE	4108

#define ID_ANI_GUNMACHINE2_RIGHTTOP	4109
#define ID_ANI_GUNMACHINE2_RIGHT	4110
#define ID_ANI_GUNMACHINE2_RIGHTDOWN	4111

#define ID_ANI_GUNMACHINE2_DOWNLEFT	4112
#define ID_ANI_GUNMACHINE2_DONW	4113
#define ID_ANI_GUNMACHINE2_DOWNRIGHT	4114



#define GUNMACHINE2_BOTTOM_HEIGHT 16
#define GUNMACHINE2_MID_HEIGHT 24
#define GUNMACHINE2_TOP_HEIGHT 32

class CGunMachine2 : public CGameObject
{

protected:
	float yLimit;
	float activeRange;
	float ax;
	float ay;
	float ani_pow_id;

	float life;
	BOOLEAN isShooting;
	int gunLeft;

	ULONGLONG loop_start;
	ULONGLONG gun_loop_start;

	int getPlayerPosition();
	int getAniId(int flag);
	void handleShooting();
	int translateToPercent(float data, boolean isXAxis);
	float getPercent();

public:
	void handleGetAttack(int dmg);

	CGunMachine2(float x, float y);
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
};


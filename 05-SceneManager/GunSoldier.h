#pragma once
#include "GameObject.h"

#define GUNSOLDIER_SPEED 0.03
#define GUNSOLDIER_BBOX_WIDTH 16
#define GUNSOLDIER_BBOX_HEIGHT 32

#define GUNSOLDIER_GRAVITY 0.002f
#define GUNSOLDIER_DIE_DEFLECT 0.25f

#define GUNSOLDIER_LOOP_TIMEOUT 300
#define GUNSOLDIER_POW_LOOP_TIMEOUT 3000

#define GUNSOLDIER_STATE_UNACTIVE 100
#define GUNSOLDIER_STATE_BOTTOM 200
#define GUNSOLDIER_STATE_MID 300
#define GUNSOLDIER_STATE_TOP 400
#define GUNSOLDIER_STATE_DIE 500

#define ID_ANI_GUNSOLDIER_RIGHTBOTTOM	2101
#define ID_ANI_GUNSOLDIER_LEFTBOTTOM	2102
#define ID_ANI_GUNSOLDIER_RIGHT	2103
#define ID_ANI_GUNSOLDIER_LEFT	2104
#define ID_ANI_GUNSOLDIER_RIGHTTOP	2105
#define ID_ANI_GUNSOLDIER_LEFTTOP	2106

#define ID_ANI_GUNSOLDIER_SHOOTING_RIGHT	2107
#define ID_ANI_GUNSOLDIER_SHOOTING_LEFT	2108
#define ID_ANI_GUNSOLDIER_SHOOTING_RIGHTTOP	2109
#define ID_ANI_GUNSOLDIER_SHOOTING_LEFTTOP	2110

#define GUNSOLDIER_HEIGHT 24

class CGunSoldier : public CGameObject
{

protected:
	float yLimit;
	float xActive;
	float ax;
	float ay;
	float ani_pow_id;

	ULONGLONG loop_start;
	ULONGLONG die_start;


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	int getPlayerPosition();
	int getAniId(int flag);

public:
	CGunSoldier(float x, float y);
	virtual void SetState(int state);
};


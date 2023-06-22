#pragma once
#include "GameObject.h"

#define GUNSOLDIER_SPEED 0.03

#define GUNSOLDIER_POINT 500

#define GUNSOLDIER_BBOX_WIDTH 16
#define GUNSOLDIER_BBOX_HEIGHT 32

#define GUNSOLDIER_GUN_SPEED 0.05f

#define GUNSOLDIER_GRAVITY 0.002f
#define GUNSOLDIER_DIE_DEFLECT 0.25f

#define GUNSOLDIER_SHOOTING_LOOP_TIMEOUT 3000
#define GUNSOLDIER_SHOOTING_TIMEOUT 250

#define GUNSOLDIER_STATE_UNACTIVE 100
#define GUNSOLDIER_STATE_BOTTOM 200
#define GUNSOLDIER_STATE_MID 300
#define GUNSOLDIER_STATE_TOP 400
#define GUNSOLDIER_STATE_DIE 500
#define GUNSOLDIER_STATE_ACTIVE 600

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

#define ID_ANI_GUNSOLDIER_SHOOTING_HIDE	2111
#define ID_ANI_GUNSOLDIER_SHOOTING_HIDE_RIGHT	2112
#define ID_ANI_GUNSOLDIER_SHOOTING_HIDE_LEFT	2113

#define GUNSOLDIER_HEIGHT 24

class CGunSoldier : public CGameObject
{

protected:
	float activeRange;
	float ay;
	float gunLeft;
	float isHide;

	BOOLEAN isShooting;

	ULONGLONG loop_start;
	ULONGLONG gun_loop_start;
	ULONGLONG ready_shooting_start;
	ULONGLONG die_start;

	int getPlayerPosition();
	int getAniId(int flag);
	void handleShooting();
	float translateToPercent(float data, boolean isXAxis);
public:
	CGunSoldier(float x, float y, bool isHide=false);
	
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual void SetState(int state);

	bool isCollie() {
		if (isHide)
		{
			return isShooting;
		}
		else return true;
	}
};


#pragma once
#include "EnemyGun.h"

#define ENEMY_GUN_AXAY_MAX_SPEED	1.25f	

class  CEnemyGun_AxAy : public  CEnemyGun {
	float ay, ax;
	float vyMax, vxMax;

public:
	CEnemyGun_AxAy(float x, float y, float nx, float ny, float vx, float vy, int type);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};


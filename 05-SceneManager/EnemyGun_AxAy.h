#pragma once
#include "EnemyGun.h"

#define	ENEMY_GUN_HIDE_SOLDIER_BBOX_WIDTH 8
#define ENEMY_GUN_HIDE_SOLDIER_BBOX_HEIGHT 8

#define ENEMY_GUN_HIDE_SOLDIER_STATE_RELASE 100
#define ENEMY_GUN_HIDE_SOLDIER_STATE_DIE 100

#define ID_ANI_ENEMY_GUN__HIDE_SOLIDER 3309

class  CEnemyGun_AxAy : public  CEnemyGun {
	float ay, ax;
	float vyMax, vxMax;

public:
	CEnemyGun_AxAy(float x, float y, float nx, float ny, float vx, float vy, int type);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};


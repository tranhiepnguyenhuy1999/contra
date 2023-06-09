#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ENEMY_GUN_MAX_SPEED 0.1f
#define ENEMY_GUN_ACCEL	0.00025f

#define	ENEMY_GUN_BBOX_WIDTH 3
#define ENEMY_GUN_BBOX_HEIGHT 3

#define ENEMY_GUN_STATE_RELASE 100
#define ENEMY_GUN_STATE_DIE 200

//id 
#define ENEMY_GUN_DEFAULT 0
#define ENEMY_GUN_HIDESOLDIER 1
#define ENEMY_GUN_BOSS_STAGE_1 2
#define ENEMY_GUN_BOSS_STAGE_3 3
#define ENEMY_GUN_HIDESOLDIER_PIECE 4

#define ID_ANI_ENEMY_GUN_DEFAULT 3302
#define ID_ANI_ENEMY_GUN_HIDE_SOLIDER 3309
#define ID_ANI_ENEMY_GUN_BOSS_STAGE_1 3310

// explode ani
#define ID_ANI_ENEMY_GUN_EXPLODE 3003

class CEnemyGun : public CGameObject {
protected:

	int id;
	ULONGLONG count_start;
	
	int getAniID();
	int getExplodeID();

public:
	CEnemyGun(float x, float y, float nx, float ny, float vx, float vy, int type);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual void SetState(int state);
};

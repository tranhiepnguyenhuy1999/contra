#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define GUN_ATTACK_SPEED 0.2f

//bbox
#define	GUN_BBOX_WIDTH 2
#define GUN_BBOX_HEIGHT 2

#define	GUN_L_BBOX_WIDTH	16
#define	GUN_L_BBOX_HEIGHT	6

// state
#define GUN_STATE_RELASE 100
#define GUN_STATE_DIE 200

#define ID_ANI_GUN_DEFAULT 3301
#define ID_ANI_GUN_S_SMALL 3303
#define ID_ANI_GUN_S 3304
#define ID_ANI_GUN_S_BIG 3305	
#define ID_ANI_GUN_M 3306
#define ID_ANI_GUN_F 3307
#define ID_ANI_GUN_L 3308
#define ID_ANI_GUN_EXPLODE 3003
class CGun : public CGameObject {
	ULONGLONG count_start;
	int gunType;
	int dmg;

	int getAniId();
	int getDamage();

	void OnCollisionWithSoldier(LPCOLLISIONEVENT e);
	void OnCollisionWithGunSoldier(LPCOLLISIONEVENT e);
	void OnCollisionWithGunMachine1(LPCOLLISIONEVENT e);
	void OnCollisionWithGunBox(LPCOLLISIONEVENT e);
	void OnCollisionWithGunShip(LPCOLLISIONEVENT e);

public:
	CGun(float x, float y, int type, float nx, float ny);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual void SetState(int state);
};

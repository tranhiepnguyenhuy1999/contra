#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define GUN_ATTACK_SPEED 0.35f

#define GUN_WIDTH 2
#define	GUN_BBOX_WIDTH 2
#define GUN_BBOX_HEIGHT 2

#define GUN_STATE_RELASE 100
#define GUN_STATE_DIE 200

#define ID_ANI_GUN_DEFAULT 3301
#define ID_ANI_GUN_EXPLODE 3003
class CGun : public CGameObject {
	ULONGLONG count_start;
	int gunType;
public:
	CGun(float x, float y, float nx, float ny, int type);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	int getAniId();

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithSoldier(LPCOLLISIONEVENT e);
	void OnCollisionWithGunBox(LPCOLLISIONEVENT e);

	virtual void SetState(int state);
};

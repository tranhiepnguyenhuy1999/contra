#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define GUN_ATTACK_SPEED 0.2f

#define GUN_WIDTH 4
#define	GUN_BBOX_WIDTH 4
#define GUN_BBOX_HEIGHT 4

#define GUN_STATE_RELASE 100
#define GUN_STATE_DIE 200
class CGun : public CGameObject {
	ULONGLONG count_start;
public:
	CGun(float x, float y, float nx);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithSoldier(LPCOLLISIONEVENT e);
	void OnCollisionWithGunBox(LPCOLLISIONEVENT e);

	virtual void SetState(int state);
};

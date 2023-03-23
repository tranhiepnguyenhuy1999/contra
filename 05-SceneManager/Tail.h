#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define TAIL_ATTACK_SPEED 0.2f

#define TAIL_WIDTH 4
#define	TAIL_BBOX_WIDTH 4
#define TAIL_BBOX_HEIGHT 4

#define TAIL_STATE_RELASE 100
#define TAIL_STATE_DIE 200
class CTail : public CGameObject {
	ULONGLONG count_start;
public:
	CTail(float x, float y, float nx);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithSoldier(LPCOLLISIONEVENT e);
	void OnCollisionWithBreakBrick(LPCOLLISIONEVENT e);

	virtual void SetState(int state);
};

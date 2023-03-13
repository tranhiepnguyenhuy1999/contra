#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ROCK_MOVING_SPEED	0.07f
#define ROCK_FLYING_SPEED	0.35f
#define MARIO_GRAVITY	0.001f

#define ID_ANI_ROCK 10025

#define TAIL_WIDTH 4
#define	TAIL_BBOX_WIDTH 4
#define TAIL_BBOX_HEIGHT 8

#define TAIL_STATE_RELASE 100
#define TAIL_STATE_DIE 200
class CRock : public CGameObject {
	ULONGLONG count_start;
	float ax, ay;

public:
	CRock(float x, float y, float nx);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual void SetState(int state);
};

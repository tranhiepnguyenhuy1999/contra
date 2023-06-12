#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define FALL_ROCK_GRAVITY 0.0005f
#define FALL_ROCK_MAX_GRAVITY 0.25f
#define FALL_ROCK_BOUNCE_DEFECT 0.15f

#define	FALL_ROCK_BBOX_WIDTH 32
#define FALL_ROCK_BBOX_HEIGHT 32

#define FALL_ROCK_STATE_BOUNCE 100

#define ID_ANI_FALL_ROCK_DEFAULT 5001

class CFallRock : public CGameObject {
	float ay;
	bool isBounce;
public:
	CFallRock(float x, float y);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual void SetState(int state);

	bool IsBounce() { return isBounce; }

};
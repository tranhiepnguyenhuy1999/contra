#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define FIRE_SPEED 0.025f

#define	FIRE_BBOX_WIDTH 16
#define FIRE_BBOX_HEIGHT 16

#define ID_ANI_FIRE_DEFAULT 5000

class CFire : public CGameObject {
	float movingRange;
	float center;
public:
	CFire(float x, float y, float nx, float movingRange);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
};

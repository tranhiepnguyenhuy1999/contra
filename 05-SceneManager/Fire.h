#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define FIRE_GRAVITY 0.05f
#define FIRE_MOVING_SPEED 0.05f

#define ID_ANI_FIRE 12009
#define FIRE_WIDTH 8
#define	FIRE_BBOX_WIDTH 8
#define FIRE_BBOX_HEIGHT 9

#define FIRE_STATE_RELASE 100
#define FIRE_STATE_ACTIVE 200
#define FIRE_STATE_DIE 300

class CFire : public CGameObject {
public:
	CFire(float x, float y, float nx, float ny);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	//virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual void SetState(int state);
};



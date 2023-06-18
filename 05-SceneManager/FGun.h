#pragma once

#include "Gun.h"
#include "Animation.h"
#include "Animations.h"

#define FGUN_MAX_SPEED_Y	0.5f
#define FGUN_MAX_SPEED_X	0.6f


#define FGUN_ACCEL_X	0.0075f
#define FGUN_ACCEL_Y	0.0075f

//bbox
#define	FGUN_BBOX_WIDTH 8
#define FGUN_BBOX_HEIGHT 8

class CFGun : public CGun {
	int nextMovement;
public:
	CFGun(float x, float y, float nx, float ny, float vx, float vy, float type);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);

	virtual void OnNoCollision(DWORD dt);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
};

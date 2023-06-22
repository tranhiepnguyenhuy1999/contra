#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define MOVING_ROCK_SPEED 0.05f

#define	MOVING_ROCK_BBOX_WIDTH 32
#define MOVING_ROCK_BBOX_HEIGHT 32

#define ID_ANI_MOVING_ROCK_DEFAULT 5001

class CMovingRock : public CGameObject {
	float range;
	float center;
public:
	CMovingRock(float x, float y, float range);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};

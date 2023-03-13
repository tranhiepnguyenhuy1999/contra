#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define LEAF_GRAVITY 0.02f
#define LEAF_WALKING_SPEED 0.05f
#define LEAF_RELASE_SPEED 0.1f
#define ID_ANI_UNTOUCHED_LEAF_LEFT 10007
#define ID_ANI_UNTOUCHED_LEAF_RIGHT 10008

#define LEAF_WIDTH 16
#define	LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 16

#define LEAF_STATE_RELASE 100
#define LEAF_STATE_MOVING_TURN 200
#define LEAF_STATE_DIE 400

class CLeaf : public CGameObject {
	ULONGLONG count_start;
	float yLimit;
public:
	CLeaf(float x, float y);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual void SetState(int state);
};

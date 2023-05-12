#pragma once
#include "debug.h"
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define BREAK_BRICK_MOVING_SPEED 0.05f

#define ID_ANI_UNTOUCHED_BREAK_BRICK 10024
#define ID_ANI_TOUCHED_BREAK_BRICK 10006	

#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define BREAK_BRICK_TYPE_NORMAL 1
#define BREAK_BRICK_TYPE_SPECIAL 2

#define BREAK_BRICK_STATE_UNTOUCHED 100
#define BREAK_BRICK_STATE_TOUCHED_1 200
#define BREAK_BRICK_STATE_TOUCHED_2 300
#define BREAK_BRICK_STATE_STATIC 400
class CBreakBrick : public CGameObject {
	float type;
	float yLimit;
	ULONGLONG count_start;

public:
	CBreakBrick(float x, float y, float type = 1) : CGameObject(x, y) {
		SetState(BREAK_BRICK_STATE_UNTOUCHED);
		this->type = type;
		yLimit = y - 2;
		count_start = -1;
	};
	void Render();
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	float getType() { return this->type; };
	void createRockObject();
	//virtual int IsCollidable() { return 1; };
	//virtual int IsBlocking() { return 0; }
};
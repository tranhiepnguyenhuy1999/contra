#pragma once
#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define GUNTYPE_GRAVITY	0.00045f
#define GUNTYPE_SPEED	0.045f
#define GUNTYPE_GRAVITY_MAX	0.175f

//id
#define GUNTYPE_M	1
#define GUNTYPE_F	2
#define GUNTYPE_L	3
#define GUNTYPE_S	4
#define GUNTYPE_R	5
#define GUNTYPE_UNTOUCHABLE	6
#define GUNTYPE_B	7
//ani
#define ID_ANI_GUNTYPE_M	3201
#define ID_ANI_GUNTYPE_B	3202
#define ID_ANI_GUNTYPE_F	3203
#define ID_ANI_GUNTYPE_S	3204
#define ID_ANI_GUNTYPE_L	3205
#define ID_ANI_GUNTYPE_R	3206
#define ID_ANI_GUNTYPE_UNDEAD	3207

#define	GUNTYPE_BBOX_WIDTH 24
#define GUNTYPE_BBOX_HEIGHT 16

#define GUNTYPE_STATE_ACTIVE 100
#define GUNTYPE_STATE_DIE 200

class CGunType : public CGameObject {
	ULONGLONG count_start;
	float ay;

public:
	int id;
	CGunType(float x, float y, float nx, int gunType);

	int getAniID();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual void SetState(int state);
};



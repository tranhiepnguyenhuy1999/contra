#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define GUN_STATE_RELASE 100
#define GUN_STATE_DIE 200

#define ID_ANI_HUMAN_EXPLODE	3001
#define ID_ANI_INFRASTRUCTURE_EXPLODE	3002

#define INFRASTRUCTURE_EXPLODE_TIMEOUT 750
#define HUMAN_EXPLODE_TIMEOUT 400

#define EXPLODE_TYPE_INFRASTRUCTURE 1
#define EXPLODE_TYPE_HUMAN 2


class CExplode : public CGameObject {
	ULONGLONG count_start;
	float id;
	int timeout;
	int getAniId();
	int getTimeout();
public:
	CExplode(float x, float y, float type);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; };

	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);


	virtual void SetState(int state);
};

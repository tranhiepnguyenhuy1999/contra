#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define GUN_STATE_RELASE 100
#define GUN_STATE_DIE 200

#define ID_ANI_INFRASTRUCTURE_EXPLODE	3002

class CExplode : public CGameObject {
	ULONGLONG count_start;
	int exType;
public:
	CExplode(float x, float y, int type);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);


	virtual void SetState(int state);
};
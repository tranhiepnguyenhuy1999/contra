#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ENEMY_GUN_ATTACK_SPEED 0.1f

#define ENEMY_GUN_WIDTH 2
#define	ENEMY_GUN_BBOX_WIDTH 2
#define ENEMY_GUN_BBOX_HEIGHT 2

#define ENEMY_GUN_STATE_RELASE 100
#define ENEMY_GUN_STATE_DIE 200

#define ID_ANI_ENEMY_GUN_DEFAULT 3302
#define ID_ANI_ENEMY_GUN_EXPLODE 3003
class CEnemyGun : public CGameObject {
	ULONGLONG count_start;
	int gunType;
public:
	CEnemyGun(float x, float y, float nx, float ny, int type);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	int getAniId();

	void OnCollisionWithPlayer(LPCOLLISIONEVENT e);

	virtual void SetState(int state);
};
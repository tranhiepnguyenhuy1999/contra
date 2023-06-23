#pragma once
#include "GameObject.h"
#define BOSS_STAGE_1_GUN_BBOX_WIDTH 16
#define BOSS_STAGE_1_GUN_BBOX_HEIGHT 4

#define BOSS_STAGE_1_GUN_SHOOTING_LOOP_TIMEOUT 750

#define BOSS_STAGE_1_GUN_STATE_ACTIVE 100
#define BOSS_STAGE_1_GUN_STATE_DIE 200
#define BOSS_STAGE_1_GUN_STATE_DMG 300

#define ID_ANI_BOSS_STAGE_1_GUN	4201

class CBossStage1Gun : public CGameObject
{

protected:
	int life;
	ULONGLONG loop_start;

public:
	CBossStage1Gun(float x, float y);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual void SetState(int state);
	void isAttacked() {
		life--;
	}
	bool isDie() { return (life <= 0); }
};



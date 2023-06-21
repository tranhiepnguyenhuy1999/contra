#pragma once
#include "GameObject.h"
#define BOSS_STAGE_1_GUN_BBOX_WIDTH 8
#define BOSS_STAGE_1_GUN_BBOX_HEIGHT 8

#define BOSS_STAGE_1_GUN_SHOOTING_LOOP_TIMEOUT 750

#define BOSS_STAGE_1_GUN_STATE_ACTIVE 100
#define BOSS_STAGE_1_GUN_STATE_DIE 200
#define BOSS_STAGE_1_GUN_STATE_DMG 300

#define ID_ANI_BOSS_STAGE_1_GUN	4201

#define M_PI 3.14159f

class CBossStage3_HandPiece : public CGameObject
{

protected:
	int life;
	int angle;
	float initX, initY;
	ULONGLONG loop_start;

public:
	CBossStage3_HandPiece(float x, float y);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual void SetState(int state);

	bool isDie() { return (life <= 0); }
};



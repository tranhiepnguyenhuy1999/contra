#pragma once
#include "GameObject.h"
#define BOSS_STAGE_3_HANDPIECE_BBOX_WIDTH 8
#define BOSS_STAGE_3_HANDPIECE_BBOX_HEIGHT 8

#define BOSS_STAGE_3_HANDPIECE_STATE_DIE 100
#define BOSS_STAGE_3_HANDPIECE_STATE_DMG 200

#define ID_ANI_BOSS_STAGE_3_HANDPIECE	5100

#define M_PI 3.14159f

class CBossStage3_HandPiece : public CGameObject
{

protected:
	int angle;
	int accel;
	float r;
	float centerX, centerY;
	ULONGLONG loop_start;

public:
	CBossStage3_HandPiece(float x, float y, float r, float accel, float initAngle);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual void SetState(int state);

	void setAngle(int value) {
		angle = value;
	}
};



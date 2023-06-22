#pragma once
#include "GameObject.h"
#define BOSS_STAGE_3_HANDPIECE_BBOX_WIDTH 12
#define BOSS_STAGE_3_HANDPIECE_BBOX_HEIGHT 12

#define BOSS_STAGE_3_HANDPIECE_ACCEL_R 0.5f


#define BOSS_STAGE_3_HANDPIECE_STATE_DIE 100
#define BOSS_STAGE_3_HANDPIECE_STATE_MOVE_1 200
#define BOSS_STAGE_3_HANDPIECE_STATE_MOVE_2 300
#define BOSS_STAGE_3_HANDPIECE_STATE_MOVE_3 400
#define BOSS_STAGE_3_HANDPIECE_STATE_MOVE_4 500
#define BOSS_STAGE_3_HANDPIECE_STATE_MOVE_5 600


#define ID_ANI_BOSS_STAGE_3_HANDPIECE	5100

#define M_PI 3.14159f

class CBossStage3_HandPiece : public CGameObject
{

protected:
	int angle;
	int accel;
	int limit1, limit2, limit3, limit4;
	float r;
	float centerX, centerY;
	ULONGLONG loop_start;

	int isFinishState1;
	int isFinishState2;
	bool isFinishState3;
	int isFinishState4;
	float translateToPercent(float data, boolean isXAxis);
	void getAngleBaseOnPlayer();
public:
	CBossStage3_HandPiece(float x, float y, float r, float accel, float initAngle, int li1=0, int li2=0, int li3=0, int li4=0);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual void SetState(int state);

	void setAngle(int value) {
		angle = value;
	}
	void setR(float value) {
		r = value;
	}
};



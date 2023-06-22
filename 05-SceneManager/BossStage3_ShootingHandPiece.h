#pragma once

#include "BossStage3_HandPiece.h"

#define ID_ANI_BOSS_STAGE_3_SHOOTING_HANDPIECE	5101

#define M_PI 3.14159f

class CBossStage3_ShootingHandPiece : public CBossStage3_HandPiece
{

public:
	CBossStage3_ShootingHandPiece(float x, float y, float r, float accel, float initAngle, int li1=0, int li2=0, int li3=0, int li4=0);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	bool IsFinishState1() { return isFinishState1<=0;}
	bool IsFinishState2() { return isFinishState2 <= 0; }
	bool IsFinishState3() { return isFinishState3; }
	bool IsFinishState4() { return isFinishState4;}

};
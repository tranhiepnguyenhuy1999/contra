#pragma once

#include "BossStage3_HandPiece.h"

#define ID_ANI_BOSS_STAGE_3_SHOOTING_HANDPIECE	5101

#define M_PI 3.14159f

class CBossStage3_ShootingHandPiece : public CBossStage3_HandPiece
{

public:
	CBossStage3_ShootingHandPiece(float x, float y, float r, float accel, float initAngle);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};
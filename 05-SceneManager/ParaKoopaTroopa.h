#pragma once
#include "GameObject.h"

#define PARAKOOPATROOPA_WALKING_SPEED 0.03f
#define PARAKOOPATROOPA_KICKING_SPEED 0.2f
#define PARAKOOPATROOPA_JUMP_SPEED 0.2f

#define PARAKOOPATROOPA_MAX_Y 50

#define PARAKOOPATROOPA_BBOX_WIDTH 15
#define PARAKOOPATROOPA_BBOX_HEIGHT 26
#define PARAKOOPATROOPA_BBOX_HEIGHT_DIE 16

#define PARAKOOPATROOPA_DIE_TIMEOUT 3000
#define PARAKOOPATROOPA_ALIVE_TIMEOUT 2000
#define PARAKOOPATROOPA_JUMP_TIMEOUT 500

#define PARAKOOPATROOPA_STATE_WALKING 100
#define PARAKOOPATROOPA_STATE_DIE 200
#define PARAKOOPATROOPA_STATE_ALIVE 300
#define PARAKOOPATROOPA_STATE_KICKING 400
#define PARAKOOPATROOPA_STATE_JUMP 500
#define PARAKOOPATROOPA_STATE_RELEASE_JUMP 600

#define ID_ANI_PARAKOOPATROOPA_WALKING 7000
#define ID_ANI_PARAKOOPATROOPA_DIE 7001
#define ID_ANI_PARAKOOPATROOPA_KICKING 7002
#define ID_ANI_PARAKOOPATROOPA_ALIVE 7003

class CParaKoopaTroopa : public CGameObject
{
protected:
	float ax;
	float ay;
	float type;
	bool isActive = false;
	ULONGLONG count_start;
	ULONGLONG ready_jump_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
public:
	CParaKoopaTroopa(float x, float y);
	virtual void SetState(int state);
};
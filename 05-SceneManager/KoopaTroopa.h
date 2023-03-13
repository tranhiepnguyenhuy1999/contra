#pragma once
#include "GameObject.h"
#include "FallObject.h"
#include "AssetIDs.h"

#define KOOPATROOPA_GRAVITY 0.002f
#define KOOPATROOPA_WALKING_SPEED 0.03f
#define KOOPATROOPA_KICKING_SPEED 0.2f

#define KOOPATROOPA_MAX_Y 50

#define KOOPATROOPA_BBOX_WIDTH 15
#define KOOPATROOPA_BBOX_HEIGHT 26
#define KOOPATROOPA_BBOX_HEIGHT_DIE 16

#define KOOPATROOPA_DIE_TIMEOUT 3000
#define KOOPATROOPA_ALIVE_TIMEOUT 2000
#define KOOPATROOPA_JUMP_TIMEOUT 500

#define KOOPATROOPA_STATE_WALKING 100
#define KOOPATROOPA_STATE_DIE 200
#define KOOPATROOPA_STATE_ALIVE 300
#define KOOPATROOPA_STATE_KICKING_LEFT 400
#define KOOPATROOPA_STATE_KICKING_RIGHT 500
#define KOOPATROOPA_STATE_TURNBACK 600

#define ID_ANI_KOOPATROOPA_WALKING_LEFT 7000
#define ID_ANI_KOOPATROOPA_WALKING_RIGHT 7004
#define ID_ANI_KOOPATROOPA_DIE 7001
#define ID_ANI_KOOPATROOPA_KICKING 7002
#define ID_ANI_KOOPATROOPA_ALIVE 7003

class CKoopaTroopa : public CGameObject
{
protected:
	float ax;
	float ay;
	float type;
	bool isActive;

	ULONGLONG count_start;
	ULONGLONG ready_jump_start;
	LPGAMEOBJECT fallObj;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithDownBrick(LPCOLLISIONEVENT e);

	void createFallObject() {
		if (vx < 0) {
			CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_FALLOBJECT, x - KOOPATROOPA_BBOX_WIDTH / 2 - FALLOBJECT_BBOX_WIDTH / 2, y, 0, 0, this);
		}
		else
		{
			CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_FALLOBJECT, x + KOOPATROOPA_BBOX_WIDTH / 2 + FALLOBJECT_BBOX_WIDTH / 2, y, 0, 0, this);

		}
	}
public:
	CKoopaTroopa(float x, float y);
	virtual void SetState(int state);
	void addFallObject(LPGAMEOBJECT obj) {
		fallObj = obj;
	}
};
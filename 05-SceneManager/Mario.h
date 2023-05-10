#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f
#define MARIO_PRE_DIE_SPEED		0.05f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.008f

#define MARIO_JUMP_SPEED_Y		0.6f
#define MARIO_JUMP_RUN_SPEED_Y	0.7f

#define MARIO_GRAVITY	0.002f

#define MARIO_JUMP_DEFLECT_SPEED  0.3f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_DOWN_RIGHT	401
#define MARIO_STATE_RUNNING_UP_RIGHT	402
#define MARIO_STATE_RUNNING_LEFT	500
#define MARIO_STATE_RUNNING_DOWN_LEFT	501
#define MARIO_STATE_RUNNING_UP_LEFT	502

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_SHOOTING		700

#define MARIO_STATE_LOOKUP		800
#define	MARIO_STATE_LOOKUP_RELEASE	801

#define MARIO_STATE_PRE_DIE	900

#pragma region ANIMATION_ID
//BIG
#define ID_ANI_MARIO_IDLE_RIGHT 1001
#define ID_ANI_MARIO_IDLE_LEFT 1002

#define ID_ANI_MARIO_WALKING_RIGHT 1005
#define ID_ANI_MARIO_WALKING_LEFT 1006

#define ID_ANI_MARIO_RUNNING_DOWN_RIGHT 1017
#define ID_ANI_MARIO_RUNNING_UP_RIGHT 1015
#define ID_ANI_MARIO_RUNNING_DOWN_LEFT 1018
#define ID_ANI_MARIO_RUNNING_UP_LEFT 1016

#define ID_ANI_MARIO_JUMP_RIGHT 1007
#define ID_ANI_MARIO_JUMP_LEFT 1008

#define ID_ANI_MARIO_SIT_RIGHT 1009
#define ID_ANI_MARIO_SIT_LEFT 1010

#define ID_ANI_MARIO_DIE 1019
#define ID_ANI_MARIO_DIE_LEFT 1020

#define ID_ANI_MARIO_PRE_DIE 1012
#define ID_ANI_MARIO_PRE_DIE_LEFT 1011

#define ID_ANI_MARIO_STAND_UP_RIGHT 1013
#define ID_ANI_MARIO_STAND_UP_LEFT 1014

#define ID_ANI_MARIO_SHOOTING_RIGHT 1021
#define ID_ANI_MARIO_SHOOTING_LEFT 1022

#pragma endregion

#define GROUND_Y 160.0f

#define MARIO_BIG_BBOX_WIDTH  23
#define MARIO_BIG_BBOX_HEIGHT 34
#define MARIO_BIG_SITTING_BBOX_WIDTH  32
#define MARIO_BIG_SITTING_BBOX_HEIGHT 12

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  12
#define MARIO_SMALL_BBOX_HEIGHT 12


#define MARIO_UNTOUCHABLE_TIME 2500

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	BOOLEAN isLookingUp;
	BOOLEAN isPreDied;
	BOOLEAN isShooting;



	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	int untouchable;


	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;
	ULONGLONG count_start;
	ULONGLONG shooting_start;

	int coin;
	int point;
	int life;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithSoldier(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	int GetAniId();
public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		isLookingUp = false;
		isPreDied = false;
		isShooting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = -MARIO_GRAVITY;
		untouchable = 0;
		untouchable_start = -1;
		count_start = -1;
		shooting_start = -1;
		isOnPlatform = false;
		coin = 0;
		point = 0;
		life = 1;

	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	int IsCollidable()
	{
		//return (state != MARIO_STATE_DIE);
		return 1;
	}
	void GetProps(int& coinP, int& pointP, int& lifeP, int& isFlyStakP) {
		coinP = coin;
		pointP = point;
		lifeP = life;
	}
	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable == 0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); };
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void createTailObject();

};
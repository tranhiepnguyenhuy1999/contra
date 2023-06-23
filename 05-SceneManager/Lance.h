#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define LANCE_WALKING_SPEED		0.075f
#define LANCE_PRE_DIE_SPEED		0.075f

#define LANCE_ACCEL_WALK_X	0.0005f

#define LANCE_JUMP_SPEED_Y		0.2f

#define LANCE_GRAVITY	0.00035f

#define LANCE_DIE_DEFLECT_SPEED  0.2f

#define LANCE_STATE_DIE				-10
#define LANCE_STATE_IDLE			0
#define LANCE_STATE_WALKING_RIGHT	100
#define LANCE_STATE_WALKING_LEFT	200

#define LANCE_STATE_JUMP			300
#define LANCE_STATE_RELEASE_JUMP    301

#define LANCE_STATE_RUNNING_RIGHT	400
#define LANCE_STATE_RUNNING_DOWN_RIGHT	401
#define LANCE_STATE_RUNNING_UP_RIGHT	402
#define LANCE_STATE_RUNNING_LEFT	500
#define LANCE_STATE_RUNNING_DOWN_LEFT	501
#define LANCE_STATE_RUNNING_UP_LEFT	502

#define LANCE_STATE_SIT				600
#define LANCE_STATE_SIT_RELEASE		601

#define LANCE_STATE_SHOOTING		700

#define LANCE_STATE_LOOKUP		800
#define	LANCE_STATE_LOOKUP_RELEASE	801

#define LANCE_STATE_PRE_DIE	900

#define LANCE_STATE_MOVING_RELEASE 1000

#define LANCE_STATE_SWIMMING 1100

#define LANCE_STATE_CLIMB 1200

#define LANCE_STATE_GO_DOWN 1300

#define LANCE_STATE_UNTOUCHABLE 1400

#pragma region ANIMATION_ID
//BIG
#define ID_ANI_LANCE_IDLE_RIGHT 1001
#define ID_ANI_LANCE_IDLE_LEFT 1002

#define ID_ANI_LANCE_WALKING_RIGHT 1005
#define ID_ANI_LANCE_WALKING_LEFT 1006

#define ID_ANI_LANCE_RUNNING_DOWN_RIGHT 1017
#define ID_ANI_LANCE_RUNNING_UP_RIGHT 1015
#define ID_ANI_LANCE_RUNNING_DOWN_LEFT 1018
#define ID_ANI_LANCE_RUNNING_UP_LEFT 1016

#define ID_ANI_LANCE_JUMP_RIGHT 1007
#define ID_ANI_LANCE_JUMP_LEFT 1008

#define ID_ANI_LANCE_SIT_RIGHT 1009
#define ID_ANI_LANCE_SIT_LEFT 1010

#define ID_ANI_LANCE_DIE 1019
#define ID_ANI_LANCE_DIE_LEFT 1020

#define ID_ANI_LANCE_PRE_DIE 1012
#define ID_ANI_LANCE_PRE_DIE_LEFT 1011

#define ID_ANI_LANCE_STAND_UP_RIGHT 1013
#define ID_ANI_LANCE_STAND_UP_LEFT 1014

#define ID_ANI_LANCE_SHOOTING_RIGHT 1021
#define ID_ANI_LANCE_SHOOTING_LEFT 1022

#define ID_ANI_LANCE_DIVE 1023
#define ID_ANI_LANCE_SWIMMING_SHOOTING_LEFT 1026
#define ID_ANI_LANCE_SWIMMING_SHOOTING_RIGHT 1027
#define ID_ANI_LANCE_SWIMMING_LEFT 1025
#define ID_ANI_LANCE_SWIMMING_RIGHT 1024

#define ID_ANI_LANCE_FALLING_RIGHT 1028
#define ID_ANI_LANCE_FALLING_LEFT 1029

#define ID_ANI_LANCE_LIFE 3401
#pragma endregion

#define GROUND_Y 160.0f

#define LANCE_BIG_BBOX_WIDTH  16
#define LANCE_BIG_BBOX_HEIGHT 34

#define LANCE_BIG_LOOKUP_BBOX_HEIGHT 46
#define LANCE_BIG_LOOKUP_BBOX_WIDTH 14

#define LANCE_BIG_SITTING_BBOX_WIDTH  32
#define LANCE_BIG_SITTING_BBOX_HEIGHT 12

#define LANCE_BIG_SWIMMING_BBOX_WIDTH  16
#define LANCE_BIG_SWIMMING_BBOX_HEIGHT 16

#define LANCE_BIG_JUMPING_BBOX_WIDTH  16
#define LANCE_BIG_JUMPING_BBOX_HEIGHT 16

#define LANCE_SIT_HEIGHT_ADJUST ((LANCE_BIG_BBOX_HEIGHT-LANCE_BIG_SITTING_BBOX_HEIGHT)/2)

#define LANCE_SMALL_BBOX_WIDTH  12
#define LANCE_SMALL_BBOX_HEIGHT 12


#define LANCE_UNTOUCHABLE_TIME 5000

class CLance : public CGameObject
{
	BOOLEAN isSitting;
	BOOLEAN isJumping;
	BOOLEAN isLookingUp;
	BOOLEAN isPreDied;
	BOOLEAN isShooting;
	BOOLEAN isRunning;
	BOOLEAN isSwimming;
	BOOLEAN isClimb;
	BOOLEAN isOnDownBrick;


	CGameObject* preGun;

	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 
	float movingObjVx;

	int untouchable;

	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;
	ULONGLONG count_start;
	ULONGLONG shooting_start;

	float gunType;

	void OnCollisionWithSoldier(LPCOLLISIONEVENT e);
	void OnCollisionWithGunSoldier(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithGunType(LPCOLLISIONEVENT e);
	void OnCollisionWithEnemyGun(LPCOLLISIONEVENT e);
	void OnCollisionWithFire(LPCOLLISIONEVENT e);

	int GetAniId();
	void handleShooting();
public:
	CLance(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		isOnDownBrick = false;
		isJumping = false;
		isLookingUp = false;
		isPreDied = false;
		isShooting = false;
		isRunning = false;
		isSwimming = false;
		isClimb = false;
		isActive = true;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = -LANCE_GRAVITY;
		movingObjVx = 0;
		//ay =0;

		untouchable = 0;
		untouchable_start = -1;
		count_start = -1;
		shooting_start = -1;
		isOnPlatform = false;
		gunType =0;

		preGun = NULL;

	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	int IsCollidable()
	{
		//return (state != LANCE_STATE_DIE);
		return 1;
	}
	int IsBlocking() { return 0; }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); };
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void setPreGun(CGameObject* pre) { preGun = pre; };
	CGameObject* getPreGun() { return preGun; };

};
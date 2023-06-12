#pragma once
#include "GameObject.h"
#include "FallObject.h"

#define SOLDIER_GRAVITY 0.002f
#define SOLDIER_SPEED 0.05f
#define SOLDIER_DIE_DEFLECT 0.25f

#define SOLDIER_BBOX_WIDTH 16
#define SOLDIER_BBOX_HEIGHT 34

#define SOLDIER_DIE_TIMEOUT 300
#define SOLDIER_WAITING_SHOOTING_TIMEOUT 3000
#define SOLDIER_SHOOTING_TIMEOUT 1000

#define SOLDIER_STATE_WALKING 100
#define SOLDIER_STATE_DIE 200
#define SOLDIER_STATE_SHOOTING 300

#define ID_ANI_SOLDIER_WALKING_RIGHT 2001
#define ID_ANI_SOLDIER_WALKING_LEFT	2002

#define ID_ANI_SOLDIER_DIE_RIGHT	2003
#define ID_ANI_SOLDIER_DIE_LEFT		2004

#define ID_ANI_SOLDIER_SHOOTING_RIGHT	2005
#define ID_ANI_SOLDIER_SHOOTING_LEFT	2006

class CSoldier : public CGameObject
{
protected:
	float ay;
	bool isActive = false;
	bool isOnPlatform;
	
	bool isShooting; // flag if soldier can shoot or not
	bool isHaveFallObj; // flag if soldier can have fall obj or not


	ULONGLONG die_start;
	ULONGLONG count_start;


	CFallObject* fallObject;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CSoldier(float x, float y, float isShooting, float isHaveFallObj);
	void addFallObject(LPGAMEOBJECT obj) {

		if (dynamic_cast<CFallObject*>(obj))
		{
			CFallObject* i = dynamic_cast<CFallObject*>(obj);
			fallObject = i;
		}
		else fallObject = NULL;
	}
	void removeFallObject() { fallObject->Delete(); fallObject = NULL; }
	virtual void SetState(int state);
};
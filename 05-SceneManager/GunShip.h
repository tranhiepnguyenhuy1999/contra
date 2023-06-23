#pragma once
#include "GameObject.h"

#define GUNSHIP_GRAVITY 0.0002f
#define GUNSHIP_SPEED 0.15f
#define GUNSHIP_GRAVITY_MAX	0.125f 

#define GUNSHIP_BBOX_WIDTH 16
#define GUNSHIP_BBOX_HEIGHT 14

#define GUNSHIP_STATE_ACTIVE 100	
#define GUNSHIP_STATE_DIE 200

#define ID_ANI_GUNSHIP_STATIC	3104
class CGunShip : public CGameObject
{
protected:
	float ay,ax;
	float gunType;
	int dirct;
	bool isWorking;

	ULONGLONG count_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

public:
	bool IsWorking() { return isWorking; }
	CGunShip(float x, float y, float type, float ny=-1);
	virtual void SetState(int state);
};
#pragma once
#include "GameObject.h"

#define GUNBOX_BBOX_WIDTH 32
#define GUNBOX_BBOX_HEIGHT 32

#define GUNBOX_LOOP_TIMEOUT 450
#define GUNBOX_OPEN_TIMEOUT 1500
#define GUNBOX_CLOSE_TIMEOUT 1500

#define GUNBOX_STATE_UNACTIVE 100
#define GUNBOX_STATE_PRE_OPEN 200
#define GUNBOX_STATE_OPEN 300
#define GUNBOX_STATE_DIE 400
#define GUNBOX_STATE_CLOSE 500

#define ID_ANI_GUNBOX_UNACTIVE	3101
#define ID_ANI_GUNBOX_PRE_OPEN	3102
#define ID_ANI_GUNBOX_OPEN	3103

class CGunBox : public CGameObject
{

protected:
	
	bool isOpen;
	float typeGun;

	ULONGLONG loop_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);


	int getAniId();

public:
	CGunBox(float x, float y, float typeGun=1);
	virtual void SetState(int state);
};


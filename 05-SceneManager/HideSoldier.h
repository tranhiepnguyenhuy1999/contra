#pragma once
#include "GameObject.h"

#define HIDE_SOLDIER_BBOX_WIDTH 16
#define HIDE_SOLDIER_BBOX_HEIGHT 16

#define HIDE_SOLDIER_WAITING_SHOOTING_TIMEOUT 3000

#define HIDE_SOLDIER_STATE_UNACTIVE 100
#define HIDE_SOLDIER_STATE_DIE 200
#define HIDE_SOLDIER_STATE_SHOOTING 300
#define HIDE_SOLDIER_STATE_ACTIVE 400

#define ID_ANI_HIDE_SOLDIER_UNACTIVE_RIGHT 2203
#define ID_ANI_HIDE_SOLDIER_UNACTIVE_LEFT	2204

#define ID_ANI_HIDE_SOLDIER_ACTIVE_RIGHT	2201
#define ID_ANI_HIDE_SOLDIER_ACTIVE_LEFT		2202

class CHideSoldier : public CGameObject
{
protected:
	bool isWorking;
	float activeRange;
	ULONGLONG count_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
public:
	bool IsWorking() { return isWorking; }
	CHideSoldier(float x, float y);
	virtual void SetState(int state);
};
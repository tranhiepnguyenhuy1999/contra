#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define MARIO_WALKING_SPEED		0.05f
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_WALKING_UP		300
#define MARIO_STATE_WALKING_DOWN	400

#pragma region ANIMATION_ID

#define ID_ANI_MARIO_WALKING_IDLE_RIGHT 1100

class CMario2 : public CGameObject
{
public:
	CMario2(float x, float y) : CGameObject(x, y)
	{

	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};
#pragma once
#include "GameObject.h"

#define SOLDIER_INTRO_SPEED 0.05f

#define INTRO_STATE_LOADING 100
#define INTRO_STATE_DONE 200
#define INTRO_STATE_CHOSE 300

#define ID_ANI_INTRO	10000

class CIntroScene : public CGameObject
{
protected:

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

public:
	CIntroScene(float x, float y);
	virtual void SetState(int state);
};
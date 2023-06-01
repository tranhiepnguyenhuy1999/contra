#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "debug.h"

#define ENDWALL_WIDTH 16
#define ENDWALL_BBOX_WIDTH 16
#define ENDWALL_BBOX_HEIGHT 240

class CEndWall : public CGameObject {
	static CEndWall* __instance;
public:

	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Render();
	virtual void Update(DWORD dt) {};

	CEndWall(float x, float y) : CGameObject(x, y) {};
	CEndWall() : CGameObject(0, ENDWALL_BBOX_HEIGHT/2) {};
	static CEndWall* GetInstance();
};
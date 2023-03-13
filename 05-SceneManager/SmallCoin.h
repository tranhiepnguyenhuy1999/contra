#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "debug.h"

#define ID_ANI_SMALL_COIN 11001

#define COIN_SPEED 0.2f
#define COIN_MAX_Y 50
#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 9
#define COIN_BBOX_HEIGHT 14


class CSmallCoin : public CGameObject {
	float yLimit;
public:
	CSmallCoin(float x, float y) : CGameObject(x, y) {
		yLimit = y - COIN_MAX_Y;
		vy = -COIN_SPEED;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
		y += vy * dt;
		if (y < yLimit) {
			y = yLimit;
			vy = -vy;
		}
		if (y >= yLimit + COIN_MAX_Y) {
			isDeleted = true; return;
		}
		CGameObject::Update(dt, coObjects);
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};
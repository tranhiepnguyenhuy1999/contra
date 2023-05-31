#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define ID_ANI_LIFE 3401

#define LIFE_WIDTH 8
#define LIFE_HEIGHT 16
class CPlayerData {
	static CPlayerData* __instance;
	int life;
	float point;

public:
	CPlayerData() { life = 2; point = 0; };
	
	void updatePoint(float uPoint, int isPlus = 1) { point += isPlus*uPoint; };
	void updateLife(int uLife, int isPlus = 1) { life += isPlus * uLife; };
	
	void Render();
	
	static CPlayerData* GetInstance();
};
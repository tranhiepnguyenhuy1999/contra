#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "debug.h"

#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define DOWNBRICK_STATE_STATIC 100
#define DOWNBRICK_STATE_ONTOP 200

class CDownBrick : public CGameObject {
	float length;

	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Render();
	void Update(DWORD dt) {};

	public:
	CDownBrick(float x, float y, float length) : CGameObject(x, y) {
		this->length = length;
	}
};
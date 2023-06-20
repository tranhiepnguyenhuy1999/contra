#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "debug.h"

#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 10

class CDownBrick : public CGameObject {
	float length;

	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Render();
	void Update(DWORD dt) {};
	void RenderBoundingBox();
	public:
	CDownBrick(float x, float y, float length) : CGameObject(x, y) {
		this->length = length;
	}
};	
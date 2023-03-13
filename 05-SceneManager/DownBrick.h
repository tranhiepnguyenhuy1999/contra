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
	int length;

	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Render();
	void Update(DWORD dt) {};
	int IsBlocking() { return 0; };

	public:
	CDownBrick(float x, float y, int length) : CGameObject(x, y) {
		this->length = length;
	}
};
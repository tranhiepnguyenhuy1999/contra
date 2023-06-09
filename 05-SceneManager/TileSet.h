#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

class CTileSet : public CGameObject {
	float tileSetID;
public:
	CTileSet(float x, float y, float id) : CGameObject(x, y) {
		tileSetID = id;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};
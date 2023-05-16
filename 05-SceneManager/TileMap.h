#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

class CTileMap : public CGameObject {
	vector<vector<string>> tileMap;
public:
	CTileMap(float x, float y, vector<vector<string>> content) : CGameObject(x, y) {
		tileMap = content;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};
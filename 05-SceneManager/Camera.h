#pragma once

using namespace std;
#include <Windows.h>

#define MAX_MAP_WIDTH 2000
#define MAX_MAP_HEIGHT 2000

class Camera
{
	static Camera* __instance;

	float l, t, r, b;

	float cWidth;
	float cHeight;

public:
	Camera() {
		l = 0; t = 0; r = 0; b = 0;
		cWidth = 0;
		cHeight = 0;

	}
	void transformCoordinates( float &ix, float &iy);
	void setCamWidth(float width) { cWidth = width; };
	void setCamHeight(float height) { cHeight = height; };
	void getCamWidth(float &width) { width = cWidth; };
	void getCamHeight(float &height) { height = cHeight; };
	void getCamBoundingBox(float& cl, float& ct, float& cr, float& cb) { cl = l; ct = t; cr = r; cb = b; };
	void setCamPosition(float px, float py) {
		l = px - cWidth / 2;
		r = l + cWidth;
		//b = py;
		b = 0;
		t = b + cHeight;

		if (l < 0) { l = 0; r = l + cWidth; }
		else if (r > MAX_MAP_WIDTH) { r = MAX_MAP_WIDTH; l = r - cWidth; }
			
		if (b < 0) { b = 0; t = b + cHeight; }
		else if (t > MAX_MAP_HEIGHT) { t = MAX_MAP_HEIGHT; b = t - cHeight; }

	}
	void getCamPosition(float &cx, float &cy) {
		cx = l;
		cy = b;
	}
	static Camera* GetInstance();
	bool isCamContain(float objl, float objt, float objr, float objb) {
		return !( objr < l || objl > r || objt < b || objb > t);

	}
};


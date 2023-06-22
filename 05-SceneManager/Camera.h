#pragma once

using namespace std;
#include <Windows.h>
#include "EndWall.h"
#include "GunShip.h"

#define MAX_MAP_WIDTH 3328
#define MAX_MAP_HEIGHT 3328

class Camera
{
	static Camera* __instance;

	float l, t, r, b;

	int cWidth;
	int cHeight;
		
	float px; // player x
	float py; // player y

	int dirct;

public:
	Camera() {
		l = 0; t = 0; r = 0; b = 0;
		cWidth = 0;
		cHeight = 0;
		dirct = 0;
		px = 0; py = 0;
	}
	void transformCoordinates( float &ix, float &iy);
	void setCamWidth(int width) { cWidth = width; };
	void setCamDirection(int direction) { dirct = direction; };
	void getCamDirection(int& direction) { direction = dirct; };
	void setCamHeight(int height) { cHeight = height; t = b + cHeight; };
	void getCamWidth(int& width) { width = cWidth; r = l + cWidth; };
	void getCamHeight(int &height) { height = cHeight; };
	void getPlayerPosition(float& x, float& y) { x = px; y = py; };
	void getCamBoundingBox(float& cl, float& ct, float& cr, float& cb) { cl = l; ct = t; cr = r; cb = b; };
	void setCamPosition(float px, float py) {
		
		this->px = px;
		this->py = py;

		if (dirct == 0)
		{
			b = -8;
			t = b + cHeight;
			
			if (px < l + cWidth / 2) return;
			else {
				l = px - cWidth / 2;
				r = l + cWidth;
			}
		}
		else if (dirct == 1)
		{
			l = 0;
			r = l + cWidth;

			if (py < t - cHeight / 4) return;
			else {
				t = py + cHeight / 4;
				b = t - cHeight;
			}

		};

		if (l < 0) { l = 0; r = l + cWidth; }
		else if (r > MAX_MAP_WIDTH) { r = MAX_MAP_WIDTH; l = r - cWidth; }
			
		if (b < 0) { b = 0; t = b + cHeight; }
		else if (t > MAX_MAP_HEIGHT) { t = MAX_MAP_HEIGHT; b = t - cHeight; }

		CEndWall::GetInstance()->SetPosition(l + ENDWALL_BBOX_WIDTH / 2, (float)cHeight / 2);

	}
	void getCamPosition(float &cx, float &cy) {
		cx = l;
		cy = b;
	}
	static Camera* GetInstance();
	bool isCamContain(float objl, float objt, float objr, float objb) {
		return !( objr < l || objl > r || objt < b || objb > t);
	}
	void checkIsCameraOver(vector<LPGAMEOBJECT> &objects) {
		for (size_t i = 0; i < objects.size(); i++)
		{
			float objl, objt, objr, objb;
			objects[i]->GetBoundingBox(objl, objt, objr, objb);
			if (isCamContain(objl, objt, objr, objb))
			{
				objects[i]->setIsActiveTrue();
			}
			if (dynamic_cast<CGunShip*>(objects[i])) continue;
			else if (objr  < l || objt < b) {
				objects[i]->setIsCameraOverTrue();
			}
		}
	}
};


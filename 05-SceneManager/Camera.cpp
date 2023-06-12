#include "Camera.h"

Camera* Camera::__instance = NULL;

void Camera::transformCoordinates(float &x, float &y) {
	float cx, cy;

	cx = (FLOAT)floor(l);
	cy = (FLOAT)floor(b);
	x = (FLOAT)floor(x);
	y = (FLOAT)floor(y);

	x -= cx;
	y -= cy;
}
Camera* Camera::GetInstance()
{
	if (__instance == NULL) __instance = new Camera();
	return __instance;
}

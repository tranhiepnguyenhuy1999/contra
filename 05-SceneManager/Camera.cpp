#include "Camera.h"
#include <cstddef>
#include <corecrt_math.h>

Camera* Camera::__instance = NULL;

void Camera::transformCoordinates(float &ix, float &iy) {
	float cx, cy;

	cx = (FLOAT)floor(l);
	cy = (FLOAT)floor(b);
	ix = (FLOAT)floor(ix);
	iy = (FLOAT)floor(iy);

	ix = ix- cx;
	iy = iy-cy;
}
Camera* Camera::GetInstance()
{
	if (__instance == NULL) __instance = new Camera();
	return __instance;
}

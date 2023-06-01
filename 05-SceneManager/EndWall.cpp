#include "EndWall.h"

CEndWall* CEndWall::__instance = NULL;
void CEndWall::Render()
{
	RenderBoundingBox();
}
void CEndWall::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - ENDWALL_BBOX_WIDTH / 2;
	t = y + ENDWALL_BBOX_HEIGHT / 2;
	r = l + ENDWALL_BBOX_WIDTH;
	b = t - ENDWALL_BBOX_HEIGHT;
}
CEndWall* CEndWall::GetInstance()
{
	if (__instance == NULL) __instance = new CEndWall();
	return __instance;
}
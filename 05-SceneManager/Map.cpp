#include "Map.h"

void CMap::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_MAP)->Render(x, y);
	RenderBoundingBox();
}

void CMap::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}
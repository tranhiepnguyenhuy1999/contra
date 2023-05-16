#include "TileSet.h"

void CTileSet::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(tileSetID)->Render(x, y);
	//RenderBoundingBox();
}
void CTileSet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}
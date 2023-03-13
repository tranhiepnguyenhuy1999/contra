#include "Pile.h"

void CPile::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_PILE_HEAD_FIRST)->Render(x, y);
	animations->Get(ID_ANI_PILE_HEAD_LAST)->Render(x+16, y);
	for (int i = 0; i < height; i++) {
		animations->Get(ID_ANI_PILE_BODY_FIRST)->Render(x, y+16 + i*16);
		animations->Get(ID_ANI_PILE_BODY_LAST)->Render(x + 16, y+16+ i*16);
	}
	//RenderBoundingBox();
}

void CPile::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PILE_BBOX_WIDTH / 2;
	t = y - PILE_BBOX_HEIGHT / 2;
	r = l + PILE_BBOX_WIDTH*2;
	b = t + PILE_BBOX_HEIGHT*height + PILE_BBOX_HEIGHT;
}
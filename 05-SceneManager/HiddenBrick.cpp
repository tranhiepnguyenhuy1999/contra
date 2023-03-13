#include "Hiddenbrick.h"

void CHiddenBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(1100)->Render(x, y);
	RenderBoundingBox();
}

void CHiddenBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}
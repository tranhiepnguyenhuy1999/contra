#include "DownBrick.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"

#include "Camera.h"

void CDownBrick::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)t - (int)b;

	float cx, cy;
	Camera::GetInstance()->getCamPosition(cx, cy);

	float xx = x - BRICK_BBOX_WIDTH / 2 + rect.right / 2;

	CGame::GetInstance()->Draw(xx - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CDownBrick::Render()
{
	RenderBoundingBox();
}

void CDownBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y + BRICK_BBOX_HEIGHT / 2;
	r = l + length * BRICK_BBOX_WIDTH;
	b = t - BRICK_BBOX_HEIGHT;
}

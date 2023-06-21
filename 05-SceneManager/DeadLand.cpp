#include "DeadLand.h"
#include "Camera.h"

#include "Sprite.h"
#include "Sprites.h"
#include "Textures.h"

void CDeadLand::RenderBoundingBox()
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

	float xx = x + rect.right / 2;

	CGame::GetInstance()->Draw(xx - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CDeadLand::Render()
{
	RenderBoundingBox();
}

void CDeadLand::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cellWidth / 2;
	l = x;
	t = y + this->cellHeight / 2;
	r = l + this->cellWidth * this->length;
	b = t - this->cellHeight;
}
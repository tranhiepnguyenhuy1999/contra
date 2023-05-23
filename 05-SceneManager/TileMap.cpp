#include "TileMap.h"
#include "debug.h"
#include "Camera.h"

void CTileMap::Render()
{
	float cx, cy;
	Camera* cam = Camera::GetInstance();
	cam->getCamPosition(cx, cy);

	CAnimations* animations = CAnimations::GetInstance();
	
	int lengthY = tileMap.size();

	for (int i = lengthY; i > 0; i--)
	{
		for (float j = 0; j < tileMap[i-1].size(); j++)
		{
			float l, t, r, b;
			l = j * 16;
			t = (lengthY - i) * 16;
			r = l + 16;
			b = t - 16;

			if (cam->isCamContain(l,t, r, b))
			{
				animations->Get(stoi(tileMap[i-1][j]))->Render(l, t);
			}
		}
	}

}
void CTileMap::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	//
}
#include "TileMap.h"
#include "debug.h" 

void CTileMap::Render()
{
		float cx, cy;
	CGame* game = CGame::GetInstance();
	game->GetCamPos(cx, cy);
	CAnimations* animations = CAnimations::GetInstance();
	
	int lengthY = tileMap.size();

	for (int i = lengthY; i > 0; i--)
	{
		for (float j = 0; j < tileMap[i-1].size(); j++)
		{
			float l, t;
			l = j * 16;
			t = (lengthY - i) * 16;
			if (l >= cx - 16 
				&& l <= cx + game->GetBackBufferWidth() + 16 
				&& t >= cy - 16 
				&& t <= cy + game->GetBackBufferHeight())
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
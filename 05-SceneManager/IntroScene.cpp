#include "IntroScene.h"
#include "debug.h"
#include "AssetIDs.h"
CIntroScene::CIntroScene(float x, float y) :CGameObject(x, y)
{
	SetState(INTRO_STATE_LOADING);
}

void CIntroScene::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

}
void CIntroScene::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	if (!isActive)
	{
		if (x < cx + 1.5 * CGame::GetInstance()->GetBackBufferWidth()) {
			isActive = true;
		}
		return;
	}

	vy += ay * dt;
	vx += ax * dt;

	if (state == INTRO_STATE_CHOSE)
	{

		return;
	}

	CGameObject::Update(dt, coObjects);
}


void CIntroScene::Render()
{
	int aniId = ID_ANI_INTRO;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CIntroScene::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case INTRO_STATE_LOADING:
		vx = -SOLDIER_INTRO_SPEED;
		break;
	case INTRO_STATE_DONE:
		vx = 0;
		break;
	case INTRO_STATE_CHOSE:
		break;
	}
}

#include "IntroScene.h"
#include "debug.h"
#include "AssetIDs.h"
CIntroScene::CIntroScene(float x, float y) :CGameObject(x, y)
{
	vx = 0.05f;
	isActive = true;
	SetState(INTRO_STATE_LOADING);
}

void CIntroScene::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

}
void CIntroScene::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	x += vx * dt;
	
	if (x < 128)
	{
		x = 128.0f;
		vx = 0;
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

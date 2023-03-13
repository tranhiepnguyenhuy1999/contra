#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "Mario2.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer(); 
	int level;
	mario->getLevel(level);
	float x, y;
	switch (KeyCode)
	{
	case DIK_L:
		CGame::GetInstance()->InitiateSwitchScene(5);
		break;
	case DIK_DOWN:

		mario->GetPosition(x, y);
		if (x >=2256 && x <= 2288) {
			mario->SetPosition(2114, 544);
		}
		mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_UP:
		mario->GetPosition(x, y);
		if (x >= 2320 && x <= 2352) {
			mario->SetPosition(2328, 336);
		}
		break;
	case DIK_A:
		mario->SetState(MARIO_STATE_ATTACK);
		break;
	case DIK_S:
		if (mario->isFlying() && level==3)
		{
			mario->SetState(MARIO_STATE_FLY);
		}
		else
		{
			mario->SetState(MARIO_STATE_JUMP);
		}
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_RACOON);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_R: // reset
		//Reload();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int level;
	mario->getLevel(level);
	switch (KeyCode)
	{
	case DIK_S:
		if (mario->isFlying() && level == 3)
		{
			mario->SetState(MARIO_STATE_RELEASE_FLY);
		}
		else
		{
			mario->SetState(MARIO_STATE_RELEASE_JUMP);
		}
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A))
		{
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		}
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	//else if (game->IsKeyDown(DIK_UP))
	//{
	//	mario->SetState(MARIO_STATE_WALKING_UP);
	//}
	//else if (game->IsKeyDown(DIK_DOWN)) {
	//	mario->SetState(MARIO_STATE_WALKING_DOWN);
	//}
	else mario->SetState(MARIO_STATE_IDLE);

}
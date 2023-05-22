#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Lance.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CLance* mario = (CLance *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer(); 
	int level;
	mario->getLevel(level);
	switch (KeyCode)
	{
	case DIK_L:
		CGame::GetInstance()->InitiateSwitchScene(5);
		break;
	case DIK_DOWN:
		mario->SetState(LANCE_STATE_SIT);
		break;
	case DIK_UP:
		mario->SetState(LANCE_STATE_LOOKUP);
		break;
	case DIK_Z:
		mario->SetState(LANCE_STATE_SHOOTING);
		break;
	case DIK_X:
			mario->SetState(LANCE_STATE_JUMP);
		break;
	case DIK_0:
		mario->SetState(LANCE_STATE_DIE);
		break;
	case DIK_R: // reset
		//Reload();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CLance* mario = (CLance*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int level;
	mario->getLevel(level);
	switch (KeyCode)
	{
	case DIK_X:
			mario->SetState(LANCE_STATE_RELEASE_JUMP);
		break;
	case DIK_DOWN:
		mario->SetState(LANCE_STATE_SIT_RELEASE);
		break;
	case DIK_UP:
		mario->SetState(LANCE_STATE_LOOKUP_RELEASE);
		break;
	case DIK_RIGHT:
	case DIK_LEFT:
		mario->SetState(LANCE_STATE_MOVING_RELEASE);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CLance* mario = (CLance*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_DOWN))
		{
			mario->SetState(LANCE_STATE_RUNNING_DOWN_RIGHT);
		}
		else if (game->IsKeyDown(DIK_UP))
		{
			mario->SetState(LANCE_STATE_RUNNING_UP_RIGHT);
		}
		else
			mario->SetState(LANCE_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_DOWN))
		{
			mario->SetState(LANCE_STATE_RUNNING_DOWN_LEFT);
		}
		else if (game->IsKeyDown(DIK_UP))
		{
			mario->SetState(LANCE_STATE_RUNNING_UP_LEFT);
		}
		else
			mario->SetState(LANCE_STATE_WALKING_LEFT);
	}
	else mario->SetState(LANCE_STATE_IDLE);

}
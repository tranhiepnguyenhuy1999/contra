#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Lance.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CLance* lance = (CLance *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer(); 
	int level;
	lance->getLevel(level);
	switch (KeyCode)
	{
	case DIK_L:
		CGame::GetInstance()->InitiateSwitchScene(5);
		break;
	case DIK_DOWN:
		lance->SetState(LANCE_STATE_SIT);
		break;
	case DIK_UP:
		lance->SetState(LANCE_STATE_LOOKUP);
		break;
	case DIK_Z:
		lance->SetState(LANCE_STATE_SHOOTING);
		break;
	case DIK_X:
			lance->SetState(LANCE_STATE_JUMP);
		break;
	case DIK_0:
		lance->SetState(LANCE_STATE_DIE);
		break;
	case DIK_R: // reset
		//Reload();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CLance* lance = (CLance*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int level;
	lance->getLevel(level);
	switch (KeyCode)
	{
	case DIK_DOWN:
		lance->SetState(LANCE_STATE_SIT_RELEASE);
		break;
	case DIK_UP:
		lance->SetState(LANCE_STATE_LOOKUP_RELEASE);
		break;
	case DIK_RIGHT:
	case DIK_LEFT:
		lance->SetState(LANCE_STATE_MOVING_RELEASE);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CLance* lance = (CLance*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_DOWN))
		{
			lance->SetState(LANCE_STATE_RUNNING_DOWN_RIGHT);
		}
		else if (game->IsKeyDown(DIK_UP))
		{
			lance->SetState(LANCE_STATE_RUNNING_UP_RIGHT);
		}
		else
			lance->SetState(LANCE_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_DOWN))
		{
			lance->SetState(LANCE_STATE_RUNNING_DOWN_LEFT);
		}
		else if (game->IsKeyDown(DIK_UP))
		{
			lance->SetState(LANCE_STATE_RUNNING_UP_LEFT);
		}
		else
			lance->SetState(LANCE_STATE_WALKING_LEFT);
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (game->IsKeyDown(DIK_X))
		{
			lance->SetState(LANCE_STATE_GO_DOWN);
		}
		else lance->SetState(LANCE_STATE_IDLE);
	}
	else lance->SetState(LANCE_STATE_IDLE);

}
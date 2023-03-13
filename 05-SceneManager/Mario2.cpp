#include <algorithm>
#include "debug.h"

#include "Mario2.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Mushroom.h"
#include "Portal.h"
#include "QuestionBrick.h"
#include "DownBrick.h"
#include "KoopaTroopa.h"
#include "ParaKoopaTroopa.h"
#include "Flower.h"
#include "Fire.h"
#include "ParaGoomba.h"
#include "Leaf.h"
#include "Tail.h"

#include "AssetIDs.h"
#include "Collision.h"
#include "UserBoard.h"

void CMario2::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario2::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario2::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CMario2::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = ID_ANI_MARIO_WALKING_IDLE_RIGHT;
	animations->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}
void CMario2::SetState(int state)
{

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		vx= MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		vx = -MARIO_WALKING_SPEED;
		break;
	case MARIO_STATE_WALKING_UP:
		vy = -MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_DOWN:
		vy = MARIO_WALKING_SPEED;
		break;
	case MARIO_STATE_IDLE:
		vx = 0.0f;
		vy = 0.0f;
		break;
	default:
		break;
	}

	CGameObject::SetState(state);
}

void CMario2::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

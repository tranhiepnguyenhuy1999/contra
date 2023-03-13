#include "ParaGoomba.h"
#include "QuestionBrick.h"
#include "AssetIDs.h"

CParaGoomba::CParaGoomba(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	ready_jump_start = -1;
	//this->ay = PARAGOOMBA_GRAVITY;
	count_start = -1;
	SetState(PARAGOOMBA_STATE_FLYING);
}

void CParaGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == PARAGOOMBA_STATE_DIE)
	{
		left = x - PARAGOOMBA_BBOX_WIDTH / 2;
		top = y - (PARAGOOMBA_BBOX_HEIGHT_DIE / 2);
		right = left + PARAGOOMBA_BBOX_WIDTH;
		bottom = top + PARAGOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - PARAGOOMBA_BBOX_WIDTH / 2;
		top = y - (PARAGOOMBA_BBOX_HEIGHT / 2);
		right = left + PARAGOOMBA_BBOX_WIDTH;
		bottom = top + PARAGOOMBA_BBOX_HEIGHT;
	}
}

void CParaGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CParaGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CParaGoomba*>(e->obj)) return;

	if (e->ny != 0)
	{
		if (state != PARAGOOMBA_STATE_WALKING) {
			if (e->ny < 0)
			{
				if (ready_jump_start == -1)
				{
					ready_jump_start = GetTickCount64();
					return;
				}
				if (GetTickCount64() - ready_jump_start > PARAGOOMBA_JUMP_TIMEOUT)
				{
					SetState(PARAGOOMBA_STATE_JUMP);
					ready_jump_start = -1;
				}
			}
		}
		else vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CParaGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
	//DebugOut(L"count %d", GetTickCount64() - count_start);
	if ((state == PARAGOOMBA_STATE_DIE) && (GetTickCount64() - count_start > PARAGOOMBA_DIE_TIMEOUT))
	{
		this->isDeleted = true;
		return;
	}
	if ((state == PARAGOOMBA_STATE_JUMP) && (GetTickCount64() - count_start > PARAGOOMBA_JUMP_TIMEOUT))
	{
		SetState(PARAGOOMBA_STATE_RELEASE_JUMP);
		count_start = -1;
		return;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CParaGoomba::Render()
{
	int aniId = ID_ANI_PARAGOOMBA_WALKING;
	if (state == PARAGOOMBA_STATE_DIE)
	{
		aniId = ID_ANI_PARAGOOMBA_DIE;
	}
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	if (state != PARAGOOMBA_STATE_WALKING && state != PARAGOOMBA_STATE_DIE)
	{
		CAnimations::GetInstance()->Get(ID_ANI_PARAGOOMBA_WINNGS_RIGHT)->Render(x + PARAGOOMBA_BBOX_WIDTH/2, y-PARAGOOMBA_BBOX_HEIGHT/2);
		CAnimations::GetInstance()->Get(ID_ANI_PARAGOOMBA_WINNGS_LEFT)->Render(x - PARAGOOMBA_BBOX_WIDTH / 2, y - PARAGOOMBA_BBOX_HEIGHT / 2);
	}
	RenderBoundingBox();
}

void CParaGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	
	case PARAGOOMBA_STATE_DIE:
		count_start = GetTickCount64();
		y += (PARAGOOMBA_BBOX_HEIGHT - PARAGOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case PARAGOOMBA_STATE_JUMP:
		count_start = GetTickCount64();
		vy = -PARAGOOMBA_JUMP_SPEED;
		break;
	case PARAGOOMBA_STATE_RELEASE_JUMP:
		vy = PARAGOOMBA_JUMP_SPEED;
		break;
	case PARAGOOMBA_STATE_WALKING:
		vy = PARAGOOMBA_JUMP_SPEED;
		vx = -PARAGOOMBA_WALKING_SPEED;
		break;
	case PARAGOOMBA_STATE_FLYING:
		vy = PARAGOOMBA_JUMP_SPEED;
		vx = -PARAGOOMBA_WALKING_SPEED;
		break;
	}
}

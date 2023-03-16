#include "KoopaTroopa.h"
#include "DownBrick.h"
#include "QuestionBrick.h"
#include "Goomba.h"
#include "Mario.h"

CKoopaTroopa::CKoopaTroopa(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPATROOPA_GRAVITY;
	ready_jump_start = -1;
	count_start = -1;
	vx = -KOOPATROOPA_WALKING_SPEED;
	SetState(KOOPATROOPA_STATE_WALKING);
	createFallObject();
}

void CKoopaTroopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != KOOPATROOPA_STATE_WALKING && state != KOOPATROOPA_STATE_TURNBACK)
	{
		left = x - KOOPATROOPA_BBOX_WIDTH / 2;
		top = y - (KOOPATROOPA_BBOX_HEIGHT_DIE / 2);	
		right = left + KOOPATROOPA_BBOX_WIDTH;
		bottom = top + KOOPATROOPA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - KOOPATROOPA_BBOX_WIDTH / 2;
		top = y - (KOOPATROOPA_BBOX_HEIGHT/2);
		right = left + KOOPATROOPA_BBOX_WIDTH;
		bottom = top + KOOPATROOPA_BBOX_HEIGHT ;
	}
}

void CKoopaTroopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopaTroopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CKoopaTroopa*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0 && !dynamic_cast<CDownBrick*>(e->obj))
	{
		vx = -vx;
	}
	
	if (dynamic_cast<CGoomba*>(e->obj))OnCollisionWithGoomba(e);
}
void CKoopaTroopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	if (goomba->GetState() != GOOMBA_STATE_DIE)
	{
		goomba->SetState(GOOMBA_STATE_DIE);
	}
}
void CKoopaTroopa::OnCollisionWithDownBrick(LPCOLLISIONEVENT e)
{
	if (e->ny < 0)
		vy = 0;
}
void CKoopaTroopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);
	if (!isActive)
	{
		if (x < cx + CGame::GetInstance()->GetBackBufferWidth()) {
			isActive = true;
		}
		return;
	}
	vy += ay * dt;
	vx += ax * dt;
	
	//DebugOut(L"count %d", GetTickCount64() - count_start);
	if ((state == KOOPATROOPA_STATE_DIE) && (GetTickCount64() - count_start > KOOPATROOPA_DIE_TIMEOUT))
	{
		SetState(KOOPATROOPA_STATE_ALIVE);
		return;
	}
	if ((state == KOOPATROOPA_STATE_TURNBACK) && (GetTickCount64() - count_start > 100))
	{
		createFallObject();
		count_start = -1;
		SetState(KOOPATROOPA_STATE_WALKING);

		return;
	}
	if ((state == KOOPATROOPA_STATE_ALIVE) && (GetTickCount64() - count_start > KOOPATROOPA_ALIVE_TIMEOUT))
	{
		SetState(KOOPATROOPA_STATE_WALKING);
		count_start = -1;
		vx = -KOOPATROOPA_WALKING_SPEED;
		ay = KOOPATROOPA_GRAVITY;
		createFallObject();
		y = y + (KOOPATROOPA_BBOX_HEIGHT_DIE/2) - KOOPATROOPA_BBOX_HEIGHT/2 ;
		return;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CKoopaTroopa::Render()
{
	int aniId = ID_ANI_KOOPATROOPA_WALKING_LEFT;
	if (vx > 0) {
		aniId = ID_ANI_KOOPATROOPA_WALKING_RIGHT;
	}
	if (state == KOOPATROOPA_STATE_DIE)
	{
		aniId = ID_ANI_KOOPATROOPA_DIE;
	}
	if (state == KOOPATROOPA_STATE_ALIVE)
	{
		aniId = ID_ANI_KOOPATROOPA_ALIVE;
	}
	if (state == KOOPATROOPA_STATE_KICKING_LEFT || state == KOOPATROOPA_STATE_KICKING_RIGHT)
	{
		aniId = ID_ANI_KOOPATROOPA_KICKING;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CKoopaTroopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPATROOPA_STATE_DIE:
		count_start = GetTickCount64();
		y += (KOOPATROOPA_BBOX_HEIGHT - KOOPATROOPA_BBOX_HEIGHT_DIE) / 2;
		if (fallObj)
		{
			fallObj->SetState(FALLOBJECT_STATE_DIE);
		}
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case KOOPATROOPA_STATE_ALIVE:
		count_start = GetTickCount64();
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case KOOPATROOPA_STATE_WALKING:
		break;
	case KOOPATROOPA_STATE_KICKING_LEFT:
		ay = KOOPATROOPA_GRAVITY;
		vx = KOOPATROOPA_KICKING_SPEED;
		break;
	case KOOPATROOPA_STATE_KICKING_RIGHT:
		ay = KOOPATROOPA_GRAVITY;
		vx = -KOOPATROOPA_KICKING_SPEED;
		break;
	case KOOPATROOPA_STATE_TURNBACK:
		count_start = GetTickCount64();
		vx = -vx;
		break;
	}
}

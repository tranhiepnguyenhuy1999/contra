#include "ParaKoopaTroopa.h"
#include "QuestionBrick.h"
#include "Goomba.h"
#include "AssetIDs.h"

CParaKoopaTroopa::CParaKoopaTroopa(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0.0005f;
	ready_jump_start = -1;
	count_start = -1;
	vy = PARAKOOPATROOPA_JUMP_SPEED;
	vx = -PARAKOOPATROOPA_WALKING_SPEED;
	SetState(PARAKOOPATROOPA_STATE_JUMP);
}

void CParaKoopaTroopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == PARAKOOPATROOPA_STATE_DIE || state == PARAKOOPATROOPA_STATE_ALIVE)
	{
		left = x - PARAKOOPATROOPA_BBOX_WIDTH / 2;
		top = y - (PARAKOOPATROOPA_BBOX_HEIGHT_DIE / 2);
		right = left + PARAKOOPATROOPA_BBOX_WIDTH;
		bottom = top + PARAKOOPATROOPA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - PARAKOOPATROOPA_BBOX_WIDTH / 2;
		top = y - (PARAKOOPATROOPA_BBOX_HEIGHT / 2);
		right = left + PARAKOOPATROOPA_BBOX_WIDTH;
		bottom = top + PARAKOOPATROOPA_BBOX_HEIGHT;
	}
}

void CParaKoopaTroopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CParaKoopaTroopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CParaKoopaTroopa*>(e->obj)) return;

	if (e->ny != 0)
	{
		if (state == PARAKOOPATROOPA_STATE_JUMP) {
			if (e->ny < 0)
			{
				if (ready_jump_start == -1)
				{
					ready_jump_start = GetTickCount64();
					return;
				}
				if (GetTickCount64() - ready_jump_start > 300)
				{
					SetState(PARAKOOPATROOPA_STATE_JUMP);
					ready_jump_start = -1;
				}
			}
		}
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
	if (dynamic_cast<CQuestionBrick*>(e->obj))OnCollisionWithQuestionBrick(e);
	else if (dynamic_cast<CGoomba*>(e->obj))OnCollisionWithGoomba(e);
}
void CParaKoopaTroopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	if (goomba->GetState() != GOOMBA_STATE_DIE)
	{
		goomba->SetState(GOOMBA_STATE_DIE);
	}
}
void CParaKoopaTroopa::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e)
{
	CQuestionBrick* questionBrick = dynamic_cast<CQuestionBrick*>(e->obj);
	if (questionBrick->GetState() == QUESTIONBRICK_STATE_UNTOUCHED)
	{
		float qx, qy, qvx;
		questionBrick->GetPosition(qx, qy);

		if (questionBrick->getType() == 1)
		{
			CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_SMALLCOIN, qx, qy - 16);
		}
		else
		{
			// check direction
			if (this->x <= qx + 8) {
				qvx = -1;
			}
			else
			{
				qvx = 1;
			}
			CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_MUSHROOM, qx, qy, qvx);
		}
		questionBrick->SetState(QUESTIONBRICK_STATE_TOUCHED_1);
	}

}
void CParaKoopaTroopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

	if ((state == PARAKOOPATROOPA_STATE_DIE) && (GetTickCount64() - count_start > PARAKOOPATROOPA_DIE_TIMEOUT))
	{
		SetState(PARAKOOPATROOPA_STATE_ALIVE);
		return;
	}
	if ((state == PARAKOOPATROOPA_STATE_ALIVE) && (GetTickCount64() - count_start > PARAKOOPATROOPA_ALIVE_TIMEOUT))
	{
		SetState(PARAKOOPATROOPA_STATE_WALKING);
		count_start = -1;
		y = y + (PARAKOOPATROOPA_BBOX_HEIGHT_DIE / 2) - PARAKOOPATROOPA_BBOX_HEIGHT / 2;
		return;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CParaKoopaTroopa::Render()
{
	int aniId = ID_ANI_PARAKOOPATROOPA_WALKING;
	if (state == PARAKOOPATROOPA_STATE_DIE)
	{
		aniId = ID_ANI_PARAKOOPATROOPA_DIE;
	}
	if (state == PARAKOOPATROOPA_STATE_ALIVE)
	{
		aniId = ID_ANI_PARAKOOPATROOPA_ALIVE;
	}
	if (state == PARAKOOPATROOPA_STATE_KICKING)
	{
		aniId = ID_ANI_PARAKOOPATROOPA_KICKING;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	if(state== PARAKOOPATROOPA_STATE_JUMP ) CAnimations::GetInstance()->Get(6002)->Render(x + 6 , y-4);
	RenderBoundingBox();
}

void CParaKoopaTroopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PARAKOOPATROOPA_STATE_DIE:
		count_start = GetTickCount64();
		y += (PARAKOOPATROOPA_BBOX_HEIGHT - PARAKOOPATROOPA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		break;
	case PARAKOOPATROOPA_STATE_ALIVE:
		count_start = GetTickCount64();
		vx = 0;
		vy = 0;
		break;
	case PARAKOOPATROOPA_STATE_JUMP:
		count_start = GetTickCount64();
		vy = -PARAKOOPATROOPA_JUMP_SPEED;
		break;
	case PARAKOOPATROOPA_STATE_RELEASE_JUMP:
		vy = PARAKOOPATROOPA_JUMP_SPEED;
		break;
	case PARAKOOPATROOPA_STATE_WALKING:
		vx = -PARAKOOPATROOPA_WALKING_SPEED;
		break;
	case PARAKOOPATROOPA_STATE_KICKING:
		vx = PARAKOOPATROOPA_KICKING_SPEED;
		break;
	}
}

#include "BossStage1.h"
#include "AssetIDs.h"
#include "debug.h"

CBossStage1::CBossStage1(float x, float y) :CGameObject(x, y)
{
	life = 1;
}

void CBossStage1::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - BOSS_STAGE_1_BBOX_WIDTH / 2;
	top = y + (BOSS_STAGE_1_BBOX_HEIGHT / 2);
	right = left + BOSS_STAGE_1_BBOX_WIDTH;
	bottom = top - BOSS_STAGE_1_BBOX_HEIGHT;
}

void CBossStage1::OnNoCollision(DWORD dt)
{
	y += vy * dt;
};

void CBossStage1::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CBossStage1::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (life <= 0)
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 2);
		IsDeleteChildren();
		isDeleted = true;
		return;
	}
	for (UINT i = 0; i < children.size(); i++) {
		if (children[i] != NULL && children[i]->isDie()) {
			children[i]->IsDeleted();
		}
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CBossStage1::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_BOSS_STAGE_1)->Render(x, y);
	RenderBoundingBox();
}
void CBossStage1::SetState(int state)
{
	switch (state)
	{
	case BOSS_STAGE_1_STATE_DMG:
		life--;
	default:
		break;
	}
	CGameObject::SetState(state);
}

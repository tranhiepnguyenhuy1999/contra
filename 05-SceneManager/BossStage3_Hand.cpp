#include "BossStage3_Hand.h"
#include "BossStage3_HandPiece.h"
#include "AssetIDs.h"
#include "EnemyGun.h"

CBossStage3_Hand::CBossStage3_Hand(float x, float y):CGameObject(x, y)
{
	loop_start = -1;
}

void CBossStage3_Hand::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == BOSS_STAGE_3_HAND_STATE_DIE)
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 0, 0, 0, 0, 1);
		isDeleted = true;
		return;
	}
	if (state == BOSS_STAGE_3_HAND_STATE_ACTIVE) {
		SetState(BOSS_STAGE_3_HAND_STATE_1);
	}
	else if (state == BOSS_STAGE_3_HAND_STATE_1) {
		if(lastChild->IsFinishState1()) SetState(BOSS_STAGE_3_HAND_STATE_2);
	}
	else if (state == BOSS_STAGE_3_HAND_STATE_2) {
		if(lastChild->IsFinishState2())	SetState(BOSS_STAGE_3_HAND_STATE_3);
	}
	else if (state == BOSS_STAGE_3_HAND_STATE_3) {
		if (lastChild->IsFinishState3())SetState(BOSS_STAGE_3_HAND_STATE_4);
	}
	else if (state == BOSS_STAGE_3_HAND_STATE_4) {
		if (lastChild->IsFinishState4())SetState(BOSS_STAGE_3_HAND_STATE_5);

	}
	CGameObject::Update(dt, coObjects);
	//CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBossStage3_Hand::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

void CBossStage3_Hand::Render()
{
}

void CBossStage3_Hand::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	//case BOSS_STAGE_3_HAND_STATE_DMG:
	//	life--;
	//	break;
	case BOSS_STAGE_3_HAND_STATE_ACTIVE:
		break;
	case BOSS_STAGE_3_HAND_STATE_1:
		changeStateChildren(BOSS_STAGE_3_HANDPIECE_STATE_MOVE_1);
		break;
	case BOSS_STAGE_3_HAND_STATE_2:
		changeStateChildren(BOSS_STAGE_3_HANDPIECE_STATE_MOVE_2);
		break;
	case BOSS_STAGE_3_HAND_STATE_3:
		changeStateChildren(BOSS_STAGE_3_HANDPIECE_STATE_MOVE_3);
		break;
	case BOSS_STAGE_3_HAND_STATE_4:
		changeStateChildren(BOSS_STAGE_3_HANDPIECE_STATE_MOVE_4);
		break;
	case BOSS_STAGE_3_HAND_STATE_5:
		changeStateChildren(BOSS_STAGE_3_HANDPIECE_STATE_MOVE_5);
		break;
	case BOSS_STAGE_3_HAND_STATE_DIE:
		loop_start = -1;
		break;
	}
}
void CBossStage3_Hand::changeStateChildren(int s)
{
	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->SetState(s);
	}
}

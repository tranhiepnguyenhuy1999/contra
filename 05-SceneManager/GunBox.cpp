#include "GunBox.h"
#include "AssetIDs.h"

CGunBox::CGunBox(float x, float y, float typeGun) :CGameObject(x, y)
{
	SetState(GUNBOX_STATE_UNACTIVE);
	isOpen = false;
	this->typeGun = (int)typeGun;
	loop_start = -1;
}

void CGunBox::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
		left = x - GUNBOX_BBOX_WIDTH / 2;
		top = y + (GUNBOX_BBOX_HEIGHT / 2);
		right = left + GUNBOX_BBOX_WIDTH;
		bottom = top - GUNBOX_BBOX_HEIGHT;
}

void CGunBox::OnNoCollision(DWORD dt)
{
	y += vy * dt;
};

void CGunBox::OnCollisionWith(LPCOLLISIONEVENT e)
{

}

void CGunBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//DebugOut(L">>> Count time >>> %d \n", GetTickCount64() - loop_start);
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	if (state == GUNBOX_STATE_CLOSE) return;

	if (cx>x)
	{
		SetState(GUNBOX_STATE_CLOSE);
	}

	if (state == GUNBOX_STATE_DIE)
	{
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 1);
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_GUNTYPE, x, y, 1, 1, 0, 0, typeGun);
		isDeleted = true;
		return;
	}
	if ((state == GUNBOX_STATE_UNACTIVE) && (GetTickCount64() - loop_start > GUNBOX_CLOSE_TIMEOUT))
	{
		return	SetState(GUNBOX_STATE_PRE_OPEN);
	}
	if ((state == GUNBOX_STATE_PRE_OPEN) && (GetTickCount64() - loop_start > GUNBOX_LOOP_TIMEOUT))
	{
		if(isOpen)
			//losing
			return SetState(GUNBOX_STATE_UNACTIVE);
		else
			//opening
			return	SetState(GUNBOX_STATE_OPEN);
	}
	if ((state == GUNBOX_STATE_OPEN) && (GetTickCount64() - loop_start > GUNBOX_OPEN_TIMEOUT))
	{
		return	SetState(GUNBOX_STATE_PRE_OPEN);
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
int CGunBox::getAniId() {

	if (this->state == GUNBOX_STATE_UNACTIVE || this->state== GUNBOX_STATE_CLOSE) return ID_ANI_GUNBOX_UNACTIVE;
	else if (this->state == GUNBOX_STATE_PRE_OPEN) return ID_ANI_GUNBOX_PRE_OPEN;
	else if (this->state == GUNBOX_STATE_OPEN) return	ID_ANI_GUNBOX_OPEN;
	else return -1;

};

void CGunBox::Render()
{
	int aniId = getAniId();
	if (aniId != -1) CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}
void CGunBox::SetState(int state)
{
	CGameObject::SetState(state);

	loop_start = GetTickCount64();

	switch (state)
	{
	case GUNBOX_STATE_UNACTIVE:
		isOpen = false;
		break;
	case GUNBOX_STATE_PRE_OPEN:
		break;
	case GUNBOX_STATE_OPEN:
		isOpen = true;
		break;
	case GUNBOX_STATE_DIE:
		break;
	case GUNBOX_STATE_CLOSE:
		isOpen = false;
		break;
	}
}

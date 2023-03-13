#include "FallObject.h"
#include "AssetIDs.h"
#include "KoopaTroopa.h"

CFallObject::CFallObject(float x, float y, LPGAMEOBJECT obj):CGameObject(x, y)
{

	this->ax = 0;
	this->ay = FALLOBJECT_GRAVITY;
	yLimit = y + FALLOBJECT_BBOX_HEIGHT*2;
	count_start = -1;
	this->obj = obj;
	if (dynamic_cast<CKoopaTroopa*>(obj)) {
		CKoopaTroopa* objSrc = dynamic_cast<CKoopaTroopa*>(obj);
		objSrc->addFallObject(this);
	}
	SetState(FALLOBJECT_STATE_WALKING);
}

void CFallObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

		left = x - FALLOBJECT_BBOX_WIDTH / 2;
		top = y - (FALLOBJECT_BBOX_HEIGHT / 2);
		right = left + FALLOBJECT_BBOX_WIDTH;
		bottom = top + FALLOBJECT_BBOX_HEIGHT;
}

void CFallObject::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CFallObject::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CFallObject*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}
void CFallObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == FALLOBJECT_STATE_DIE))
	{
		isDeleted = true;
		return;
	}
	//DebugOut(L"count %d", GetTickCount64() - count_start);
	if (y >= yLimit) {
		obj->SetState(KOOPATROOPA_STATE_TURNBACK);
		SetState(FALLOBJECT_STATE_DIE);
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CFallObject::Render()
{
	RenderBoundingBox();
}

void CFallObject::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FALLOBJECT_STATE_DIE:
		count_start = GetTickCount64();
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case FALLOBJECT_STATE_WALKING:
		float ovx, ovy;
		obj->GetSpeed(ovx, ovy);
		vx = ovx;
		break;
	}
}

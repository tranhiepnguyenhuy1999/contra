#include "Gun.h"
#include "Soldier.h"
#include "GunSoldier.h"
#include "GunBox.h"
#include "GunShip.h"
#include "GunMachine1.h"
#include "BossStage1.h"
#include "BossStage1Gun.h"
#include "BossStage3_HandPiece.h"
#include "debug.h"

CGun::CGun(float x, float y, float nx, float ny, float vx, float vy, int type ) :CGameObject(x, y)
{
	this->vx = vx;
	this->vy = vy;
	id = type;
	dmg = getDamage();
	SetState(GUN_STATE_RELEASE);
}
void CGun::Render()
{
	int aniId = -1;
	aniId=getAniID();
	if (id == 3)
		for (int i = 0; i < 3; i++) {
			if(nx!=0)
			CAnimations::GetInstance()->Get(aniId)->Render(x + i * GUN_L_BBOX_WIDTH + GUN_L_BBOX_WIDTH / 2, y);
			else 
			CAnimations::GetInstance()->Get(aniId)->Render(x, y + i * GUN_L_BBOX_HEIGHT + GUN_L_BBOX_HEIGHT / 2);
		}
	else if (aniId != -1) CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}
int CGun::getAniID() {
	if (state == GUN_STATE_DIE)
		return ID_ANI_GUN_EXPLODE;
	else switch (id)
	{
		case GUN_ID_M:
			return ID_ANI_GUN_M;
		case GUN_ID_F:
			return ID_ANI_GUN_F;
		case GUN_ID_L:
			return ID_ANI_GUN_L;
		case GUN_ID_S:
			return ID_ANI_GUN_S;
	default:
		return ID_ANI_GUN_DEFAULT;
	}
}
int CGun::getDamage() {

	switch (id)
	{
	case GUN_ID_M:
		return 1;
	case GUN_ID_F:
		return 1;
	case GUN_ID_L:
		return 1;
	case GUN_ID_S:
		return 1;
	default:
		return 1;
	}
}

void CGun::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGun::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CSoldier*>(e->obj))
		OnCollisionWithSoldier(e);
	else if (dynamic_cast<CGunSoldier*>(e->obj))
		OnCollisionWithGunSoldier(e);
	else if (dynamic_cast<CGunBox*>(e->obj))
		OnCollisionWithGunBox(e);
	else if (dynamic_cast<CGunMachine1*>(e->obj))
		OnCollisionWithGunMachine1(e);
	else if (dynamic_cast<CGunShip*>(e->obj))
		OnCollisionWithGunShip(e);
	else if (dynamic_cast<CBossStage1*>(e->obj))
		OnCollisionWithBossStage1(e);
	else if (dynamic_cast<CBossStage1Gun*>(e->obj))
		OnCollisionWithBossStage1Gun(e);
	else if (dynamic_cast<CBossStage3_HandPiece*>(e->obj))
		OnCollisionWithBossStage3_HandPiece(e);
}
void CGun::OnCollisionWithSoldier(LPCOLLISIONEVENT e)
{
	CSoldier* i = dynamic_cast<CSoldier*>(e->obj);
	i->SetState(SOLDIER_STATE_DIE);
	SetState(GUN_STATE_DIE);
}
void CGun::OnCollisionWithGunSoldier(LPCOLLISIONEVENT e)
{
	CGunSoldier* i = dynamic_cast<CGunSoldier*>(e->obj);
	i->SetState(GUNSOLDIER_STATE_DIE);
	SetState(GUN_STATE_DIE);
}
void CGun::OnCollisionWithGunMachine1(LPCOLLISIONEVENT e)
{
	CGunMachine1* i = dynamic_cast<CGunMachine1*>(e->obj);
	i->handleGetAttack(this->dmg);
	SetState(GUN_STATE_DIE);
}
void CGun::OnCollisionWithGunBox(LPCOLLISIONEVENT e)
{
	CGunBox* i = dynamic_cast<CGunBox*>(e->obj);
	if (i->GetState() == GUNBOX_STATE_OPEN) {
		i->SetState(GUNBOX_STATE_DIE);
		}
	SetState(GUN_STATE_DIE);
}
void CGun::OnCollisionWithGunShip(LPCOLLISIONEVENT e)
{
	CGunShip* i = dynamic_cast<CGunShip*>(e->obj);
	i->SetState(GUNSHIP_STATE_DIE);
	SetState(GUN_STATE_DIE);
}
void CGun::OnCollisionWithBossStage1(LPCOLLISIONEVENT e)
{
	CBossStage1* i = dynamic_cast<CBossStage1*>(e->obj);
	i->SetState(BOSS_STAGE_1_STATE_DMG);
	SetState(GUN_STATE_DIE);
}
void CGun::OnCollisionWithBossStage3_HandPiece(LPCOLLISIONEVENT e)
{
	CBossStage3_HandPiece* i = dynamic_cast<CBossStage3_HandPiece*>(e->obj);
	//i->SetState(BOSS_STAGE_3_HANDPIECE_STATE_DMG);
	SetState(GUN_STATE_DIE);
}
void CGun::OnCollisionWithBossStage1Gun(LPCOLLISIONEVENT e)
{
	CBossStage1Gun* i = dynamic_cast<CBossStage1Gun*>(e->obj);
	i->SetState(BOSS_STAGE_1_GUN_STATE_DMG);
	SetState(GUN_STATE_DIE);
}
void CGun::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state != GUN_STATE_RELEASE && (GetTickCount64() - count_start > 3000))
	{
		count_start = -1;
		isDeleted = true;
		return;
	}
	else if (state == GUN_STATE_DIE)
	{
		count_start = -1;
		isDeleted = true;
		return;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CGun::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (id == 3)
	{
		l = x - GUN_L_BBOX_WIDTH / 2;
		t = y + GUN_L_BBOX_HEIGHT / 2;
		r = l + GUN_BBOX_WIDTH * 3;
		b = t - GUN_L_BBOX_HEIGHT;
	}
	else
	{
	l = x - GUN_BBOX_WIDTH / 2;
	t = y + GUN_BBOX_HEIGHT / 2;
	r = l + GUN_BBOX_WIDTH;
	b = t - GUN_BBOX_HEIGHT;
	}
}
void CGun::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GUN_STATE_RELEASE:
		count_start = GetTickCount64();
		break;
	}
}

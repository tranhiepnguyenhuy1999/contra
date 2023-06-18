#include "Gun.h"
#include "Soldier.h"
#include "GunSoldier.h"
#include "GunBox.h"
#include "GunShip.h"
#include "GunMachine1.h"
#include "BossStage1.h"
#include "BossStage1Gun.h"
#include "debug.h"

CGun::CGun(float x, float y, float nx, float ny, float vx, float vy, float type ) :CGameObject(x, y)
{
	if (nx > 0) ax = GUN_ACCEL; else ax = -GUN_ACCEL;
	if (ny > 0) ay = GUN_ACCEL; else ay = -GUN_ACCEL;
	this->vx = vx;
	this->vy = vy;
	vxMax = nx * GUN_MAX_SPEED;
	vyMax = ny * GUN_MAX_SPEED;
	id = type;
	dmg = getDamage();
	SetState(GUN_STATE_RELEASE);
}
void CGun::Render()
{
	int aniId = -1;
	aniId=getAniId();
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
int CGun::getAniId() {
	if (state == GUN_STATE_DIE)
		return ID_ANI_GUN_EXPLODE;
	switch (id)
	{
		case 0: return ID_ANI_GUN_DEFAULT;
		case 1:
			return ID_ANI_GUN_M;
		case 2:
			return ID_ANI_GUN_F;
		case 3:
			return ID_ANI_GUN_L;
		case 4:
			return ID_ANI_GUN_S;
	default:
		break;
	}
}
int CGun::getDamage() {

	switch (id)
	{
	case 0: return 1;
	case 1:
		return 1;
	case 2:
		return 1;
	case 3:
		return 1;
	case 4:
		return 1;
	default:
		break;
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
	//else if (dynamic_cast<CGunSoldier*>(e->obj))
	//	OnCollisionWithGunSoldier(e);
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
	DebugOut(L"yeah");
	CBossStage1* i = dynamic_cast<CBossStage1*>(e->obj);
	i->SetState(BOSS_STAGE_1_STATE_DMG);
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
	vx += ax * dt;
	vy += ay * dt;

	if (abs(vx) > abs(vxMax)) vx = vxMax;
	if (abs(vy) > abs(vyMax)) vy = vyMax;

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

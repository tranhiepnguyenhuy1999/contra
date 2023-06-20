#pragma once
#include "GameObject.h"
#include "BossStage1Gun.h"

#define BOSS_STAGE_1_BBOX_WIDTH 32
#define BOSS_STAGE_1_BBOX_HEIGHT 32

#define BOSS_STAGE_1_STATE_ACTIVE 100
#define BOSS_STAGE_1_STATE_DIE 200
#define BOSS_STAGE_1_STATE_DMG 300


#define ID_ANI_BOSS_STAGE_1	4203

class CBossStage1 : public CGameObject
{

protected:
	int life;

	vector<CBossStage1Gun*> children;
public:
	CBossStage1(float x, float y);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void SetState(int state);

	void IsDeleteChildren();
	void addChild(CBossStage1Gun* e) {
		children.push_back(e);
	}
};


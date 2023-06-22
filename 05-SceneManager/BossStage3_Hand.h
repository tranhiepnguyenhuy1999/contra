#pragma once
#include "GameObject.h"
#include "BossStage3_HandPiece.h"

#define BOSS_STAGE_3_HAND_STATE_DIE 100

class CBossStage3_Hand : public CGameObject
{

protected:
	int life;
	ULONGLONG loop_start;

	vector<CBossStage3_HandPiece*> children;
public:
	CBossStage3_Hand(float x, float y);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual void SetState(int state);

	bool isDie() { return (life <= 0); }
	
	void addNewChild(CBossStage3_HandPiece* e) {
		children.push_back(e);
	}

};



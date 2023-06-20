#pragma once
#include "GameObject.h"
#include "BombBridgeChild.h"

#define BOMB_BRIDGE_CHILD_BBOX_WIDTH 32
#define BOMB_BRIDGE_CHILD_BBOX_HEIGHT 32

#define BOMB_BRIDGE_EXPLODE_TIMEOUT 750

class CBombBridge : public CGameObject
{
protected:
	float type;
	bool isDestroy;

	ULONGLONG active_start;

	vector<CBombBridgeChild*> child;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void RenderBoundingBox();
	void setupBombBridgeChild(float top1, float top2, float bot1, float bot2, float bot3);
public:
	CBombBridge(float x, float y, float top1, float top2, float bot1, float bot2, float bot3);
	virtual void SetState(int state);
};
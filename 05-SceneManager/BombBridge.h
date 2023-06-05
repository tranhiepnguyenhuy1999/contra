#pragma once
#include "GameObject.h"
#include "BombBridgeChild.h"

#define BOMB_BRIDGE_CHILD_BBOX_WIDTH 32
#define BOMB_BRIDGE_CHILD_BBOX_HEIGHT 32

#define BOMB_BRIDGE_EXPLODE_TIMEOUT 1500

class CBombBridge : public CGameObject
{
protected:
	float type;
	bool isActive;

	ULONGLONG active_start;

	vector<CBombBridgeChild*> child;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void RenderBoundingBox();
	void setupBombBridgeChild();
public:
	CBombBridge(float x, float y, float type);
	virtual void SetState(int state);
};
#pragma once
#include "GameObject.h"
#include "FallObject.h"

#define BOMB_BRIDGE_CHILD_PIECE_BBOX_WIDTH 16
#define BOMB_BRIDGE_CHILD_PIECE_BBOX_HEIGHT 16

#define BOMB_BRIDGE_DIE_TIMEOUT 1000

#define BOMB_BRIDGE_STATE_UNACTIVE 100
#define BOMB_BRIDGE_STATE_ACTIVE 200


class CBombBridgeChild : public CGameObject
{
protected:
	float ani1, ani2, ani3, ani4;

public:

	CBombBridgeChild(float x, float y, int ani1, int ani2, int ani3, int ani4);
	
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt){};
	virtual void Render();
};
#include "BombBridgeChild.h"

#include "AssetIDs.h"
#include "PlayerData.h"

#include "debug.h"

CBombBridgeChild::CBombBridgeChild(float x, float y, int ani1, int ani2, int ani3, int ani4) :CGameObject(x, y)
{
	this->ani1 = ani1;
	this->ani2 = ani2;
	this->ani3 = ani3;
	this->ani4 = ani4;
}
void CBombBridgeChild::Render()
{
	//top
	CAnimations::GetInstance()->Get(ani1)->Render(x, y);
	CAnimations::GetInstance()->Get(ani2)->Render(x +  BOMB_BRIDGE_CHILD_PIECE_BBOX_WIDTH, y);
	//bottom
	CAnimations::GetInstance()->Get(ani3)->Render(x, y - BOMB_BRIDGE_CHILD_PIECE_BBOX_HEIGHT);
	CAnimations::GetInstance()->Get(ani4)->Render(x + BOMB_BRIDGE_CHILD_PIECE_BBOX_WIDTH, y - BOMB_BRIDGE_CHILD_PIECE_BBOX_HEIGHT);
}

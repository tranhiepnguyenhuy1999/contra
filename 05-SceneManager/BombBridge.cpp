#include "BombBridge.h"

#include "BombBridgeChild.h"
#include "Lance.h"

#include "AssetIDs.h"
#include "PlayerData.h"

#include "debug.h"
#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"

#include "Camera.h"

void CBombBridge::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)t - (int)b;

	float cx, cy;
	Camera::GetInstance()->getCamPosition(cx, cy);

	float xx = x - BOMB_BRIDGE_CHILD_BBOX_WIDTH / 2 + rect.right / 2;

	CGame::GetInstance()->Draw(xx - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

CBombBridge::CBombBridge(float x, float y, float type=1) :CGameObject(x, y)
{
	isActive = false;
	this->type = type;
	setupBombBridgeChild();
	SetState(BOMB_BRIDGE_STATE_UNACTIVE);
}

void CBombBridge::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	
	left = x - BOMB_BRIDGE_CHILD_BBOX_WIDTH / 2;
	top = y + BOMB_BRIDGE_CHILD_BBOX_HEIGHT / 2;
	right = left + BOMB_BRIDGE_CHILD_BBOX_WIDTH * child.size();
	bottom = top - BOMB_BRIDGE_CHILD_BBOX_HEIGHT;
}

void CBombBridge::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float px, py;
	Camera::GetInstance()->getPlayerPosition(px, py);
	if (!isActive)
	{
		if (px > x ) {
			isActive = true;
			active_start = GetTickCount64();
		}
		return;
	}
	else if (isActive && (GetTickCount64() - active_start > BOMB_BRIDGE_EXPLODE_TIMEOUT))
	{
		CBombBridgeChild* i = child.front();
		child.erase(child.begin());
		active_start= GetTickCount64();
		
		// create explode
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y, 1);
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x, y + BOMB_BRIDGE_CHILD_PIECE_BBOX_HEIGHT / 2, 1);
		CGame::GetInstance()->GetCurrentScene()->createNewObject(OBJECT_TYPE_EXPLODE, x + BOMB_BRIDGE_CHILD_PIECE_BBOX_WIDTH, y, 1);

		delete i;

		x = x + BOMB_BRIDGE_CHILD_BBOX_WIDTH;
		if (child.empty())
		{
			this->Delete();
			return;
		}
	}
	CGameObject::Update(dt, coObjects);
}


void CBombBridge::Render()
{
	for (UINT i = 0; i < child.size(); i++) {
		child[i]->Render();
	}
	RenderBoundingBox();
}

void CBombBridge::SetState(int state)
{
	CGameObject::SetState(state);
}
void CBombBridge::setupBombBridgeChild() {
	float ani1 = 154, ani2 = 179;
	if (type == 2) {
		ani1 = 155;
		ani2 = 180;
	}
	float childX, childY;
	childX = x - BOMB_BRIDGE_CHILD_PIECE_BBOX_WIDTH / 2;
	childY = y + BOMB_BRIDGE_CHILD_PIECE_BBOX_HEIGHT / 2;
	CBombBridgeChild*  start = new CBombBridgeChild( childX, childY, 152, 153, 176, 177);
	CBombBridgeChild*  mid1 = new CBombBridgeChild(childX + BOMB_BRIDGE_CHILD_BBOX_WIDTH, childY, 152, 153, 177, 177);
	CBombBridgeChild*  mid2 = new CBombBridgeChild(childX + BOMB_BRIDGE_CHILD_BBOX_WIDTH*2, childY, 152, 153, 177, 177);
	CBombBridgeChild*  end = new CBombBridgeChild(childX + BOMB_BRIDGE_CHILD_BBOX_WIDTH*3, childY, 152, ani1, 178, ani2);

	child.push_back(start);
	child.push_back(mid1);
	child.push_back(mid2);
	child.push_back(end);
}
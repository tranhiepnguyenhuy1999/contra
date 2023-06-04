#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Lance.h"
#include "Goomba.h"
#include "TileSet.h"
#include "DownBrick.h"
#include <sstream>
//#include "Koopas.h"


class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;					

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> tiledMapObject;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEDMAP(string line);
	void _ParseSection_TILESET(string line);
	void LoadAssets(LPCWSTR assetFile);

	virtual void createNewObject(int id, float x, float y, float nx, float ny, int type, LPGAMEOBJECT srcObject);
	void  DetectCollision(vector<LPGAMEOBJECT>& coObjects);
	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void getPlayerPosition(float& x, float& y) {
		player->GetPosition(x, y);
	};
	virtual void getPlayerLevel(int& level) {
		player->getLevel(level);
	};
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;


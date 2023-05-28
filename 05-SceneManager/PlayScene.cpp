#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Platform.h"
#include "BreakBrick.h"
#include "HiddenBrick.h"

#include "Soldier.h"
#include "GunSoldier.h"
#include "GunMachine1.h"
#include "GunMachine2.h"
#include "GunBox.h"
#include "GunShip.h"
#include "GunType.h"
#include "Gun.h"
#include "EnemyGun.h"
#include "Explode.h"
#include "Water.h"

#include "Camera.h"
#include "QuadTree.h"
#include "TileMap.h"
#include "SampleKeyEventHandler.h"


using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_TILEDMAP	3
#define SCENE_SECTION_TILESET	4

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_TILESET(string line)
{
	vector<string> tokens = split(line);
	// skip flag line
	if (tokens.size() < 7) return;
	else DebugOut(L"Start loading tileset firsttime!\n");

	int width = atoi(tokens[0].c_str());
	int height = atoi(tokens[1].c_str());
	int maxTiles = atoi(tokens[2].c_str());
	int lengthTilesX = atoi(tokens[3].c_str());
	int lengthTilesY = atoi(tokens[4].c_str());
	int frameTime = atoi(tokens[5].c_str());
	int texID = atoi(tokens[6].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);

	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	for (int i = 0; i < lengthTilesX+1; i++)
	{
		for (int j = 0; j < lengthTilesY+1; j++)
		{
			if (i * (lengthTilesY + 1) + j > maxTiles) continue;
			// load sprite
			int ID = 1000 + i * (lengthTilesY + 1) + j;
			int l = j * width;
			int r = l + width - 1;
			int t = i * height;
			int b = t + height - 1;
			CSprites::GetInstance()->Add(ID, l, t, r, b, tex);

			// load animation
			LPANIMATION ani = new CAnimation();
			int ani_id = i * (lengthTilesY + 1) + j;
			int sprite_id = ID;
			int frame_time = frameTime;
			ani->Add(sprite_id, frame_time);

			CAnimations::GetInstance()->Add(ani_id, ani);
		}
	}

}
/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CLance(x,y); 
		player = (CLance*)obj;  

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(x,y); break;
	case OBJECT_TYPE_GUNBOX:
	{
		float typeGun = (float)atof(tokens[3].c_str());
		obj = new CGunBox(x, y, typeGun); break;
	}
	case OBJECT_TYPE_GUNSHIP:
	{
		float typeGun = (float)atof(tokens[3].c_str());
		obj = new CGunShip(x, y, typeGun); break;
	}
	case OBJECT_TYPE_SOLDIER: obj = new CSoldier(x, y); break;
	case OBJECT_TYPE_GUN_SOLDIER: obj = new CGunSoldier(x, y); break;
	case OBJECT_TYPE_GUNMACHINE1: obj = new CGunMachine1(x, y); break;
	case OBJECT_TYPE_GUNMACHINE2: obj = new CGunMachine2(x, y); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(x,y); break;
	case OBJECT_TYPE_BREAKBRICK: obj = new CBreakBrick(x, y); break;
	case OBJECT_TYPE_DOWNBRICK:
	{
		float length = (float)atof(tokens[3].c_str());
	obj = new CDownBrick(x, y, length); break;
	}
	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}

	case OBJECT_TYPE_WATER_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());

		obj = new CWater(
			x, y,
			cell_width, cell_height, length
		);

		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);


	objects.push_back(obj);
}
void CPlayScene::createNewObject(int id, float x, float y, float nx=0, float ny=0, int type =0)
{
	CGameObject* obj = NULL;
	switch (id)
	{
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(x, y); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(x, y); break;
	case OBJECT_TYPE_GUNTYPE: obj = new CGunType(x, y,1,1); break;
	case OBJECT_TYPE_GUN: obj = new CGun(x, y, nx, ny, type); break;
	case OBJECT_TYPE_ENEMY_GUN: obj = new CEnemyGun(x, y, nx, ny, type); break;
	case OBJECT_TYPE_EXPLODE: obj = new CExplode(x, y, nx); break;
	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", id);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);


	objects.push_back(obj);
}
void CPlayScene::_ParseSection_TILEDMAP()
{
	CGameObject* obj = NULL;
	string fname;
	fname = "map1_map.csv";
	vector<vector<string>> content;
	vector<string> row;
	string line, word;

	fstream file(fname, ios::in);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			row.clear();

			stringstream str(line);

			while (getline(str, word, ','))
				row.push_back(word);
			content.push_back(row);
		}
	}
	else
		DebugOut(L"[INFO] Could not open the file\n");

	obj = new CTileMap(0, 0, content);
	tiledMapObject.push_back(obj);
}
void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[TILESET]") { section = SCENE_SECTION_TILESET; continue; };
		if (line == "[TILEDMAP]") { section = SCENE_SECTION_TILEDMAP; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
			case SCENE_SECTION_TILESET: _ParseSection_TILESET(line); break;
			case SCENE_SECTION_TILEDMAP: _ParseSection_TILEDMAP(); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

Quadtree* CreateQuadTree(vector<LPGAMEOBJECT> objList)
{
	// Init base game region for detecting collision
	Quadtree* quadtree = new Quadtree(0, 0, 1000, 1000);
	// add objects to quadtree
	for (auto i = objList.begin(); i != objList.end(); i++)
		quadtree->Insert(*i);

	return quadtree;
}

void CPlayScene::DetectCollision(vector<LPGAMEOBJECT>& coObjects)
{
	Quadtree* quadtree = CreateQuadTree(this->objects);

	for (size_t i = 1; i < objects.size(); i++)
	{
		//Get all objects that can collide with current entity
		quadtree->Retrieve(coObjects, objects[i]);
	}

	quadtree->Clear();

	delete quadtree;
}
void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	coObjects.clear();

	//DetectCollision(coObjects);
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow mario
	float px, py;
	player->GetPosition(px, py);

	Camera::GetInstance()->setCamPosition(px, 0);

	//CUserBoard::GetInstance()->SetPosition(200, 432);

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	// render tilemap first
	tiledMapObject[0]->Render();

	// obj render
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}
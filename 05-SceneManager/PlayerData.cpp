#include "PlayerData.h"
#include "Camera.h"


CPlayerData* CPlayerData::__instance = NULL;

void CPlayerData::Render()
{
	float cl, ct, cr, cb;
	Camera::GetInstance()->getCamBoundingBox (cl, ct, cr, cb);
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	for (int i = 0; i < life; i++)
	{
		animations->Get(ID_ANI_LIFE)->Render( cl + 16 + (LIFE_WIDTH + 3) *i + LIFE_WIDTH/2, ct - 3 - LIFE_HEIGHT/2);
	}
}
CPlayerData* CPlayerData::GetInstance()
{
	if (__instance == NULL) __instance = new CPlayerData();
	return __instance;
}
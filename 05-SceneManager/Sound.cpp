#include "Sound.h"
CSound* CSound::__instance = NULL;

CSound* CSound::GetInstance()
{
	if (__instance == NULL) __instance = new CSound();
	return __instance;
}

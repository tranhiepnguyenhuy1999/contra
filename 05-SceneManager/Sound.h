#pragma once
#pragma comment (lib, "winmm.lib")
#include <Windows.h>
using namespace std;
class CSound
{
	static CSound* __instance;
public:
	CSound() {}
	void playSound_Pause() {
		PlaySound(L"pause.wav", NULL, SND_ASYNC);
	}
	static CSound* GetInstance();
};


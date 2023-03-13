#include "CountdownTimer.h"
#include <Windows.h>
#include "debug.h"
CountdownTimer* CountdownTimer::__instance = NULL;
CountdownTimer* CountdownTimer::GetInstance()
{
	if (__instance == NULL) __instance = new CountdownTimer();
	return __instance;
}
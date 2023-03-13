#pragma once
class CountdownTimer
{
	static CountdownTimer* __instance;
	int counter = 300;
public:
	CountdownTimer() {
	};
	static class CountdownTimer* GetInstance();
	void getCounter(int& counter) {

		counter = this->counter;
	}
};


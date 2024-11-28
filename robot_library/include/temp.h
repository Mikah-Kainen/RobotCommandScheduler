#pragma once
#include <vector>

class Temp
{
private:
	int speed;
	int currentStep;
	int previousTime;

public:

	Temp();

	bool SetSpeed(int targetSpeed);

	bool Display();

	//As opposed to being initialized, functions like ResetCurrentStep and MultiStepMove are scheduled together in a command group
	bool ResetCurrentStep();

	bool MultiStepMove(int numberOfSteps, std::vector<int>* loggingVector = nullptr);


	bool UpdateTime();

	bool MoveByTime(int milliseconds);
};
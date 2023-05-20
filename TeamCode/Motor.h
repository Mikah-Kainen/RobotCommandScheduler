#pragma once
#include "Static.h"

class Motor
{
private:
	int speed;
	int currentStep;
	int previousTime;

public:
	Systems MotorID;

	Motor(Systems motor);

	bool SetSpeed(int targetSpeed);

	bool Display();

	//As opposed to being initialized, functions like ResetCurrentStep and MultiStepMove are scheduled together in a command group
	bool ResetCurrentStep();

	bool MultiStepMove(int numberOfSteps);


	bool UpdateTime();

	bool MoveByTime(int milliseconds);
};
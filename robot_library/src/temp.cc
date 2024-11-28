#include "temp.h"

#include <iostream>

Temp::Temp()
	: currentStep{ 0 }, speed{ 0 }, previousTime{ 0 }
{

}

bool Temp::SetSpeed(int targetSpeed)
{
	speed = targetSpeed;
	return true;
}

bool Temp::Display()
{
	std::cout << "Motor: [REDACTED], Speed: " << speed << std::endl;
	return true;
}

//As opposed to being initialized, functions like ResetCurrentStep and MultiStepMove are scheduled together in a command group
bool Temp::ResetCurrentStep()
{
	Temp::currentStep = 0;
	return true;
}

bool Temp::MultiStepMove(int numberOfSteps, std::vector<int>* loggingVector)
{
	if (loggingVector != nullptr) {
		loggingVector->push_back(currentStep);
	}
	//numberOfSteps = 3;
	std::cout << "CurrentStep: " << currentStep << " MotorID: [REDACTED]" << std::endl;
	currentStep++;
	return currentStep >= numberOfSteps;
}


bool Temp::UpdateTime()
{
	previousTime = 1000;
	return true;
}

bool Temp::MoveByTime(int milliseconds)
{
	if (1000 - previousTime >= milliseconds)
	{
		return true;
	}
	return false;
}
#include "temp2.h"

#include <iostream>

Temp2::Temp2()
	: currentStep{ 0 }, speed{ 0 }, previousTime{ 0 }
{

}

bool Temp2::SetSpeed(int targetSpeed)
{
	speed = targetSpeed;
	return true;
}

bool Temp2::Display()
{
	std::cout << "Motor: [REDACTED], Speed: " << speed << std::endl;
	return true;
}

//As opposed to being initialized, functions like ResetCurrentStep and MultiStepMove are scheduled together in a command group
bool Temp2::ResetCurrentStep()
{
	Temp2::currentStep = 0;
	return true;
}

bool Temp2::MultiStepMove(int numberOfSteps, std::vector<int>* loggingVector)
{
	if (loggingVector != nullptr) {
		loggingVector->push_back(currentStep);
	}
	//numberOfSteps = 3;
	std::cout << "CurrentStep: " << currentStep << " MotorID: [REDACTED]" << std::endl;
	currentStep++;
	return currentStep >= numberOfSteps;
}


bool Temp2::UpdateTime()
{
	previousTime = 1000;
	return true;
}

bool Temp2::MoveByTime(int milliseconds)
{
	if (1000 - previousTime >= milliseconds)
	{
		return true;
	}
	return false;
}
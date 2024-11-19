#include "../../inc/virtual_robot/motor.h"

#include <iostream>

#include "../../inc/virtual_robot/timer.h"

Motor::Motor(Systems motor)
	: MotorID{ motor }, currentStep{ 0 }, speed{ 0 }, previousTime{ (int)Timer::GetInstance().ElapsedMilliseconds() }
{

}

bool Motor::SetSpeed(int targetSpeed)
{
	speed = targetSpeed;
	return true;
}

bool Motor::Display()
{
	std::cout << "Motor: " << (int)MotorID << ", Speed: " << speed << std::endl;
	return true;
}

//As opposed to being initialized, functions like ResetCurrentStep and MultiStepMove are scheduled together in a command group
bool Motor::ResetCurrentStep()
{
	Motor::currentStep = 0;
	return true;
}

bool Motor::MultiStepMove(int numberOfSteps)
{
	//numberOfSteps = 3;
	std::cout << "CurrentStep: " << currentStep << " MotorID: " << (int)MotorID << std::endl;
	currentStep++;
	return currentStep >= numberOfSteps;
}


bool Motor::UpdateTime()
{
	previousTime = Timer::GetInstance().ElapsedMilliseconds();
	return true;
}

bool Motor::MoveByTime(int milliseconds)
{
	if (Timer::GetInstance().ElapsedMilliseconds() - previousTime >= milliseconds)
	{
		return true;
	}
	return false;
}
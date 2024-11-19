#pragma once
#include <memory>

#include "motor.h"


class Robot
{
private:
	std::shared_ptr<Motor> leftMotor;
	std::shared_ptr<Motor> rightMotor;

	Robot(Systems leftMotorSystem, Systems rightMotorSystem);

public:

	static Robot& GetInstance();

	std::shared_ptr<Motor> GetLeftMotor();

	std::shared_ptr<Motor> GetRightMotor();
};


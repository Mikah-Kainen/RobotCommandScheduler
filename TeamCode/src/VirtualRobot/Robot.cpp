#include "Robot.h"


Robot::Robot(Systems leftMotorSystem, Systems rightMotorSystem)
	: leftMotor{ std::make_shared<Motor>(leftMotorSystem) }, rightMotor{std::make_shared<Motor>(rightMotorSystem)}
{

}

Robot& Robot::GetInstance()
{
	static Robot robot = Robot(Systems::LeftMotor, Systems::RightMotor);
	return robot;
}

std::shared_ptr<Motor> Robot::GetLeftMotor()
{
	return leftMotor;
}

std::shared_ptr<Motor> Robot::GetRightMotor()
{
	return rightMotor;
}
#include <iostream>
#include "Command.cpp"
#include <string>
#include <list>
#include "Scheduler.h"
#include "Static.h"
#include "ParallelGroup.h"
#include "SequentialGroup.h"

#pragma region CatStuff
class Cat
{
public:
	int Age;
	std::string Name;

	Cat(int age, std::string name)
		: Age{ age }, Name{ name } {}
};

bool CoolFunction()
{
	std::cout << "Wow this is so cool!\n";
	return true;
}

bool CatFunction(Cat cat)
{
	std::cout << "Age: " << cat.Age << " Name: " << cat.Name << std::endl;
	return true;
}

bool CoolCatFunction()
{
	Cat bobCat = Cat(10, "Bob");
	return CatFunction(bobCat);
}
#pragma endregion


class Motor
{
private:
	int speed;
	int currentStep;

public:
	Systems MotorID;

	Motor(Systems motor)
		: MotorID{ motor }, currentStep{ 0 }
	{

	}

	bool SetSpeed(int targetSpeed)
	{
		speed = targetSpeed;
		return true;
	}

	bool Display()
	{
		std::cout << "Motor: " << (int)MotorID << ", Speed: " << speed << std::endl;
		return true;
	}

	//As opposed to being initialized, functions like ResetCurrentStep and MultiStepMove are scheduled together in a command group
	bool ResetCurrentStep()
	{
		currentStep = 0;
		return true;
	}

	bool MultiStepMove(int numberOfSteps)
	{
		//int numberOfSteps = 3;
		std::cout << "CurrentStep: " << currentStep << " MotorID: " << (int)MotorID << std::endl;
		currentStep++;
		return currentStep >= numberOfSteps;
	}
};

Motor MotorA = Motor(Systems::MotorA);
Motor MotorB = Motor(Systems::MotorB);
Motor MotorC = Motor(Systems::MotorC);

bool test()
{
	return MotorA.MultiStepMove(5);
}

bool MotorAMultiStepMove(int params)
{
	return MotorA.MultiStepMove(params);
}

bool MotorBMultiStepMove(int params)
{
	return MotorB.MultiStepMove(params);
}

bool MotorCMultiStepMove(int params)
{
	return MotorC.MultiStepMove(params);
}

bool MotorAResetCurrentStep()
{
	return MotorA.ResetCurrentStep();
}

bool MotorBResetCurrentStep()
{
	return MotorB.ResetCurrentStep();
}

bool MotorCResetCurrentStep()
{
	return MotorC.ResetCurrentStep();
}

int main() //Unit tests with GoogleTest
{
		//**Things to do
	//scheduling stuff should just take in Systems and convert it to unsigned char(or maybe just make Command wrapper more convenient)
	//Add Initialization and EndBehavior functions to commands
	//give schedulers unique IDs for debugging purposes

	Command<int> MotorAMultiStepMoveCommand = Command<int>(MotorAMultiStepMove, Systems::MotorA);
	Command<int> MotorBMultiStepMoveCommand = Command<int>(MotorBMultiStepMove, Systems::MotorB);
	Command<int> MotorCMultiStepMoveCommand = Command<int>(MotorCMultiStepMove, Systems::MotorC);

	Scheduler scheduler = Scheduler::GetInstance();

	SequentialGroup* sequentialGroup = new SequentialGroup({
		new ParallelGroup({
			MotorAMultiStepMoveCommand.Schedule(3),
			//new ScheduledCommand([&]() {return MotorA.MultiStepMove(3); }, (unsigned char)Systems::MotorA),
			new ScheduledCommand([&]() {return MotorA.ResetCurrentStep(); }, (unsigned char)Systems::MotorA),
		}),
		MotorBMultiStepMoveCommand.Schedule(5),
		//new ScheduledCommand([&]() {return MotorB.MultiStepMove(5); }, (unsigned char)Systems::MotorB),
		new ScheduledCommand([&]() {return MotorB.ResetCurrentStep(); }, (unsigned char)Systems::MotorB),
		new ParallelGroup({
			MotorAMultiStepMoveCommand.Schedule(5),
			//new ScheduledCommand([&]() {return MotorA.MultiStepMove(5); }, (unsigned char)Systems::MotorA),
			new ScheduledCommand([&]() {return MotorA.ResetCurrentStep(); }, (unsigned char)Systems::MotorA),
			new SequentialGroup({
				MotorBMultiStepMoveCommand.Schedule(4),
				MotorCMultiStepMoveCommand.Schedule(5),
				//new ScheduledCommand([&]() {return MotorB.MultiStepMove(4); }, (unsigned char)Systems::MotorB), //scheduling stuff should just take in Systems and convert it to unsigned char(or maybe just make Command wrapper more convenient)
				//new ScheduledCommand([&]() {return MotorC.MultiStepMove(5); }, (unsigned char)Systems::MotorC), //Add Initialization and EndBehavior functions to commands
			}),
		}),
	});

	scheduler.Schedule(sequentialGroup);
	while (!scheduler.Run()) //scheduler needs a bool for when it is done
	{
		std::cout << "\n";
	}
}

//scheduler.Schedule([&]() {return MotorA.MultiStepMove(3); }, (unsigned char)Systems::MotorA);
//scheduler.Schedule([&]() {return MotorA.ResetCurrentStep(); }, (unsigned char)Systems::MotorA);
//scheduler.Schedule(test, (unsigned char)Systems::MotorA);
//scheduler.Schedule([&]() {return MotorB.MultiStepMove(4); }, (unsigned char)Systems::MotorB);
//scheduler.Schedule([&]() {return MotorC.MultiStepMove(5); }, (unsigned char)Systems::MotorC);

//new ParallelGroup(std::vector<FunctionManager::Scheduleable*>({
//	new ScheduledCommand([&]() {return MotorA.MultiStepMove(3); }, (unsigned char)Systems::MotorA),
//	new ScheduledCommand([&]() {return MotorA.ResetCurrentStep(); }, (unsigned char)Systems::MotorA),
//	})),

//scheduler.Schedule(CoolFunction, (unsigned char)1);
//scheduler.Schedule(CoolCatFunction, (unsigned char)1);
//scheduler.Schedule(CoolFunction, (unsigned char)1);
//for(int i = 0; i < 4; i ++)
//{
//    scheduler.Run();
//    std::cout << "\n";
//}
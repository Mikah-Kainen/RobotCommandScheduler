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
		: MotorID{ motor }, currentStep{ 0 }, speed{0}
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
		//numberOfSteps = 3;
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

#pragma region WrapperFunctions

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

#pragma endregion

int main() //Unit tests with GoogleTest
{
		//**Things to do
	//scheduling stuff should just take in Systems and convert it to unsigned char(or maybe just make Command wrapper more convenient)
	//Add Initialization and EndBehavior functions to commands
	//give schedulers unique IDs for debugging purposes

	//new ScheduledCommand([&]() {return MotorB.MultiStepMove(4); }, (unsigned char)Systems::MotorB), //scheduling stuff should just take in Systems and convert it to unsigned char(or maybe just make Command wrapper more convenient)
	//new ScheduledCommand([&]() {return MotorC.MultiStepMove(5); }, (unsigned char)Systems::MotorC), //Add Initialization and EndBehavior functions to commands


	Command<int> MotorAMultiStepMoveCommand = Command<int>(MotorAMultiStepMove, Systems::MotorA);
	Command<int> MotorBMultiStepMoveCommand = Command<int>(MotorBMultiStepMove, Systems::MotorB);
	Command<int> MotorCMultiStepMoveCommand = Command<int>(MotorCMultiStepMove, Systems::MotorC);

	Command<> MotorAResetCurrentStepCommand = Command<>(MotorAResetCurrentStep, Systems::MotorA);
	Command<> MotorBResetCurrentStepCommand = Command<>(MotorBResetCurrentStep, Systems::MotorB);
	Command<> MotorCResetCurrentStepCommand = Command<>(MotorCResetCurrentStep, Systems::MotorC);

	int three = 3;
	int four = 4;
	int five = 5;


	Scheduler scheduler = Scheduler::GetInstance();

	SequentialGroup* sequentialGroup = new SequentialGroup({
		new ParallelGroup({
			MotorAMultiStepMoveCommand.ScheduleWith(three),
			MotorAResetCurrentStepCommand.ScheduleWith(),
		}),
		MotorBMultiStepMoveCommand.ScheduleWith(five),
		MotorBResetCurrentStepCommand.ScheduleWith(),
		new ParallelGroup({
			MotorAMultiStepMoveCommand.ScheduleWith(five),
			MotorAResetCurrentStepCommand.ScheduleWith(),
			new SequentialGroup({
				MotorBMultiStepMoveCommand.ScheduleWith(four),
				MotorCMultiStepMoveCommand.ScheduleWith(five),
			}),
		}),
	});

	scheduler.Schedule(sequentialGroup);
	while (!scheduler.Run()){}
	std::cout << "Program Finished!";
}


//SequentialGroup* sequentialGroup = new SequentialGroup({
//	new ParallelGroup({
//		MotorAMultiStepMoveCommand.ScheduleWith(three),
//		MotorAResetCurrentStepCommand.ScheduleWith(),
//		//new ScheduledCommand([&]() {return MotorA.MultiStepMove(3); }, (unsigned char)Systems::MotorA),
//		//new ScheduledCommand([&]() {return MotorA.ResetCurrentStep(); }, (unsigned char)Systems::MotorA),
//	}),
//	MotorBMultiStepMoveCommand.ScheduleWith(five),
//	MotorBResetCurrentStepCommand.ScheduleWith(),
//	//new ScheduledCommand([&]() {return MotorB.MultiStepMove(5); }, (unsigned char)Systems::MotorB),
//	//new ScheduledCommand([&]() {return MotorB.ResetCurrentStep(); }, (unsigned char)Systems::MotorB),
//	new ParallelGroup({
//		MotorAMultiStepMoveCommand.ScheduleWith(five),
//		MotorAResetCurrentStepCommand.ScheduleWith(),
//		//new ScheduledCommand([&]() {return MotorA.MultiStepMove(5); }, (unsigned char)Systems::MotorA),
//		//new ScheduledCommand([&]() {return MotorA.ResetCurrentStep(); }, (unsigned char)Systems::MotorA),
//		new SequentialGroup({
//			MotorBMultiStepMoveCommand.ScheduleWith(four),
//			MotorCMultiStepMoveCommand.ScheduleWith(five),
//			//new ScheduledCommand([&]() {return MotorB.MultiStepMove(4); }, (unsigned char)Systems::MotorB), //scheduling stuff should just take in Systems and convert it to unsigned char(or maybe just make Command wrapper more convenient)
//			//new ScheduledCommand([&]() {return MotorC.MultiStepMove(5); }, (unsigned char)Systems::MotorC), //Add Initialization and EndBehavior functions to commands
//		}),
//	}),
//	});
//
//scheduler.Schedule(sequentialGroup);
//while (!scheduler.Run()) //scheduler needs a bool for when it is done
//{
//	std::cout << "\n";
//}


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
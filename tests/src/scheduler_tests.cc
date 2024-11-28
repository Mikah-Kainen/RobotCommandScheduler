#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <list>

//scheduler includes
#include "static.h"
#include "schedulable_groups/scheduler.h"
#include "schedulable_groups/parallel_group.h"
#include "schedulable_groups/sequential_group.h"
#include "schedulable_groups/loop_group.h"
#include "schedulable_groups/condition_group.h"
#include "schedulable_groups/when_any_group.h"

#include "command_builder.cc"

//virtual robot includes
#include "robot.h"
#include "motor.h"
#include "timer.h"

#pragma region CatStuff
class Cat
{
public:
	int Age;
	std::string Name;
	bool IsDead;

	Cat(int age, std::string name)
		: Age{ age }, Name{ name }, IsDead{ false }
	{
		//std::cout << "Cat Made" << std::endl;
	}

	Cat(const Cat& copyCat)
		: Age{ copyCat.Age }, Name{ copyCat.Name }, IsDead{ copyCat.IsDead }
	{
		//std::cout << "Cat Copied" << std::endl;
	}

	~Cat()
	{
		if (IsDead)
		{
			std::cout << "A Dead Cat is Back" << std::endl;
		}
		//std::cout << "A Cat Just Died\n";
		IsDead = true;
	}

	bool Display()
	{
		if (IsDead)
		{
			std::cout << "Displaying Dead Cat!" << std::endl;
		}
		else
		{
			std::cout << "Name: " << Name << ", Age: " << Age << std::endl;
		}
		return true;
	}
};

//class OldCat : public Cat
//{
//public:
//
//	OldCat(std::string name)
//		: Cat(38, name)
//	{
//	}
//
//	~OldCat()
//	{
//
//	}
//};

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

Motor MotorA = Motor(Systems::MotorA);
Motor MotorB = Motor(Systems::MotorB);
Motor MotorC = Motor(Systems::MotorC);

bool test()
{
	return MotorA.MultiStepMove(5);
}

#pragma region WrapperFunctions

bool MotorAMultiStepMove(int params, std::vector<int>* loggingVector)
{
	return MotorA.MultiStepMove(params, loggingVector);
}

bool MotorBMultiStepMove(int params, std::vector<int>* loggingVector)
{
	return MotorB.MultiStepMove(params, loggingVector);
}

bool MotorCMultiStepMove(int params, std::vector<int>* loggingVector)
{
	return MotorC.MultiStepMove(params, loggingVector);
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


Robot& robot = Robot::GetInstance();

void TakeConstRef(const int& constRef)
{
	std::cout << constRef << std::endl;
}


TEST(SequentialGroup, BasicAssertions) {
	CommandBuilder<int, std::vector<int>*> MotorAMultiStepMoveCommand = CommandBuilder<int, std::vector<int>*>(MotorAMultiStepMove, Systems::MotorA);
	CommandBuilder<int, std::vector<int>*> MotorBMultiStepMoveCommand = CommandBuilder<int, std::vector<int>*>(MotorBMultiStepMove, Systems::MotorB);
	CommandBuilder<int, std::vector<int>*> MotorCMultiStepMoveCommand = CommandBuilder<int, std::vector<int>*>(MotorCMultiStepMove, Systems::MotorC);

	CommandBuilder<> MotorAResetCurrentStepCommand = CommandBuilder<>(MotorAResetCurrentStep, Systems::MotorA);
	CommandBuilder<> MotorBResetCurrentStepCommand = CommandBuilder<>(MotorBResetCurrentStep, Systems::MotorB);
	CommandBuilder<> MotorCResetCurrentStepCommand = CommandBuilder<>(MotorCResetCurrentStep, Systems::MotorC);

  std::vector<int>* loggingVector = new std::vector<int>();

	int three = 3;
	int four = 4;
	int five = 5;


	Scheduler scheduler = Scheduler::GetInstance();

	std::shared_ptr<SequentialGroup> sequentialGroup = std::make_shared<SequentialGroup>(SequentialGroup({
		std::make_shared<ParallelGroup>(ParallelGroup({
			MotorAMultiStepMoveCommand.CreateCommand(three, loggingVector),
			MotorAResetCurrentStepCommand.CreateCommand(),
		})),
		MotorBMultiStepMoveCommand.CreateCommand(five, loggingVector),
		MotorBResetCurrentStepCommand.CreateCommand(),
		std::make_shared<ParallelGroup>(ParallelGroup({
			MotorAMultiStepMoveCommand.CreateCommand(five, loggingVector),
			MotorAResetCurrentStepCommand.CreateCommand(),
			std::make_shared<SequentialGroup>(SequentialGroup({
				MotorBMultiStepMoveCommand.CreateCommand(four, loggingVector),
				MotorCMultiStepMoveCommand.CreateCommand(five, loggingVector),
			})),
		})),
	}));

	scheduler.Schedule(sequentialGroup);
	while (!scheduler.Run()){}
	std::cout << "Program Finished!";
  bool inOrder = true;
  int previous = -100;
  for (int i : *loggingVector) {
    if (i >= previous) {
      i = previous;
    }
    else {
      inOrder = false;
      break;
    }
  }
  EXPECT_EQ(inOrder, true);
}
#include <iostream>
#include "Command.cpp"
#include <string>
#include <list>
#include "Scheduler.h"
#include "Static.h"
#include "ParallelGroup.h"
#include "SequentialGroup.h"
#include "Robot.h"
#include "Motor.h"
#include "Timer.h"

#pragma region CatStuff
class Cat
{
public:
	int Age;
	std::string Name;

	Cat(int age, std::string name)
		: Age{ age }, Name{ name } {}

	~Cat()
	{
		std::cout << "A Cat Just Died\n";
	}
};

class OldCat : public Cat
{
public:

	OldCat(std::string name)
		: Cat(38, name)
	{
	}

	~OldCat()
	{

	}
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


Robot& robot = Robot::GetInstance();

#pragma region 

SequentialGroup* sequentialGroup0 = new SequentialGroup({
new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
	});

SequentialGroup* sequentialGroup1 = new SequentialGroup({
new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
	});

SequentialGroup* sequentialGroup2 = new SequentialGroup({
new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
	});

SequentialGroup* sequentialGroup3 = new SequentialGroup({
new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
	});

SequentialGroup* sequentialGroup4 = new SequentialGroup({
new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
new FunctionManager::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
	});
#pragma endregion

int main() //Unit tests with GoogleTest
{
	//**Things to do
//scheduling stuff should just take in Systems and convert it to unsigned char(or maybe just make Command wrapper more convenient)
//Add Initialization and EndBehavior functions to commands
//give schedulers unique IDs for debugging purposes

//new ScheduledCommand([&]() {return MotorB.MultiStepMove(4); }, (unsigned char)Systems::MotorB), //scheduling stuff should just take in Systems and convert it to unsigned char(or maybe just make Command wrapper more convenient)
//new ScheduledCommand([&]() {return MotorC.MultiStepMove(5); }, (unsigned char)Systems::MotorC), //Add Initialization and EndBehavior functions to commands

#pragma region CommandExample
	//Command<int> MotorAMultiStepMoveCommand = Command<int>(MotorAMultiStepMove, Systems::MotorA);
	//Command<int> MotorBMultiStepMoveCommand = Command<int>(MotorBMultiStepMove, Systems::MotorB);
	//Command<int> MotorCMultiStepMoveCommand = Command<int>(MotorCMultiStepMove, Systems::MotorC);

	//Command<> MotorAResetCurrentStepCommand = Command<>(MotorAResetCurrentStep, Systems::MotorA);
	//Command<> MotorBResetCurrentStepCommand = Command<>(MotorBResetCurrentStep, Systems::MotorB);
	//Command<> MotorCResetCurrentStepCommand = Command<>(MotorCResetCurrentStep, Systems::MotorC);

	//int three = 3;
	//int four = 4;
	//int five = 5;


	//Scheduler scheduler = Scheduler::GetInstance();

	//SequentialGroup* sequentialGroup = new SequentialGroup({
	//	new ParallelGroup({
	//		MotorAMultiStepMoveCommand.ScheduleWith(three),
	//		MotorAResetCurrentStepCommand.ScheduleWith(),
	//	}),
	//	MotorBMultiStepMoveCommand.ScheduleWith(five),
	//	MotorBResetCurrentStepCommand.ScheduleWith(),
	//	new ParallelGroup({
	//		MotorAMultiStepMoveCommand.ScheduleWith(five),
	//		MotorAResetCurrentStepCommand.ScheduleWith(),
	//		new SequentialGroup({
	//			MotorBMultiStepMoveCommand.ScheduleWith(four),
	//			MotorCMultiStepMoveCommand.ScheduleWith(five),
	//		}),
	//	}),
	//});

	//scheduler.Schedule(sequentialGroup);
	//while (!scheduler.Run()){}
	//std::cout << "Program Finished!";
#pragma endregion

	Command<int> MoveLeftMotorByTime = Command<int>(std::function<bool(int)>([&](int seconds) {return std::bind(&Motor::MoveByTime, robot.GetLeftMotor(), seconds)(/*6, "hello", seconds, seconds, "Wow"*/); }), (unsigned char)Systems::LeftMotor);

	std::cout << "Robot Made!\n";

	Scheduler& scheduler = Scheduler::GetInstance();

	//SequentialGroup* sequentialGroup = new SequentialGroup({
	//new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
	//new FunctionManager::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
	//new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
	//new FunctionManager::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
	//new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
	//new FunctionManager::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
	//	});

	int oneThousand = 1000;
	int fiveThousand = 5000;

	SequentialGroup* sequentialGroup = new SequentialGroup({
		new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
		MoveLeftMotorByTime.ScheduleWith(oneThousand),
		new FunctionManager::Scheduleable([&]() {std::cout << "Moving 5000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
		MoveLeftMotorByTime.ScheduleWith(fiveThousand),
		new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
		MoveLeftMotorByTime.ScheduleWith(oneThousand),
		});

	FunctionManager::Scheduleable* endFunction = new FunctionManager::Scheduleable([&]() {std::cout << "End of Functions, Current Time: " << Timer::GetInstance().ElapsedMilliseconds() << std::endl; return true; }, (unsigned char)Systems::All);

	//scheduler.Schedule(sequentialGroup2);
	scheduler.Schedule(sequentialGroup);
	scheduler.Schedule(endFunction);

	//if (true)
	//{
	//	OldCat cat = OldCat("Bob");
	//}

	while (!scheduler.Run()) {}
	std::cout << "EVERYTHING DONE!!!!!!!\n";
	while (true)
	{
	}
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
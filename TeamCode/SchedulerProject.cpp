#include <iostream>
#include "CommandBuilder.cpp"
#include <string>
#include <list>
#include "Scheduler.h"
#include "Static.h"
#include "ParallelGroup.h"
#include "SequentialGroup.h"
#include "Robot.h"
#include "Motor.h"
#include "Timer.h"
#include "MemoryEater.cpp"

#pragma region CatStuff
class Cat
{
public:
	int Age;
	std::string Name;
	bool IsDead;

	Cat(int age, std::string name)
		: Age{ age }, Name{ name }, IsDead{false} {}

	Cat(const Cat& copyCat)
		: Age{ copyCat.Age }, Name{ copyCat.Name }, IsDead{copyCat.IsDead}
	{
		std::cout << "Cat Copied" << std::endl;
	}

	~Cat()
	{
		if (IsDead)
		{
			std::cout << "A Dead Cat is Back" << std::endl;
		}
		std::cout << "A Cat Just Died\n";
		IsDead = true;
	}

	bool Display() const
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

void TakeConstRef(const int& constRef)
{
	std::cout << constRef << std::endl;
}

int main() //Unit tests with GoogleTest
{
	//**Things to do
//scheduling stuff should just take in Systems and convert it to unsigned char(or maybe just make Command wrapper more convenient)
//Add Initialization and EndBehavior functions to commands
//give schedulers unique IDs for debugging purposes

//ScheduleWith should return a shared pointer and Schedulers should use shared pointers for Scheduleable
//MemoryEater should return a key/provide some functionality so that memory can be uneaten at some point
//	^also it should take in a bool for if the uneated memory can be cleaned up(always should be true in the case of values but references might want to be preserved if its from an important function)
//look into and start using const ref
//maybe change MemoryEater to use void* instead of TypeBase and Type

//I should add proper destructors at some point

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

	CommandBuilder<const int&> MoveLeftMotorByTime = CommandBuilder<const int&>(std::bind(&Motor::MoveByTime, robot.GetLeftMotor(), std::placeholders::_1), (unsigned char)Systems::LeftMotor);
	CommandBuilder<int> MoveLeftMotorByTimeVal = CommandBuilder<int>(std::bind(&Motor::MoveByTime, robot.GetLeftMotor(), std::placeholders::_1), (unsigned char)Systems::LeftMotor);
	//Command<const std::vector<std::string>&> DisplayMessage = Command<const std::vector<std::string>&>([&](const std::vector<std::string>& messages) {for (std::string message : messages) { std::cout << message; } std::cout << std::flush; return true; }, (unsigned char)Systems::RightMotor);
	std::function<bool(const std::string&)> myFunc = std::function<bool(const std::string&)>([&](const std::string& message) {std::cout << message; return true; });
	myFunc("Hi\n\n");
	CommandBuilder<std::string*> DisplayString = CommandBuilder<std::string*>(std::function<bool(std::string*)>([&](std::string* message) {std::cout << *message; return true; }), (unsigned char)Systems::RightMotor);

	CommandBuilder<const int&> DisplayNumber = CommandBuilder<const int&>(std::function<bool(const int&)>([&](const int& number) {std::cout << number << std::endl; return true; }), (unsigned char)Systems::LeftMotor);
	CommandBuilder<int> DisplayNumberVal = CommandBuilder<int>(std::function<bool(int)>([&](int number) {std::cout << number << std::endl; return true; }), (unsigned char)Systems::LeftMotor);
	CommandBuilder<> UpdateLeftMotorTime = CommandBuilder<>(std::bind(&Motor::UpdateTime, robot.GetLeftMotor()), (unsigned char)Systems::LeftMotor);

	CommandBuilder<Cat> TakesCatVal = CommandBuilder<Cat>(std::function<bool(Cat)>([&](Cat cat) {return cat.Display(); }), Systems::RightMotor);
	CommandBuilder<const Cat&> TakesCat = CommandBuilder<const Cat&>(std::function<bool(const Cat&)>([&](const Cat& cat) {return cat.Display(); }), Systems::RightMotor);
	CommandBuilder<Cat*> TakesCatPointer = CommandBuilder<Cat*>(std::function<bool(Cat*)>([&](Cat* cat) {return cat->Display(); }), Systems::RightMotor);

	//Cat cat = Cat(600, "SuperCat");
	//auto bindResult = std::bind(&Cat::Display, Cat(600, "SuperCat"));
	//Command<> TakesCatBind = Command<>(std::bind(&Cat::Display, cat), Systems::RightMotor);
	//cat.Age = 0;

	std::cout << "Robot Made!\n";

	Scheduler& scheduler = Scheduler::GetInstance();
	MemoryEater& eater = MemoryEater::GetInstance();

	//TakeConstRef(5);

	//SequentialGroup* sequentialGroup = new SequentialGroup({
	//new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
	//new FunctionManager::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
	//new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
	//new FunctionManager::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
	//new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
	//new FunctionManage

	//int oneThousand = 1000;
	//int fiveThousand = 5000;
	//SequentialGroup* sequentialGroup = new SequentialGroup({
	//	new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
	//	MoveLeftMotorByTime.ScheduleWith(oneThousand),
	//	new FunctionManager::Scheduleable([&]() {std::cout << "Moving 5000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
	//	MoveLeftMotorByTime.ScheduleWith(fiveThousand),
	//	new FunctionManager::Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
	//	MoveLeftMotorByTime.ScheduleWith(oneThousand),
	//	});r::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
	//	});

	std::vector<std::shared_ptr<FunctionManager::Scheduleable>> delayFunctions = std::vector<std::shared_ptr<FunctionManager::Scheduleable>>();
	int delays[] = { 1000, 3000, 1000, };
	//std::vector<std::string> message = { "Displaying Message\n" };
	for (int i = 0; i < 3; i++)
	{
		//scheduler.Schedule(UpdateLeftMotorTime.ScheduleWith());
		delayFunctions.push_back(UpdateLeftMotorTime.CreateCommand());
		
		//auto result = DisplayString.ScheduleWith("Moving ");
		//scheduler.Schedule(DisplayString.ScheduleWith("Moving "));
		//scheduler.Schedule(DisplayNumber.ScheduleWith(6));
		//delayFunctions.push_back(DisplayString.ScheduleWith("Moving "));
		delayFunctions.push_back(DisplayNumberVal.CreateCommand(delays[i]));

		delayFunctions.push_back(DisplayString.CreateCommand(new std::string(std::to_string(delays[i])))); //Uncomment this for a fun suprise
		delayFunctions.push_back(DisplayString.CreateCommand(new std::string(" milliseconds\n"))); //Uncomment this for a fun suprise

		//scheduler.Schedule(MoveLeftMotorByTime.ScheduleWith(delays[i]));
		delayFunctions.push_back(MoveLeftMotorByTimeVal.CreateCommand(delays[i]));

	}
	//Cat cat = Cat(10, "Tim");
	//delayFunctions.push_back(TakesCatVal.ScheduleWith(cat));
	//cat.Age = 100;
	delayFunctions.push_back(TakesCatPointer.CreateCommand(new Cat(12, "John")));
	//delayFunctions.push_back(TakesCatBind.ScheduleWith());
	delays[0] = 500;
	delays[1] = 500;
	delays[2] = 500;
	//delays = { 2000, 1000, 2000 };

	std::shared_ptr<SequentialGroup> sequentialGroup = std::make_shared<SequentialGroup>(delayFunctions);

	std::string otherMessage = "End of Functions, Current Time: ";
	std::shared_ptr<FunctionManager::Scheduleable> endFunction = std::make_shared<FunctionManager::Scheduleable>([&]() {std::cout << otherMessage << Timer::GetInstance().ElapsedMilliseconds() << std::endl; return true; }, (unsigned char)Systems::All);
	//otherMessage = "Haha I changed it ";

	//scheduler.Schedule(sequentialGroup2);
	scheduler.Schedule(sequentialGroup);
	scheduler.Schedule(endFunction);

	//if (true)
	//{
	//	OldCat cat = OldCat("Bob");
	//}

	while (!scheduler.Run()) {}
	std::cout << "EVERYTHING DONE!!!!!!!" << std::endl;
	//while (true)
	//{
	//}
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
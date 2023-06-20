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

#pragma region CatStuff
class Cat
{
public:
	int Age;
	std::string Name;
	bool IsDead;

	Cat(int age, std::string name)
		: Age{ age }, Name{ name }, IsDead{false} 
	{
		//std::cout << "Cat Made" << std::endl;
	}

	Cat(const Cat& copyCat)
		: Age{ copyCat.Age }, Name{ copyCat.Name }, IsDead{copyCat.IsDead}
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

/* To do list:
	Remove FunctionManager
	Add its dictionaries to GroupBase(scheduleables and endBehavior)
	Give GroupBase initialize functions that take in a GroupBase&(don't forget to get rid of the part where I remove cleanup functions once they have been run)
	Change setting flags to have two functions, change int and set int
	Add dictionary of list of functions that run when a scheduleable is over to FunctionManager
	Add initialized flag to GroupBase IDs
	Stop removing from FunctionManager when scheduleables are done
	Add reset functions to initialize of scheduleables and reset initialize flag after things are run and figure out general initialize stuff
	Add state enum to scheduleable to transition between Initialize, Run, and Cleanup
	Add Systems::Other
	Change Schedule to keep track of current index for each system instead of just removing
	Change deconstructor of GroupBase to clean up Scheduleables
	Maybe make ProgramScheduler remove scheduleables from the schedule when they are finished (virtual bool cleanup that can be overriden by Scheduler)
	Polyfill std::apply
	Add interruptable flag
	Add while groups
	Add GroupBuilder
	Make FunctionManager and GroupBase have templates that take in unsigned types for their ID tracking
*/
int main() //Unit tests with GoogleTest
{
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
	CommandBuilder<std::string*> DisplayStringPointer = CommandBuilder<std::string*>(std::function<bool(std::string*)>([&](std::string* message) {std::cout << *message; return true; }), Systems::RightMotor);
	CommandBuilder<const std::string&> DisplayStringReference = CommandBuilder<const std::string&>([&](const std::string& message){std::cout << message; return true; }, Systems::RightMotor);
	CommandBuilder<std::string> DisplayStringValue = CommandBuilder<std::string>([&](std::string message) {std::cout << message; return true; }, Systems::RightMotor);

	CommandBuilder<Cat*> DisplayCatPointerFive = CommandBuilder<Cat*>([&](Cat* cat) {return cat->Display(); }, Systems::Five);
	CommandBuilder<Cat&> DisplayCatReferenceFive = CommandBuilder<Cat&>([&](Cat& cat) {return cat.Display(); }, Systems::Five);
	CommandBuilder<Cat> DisplayCatValueFive = CommandBuilder<Cat>([&](Cat cat) {return cat.Display(); }, Systems::Five);

	CommandBuilder<Cat*> DisplayCatPointerFour = CommandBuilder<Cat*>([&](Cat* cat) {return cat->Display(); }, Systems::Four);
	CommandBuilder<Cat&> DisplayCatReferenceFour = CommandBuilder<Cat&>([&](Cat& cat) {return cat.Display(); }, Systems::Four);
	CommandBuilder<Cat> DisplayCatValueFour = CommandBuilder<Cat>([&](Cat cat) {return cat.Display(); }, Systems::Four);

	CommandBuilder<Cat*> DisplayCatPointerSix = CommandBuilder<Cat*>([&](Cat* cat) {return cat->Display(); }, Systems::Six);
	CommandBuilder<Cat&> DisplayCatReferenceSix = CommandBuilder<Cat&>([&](Cat& cat) {return cat.Display(); }, Systems::Six);
	CommandBuilder<Cat> DisplayCatValueSix = CommandBuilder<Cat>([&](Cat cat) {return cat.Display(); }, Systems::Six);
	CommandBuilder<Cat&, int> IncrimentCatAgeSix = CommandBuilder<Cat&, int>([&](Cat& cat, int age) {cat.Age += age; std::cout << "System Six being used" << std::endl; return true; }, Systems::Six);

	CommandBuilder<int&> DisplayNumber = CommandBuilder<int&>(std::function<bool(int&)>([&](int& number) {std::cout << number << std::endl; return true; }), (unsigned char)Systems::LeftMotor);
	CommandBuilder<int> DisplayNumberVal = CommandBuilder<int>(std::function<bool(int)>([&](int number) {std::cout << number << std::endl; return true; }), (unsigned char)Systems::LeftMotor);
	CommandBuilder<> UpdateLeftMotorTime = CommandBuilder<>(std::bind(&Motor::UpdateTime, robot.GetLeftMotor()), (unsigned char)Systems::LeftMotor);

	//Cat cat = Cat(600, "SuperCat");
	//auto bindResult = std::bind(&Cat::Display, Cat(600, "SuperCat"));
	//Command<> TakesCatBind = Command<>(std::bind(&Cat::Display, cat), Systems::RightMotor);
	//cat.Age = 0;

	std::cout << "Robot Made!\n";

	Scheduler& scheduler = Scheduler::GetInstance();
	///MemoryEater& eater = MemoryEater::GetInstance();

	//TakeConstRef(5);

	//SequentialGroup* sequentialGroup = new SequentialGroup({
	//new Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
	//new Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
	//new Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
	//new Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
	//new Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
	//new FunctionManage

	//int oneThousand = 1000;
	//int fiveThousand = 5000;
	//SequentialGroup* sequentialGroup = new SequentialGroup({
	//	new Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
	//	MoveLeftMotorByTime.ScheduleWith(oneThousand),
	//	new Scheduleable([&]() {std::cout << "Moving 5000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
	//	MoveLeftMotorByTime.ScheduleWith(fiveThousand),
	//	new Scheduleable([&]() {std::cout << "Moving 1000 milliseconds\n"; return robot.GetLeftMotor()->UpdateTime(); }, (unsigned char)Systems::LeftMotor),
	//	MoveLeftMotorByTime.ScheduleWith(oneThousand),
	//	});r::Scheduleable([&]() {return robot.GetLeftMotor()->MoveByTime(1000); }, (unsigned char)Systems::LeftMotor),
	//	});

	std::vector<std::shared_ptr<Scheduleable>> delayFunctions = std::vector<std::shared_ptr<Scheduleable>>();
	int delays[] = { 1000, 3000, 1000, };
	//std::vector<std::string> message = { "Displaying Message\n" };
	for (int i = 0; i < 3; i++)
	{
		////scheduler.Schedule(UpdateLeftMotorTime.ScheduleWith());
		//delayFunctions.push_back(UpdateLeftMotorTime.CreateCommand());
		//
		////auto result = DisplayString.ScheduleWith("Moving ");
		////scheduler.Schedule(DisplayString.ScheduleWith("Moving "));
		////scheduler.Schedule(DisplayNumber.ScheduleWith(6));
		////delayFunctions.push_back(DisplayString.ScheduleWith("Moving "));
		//delayFunctions.push_back(DisplayNumberVal.CreateCommand(delays[i]));

		//delayFunctions.push_back(DisplayStringPointer.CreateCommand(new std::string(std::to_string(delays[i])))); //Uncomment this for a fun suprise
		//delayFunctions.push_back(DisplayStringPointer.CreateCommand(new std::string(" milliseconds\n"))); //Uncomment this for a fun suprise

		////scheduler.Schedule(MoveLeftMotorByTime.ScheduleWith(delays[i]));
		//delayFunctions.push_back(MoveLeftMotorByTimeVal.CreateCommand(delays[i]));
	}

	//Cat cat = Cat(10, "Tim");
	//delayFunctions.push_back(TakesCatVal.ScheduleWith(cat));
	//cat.Age = 100;
	// 
	//std::shared_ptr<Scheduleable> command = TakesCatPointer.CreateCommand(new Cat(12, "John"));
	//delayFunctions.push_back(command);
	// 
	//delayFunctions.push_back(TakesCatBind.ScheduleWith());
	delays[0] = 500;
	delays[1] = 500;
	delays[2] = 500;
	//delays = { 2000, 1000, 2000 };

	//std::shared_ptr<SequentialGroup> sequentialGroup = std::make_shared<SequentialGroup>(delayFunctions);
	std::string* message = new std::string("This is running\n");
	//Cat* cat = new Cat(50, "PLEASE WORK");


	std::shared_ptr<Scheduleable> newLineFunction = std::make_shared<Scheduleable>([&]() {std::cout << std::endl; return true; }, Systems::Five);
	//Don't forget to add Systems::None to the tests once that is supported

#pragma region SequentialGroupTest
	Cat  sequentialGroupTestCat3 = Cat(3, "Systems 5");
	Cat* sequentialGroupTestCat5 = new Cat(5, "Systems 4");
	Cat  sequentialGroupTestCat6 = Cat(-1, "Systems 4");
	Cat  sequentialGroupTestCat8and9 = Cat(8, "Systems 6");
	std::shared_ptr<SequentialGroup> sequentialGroupTest = std::make_shared<SequentialGroup>(SequentialGroup({
		DisplayCatValueFive.CreateCommand(Cat(1, "Systems 5")),
		DisplayCatValueFour.CreateCommand(Cat(2, "Systems 4")),
		DisplayCatReferenceFive.CreateCommand(sequentialGroupTestCat3),
		DisplayCatPointerFour.CreateCommand(new Cat(4, "Systems 4")),
		DisplayCatPointerFour.CreateCommand(sequentialGroupTestCat5),
		DisplayCatReferenceFour.CreateCommand(sequentialGroupTestCat6),
		DisplayCatValueFive.CreateCommand(Cat(sequentialGroupTestCat3.Age + 4, sequentialGroupTestCat3.Name)),
		DisplayCatValueSix.CreateCommand(sequentialGroupTestCat8and9),
		DisplayCatReferenceSix.CreateCommand(sequentialGroupTestCat8and9),
	}));
	sequentialGroupTestCat6.Age = 6;
	sequentialGroupTestCat8and9.Age = 9;
#pragma endregion

#pragma region ParallelGroupTest
	Cat  parallelGroupTestCat2 = Cat(2, "Systems 5");
	Cat* parallelGroupTestCat3 = new Cat(3, "Systems 4");
	Cat  parallelGroupTestCat4 = Cat(-1, "Systems 4");
	Cat  parallelGroupTestCat1and2 = Cat(1, "Systems 6");
	std::shared_ptr<ParallelGroup> parallelGroupTest = std::make_shared<ParallelGroup>(ParallelGroup({
		DisplayCatValueFive.CreateCommand(Cat(1, "Systems 5")),
		DisplayCatValueFour.CreateCommand(Cat(1, "Systems 4")),
		DisplayCatReferenceFive.CreateCommand(parallelGroupTestCat2),
		DisplayCatPointerFour.CreateCommand(new Cat(2, "Systems 4")),
		DisplayCatPointerFour.CreateCommand(  parallelGroupTestCat3),
		DisplayCatReferenceFour.CreateCommand(parallelGroupTestCat4),
		DisplayCatValueFive.CreateCommand(Cat(parallelGroupTestCat3->Age, parallelGroupTestCat2.Name)),
		DisplayCatValueSix.CreateCommand(     parallelGroupTestCat1and2),
		DisplayCatReferenceSix.CreateCommand( parallelGroupTestCat1and2),
		}));
	parallelGroupTestCat4.Age = 4;
	parallelGroupTestCat1and2.Age = 2;
#pragma endregion

#pragma region ParallelGroupCombinationTests

	Cat* parallelGroup4and5TestCat1Pointer = new Cat(1, "Systems 4");
	Cat  parallelGroup4and5TestCat1 = Cat(-1, "ERROR");
	Cat  parallelGroup4and5TestCat2 = Cat(2, "Systems 5");
	Cat  parallelGroup4and5TestCat3and4 = Cat(3, "Systems 5");
	Cat  parallelGroup4and5TestCat2Systems4 = Cat(2, "Systems 4");
	std::shared_ptr<ParallelGroup> parallelGroup4and5 = std::make_shared<ParallelGroup>(ParallelGroup({
		DisplayCatReferenceFive.CreateCommand(parallelGroup4and5TestCat1),
		DisplayCatValueFive.CreateCommand(parallelGroup4and5TestCat2),
		DisplayCatPointerFour.CreateCommand(parallelGroup4and5TestCat1Pointer),
		DisplayCatValueFive.CreateCommand(parallelGroup4and5TestCat3and4),
		DisplayCatReferenceFive.CreateCommand(parallelGroup4and5TestCat3and4),
		DisplayCatValueFour.CreateCommand(parallelGroup4and5TestCat2Systems4),
	}));
	parallelGroup4and5TestCat1 = Cat(1, "Systems 5");
	parallelGroup4and5TestCat3and4.Age = 4;

	Cat parallelGroup6TestCat1and2and4 = Cat(1, "Systems 6");
	Cat parallelGroup6TestCat6and7 = Cat(6, "Systems 6");
	std::shared_ptr<ParallelGroup> parallelGroup6 = std::make_shared<ParallelGroup>(ParallelGroup({
		DisplayCatValueSix.CreateCommand(parallelGroup6TestCat1and2and4),
		DisplayCatReferenceSix.CreateCommand(parallelGroup6TestCat1and2and4),
		IncrimentCatAgeSix.CreateCommand(parallelGroup6TestCat1and2and4, 2),
		DisplayCatReferenceSix.CreateCommand(parallelGroup6TestCat1and2and4),
		IncrimentCatAgeSix.CreateCommand(parallelGroup6TestCat6and7, 1),
		DisplayCatValueSix.CreateCommand(parallelGroup6TestCat6and7),
		DisplayCatReferenceSix.CreateCommand(parallelGroup6TestCat6and7),
	}));
	parallelGroup6TestCat1and2and4.Age++;

	std::shared_ptr<ParallelGroup> parallelGroupCombinationTest = std::make_shared<ParallelGroup>(ParallelGroup({
		parallelGroup4and5,
		parallelGroup6,
	}));

	std::shared_ptr<SequentialGroup> tempSequentialGroupCombinationTest = std::make_shared<SequentialGroup>(SequentialGroup({
		parallelGroup6,
		parallelGroup4and5,
	}));
#pragma endregion


	std::string otherMessage = "End of Functions, Current Time: ";
	std::shared_ptr<Scheduleable> endFunction = std::make_shared<Scheduleable>([&]() {std::cout << otherMessage << Timer::GetInstance().ElapsedMilliseconds() << std::endl; return true; }, (unsigned char)Systems::All);
	//otherMessage = "Haha I changed it ";

	//ERROR! ERROR! FOUND ERROR! ApparentCase: In a sequential group that uses 2 groups, if you schedule the same 2 groups after in a parallel group the first one will be skipped one time(Maybe because of left over bar flag?)!
	/* Error program
	scheduler.Schedule(std::make_shared<SequentialGroup>(SequentialGroup({ parallelGroupCombinationTest, tempSequentialGroupCombinationTest })));
	scheduler.Schedule(parallelGroupCombinationTest);
	scheduler.Schedule(tempSequentialGroupCombinationTest);
	*/
	
	scheduler.Schedule(std::make_shared<SequentialGroup>(SequentialGroup({ parallelGroupCombinationTest, tempSequentialGroupCombinationTest })));
	//Make sure scheduleables are destructed properly in the main Scheduler
	scheduler.Schedule(parallelGroupCombinationTest);
	scheduler.Schedule(tempSequentialGroupCombinationTest);
	//scheduler.Schedule([&]() {std::cout << std::endl; return true; }, (unsigned char)Systems::Six);
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

//new ParallelGroup(std::vector<Scheduleable*>({
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
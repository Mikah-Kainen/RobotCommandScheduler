#include <iostream>
#include "Command.cpp"
#include <string>
#include <list>
#include "Scheduler.h"
#include "Static.h"

#pragma region CatStuff
class Cat
{
public:
    int Age;
    std::string Name;

    Cat(int age, std::string name)
        : Age{age}, Name{name} {}
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
        : MotorID{ motor }, currentStep{0}
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

int main()
{

    Scheduler scheduler = Scheduler::GetInstance();
    //scheduler.Schedule(MotorA.Display, (char)Systems::MotorA);
    scheduler.Schedule([&]() {return MotorA.MultiStepMove(3); }, (char)Systems::MotorA);
    scheduler.Schedule([&]() {return MotorA.ResetCurrentStep(); }, (char)Systems::MotorA);
    scheduler.Schedule(test, (char)Systems::MotorA);
    scheduler.Schedule([&]() {return MotorB.MultiStepMove(4); }, (char)Systems::MotorB);
    scheduler.Schedule([&]() {return MotorC.MultiStepMove(5); }, (char)Systems::MotorC);

    while (true) //scheduler needs a bool for when it is done
    {
        scheduler.Run();
        //std::cout << "\n";
    }



    //scheduler.Schedule(CoolFunction, (char)1);
    //scheduler.Schedule(CoolCatFunction, (char)1);
    //scheduler.Schedule(CoolFunction, (char)1);
    //for(int i = 0; i < 4; i ++)
    //{
    //    scheduler.Run();
    //    std::cout << "\n";
    //}
}
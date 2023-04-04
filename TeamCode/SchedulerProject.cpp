#include <iostream>
#include "Command.cpp"
#include <string>
#include <list>
#include "Scheduler.h"


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


int main()
{
    Scheduler scheduler = Scheduler::GetInstance();
    scheduler.Schedule(CoolFunction, (char)1);
    scheduler.Schedule(CoolCatFunction, (char)1);
    scheduler.Schedule(CoolFunction, (char)1);
    for(int i = 0; i < 4; i ++)
    {
        scheduler.Run();
        std::cout << "\n";
    }
}
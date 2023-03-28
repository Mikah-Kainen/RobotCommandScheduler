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



int main()
{
    //Command<> beCool = Command<>(CoolFunction);
    //beCool.Activate();

    //Cat cat = Cat(0, "Zero");
    //Command<Cat> catCat = Command<Cat>(CatFunction);
    //catCat.Activate(cat);
    //std::cout << "Hello World!\n";
    //Func0 func0 = Func0(&CoolFunction);b
    //func0.Activate();
    //Cat cat = Cat(0, "Zero");
    //Func1<Cat> func1 = Func1<Cat>(CatFunction);
    //func1.Activate(cat);

    std::list<int> linkedList = std::list<int>();
    int value = 5;
    std::make_unique<int>(value);

    char mask = 9;
    char otherMask = 12;
    mask |= otherMask;
    char nothing = 0;
    otherMask |= nothing;
    nothing |= otherMask;

    Scheduler scheduler = Scheduler::GetInstance();
    scheduler.Run();
}